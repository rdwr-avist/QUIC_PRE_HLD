# O01: Shared Incremental ClientHello Contract V4

**Status: O01 analysis completed; concrete design proposal for review, not user-approved implementation.**

This document resolves the O01 design question from Consolidated V5 section 11: the shared CH region API, accepted-byte accounting and continuation representation. It does not select O02 (base-class orchestration versus protocol-owned calls), modify code, or replace the official HLD. V5 and the supplied code remain unchanged.

## 1. Decision in brief

Use one logical CH context and an append-only, in-order region contract. Accept the four-byte handshake header followed by the declared ClientHello body, without transport framing. Accepting bytes means the caller need not supply those bytes again: all information still needed is either interpreted or held in valid prepared backing.

Process by bounded regions: handshake header, CH prefix including its vectors, and extension block. Reuse existing parsing/normalization/FP logic when its required region is complete. Do not restart fingerprint generation on every fragment. Collect the extension block before sorting and feeding its canonical FP contribution. A complete CH can finish in one call; a partial CH need not wait for the whole message before prefix interpretation and base/cipher FP work.

This is a shared algorithm contract, not a new component, heap object, viewer replacement, transport interface, or fixed list of virtual hooks.

## 2. Authority and evidence

### 2.1 Inputs

- Existing code: `TLS-Flow-All-Code.txt`, SHA-256 `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b`.
- Current working analysis: `tls_flow_strategy_template_continuation_consolidated_v5.md`, particularly sections 5, 6 and 11. Its architecture remains a draft.
- SRS sections 4.3.1 and 4.3.2 supplied in the current request.
- Accepted constraints supplied with the discussion: TCP may split at any byte, including headers and across TLS records; no new TCP out-of-order/gap support; no automatic expansion of TCP overlap/retransmission policy; no runtime allocations or capacity growth; maximum CH size remains undecided.

Code line numbers below refer to the aggregate text file, not original source-file line numbers. CODE FACT describes the supplied implementation. PROPOSAL describes this document's design. RFC facts constrain wire interpretation but do not prescribe our class structure.

### 2.2 Focused code findings

| Anchor in aggregate | CODE FACT | Consequence for O01 |
| --- | --- | --- |
| `Processor::beginPacket`, L5410; `resetStepState`, L5458 | Packet entry and current-result reset are separate operations; some entry paths return before the reset | CH continuation cannot depend on arbitrary last-session processor scratch; explicit context selection and current-result validity are required |
| `Processor::processSegmentedClientHelloContinuation`, L5780 | Existing continuation resumes extension work and later finalizes the digest | Reuse region-level continuation rather than inventing per-byte FP processing |
| `Processor::feedExtensionsFP`, L6575 | FP feeding emits canonical delimiters, extension types and selected content | Accepted input bytes are not the same quantity as bytes supplied to the digest |
| `Processor::processCiphers`, L6776 | Cipher parsing, de-GREASE normalization and digest appends occur together | Invoke the canonical contribution once after the required cipher region is available; do not casually change the loop or normalization |
| `Processor::processClientHello`, L6810 | Existing handling distinguishes incomplete prefix/body/extensions; some record-level fragmentation returns Unsupported | Arbitrary TCP splitting and cross-record CH support are approved changes, not already implemented capabilities |
| `TlsHandshakeParser::parse`, L8501 | Handshake parsing is separate from body parsing | Shared logical input includes the header, but the body parser must not receive that header as body bytes |
| `ClientHelloParser::parse`, L8544 | Input is a body pointer, a uint16 buffer length, a uint32 expected length and a uint16 consumed output | Existing consumed is not the new accepted-byte contract; adapting widths and partial-prefix handling requires implementation work |
| `extensionsSortingComparison`, L4787 | Ordering considers normalized type, length and potentially value bytes | Retain locator backing through sorting, including bytes needed by comparison, not only content eventually included in FP |
| `TLSFlowConductor::analyzeProcessResult`, L3514; `onClientHello`, L3605 | Conductor handles verdict reduction and callback dispatch | Shared interpretation must not commit TLSData, dispatch listeners or manufacture enforcement verdicts |

No existing function is claimed to implement the proposed API unchanged. In particular, the existing body parser is not assumed to be safely resumable at every byte merely because it returns incomplete statuses.

### 2.3 RFC basis

- [RFC 8446 section 4](https://www.rfc-editor.org/rfc/rfc8446.html#section-4): handshake messages have a type and uint24 body length. ClientHello is type 1.
- [RFC 8446 section 4.1.2](https://www.rfc-editor.org/rfc/rfc8446.html#section-4.1.2) and [section 4.2.1](https://www.rfc-editor.org/rfc/rfc8446.html#section-4.2.1): ClientHello layout and supported_versions; legacy_version alone does not establish TLS 1.3 support.
- [RFC 9000 section 12.2](https://www.rfc-editor.org/rfc/rfc9000.html#section-12.2): multiple QUIC packets can be coalesced in one datagram.
- [RFC 9000 section 19.6](https://www.rfc-editor.org/rfc/rfc9000.html#section-19.6): CRYPTO carries offset-addressed cryptographic data; its frame boundaries are not TLS-message boundaries.
- [RFC 9000 section 13.3](https://www.rfc-editor.org/rfc/rfc9000.html#section-13.3): information can be retransmitted in new frames and packets. Packet identity alone is not sufficient for logical-byte deduplication.
- [RFC 9001 sections 4 and 4.1.3](https://www.rfc-editor.org/rfc/rfc9001.html#section-4.1.3): QUIC carries TLS handshake bytes without TLS records; incremental processing and buffering are both permitted. This is protocol support for the boundary, not a requirement to embed a full TLS endpoint here.
- [RFC 9001 section 5.4](https://www.rfc-editor.org/rfc/rfc9001.html#section-5.4): the low four bits of a long-header first byte are protected. Long-header packet-type bits are not among those four bits. Packet number and its encoded length require the appropriate protection handling.

## 3. Responsibility boundary

### Protocol-specific acquisition

TCP removes TLS record headers, tracks record boundaries and existing sequence policy, and offers consecutive handshake bytes across permitted records. A record boundary is not CH completion. QUIC validates supported v1 framing, removes Initial protection, verifies AEAD before admitting plaintext, walks frames and reconstructs the relevant Initial CRYPTO ranges. It offers only the next consecutive bytes for the selected CH. AEAD success on Initial does not authenticate peer identity.

QUIC out-of-order storage, duplicate ranges, conflicting overlaps, attempt association, Retry-compatible restarts and HRR-compatible CH2 remain acquisition/lifecycle responsibilities. This document does not replace their accepted policies. No merge or reset is authorized merely because bytes resemble a CH. CH2 is a separate message context in the same permitted attempt, not a replay of CH1.

### Shared interpretation

Interpret header/body boundaries, parse CH fields, preserve the existing canonical FP algorithm, maintain continuation and expose current results through existing viewer contracts. No TCP sequence, QUIC packet number, CRYPTO offset, session policy or transport framing enters the shared parser.

### Conductor and protocol policy

Respect feedback barriers, run QUIC applicability before consumer exposure, apply the existing callback/verdict contract and perform permitted TLSData commits. Shared completion is not publication, attempt admission, handshake completion or permission to continue past a pending callback.

## 4. Concrete logical API

The names below are proposed internal operation/type names. They define inputs, outputs and behavior without prescribing C++ ownership placement, inheritance or a new interface class. A context may be embedded in existing prepared storage. Three lifecycle operations do not imply three virtual hooks.

### 4.1 Types

| Type/name | Representation and meaning |
| --- | --- |
| `CHContext` | Mutable reference to one live logical message's prepared state/backing. Not TLSData. Its association with session/attempt/message is established by the caller and must remain stable |
| `CHRegion.data` | Borrowed read-only byte pointer, equivalent to `const uint8_t*`; valid for the duration of the call |
| `CHRegion.length` | Proposed uint32 byte count of the offered contiguous region; zero permits a null pointer, nonzero does not |
| `CHRegion.messageOffset` | Proposed uint32 offset from this CH's handshake type byte, not from the TCP payload or QUIC CRYPTO stream |
| `CHAdvanceResult.accepted` | Proposed uint32 count of bytes accepted from this call's region, always between zero and length |
| `CHAdvanceResult.status` | `NeedInput`, `Complete`, `ParseFailure`, `ResourceFailure`, or `ContractFailure` |
| `CHAdvanceResult` diagnostic | Existing parsing-error vocabulary where applicable; an internal contract diagnostic for invalid invocation. Not a violation mask or enforcement verdict |
| `CHCheckpoint` | Internal phase plus the continuation data in section 6. Not an event and not a new public viewer status |

The uint32 proposal covers the TLS uint24 body extent plus header without choosing a supported maximum. QUIC's wider CRYPTO offsets stay in acquisition; conversion to a CH-relative offset is checked. No unchecked narrowing into existing uint16 parser APIs is permitted. Numeric capacity and concrete legacy-parser width migration remain O06 implementation gates, not a reason to invent a maximum here.

### 4.2 `startClientHello`

**Input:** a caller-selected `CHContext` with access to prepared backing. For a permitted start, it must not hold a live message; the caller has already selected the new logical message and ensured previous results are no longer in use.

**Output:** proposed start status `ReadyToAccept`, `ResourceFailure` or `ContractFailure`. An attempt to restart a still-live context is ContractFailure with no mutation: preserve its existing state, result, backing and validity of existing views; no new CH result is created. Release/authorized retirement must occur before a permitted start. On success the phase is Header, accepted offset is zero, previous facts/findings/locators/current-result validity are cleared, and no canonical contribution is marked fed.

**Behavior:** start exactly one new message. This operation does not reset TLSData or decide whether a new attempt is admissible. It does not allocate. A failed start on a context that does not hold a live message does not create a valid CH result. This does not invalidate an existing result or its views when a start on a still-live context is rejected as specified above. Do not call it at every packet, record, frame or resume.

### 4.3 `advanceClientHello`

**Input:** mutable `CHContext` and `CHRegion` containing only the next in-order logical handshake bytes, limited to the current transport feedback boundary.

**Output:** `CHAdvanceResult`; parsed facts and eventual FP remain in the context's existing viewer backing, not copied into the result.

**Behavior:** validate invocation, accept the region's prefix as far as possible, advance complete checkpoints and return at message completion, exhausted offered input or failure. One call may traverse several checkpoints. There is no mandatory return after each field. Internal buffered work is completed before returning NeedInput.

| Status | Exact meaning | Caller action |
| --- | --- | --- |
| NeedInput | Current offered region is exhausted and this CH needs more bytes | Obtain the next eligible region. It may be in the same packet/datagram; do not automatically report public NeedMoreData |
| Complete | Full declared CH was structurally interpreted and canonical FP finalized successfully | Apply protocol applicability and the existing publication/feedback path before further semantic work |
| ParseFailure | A blocking structural/parser failure prevents this CH from completing | Do not resume or publish this context; yield the appropriate existing exceptional path |
| ResourceFailure | Required prepared storage, locator or digest/FullString capacity cannot support this CH | Do not allocate, grow or retry a partially modified digest; suppress successful publication and follow resource policy |
| ContractFailure | Wrong offset, invalid pointer or invalid context lifecycle | No successful result; treat as an integration defect, not a peer violation |

For valid nonterminal input, NeedInput accepts the entire offered region. Complete can accept less than offered if trailing bytes belong to another message. On ParseFailure/ResourceFailure, accepted reports only the prefix actually secured or interpreted before failure; the context is nonresumable regardless of that count. Invalid invocation is checked before mutation and returns accepted zero. No successful-looking partial FP survives a failure.

Calling advance on a completed context with an empty region at its final offset returns Complete and accepted zero without re-finalization. Supplying nonempty input to that completed context is ContractFailure. The caller must not interpret repeated Complete as a new event. A failed or released context cannot resume.

### 4.4 `releaseClientHello`

**Input:** context for which **all applicable callbacks, verdict reduction, permitted commit and every final internal read have ended**, or an abandoned context with no remaining readers or uses. **The last callback returning is not sufficient while reduction, commit or an internal read remains pending.**

**Output:** no CH result; context becomes Released and its views invalid.

**Behavior:** clear associations and return owned prepared resources through the existing lifecycle. No new reclaim mechanism, parallel aging scheme or heap fallback. Release does not itself classify a violation or reset semantic session state. Final physical pool handoff remains O05.

## 5. Byte accounting

Let A be the cumulative accepted CH-relative offset before a call and N the offered length.

1. For a live context, the offered messageOffset must equal A. The shared boundary does not accept overlap/replay or gaps.
2. On return with accepted K, the new frontier is A + K, checked for overflow and declared-message bounds. The caller advances its own region cursor by K, not by a legacy parser's consumed output.
3. Every accepted byte is either no longer needed in raw form or represented in valid backing/facts. A pointer into a packet or decrypt scratch that expires after the call is not retention.
4. Bytes not accepted remain the caller's responsibility. Accepting bytes does not transfer ownership of the caller's original buffer.
5. Parsing may revisit a retained region through a side-effect-free parser, but FP contributions must never be replayed. Prefer parsing a required complete region once.
6. A raw-byte 'hashed offset' is deliberately absent. FP normalizes values, inserts delimiters, selects content and sorts extensions. Track completed canonical contributions instead.

Retained bytes and interpreted bytes are not necessarily disjoint: parsed extension values can still be required for sorting or viewer access. Therefore do not assert that accepted equals parsed plus retained, or free bytes solely because parsing advanced.

Deduplication occurs before this API. QUIC may receive the same CRYPTO data under a different packet number or split into different frames. Offer only previously unaccepted consecutive bytes. A frame containing both duplicate and new data may still advance CH. Conflicting bytes do not become harmless duplicates and must not silently replace accepted data. FP equality is not message identity.

## 6. Continuation representation and region strategy

### 6.1 Minimum checkpoint representation

| Phase | State retained | Work allowed once sufficient input exists |
| --- | --- | --- |
| Header | Up to four collected header bytes and count; accepted frontier | Verify selected type, decode uint24 length, establish exact message end |
| Prefix | Prefix collection progress, current length-field/vector boundary, declared body end and stable backing | Safely locate complete fixed fields/session ID/ciphers/compressions; parse the complete required prefix and perform canonical base/cipher contribution once |
| Extensions | Declared extension extent and collected count; prefix facts; live digest/FullString state; extension backing | Once complete, parse/extract using existing rules, build/sort locators and feed the canonical extension contribution once |
| Ready | Complete immutable current CH facts, finalized FP and live backing | No further parsing, feeding or finalization; protocol applicability/publication remains outside |
| Failed | Failure category and diagnostic; invalid successful-result flag | No continuation or successful publication |
| Released | No live result association | A subsequent explicit start may reuse the prepared storage |

Phase advancement encodes 'prefix contribution already fed' and 'extensions contribution already fed'. Do not add redundant booleans for every field unless implementation requires them. Header count, collection counts and validated region extents are real continuation data, not merely an enum.

### 6.2 KISS collection policy

Collect only the region needed for the next reusable operation. A small length-aware collector must distinguish missing bytes from a length that exceeds the declared body. Once the declared body end makes a required field impossible, report ParseFailure rather than waiting indefinitely for bytes beyond that end. It is not a second independent CH semantic parser and must not duplicate normalization or FP rules.

For Prefix, retain the complete prefix through compression methods so the existing body-layout logic can be adapted safely. Read vector length fields only when available; bound each vector against the declared body. A split uint16 length or a split cipher element is ordinary incompleteness, not a reason to invoke processCiphers on half a vector. The existing ClientHelloParser requires an adapter/refactor to expose this safe prefix boundary; do not repeatedly call it on arbitrary fragments and reinterpret every Failure as NeedInput.

If body end follows compression methods, preserve the existing supported TCP legacy case. Still emit the existing canonical empty-extension representation once before finalization; absence of extensions does not authorize omitting FP delimiters. The same rule applies to an explicitly empty block where accepted by the applicable parser contract. If bytes remain, the Extensions checkpoint first collects the two-byte extension-block length field, retaining its partial-byte count before the block extent is known and validate the declared extension extent against body end using the applicable parser contract. QUIC applicability is separate. Do not impose a new universal TLS 1.3 acceptance rule on TCP or silently tighten malformed legacy cases; O03 owns migration/error-timing equivalence.

For Extensions, collect the complete declared block before existing parsing, sorting and FP feeding. Keep value backing live through comparisons and all viewer reads. The existing comparator can inspect values even where FP content inclusion is selective. Do not hash extensions in arrival order or change extension 57 treatment for QUIC.

Contiguous complete regions may be read directly during a call if their lifetime is sufficient. Anything needed afterward must be retained in prepared backing or protected by an existing proven lifetime contract. This does not mandate a contiguous full-CH buffer or a new zero-copy lease API. Physical storage sharing and pool layout are not decided by O01.

### 6.3 Digest discipline

Initialize the canonical digest/FullString state once for a message before its first canonical contribution. Preserve existing order, delimiters, normalization, selected extension contents and finalization semantics. Feed base/ciphers once, extensions once after canonical ordering, then finalize once. Do not replace this with a TLS transcript hash.

If a feed fails after partially modifying digest/FullString state, fail the context. No rollback or restart-on-the-same-context is promised. A new evaluation requires a separately authorized fresh context, not an automatic replay of a published CH. FullString's existing API and semantics remain unchanged.

## 7. Representative flows and exact accounting

These are design walkthroughs, not executed packet tests. Numbers below describe fragment lengths, not capacity choices. Offsets are CH-relative.

### E01: Header split 2 + 2

- Start: A=0, Header, no FP contribution.
- Offer offset 0, length 2: accepted 2, A=2, NeedInput; retain two header bytes, no digest feed.
- Offer offset 2, length 2: accepted 2, A=4; decode complete header, enter Prefix, NeedInput if no body bytes were offered.
- The body parser's first byte is logical offset 4, never the handshake type byte.

### E02: Prefix/cipher vector split

Let P be the CH-relative end of the prefix through compression methods. Suppose the first region ends inside a cipher element.

- Accept the available prefix bytes into valid prepared backing. Keep Prefix; no processCiphers call on a partial vector.
- The next region begins at A, not at the cipher vector's original start. Collect through P.
- Interpret the complete required prefix, feed the existing base/cipher contribution once and advance to Extensions if an extension block follows.
- Later extension fragments do not reinitialize the digest or re-feed ciphers. Existing raw-byte counts and canonical contribution progress remain distinct.

### E03: Extension header/value split

Let E be the extension-block start and L its declared length. A region supplies only the first byte of an extension type, or ends inside its value.

- Accept those bytes into extension backing; remain Extensions. Do not feed a partial extension or treat an incomplete type/length as a structural error.
- When the complete block [E,E+L) is available, parse, sort and feed once. Preserve backing for locators and viewer fields.
- Finalize only after full declared CH validation. Earlier accepted extension bytes do not imply any extension contribution was already fed.

### E04: QUIC gap and mixed retransmission

- CRYPTO [0,40) is accepted: A=40.
- CRYPTO [80,120) arrives: acquisition retains it; no shared call, A stays 40.
- CRYPTO [20,80) arrives: acquisition handles duplicate [20,40), then offers new [40,80). After accepting it, acquisition can offer retained [80,120), subject to feedback barriers.
- No shared replay of [0,40), no repeated canonical contribution and no automatic new CH event.

### E05: TCP cross-record CH

A CH starts in one handshake record and continues in another; either record header may itself span segments. TCP acquisition reconstructs record headers and excludes them from CH regions. The shared A continues across the record boundary. Existing record callbacks/errors and Conductor feedback still occur at their required boundaries. This is the approved TCP capability extension, not proof that existing code already supports it.

### E06: Complete CH plus trailing bytes

A region contains the remaining R bytes of this CH plus T bytes of a following handshake message. Return accepted R and Complete, leaving T to acquisition. Apply applicability and Conductor feedback before selecting or processing a subsequent message. Do not assume one CH per datagram or one CH per DP session.

### E07: Interleaved sessions

Session A pauses in Prefix; session B completes a CH; session A resumes. A's checkpoint, retained bytes and digest state must still belong to A. Per-Engine reusable scratch is not durable continuation. Re-selecting A must not expose B's viewer facts. No new context-identity field or registry is mandated; the ownership invariant is mandatory.

### E08: Completion time and nonblocking finding

QUIC datagram D supplies the last missing bytes, making the whole CH available. Preserve D's arrival timestamp as clientHelloTimeStamp, even if retained bytes from earlier datagrams are interpreted afterward. A well-formed CH that does not offer the required TLS version still completes parsing/FP; QUIC applicability attaches the nonblocking finding before exposure. No automatic violation, drop or Error state is created here.

## 8. Conductor integration and exceptional outcomes

The protocol path must not cross an existing feedback barrier merely because advance can accept more bytes. Limit the offered region to that barrier. NeedInput is local; only the outer processing loop can decide that no further safe work exists in the current input.

Ready means shared interpretation succeeded, not that the message is admitted or published. QUIC applicability is performed before consumer exposure and stays in the same feedback unit as the CH. The existing viewer exposes only the correct current result with its permitted partial/complete status. No partial state is relabeled Complete; no previous session's complete result is reused as a fresh result.

The Conductor retains callback ordering, verdict reduction, permitted TLSData writes and stop behavior. A blocking parser finding stops the affected parsing path, but it is not itself an enforcement verdict. A nonblocking version finding does not prevent FP. Resource exhaustion/eviction does not create Incomplete Handshake violation. A callback Stop must be honored before the next semantic unit. No new Finished trigger is selected.

**Result/backing and its views must remain valid through all applicable callbacks (including nested viewer reads), verdict reduction, permitted commit and every final internal read. Release, invalidation or reuse is permitted only after all such uses have ended.** The existing endPacket/release/reclaim lifecycle must respect this ordering; callback return alone is not release authority. Ending an input does not release continuation or retained CH backing still required by the existing lifecycle. Candidate evaluation cannot overwrite the active context merely because this API takes a mutable reference; admission/ownership handoff remains O05.

## 9. SRS traceability and justified interpretation

| SRS requirement | Treatment within Processor + Conductor |
| --- | --- |
| UDP datagrams can contain several QUIC packets; packets contain frames | Acquisition iterates actual packet/frame boundaries; shared regions are not datagrams or frames |
| Handshake bytes are carried directly in CRYPTO, not TLS records | Shared input contains handshake header/body only; no synthetic TLS record |
| CH can span CRYPTO frames and Initial packets | Acquisition reconstructs ranges; shared context continues across regions and datagrams |
| Destination port belongs to QUIC application port group | Existing admission/subscription boundary must ensure eligibility before this processing; O01 does not add a configuration subsystem |
| Initial header 0xC0 | Do not compare the whole protected first byte to 0xC0. For the baseline v1 layout, the high nibble identifies the expected form/type/fixed-bit pattern; also validate version, structure and bounds. The low nibble varies under header protection. This is a justified technical interpretation, not a literal equality implementation |
| CRYPTO type 0x06; handshake type 0x01 | Frame type is decoded in acquisition after Initial deprotection; shared Header verifies selected ClientHello type at the actual handshake boundary, not by scanning arbitrary payload bytes |
| TLS 1.3 and newer; lower versions are a violation | Evaluate supported_versions, not legacy_version alone. Preserve the product enforcement requirement outside this scope; a well-formed CH continues through FP with the agreed finding. Unknown/GREASE values are not proof of a supported future TLS version |
| TLSFP calculated the same way as TCP | Reuse canonical base/cipher/extension algorithm, ordering and normalization; no QUIC-only fingerprint variant |
| Do not calculate TLSFP for UDP retransmissions | Deduplicate logical CRYPTO data before shared acceptance; do not skip new bytes merely because some ranges repeat. Once a message is complete, repeated transport data does not start a new evaluation |

Statistics and TOP TLSFP remain external integration requirements; they are not designed or implemented here.

## 10. Static double-check and remaining gates

### 10.1 Review performed

A second focused consistency pass checked the proposed API against the code anchors, V5 O01 definition, RFC framing and the supplied SRS. The following are document-level conclusions, not runtime test results:

- Header/body offset convention is explicit and does not misfeed the existing body parser.
- accepted includes safely retained bytes; parser consumed and canonical FP progress are not conflated.
- All three requested checkpoints have concrete continuation data and representative flows.
- NeedInput cannot hide internal work or force waiting for another datagram when safe bytes already exist.
- Canonical extension ordering is preserved; no arrival-order hashing or implicit per-byte digest frontier.
- Complete stops before trailing-message processing; duplicate transport data does not generate a fresh event.
- Failure after partial digest modification is nonresumable; no imaginary rollback contract.
- Session interleaving, pointer lifetime, feedback and applicability are explicit.
- No base class, virtual hook list, new public viewer, numeric capacity, runtime allocation or automatic enforcement behavior is approved.

V3 corrects the local startClientHello consistency defect in section 4.2: the permitted-start input no longer requires a live message; rejection of a still-live context preserves its existing state, result, backing and view validity without creating a new result; a failed start on a non-live context creates no valid CH result. The focused double-check covered these cases against sections 4.3, 4.4, 6.1 and 8, and verified that all text outside the explicitly listed edits is unchanged from V2. No blocking contradiction was identified in this focused correction review. This is document-consistency evidence, not a new independent verification of code or RFC behavior. Existing TCP malformed-input/error-timing equivalence and allocation-free implementation are not proven.

The final consistency pass also made the empty-extension FP path, split extension-block length field, impossible declared body end and live-context restart precondition explicit. V1 remains an earlier baseline; the supplied V2 and V3 are preserved unchanged; V4 is the delivery copy. The V3 change is a local contract clarification, not a new lifecycle mechanism, two-CH-per-packet check, ownership decision or condition derived from that check. O02 may proceed with the existing deferred gates unchanged.

### 10.2 Closure statement

**O01 deliverable is complete as a concrete proposed contract:** inputs/outputs, exact accepted-byte semantics, logical checkpoint representation, region collection policy and representative flows are specified. It is ready for user/design review and for the focused O02 comparison. It is not an accepted implementation API merely because the analysis is complete.

O02 remains open. O03 retains TCP record/error/callback timing and malformed-input migration checks. O04 retains public viewer integration checks. O05 retains candidate ownership/admission and physical handoff. O06 retains maximum CH/capacities and concrete width/storage acceptance. O07 retains allocation-free provider/runtime evidence. O08 retains completion-signal selection. None is silently closed by this document.

No C++ build, unit test, packet execution, performance measurement or runtime allocation audit was performed. Source and previous document snapshots were not modified.

### 10.3 V4 lifetime correction and restarted static review

V4 clarifies sections 4.4 and 8: all applicable callbacks, verdict reduction, permitted commit and every final internal read precede release, invalidation or reuse. Callback return alone is insufficient. This is an ordering correction, not a new API, ownership mechanism or reclaim path. V3 and Consolidated V5 remain unchanged.

The document-level review was restarted from the authority/scope boundary and covered the following in order:

| Check | Evidence and static conclusion |
| --- | --- |
| Scope and authority | Sections 1-3 and 10.2 preserve proposal status, protocol acquisition, Conductor authority and deferred O02-O08 gates. No implementation approval is inferred. |
| Lifecycle invocation | Sections 4.2-4.4 and 6.1 distinguish permitted start, rejected live-context restart without mutation, completion, failure and release. Sections 4.4 and 8 now use the full last-use boundary from Consolidated V5 CP06-CP08. |
| Byte accounting | Sections 4.3 and 5 distinguish offered, accepted/retained and canonical FP contributions. NeedInput exhausts valid offered input; Complete leaves trailing bytes; invalid invocation accepts zero before mutation; parser/resource failures cannot resume. |
| Checkpoints and bounds | Sections 6.1-6.3 cover split headers/vector lengths, impossible body extents, prefix collection, empty extensions, locator backing, one-time canonical feeds and finalization. Numeric capacities and concrete width migration remain O06. |
| Representative flows | E01-E08 remain consistent with sections 4-6: partial and complete input, gaps/duplicates, cross-record feedback, trailing messages, session interleaving and completing-datagram time. These are static walkthroughs, not executed tests. |
| Feedback and lifetime | Sections 3, 7 and 8 preserve applicability before exposure, callback barriers, actual-verdict-only semantics, isolated candidates and data validity through commit/final reads. No resource failure manufactures a violation or Finished trigger. |
| O02 readiness | Section 10.2 and Consolidated V5 section 11 require a concrete O01 proposal before comparing orchestration alternatives. O01 supplies that input without selecting inheritance, hooks or physical ownership. No further blocking document inconsistency was identified in this restarted review. |

**Review disposition: ready to proceed to the focused O02 design comparison.** This is a static design-review judgment only. O02 is not selected, O03-O08 remain open, and no code build, packet execution, runtime allocation proof or performance PASS is claimed. The correction does not independently revalidate RFC facts or all inherited code behavior.
