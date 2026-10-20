# O07: Provider Fit and Allocation-Free Runtime Review V5

**Status: O07 ANALYSIS COMPLETE; O08 V3 COMPLETION POLICY SELECTED. The focused O07 design-progression review is complete under the explicit contracts and downstream obligations below. Absence of future QUIC code or deployed-provider evidence is not a design blocker. No implementation, allocation-free runtime, performance, migration or activation PASS is claimed.**

V3 corrects the stage boundary in V2: we are designing the QUIC extension, not accepting an existing QUIC implementation. Existing TCP code supplies preservation facts; future QUIC behavior is specified as design obligations. Provider selection remains an explicit implementation-design dependency before crypto API/layout freeze, while linked-build evidence and instrumentation belong to later implementation/activation validation. None is silently waived. The supplied code, O06 and all earlier revisions remain unchanged. No numeric capacities, new architecture, viewer, manager or reclaim mechanism are selected.

## 1. Authority, evidence and scope

Direct inputs: `TLS-Flow-All-Code.txt`, `tls_flow_o06_capacity_width_joint_peak_review_v1.md`, and the user's pasted SRS section 4.3.1. O01 V4, O02 V2, O03 V7, O04 V2, O05 V2 and Consolidated V6 are available locally; targeted comparison of their O07 handoff/constraints was performed. O06 remains the immediate inherited contract. No fresh full FRD/SRS or external-consumer audit is claimed. Surfaced historical file metadata is not proof that those files exist locally.

Evidence labels:
- **CODE FACT:** visible source declaration, operation or branch, identified by embedded file and aggregate line anchor.
- **INHERITED CONTRACT:** constraints recorded by O06 and prior accepted decisions; not implementation evidence.
- **SRS INPUT:** the supplied section 4.3.1, not the complete SRS.
- **PROTOCOL FACT:** RFC grounding, cited in section 12; not authority to silently rewrite product requirements.
- **DESIGN JUDGMENT / PROPOSAL:** recommended implementation direction, not an accepted API/provider choice.
- **EVIDENCE GAP:** unavailable evidence, classified by the stage that needs it; absence of not-yet-written QUIC code is expected at this design stage, not a failed design gate.
- **PLANNED VALIDATION:** not executed against the Engine.

Aggregate anchors refer to physical lines in the supplied text, including blank lines, not the office viewer's reformatted lines. V3 source identities and preservation checks are recorded in section 14; earlier manifests describe their own immutable deliveries. The code hash matches O06's recorded code snapshot. Mixed CRLF/LF line endings must not be normalized before hashing.

### 1.1 Retained boundaries

Startup allocation/preparation is permitted. All affected Engine runtime, including packet processing, new-session work, callbacks, failure/diagnostics, visibility, aging, release and reuse, must use prepared storage without dynamic allocation or growth. No heap fallback. Fixed-array declarations alone do not prove transitive compliance.

Retain one active TLSData semantic context, isolated candidates, explicit accepted/rejected/deferred disposition, retired suppression, no ambiguity merge/reset, A/B/A isolation and backing through full last use. Full last use includes callbacks, verdict reduction, permitted commit and final internal reads. Resource failures do not create violations, automatic Error/drop, Finished or IncompleteHandshake violations. TLS-Enforcement owns violations. Preserve TCP D04 site-specific error/list effects and C-O03-01 deferred cross-record publication. D-TIME remains the completing datagram's arrival timestamp. O08 V3 selects the accepted completion heuristic. After successful completion/publication and permitted commit of the active attempt's CH acquired from eligible Initial CRYPTO data, the attempt can be InProgress. Only a subsequently observed, structurally qualifying, associated client Handshake packet can then move InProgress -> Finished, including ACK-only. A Handshake observed before complete authoritative CH does not establish InProgress or Finished and is not latched for later completion. No Handshake payload decryption or TLS Finished detection is required. Finished is the accepted product heuristic, not cryptographic handshake verification.

O06 P01-P16 remain unselected where open. No maximum CH, CPU/RAM budget, pool size, new timeout, new manager/reclaimer or DTLS framework is introduced.

## 2. Executive findings

| ID | Finding | Disposition |
|---|---|---|
| F01 | The supplied implementation contains a buffered TLSFP MD5 wrapper, not a QUIC Initial decrypt/KDF/header-protection implementation | Expected implementation delta. Design operations are specified in section 4; actual provider/build verification is future V-G01, not a progression blocker |
| F02 | `Md5Digest::calculate` copies canonical bytes; `finalize` invokes external `MD5` and returns true unconditionally | Wrapper behavior established; linked crypto behavior and failure propagation not proved |
| F03 | FullString borrows the digest's canonical byte array | Streaming-only replacement is insufficient; preserve exact bytes and lifetime |
| F04 | Runtime callback boundaries include `std::function`; fixed containers and SessionTable/IPC/logging internals are absent | Potential allocation boundaries, not proven allocations or proven allocation-free paths |
| F05 | Sort compares extension contents; diagnostic byte loops, copies, repeated input and crypto work require separate accounting | No CPU/performance PASS from finite memory |
| F06 | SRS `0xC0` cannot safely mean equality against the entire first byte | Record a requirement interpretation conflict; use version-aware field interpretation, not magic-byte scanning |
| F07 | SRS TLS-version violation must not become a parser-owned violation or premature parse stop | Preserve full parse/FP for nonblocking version findings; actual enforcement integration remains unverified |

Evidence: F01-F05 are supported by sections 3-8 and source anchors A01-A19. F06 is grounded in RFC 9000 section 17.2.2 and RFC 9001 section 5.4.1. F07 follows SRS input together with O06 section 8's inherited enforcement boundary.

## 3. Actual digest/provider path

### 3.1 What exists

**CODE FACT:** Engine Factory `buildProcessor` creates the digest and parsers with `make_unique`, initializes IParsersContainer, then creates Processor with `make_shared` and calls its init (A01). Processor init creates SegmentBufferManager (A02). These are visible initialization call sites, not packet-path allocations. Their startup reachability and failure handling must be verified in the integrated deployment; the snapshot is not a complete Engine boot trace.

IParsersContainer owns a concrete `unique_ptr<Md5Digest>`. Processor's current digest pointer selects an embedded digest in the initial SegmentBuffer or a retained SegmentBuffer. This is not a generic pluggable provider contract. No new provider interface is required merely to document the responsibility.

**CODE FACT, A03-A05:**
- `init` resets only the used-byte index and returns true. It does not initialize a streaming hash context or clear the entire array.
- `calculate` rejects zero-length input and rejects an aggregate append ending at or beyond the 2870-byte array extent. Otherwise it copies bytes and advances the index. For a valid index and nonoverflowing arithmetic, at most 2869 bytes are usable in the existing wrapper.
- The guard forms `len + index` before comparing. Arbitrary size_t inputs are not protected against addition overflow. Existing narrow callers are not authority for an unrestricted replacement contract.
- `finalize` calls `MD5(array, usedLength, output)` and returns true without checking the external call's result. The two visible Processor completion sites ignore the wrapper's bool result (A06).
- `copy` clamps copied bytes but assigns the original source index. It is safe only under the source-index invariant; the clamp does not establish that invariant (A05).

**EVIDENCE GAP:** the exact linked MD5 implementation, version, build flags, runtime dispatch, initialization and error behavior are not in the supplied code. No claim is made that the external MD5 call allocates, or that it is allocation-free. A development-machine OpenSSL version would not establish the deployed Engine linkage.

### 3.2 Canonical stream is part of the observable contract

**CODE FACT:** version is wrapped in parentheses; ciphers are emitted in order with GREASE normalization; extensions are located, sorted, then emitted inside square brackets. Every extension contributes type and parentheses; selected extensions also contribute length and content. Supported groups/versions use specialized feeds. `quic_transport_parameters` and the draft transport-parameters extension are included in the selected-content switch (A07-A09).

For a successful ordinary current path with N cipher suites, M extensions and the cipher parentheses emitted, the canonical length is:

**C = 8 + 2N + 4M + sum over content-included extensions of (2 + emitted content length).**

This is a derived accounting identity for the visible append sequence, not a maximum CH limit or an oracle for malformed inputs. The 8 comprises version parentheses/value (4), cipher parentheses (2), and extension brackets (2). General selected content uses the locator length. Specialized groups/versions use their actual emitted inner length and normalized entries. Prove inner-vector validation before equating those contributions with the entire locator length. Existing zero-length selected general content reaches a zero append that fails; do not silently change that legacy behavior under a provider refactor.

**CODE FACT:** `getFullString` returns the current digest array pointer and index (A10). It is a borrowed byte view, not a string allocation or ownership transfer. Segmented completion finalizes, copies the digest to the initial segment, then finishes/releases continuation before reporting Complete (A06). This is current code behavior, not proof of the future candidate/publication lifetime.

**INHERITED CONTRACT:** preserve the exact canonical byte stream, digest bytes and FullString semantics. Do not change MD5 to another fingerprint, truncate selected content, remove QUIC transport parameters, change sorting/GREASE behavior or publish partial success to fit storage. Retain canonical backing through full last use; no reuse on input end alone.

### 3.3 Minimal alternatives, without premature provider selection

| Option | Benefit | Cost / unresolved proof | Judgment |
|---|---|---|---|
| A. Retain canonical bytes and hash them once at completion | Matches current FullString and copyable retained state; no streaming context per partial CH | Finalization scans C bytes; exact linked hash behavior and adequate B_canon still need proof | Preferred working design direction for KISS; actual provider fit must be verified before implementation acceptance, not before O08 analysis |
| B. Retain canonical bytes and also maintain streaming MD5 state | Can distribute hash work if canonical contributions become available incrementally | Still needs FullString storage; adds mutable hash state, lifecycle/failure handling and possible copy/reset constraints | Only if a concrete correctness or measured runtime need justifies it |
| C. Streaming MD5 only, discard canonical bytes | Removes canonical backing | Breaks existing FullString unless an equivalent retained representation and proven reconstruction are supplied | Not acceptable as a drop-in replacement |

These are logical choices, not three new classes. A partial CH need not hold an active crypto work context if its retained canonical state can be hashed later. Conversely, choosing streaming does not prove safe provider-context cloning. Do not memcpy an opaque crypto context or assume `Md5Digest::copy` applies to it.

## 4. QUIC Initial crypto: design contract and later verification

**CODE FACT / EXPECTED TARGET DELTA:** the supplied Factory builds TLS record/handshake/CH/extension parsers and Md5Digest. No QUIC packet/frame parser, Initial KDF, header-protection remover or AEAD decrypt implementation is visible in this snapshot. The occurrence of QUIC transport-parameter extension enums is not such an implementation. The current Conductor process path still takes TCP sequence metadata and drives TLS records; it is not a verified UDP/QUIC path (A11).

**PROTOCOL FACT:** QUIC Initial protection uses the Initial-secret derivation and packet/header protection specified in RFC 9001 sections 5.2-5.4. Initial protection uses AES-128-GCM; the Initial KDF uses the specified SHA-256-based derivation. These operations are separate from TLSFP MD5. A fingerprint digest cannot replace decrypt/authentication or key derivation. See R1.

**DESIGN CONTRACT:** acquisition performs Initial derivation, header-protection removal, decoded packet-number/nonce/AAD construction and AEAD verification before frame/CRYPTO interpretation. These are logical operations, not a demand for new classes or public APIs. The actual eligible association supplies the derivation context; no shared mutable scratch is retained as candidate/CH state. Provider errors take the qualified failure path without successful publication or fabricated enforcement. Startup prepares the required bounded work storage; runtime derives actual input-dependent keys using that storage.

**Provider choice:** no concrete library, version, provider or API is selected by this correction. Before crypto API and storage layout are frozen, compare the actual candidate API's startup/reset/rekey/error and work-storage contracts against these requirements. An identified incompatibility would be a concrete design issue requiring resolution. Lack of a deployed QUIC build is not such an incompatibility. O08 completion-policy analysis does not depend on choosing a crypto provider.

**Design preparation and future verification worksheet**

| Operation | Startup preparation | Future implementation verification (not a current design gate) |
|---|---|---|
| Algorithm/provider selection | Resolve exact algorithms, build/linkage, permitted dispatch and required work storage | No lazy fetch/load/first-use allocation; no unsupported algorithm substitution |
| Initial derivation | Prepare any reusable KDF/hash work state and bounded key/output storage | Derive for the actual eligible attempt/DCID at runtime without allocation; changing identities cannot grow a cache |
| Header protection | Prepare work state and fixed sample/mask storage | Validate sample bounds within the current QUIC packet, remove protection before using protected fields; no neighboring-packet borrowing |
| Packet number / nonce / AAD | Prepare required scalar and byte storage | Use decoded validated fields and accepted association; no speculative narrowing or mutation of another attempt |
| AEAD decrypt/authenticate | Prepare work state and bounded plaintext/output scratch | Authenticate before releasing bytes to frame/CH parsing; failed authentication cannot publish tentative plaintext |
| Reuse/reset/rekey | Prepare enough contexts for actual overlapping use | Prove every rekey/reset/update/final/error path allocation-free, not just context creation |
| Error/cleanup | Prepare bounded diagnostics/error storage where needed | No allocating error strings, provider error formatting, lazy recovery context or hidden cleanup allocation |

Preparing contexts at startup does not precompute all future DCID-derived keys. Dynamic input values still require runtime crypto operations. Warm-up alone is not proof: a different key, message size, failure branch or reset can take a different provider path.

O06 P15 stays OPEN. Count actual simultaneously borrowed crypto work contexts from the synchronous/nested call graph. Single-threaded execution does not rule out callback re-entry or borrowed scratch. Do not assume one context per Engine, one per session, or one per candidate. A simple sequentially reused Engine work area is preferable only after disjoint lifetimes/non-reentrancy are established. Preserve deferred CH backing independently of ephemeral decrypt scratch.

## 5. SRS 4.3.1 mapping and the 0xC0 issue

The user-supplied section is a current input, so O06's historical statement that SRS was unavailable no longer applies to this excerpt. The complete SRS/FRD and external consumer implementations are still not audited here.

| ID | SRS input | O07 consequence / status |
|---|---|---|
| S01 | QUIC over UDP; filter destination port by QUIC Application Port group | Gate QUIC work using the authoritative application-port configuration; no hard-coded 443. Gate/config implementation not supplied |
| S02 | Several QUIC packets per datagram | Walk validated packet boundaries; scratch reuse only after each packet's last use; no one-datagram/one-packet assumption |
| S03 | One or more frames per packet | Parse frame boundaries in authenticated plaintext; CRYPTO need not be the first frame |
| S04 | Handshake directly over CRYPTO, not TLS records | Feed shared handshake/CH semantics without a synthetic TCP record header or TCP record-version validation |
| S05 | Handshake data split over frames and Initial packets | Retain bounded acquisition across inputs, order by CRYPTO offset, and hash each canonical contribution once; duplicates do not advance CH progress |
| S06 | CH in Initial; find CRYPTO type 0x06 | Decode the frame type and its offset/length; do not search ciphertext or arbitrary plaintext for a byte 0x06 |
| S07 | Handshake type 0x01 | Check at the reconstructed handshake-message boundary; continuation fragments need not begin with 0x01 |
| S08 | TLS 1.3 and newer; lower version is a violation | Preserve full parsing/FP for nonblocking version findings, route enforcement to TLS-Enforcement; do not infer version from legacy_version alone |
| S09 | Check Initial header as 0xC0 | Exact whole-byte equality is not a valid general Initial test; interpretation issue described below |

### 5.1 Exact field distinction, not a blanket protection assumption

**PROTOCOL FACT, R1/R2:** for QUIC v1 long headers, the header-form, fixed bit and long packet-type bits are not masked by header protection. The low four bits, including reserved bits and packet-number length, are masked. Thus a version-validated high-nibble classification of a v1 Initial is possible before header-protection removal. It is not equivalent to validating the packet or reading the protected packet number/length bits.

A v1 Initial has the relevant high-nibble pattern 0xC0. Its entire first byte need not equal 0xC0, either on the wire or after protection removal. Header protection and the packet-number-length encoding make whole-byte equality incorrect. RFC 9001 Appendix A.2 itself distinguishes an unprotected first byte 0xC3 from the example protected 0xC0; the example is not a universal constant.

**DESIGN JUDGMENT:** interpret the SRS wording as a field-pattern intent, subject to explicit requirements reconciliation, not an exact-byte implementation. Validate version and complete packet structure, remove header protection for protected fields, then authenticate payload before frame parsing. If literal equality is demanded, record a concrete SRS/protocol conflict rather than silently implement it.

**Inherited-contract clarification:** any earlier blanket wording that all QUIC packet-type bits are protected is too broad for v1 long headers. This factual correction does not authorize raw protected short-header key-phase/packet-number interpretation, does not prove trusted upstream classification, and does not close O08. Long-header type classification alone is insufficient; O08 V3 accepts an associated client Handshake observation only after complete authoritative CH and while the active attempt is InProgress.

### 5.2 Version and violation authority

S08 does not authorize treating TLS 1.3's legacy_version field as the offered maximum. Use the parsed supported-version information and the accepted QUIC version policy. The current viewer maps known versions into a fixed mask; future TLS-version representation is not automatically implemented by the SRS phrase 'and newer'. Preserve that phrase as a requirement and record any unsupported future-version mapping as an explicit gap, not a guessed violation.

A CH not offering TLS 1.3 is a nonblocking finding under the inherited contract: complete structurally possible parsing and fingerprinting, then use actual enforcement verdicts. A structurally blocking parser error is different. Neither Processor nor TLS-Flow invents violations. Existing TCP error behavior is not automatically changed. External enforcement support and exact QUIC finding/verdict plumbing remain unverified.

## 6. Allocation and hidden-work inventory

| ID | Visible path / boundary | Evidence and conclusion |
|---|---|---|
| N01 | Factory and Processor init | `make_unique`/`make_shared` visible in startup functions; permitted in startup, not proof all calls occur only there |
| N02 | Digest init/calculate/copy | Visible scalar reset and memcpy, no explicit heap call in wrapper bodies; external finalize still unproved |
| N03 | ExtensionsArray sort | std::sort over an embedded array; comparator uses type, length and memcmp. Verify actual standard-library implementation/stack use; do not replace with allocating stable_sort |
| N04 | SegmentBufferManager access/allocate | Handler is std::function; runtime lambdas are converted at call sites. Small-object optimization is not a portable proof for the actual captures/build |
| N05 | Visibility visitor and table fetch | Two std::function boundaries, with capturing lambdas, plus IPC send. Stack response construction does not prove conversion/queue internals allocation-free |
| N06 | Conductor notify | Templated notifier, weak_ptr lock, shared_ptr passed to callback; no std::function conversion in this notify template. Listener bodies and last-reference destruction are separate transitive boundaries |
| N07 | Subscriber map | std::map insertion/erase exists in subscribe/unsubscribe. If runtime subscription is supported, node insertion allocation conflicts with the strict contract; erase/destruction also needs lifecycle audit. Startup-only registration must be established or bounded storage used |
| N08 | Fixed/FIFO maps and vectors | CTUnorderedMap, CTVector and FIFOSimpleMap call sites visible; implementations absent. Names/capacity templates are not source proof of no allocation or bounded collision work |
| N09 | SessionTable observer operations | registerObserver/deleteObserver/getSessionByIndex are called during runtime continuation/new-session/aging flows; implementations absent |
| N10 | Diagnostics/logging | Counter update delegates externally; DIAG_LOGGER macros include failure and per-byte debug paths. Macro expansion, queue/formatting and build-mode behavior absent |
| N11 | Fingerprint/Sni conversions | std::string/stringstream conversion helpers exist; visible CH dispatch uses fixed value objects, not these string helpers. Do not falsely label all helper definitions packet-reachable |
| N12 | Controller visibility formatting | Controller uses vectors/streams; not itself an Engine-runtime violation. Engine IPC request/response side remains in scope |

Source anchors: A01-A05, A07-A25. N04-N10 are evidence gaps or conditional conflicts, not fabricated observed heap allocations.

**Minimal correction direction:** keep existing viewers and ownership. Where an actual runtime allocation is demonstrated, remove that allocation at the specific boundary. A synchronous borrowed callable or bounded callable representation may be considered for internal callback boundaries if the actual std::function conversion allocates. Do not redesign every callback, add a generic dispatcher, or replace every container without evidence. If startup-only subscriptions are the actual supported lifecycle, enforce/document that rather than build a runtime registry unnecessarily.

### 6.1 Failure and aging are part of the runtime proof

SegmentBufferManager::allocate checks fullness, removes the oldest session's aging observer, pops an entry, inserts a new entry, resolves its session and registers an aging observer (A14). It is logical slot allocation at the call site, not proof of heap allocation. Its helper/container implementations still require audit. The visible path can insert an entry and then return false when the new session lookup fails without erasing that entry. This is a concrete failure-cleanup concern for reuse/accounting, not permission to change the accepted FIFO policy or invent a new reclaimer.

Conductor aging dispatches SessionEnd then releases its extension; SegmentBufferManager aging erases continuation state (A15/A20). Preserve existing lifecycle integration. Prove callback last use, observer ordering, erase/reset/destructor behavior and no stale slot access. Do not assume a reclaimed entry is eligible while backing is borrowed. No resource-induced IncompleteHandshake violation.

A failure of the selected CH interpretation/canonical/digest operation is nonresumable for that affected evaluation under the inherited shared contract. No successful CH/FP publication or retry of its mutated digest is allowed. This is not a blanket rule that any later crypto failure invalidates every previously retained CH. A packet-local authentication failure before its bytes are admitted must not feed tentative plaintext, advance the accepted CH frontier, or mutate previously valid retained CH/candidate/active state merely because the packet failed. Discard its tentative output and reset the affected transient work state before reuse; exact packet/session disposition remains the applicable accepted policy, not a new automatic drop or violation. If a provider fault leaves work state unusable, do not reuse that state until the defined bounded reset/reinitialization path succeeds; no runtime allocation repair. Retained-state corruption or an explicit acquisition resource failure is handled separately for the actually affected acquisition. Known storage needs should be preflighted where practical; no generic rollback framework is introduced. Preserve actual verdict reduction and TCP D04 distinctions rather than assigning one generic anomaly to every failure.

## 7. Runtime work model without invented budgets

**DESIGN JUDGMENT:** account for work per input and per completed CH separately. Let D be datagram bytes, P eligible packets, F parsed frames, R retained coverage ranges, U newly admitted unique CRYPTO bytes, N cipher entries, M extension locators, C canonical bytes and Q the actual total extension-content bytes compared during sorting. These are workload variables, not selected limits.

- Datagram/packet/frame walking: validate each cursor advance and boundary; every iteration consumes bytes or advances a finite phase. Do not repeatedly scan ciphertext for magic bytes. Unknown-frame handling must use a defined parse/disposition contract, not guess a length and continue.
- Crypto: count Initial derivations/rekeys, header-protection operations and authenticated bytes for every processed packet. Duplicate CRYPTO content can still incur packet authentication work; zero new retained bytes does not mean zero CPU work.
- Coverage: account for range lookup, duplicate comparison, insertion and transient normalization. A linear-range algorithm can inspect R descriptors per insertion; no concrete range algorithm is supplied, so no claimed asymptotic implementation bound beyond that conditional example.
- CH parsing/canonical feed: avoid rescanning the full retained prefix after each tiny fragment. Charge field validation, copies and canonical append work separately; accepted progress and hash progress are not packet counts.
- Sorting: count comparator calls and Q, not only M. Equal-type/equal-length long values with long common prefixes are important worst-case inputs. Verify the actual sort implementation rather than claim a measured bound.
- Buffered hash: appends copy C bytes; finalization reads C bytes again. Retained-state copying adds its actual copied length. Streaming-plus-FullString still writes C bytes and performs hashing; it does not eliminate canonical storage work.
- Debug/failure: the calcHash byte overload visibly loops over every supplied byte for diagnostics. Release macro elimination may allow compiler removal of an empty loop, but only the actual optimized build/disassembly can establish this. Debug per-byte logging must be included in its own supported build profile.
- Visibility/aging: table iteration/cursor traversal, listener calls, erases and counter/logging operations are runtime work even without a packet. A requested result count alone does not prove the underlying table scan cost.

No finite retained-byte limit bounds total connection lifetime work under arbitrary repeated traffic. Do not add an arbitrary packet/frame/CPU budget or reject accepted input to manufacture a performance claim. Such a budget is a separate product/behavior decision. This report provides work-accounting obligations, not a numerical performance result.

## 8. Startup and reuse contract

Before activation, the selected implementation must:
1. Resolve exact linked crypto/library/provider/build and all supported Engine build profiles.
2. Resolve the O06 storage profile with checked count/stride/alignment arithmetic; validate every required preparation result. Numeric values remain deferred here.
3. Prepare retained CH/canonical/range/association/suppression storage, scratch and crypto work contexts for actual joint peaks. Include stack temporaries and provider-internal storage where applicable.
4. Establish registration/ownership and lifetime for listeners, observers, diagnostic paths and work areas. Prove startup-only operations are not lazily reachable from packets or aging.
5. Validate reset/rekey/reuse/failure behavior without allocation. Precreating an outer object does not prove its later operations reuse internal storage.
6. Do not advertise a partially prepared profile as usable; use existing activation/failure authority. No packet-time repair allocation.

During runtime, no resize/reserve, provider fetch/context creation, dynamic range/sort workspace, growing identity cache, allocating error formatter or lazy singleton construction is allowed in the affected call graph. No new framework is selected to enforce this.

## 9. Separate design-progression and future acceptance gates

### 9.1 Current design-progression review

Evaluate the scoped design checks in order and stop on a concrete design contradiction. The absence of future implementation or deployment evidence does not fail these checks. CHECKED below means static consistency within the supplied design inputs, not product approval or runtime proof.

| Check | Current design question | Result / disposition |
| --- | --- | --- |
| D-G01 Stage and authority | Are source facts, future design obligations and later verification separated? | CHECKED: sections 1, 4, 8 and 9.2 distinguish them; no QUIC implementation is required to continue design |
| D-G02 Semantic preservation | Are canonical FP/FullString, deferred TCP publication and actual-verdict authority retained? | CHECKED: sections 1.1, 3.2 and 6.1 preserve these obligations; no streaming-only substitution or parser-created violation |
| D-G03 Crypto operation boundary | Are required operations, byte eligibility, startup preparation and failure routing specified? | CHECKED: section 4 defines the logical pipeline and worksheet without pretending a provider/API is selected |
| D-G04 Ownership and bounds | Are retained CH/candidate state and transient crypto scratch separated, with full-last-use and joint-peak obligations? | CHECKED: sections 1.1, 4, 6.1 and 8 retain O05/O06 obligations; symbolic parameters remain open rather than invented |
| D-G05 Hidden runtime work | Are potential allocation/work boundaries and concrete source concerns inventoried without inventing observed failures? | CHECKED: sections 3, 6 and 7 distinguish source facts, conditional risks and future validation |
| D-G06 Downstream dependencies | Are provider choice, concrete storage fit, external readers and execution validation retained at their actual closure points? | CHECKED: section 9.2 carries them forward; no missing evidence is labeled PASS |
| D-G07 O08 handoff | Does this report avoid manufacturing Finished evidence or importing TCP completion into QUIC? | CHECKED: section 11 defines the next analysis; no new completion trigger is selected |

**Progression disposition:** ready to begin O08 design analysis. This closes the focused O07 progression question, not every provider/layout decision and not implementation acceptance. The user-approved deferred-publication principle remains accepted; final-record-length metadata is a separate working convention with external-reader verification still open.

### 9.2 Future implementation / activation acceptance register

The original G01-G08 identifiers are retained below as V-G01-V-G08 to prevent confusing them with current design checks. They are NOT RUN / NOT ESTABLISHED now because this is design. Their absence is not a failure of the current design stage. When an implementation is submitted for acceptance, evaluate applicable gates in order and stop at the first non-PASS.

| Future gate | Required evidence / closure point | Current disposition |
| --- | --- | --- |
| V-G01 Actual provider and implementation identity | QUIC KDF/HP/AEAD implementation, linked MD5/crypto identity, build/config and dependency paths before implementation acceptance | FUTURE VALIDATION. Concrete provider/API contract selection is required before crypto API/layout freeze; no deployed QUIC build required for O08 |
| V-G02 Observable semantic fit | Exact canonical bytes, FP and FullString; fragmentation/duplicate behavior and finding authority | FUTURE VALIDATION; design preservation contract specified, target execution not claimed |
| V-G03 Startup/reuse fit | Checked storage profile, preparation, reset/rekey/error behavior | FUTURE VALIDATION; O06 numeric/layout choices remain explicit before concrete storage acceptance |
| V-G04 Transitive allocation closure | Provider, callbacks, diagnostics, SessionTable, containers, visibility, aging and destruction | FUTURE VALIDATION; N04-N10 remain specific audit obligations, not proven allocations |
| V-G05 Lifetime and joint peaks | O06 J01-J10 with actual context/scratch/borrowed backing and disposition | FUTURE VALIDATION; no assumed context count or unsafe alias |
| V-G06 Failure correctness | No partial success/digest retry; actual verdict/D04 preservation and coherent cleanup | FUTURE VALIDATION; existing insertion-cleanup concern retained for focused disposition, not a generic redesign |
| V-G07 Runtime allocation evidence | Cold/repeated success/failure/rekey/reuse/aging under instrumentation on the actual supported build | NOT RUN; required before allocation-free runtime acceptance |
| V-G08 Work/performance evidence | Actual work/copy/comparison/crypto/stack results for supported profiles | NOT RUN; no invented budget or throughput claim |

O04 external-consumer coverage and O05 primary-requirements/physical-policy gaps remain open at their documented integration/final-design closure points. This correction neither resolves them by prose nor requires them as prerequisites for analyzing O08. A concrete contradiction discovered there must still be surfaced.

## 10. Planned validation pack

**All tests below are PLANNED, NOT RUN against production code.** Each eventual result must identify exact source/build/provider/profile, input, expected behavior, observed result and evidence location. Instrumentation must itself use prepared logging storage; exclude startup only by an explicit activation boundary, not by hiding first-use allocations.

| ID | Scenario | Required oracle |
|---|---|---|
| T01 | First eligible Initial after startup, no prior warm traffic | No runtime allocation including lazy provider/dispatch paths |
| T02 | New DCID/attempt and repeated rekey/reset/reuse | Correct Initial keys and authentication; no new contexts/cache growth |
| T03 | Valid/invalid tag, truncated sample/AAD/ciphertext and provider failures | No frame parse/publication of tentative plaintext; packet-local failure does not advance or corrupt prior CH progress; affected work state is safely reset or unavailable; no allocating diagnostics |
| T04 | Several Initial/non-Initial packets in one datagram | Validated boundaries, eligible processing only, no cross-packet sample or scratch borrow |
| T05 | PADDING/ACK/PING before CRYPTO and multiple CRYPTO frames | Structural frame walk; no first-frame/magic-byte assumption |
| T06 | CH split at every handshake/field boundary across CRYPTO/Initial inputs | Same canonical bytes/FP/FullString as contiguous input; no duplicate contribution |
| T07 | Out-of-order CRYPTO, exact duplicates, high sparse offsets and many ranges | Accepted association/window policy; independent byte/range/offset checks; bounded prepared work |
| T08 | Canonical append below/exactly at/above usable extent; zero append; arithmetic overflow | Existing strict guard distinguished from target; no truncation, wrap or successful partial CH |
| T09 | Sorted equal-type/length extensions with long common prefixes; GREASE; QUIC transport parameters | Exact legacy ordering/canonical semantics; comparison-byte accounting |
| T10 | Supported versions including TLS1.3, no TLS1.3 and future/unknown values | Legacy field not mistaken for negotiated/offered maximum; nonblocking findings permit full FP; actual verdict authority |
| T11 | A partial, B complete, A resumes; deferred candidate plus new input | Correct selected state/backing and no scratch overwrite; no assumed one-context sufficiency |
| T12 | Callback Stop with/without violation, nested helper/re-entry and last shared reference | Correct verdict reduction, no next pull after stop, no premature release or hidden destruction allocation |
| T13 | Range/locator/canonical/provider exhaustion affecting the selected CH evaluation after prior contribution | Apply the affected acquisition failure contract; no successful partial CH or mutated-digest retry, heap fallback or automatic violation. Distinguish packet-local authentication failure in T03 |
| T14 | Full FIFO/table, missing session after insertion, registration failure | Coherent entry/observer cleanup and accounting; no stale entry or borrowed victim reuse |
| T15 | Aging/reclaim/session reuse and late retired-attempt traffic | No stale state resurrection, early suppression erasure or resource-induced IncompleteHandshake violation |
| T16 | Engine visibility request/page, counters, error/debug/release profiles | No Engine allocation; bounded actual traversal and formatting/queue work |
| T17 | Existing TCP happy/error paths and cross-record CH projection | Preserve D04 site effects, deferred publication and feedback barrier; no QUIC behavior imposed on TCP |
| T18 | Initial first-byte variants and protected low bits | Reject whole-byte 0xC0 equality as oracle; version-aware high-nibble intent; protected fields decoded at correct stage |
| T19 | All success/failure/reset/destruction paths under allocation hooks | Zero allocation/growth after activation; cover new/delete, malloc/calloc/realloc/free and library/custom allocators as applicable |
| T20 | Work/stack accounting under repeated tiny fragments/duplicates and maximal admitted shapes | No quadratic prefix rescans, hidden copies or unaccounted crypto/sort/aging work; no fabricated throughput claim |

## 11. Double-check, completion and O08 handoff

### 11.1 Focused V3 double-check

Two separate static passes are recorded in section 14. The first checks stage classification, design contracts and preservation against V2/O06 and the literal source anchors. The second checks cross-section consistency, the D-G/V-G separation, remaining dependency placement, and O08 readiness on the unchanged V3 bytes. These are document/source checks only, not packet execution or provider instrumentation.

**Completed:** focused O07 design-progression review; required crypto-operation and startup/reuse contracts; canonical/FullString preservation; allocation/work boundary inventory; failure obligations; future validation plan; explicit handoff to O08. No concrete contradiction in these scoped contracts requires waiting for a QUIC implementation before starting O08.

**Still open at the appropriate later stage:** concrete provider/API choice and storage fit before relevant API/layout freeze; external-consumer and full primary-requirements alignment before corresponding final integration/design acceptance; actual linked-build, allocation and performance evidence after implementation and before activation. No blanket approval of these items is inferred.

### 11.2 Current O08 completion-policy handoff

After successful completion/publication and permitted commit of the active attempt's CH acquired from eligible Initial CRYPTO data, the attempt can be InProgress. Only a subsequently observed, structurally qualifying, associated client Handshake packet can then move InProgress -> Finished, including ACK-only. A Handshake observed before complete authoritative CH does not establish InProgress or Finished and is not latched for later completion. No Handshake payload decryption or TLS Finished detection is required. Finished is the accepted product heuristic, not cryptographic handshake verification.

CH completion, attempt admission, backing release and Finished remain distinct. No server visibility, later secrets, new API, capacity or architecture is introduced. Provider/runtime acceptance remains future work, not a blocker to continuing the design.

## 12. External protocol references

These references ground protocol facts only; they do not prove local library behavior.
- R1: [RFC 9001, sections 4, 4.2, 5.2-5.5 and Appendix A.2](https://www.rfc-editor.org/rfc/rfc9001.html). TLS carriage/version, Initial derivation/protection, header-protection fields and sample packet.
- R2: [RFC 9000, sections 12.2, 12.4, 17.2.2 and 19.6](https://www.rfc-editor.org/rfc/rfc9000.html). Coalescing, frame structure, Initial fields and CRYPTO offset/length.

## 13. Literal source anchor index

All anchors refer to `TLS-Flow-All-Code.txt`; embedded file names identify the source unit. They support the code facts above, not runtime PASS.
- **A01**: `context/engine/src/Factory.cpp`, aggregate L1561; literal anchor `void Factory::buildProcessor()`.
- **A02**: `processor/src/Processor.hpp`, aggregate L7072; literal anchor `segmentBufferManager = make_unique`.
- **A03**: `processor/src/Md5Digest.cpp`, aggregate L5253; literal anchor `bool Md5Digest::init()`.
- **A04**: `processor/src/Md5Digest.cpp`, aggregate L5259; literal anchor `bool Md5Digest::calculate(`.
- **A05**: `processor/src/Md5Digest.hpp`, aggregate L5306; literal anchor `void copy(const Md5Digest&`.
- **A06**: `processor/src/Processor.cpp`, aggregate L5826, L6405; literal anchor `m_pDigest->finalize(m_fp);`.
- **A07**: `processor/src/Processor.cpp`, aggregate L6575; literal anchor `Processor::RecordProcessResult Processor::feedExtensionsFP()`.
- **A08**: `processor/src/Processor.cpp`, aggregate L6648; literal anchor `Processor::RecordProcessResult Processor::feedSuppVersionsFP(`.
- **A09**: `processor/src/Processor.hpp`, aggregate L7258; literal anchor `case ExtensionTypeE::quic_transport_parameters:`.
- **A10**: `processor/src/Processor.cpp`, aggregate L6961; literal anchor `FullStringData Processor::getFullString() const`.
- **A11**: `packet-flow/src/TLSFlowConductor.cpp`, aggregate L3428; literal anchor `ObserverResult TLSFlowConductor::process(`.
- **A12**: `processor/src/ExtensionsSorting.hpp`, aggregate L4835; literal anchor `void sort()`.
- **A13**: `processor/src/ISegmentBufferManager.hpp`, aggregate L4877; literal anchor `using Handler = std::function`.
- **A14**: `processor/src/SegmentBufferManager.cpp`, aggregate L7797; literal anchor `bool SegmentBufferManager::allocate(`.
- **A15**: `processor/src/SegmentBufferManager.cpp`, aggregate L7882; literal anchor `int SegmentBufferManager::onEvent(`.
- **A16**: `packet-flow/src/TLSFlowConductor.hpp`, aggregate L4008; literal anchor `TLSFeatureVerdict notify(`.
- **A17**: `packet-flow/src/TLSFlowConductor.cpp`, aggregate L3699; literal anchor `bool TLSFlowConductor::fetchSessionExtensions(`.
- **A18**: `packet-flow/src/TLSFlowConductor.hpp`, aggregate L3955; literal anchor `std::map<TLSPriority`.
- **A19**: `processor/src/Processor.hpp`, aggregate L7196; literal anchor `bool calcHash(const uint8_t *data`.
- **A20**: `packet-flow/src/TLSFlowConductor.cpp`, aggregate L3353; literal anchor `int TLSFlowConductor::onEvent(`.
- **A21**: `extension/include/TLSSessionExtensionTable.hpp`, aggregate L2818; literal anchor `using FetchCallback = std::function`.
- **A22**: `extension/include/ITLSSessionExtensionInspector.hpp`, aggregate L2634; literal anchor `using TLSSessionExtensionInspectorVisitor =`.
- **A23**: `packet-flow/src/TLSFlowConductor.cpp`, aggregate L3691; literal anchor `m_subscribers.erase(priority);`.
- **A24**: `processor/src/Md5Digest.cpp`, aggregate L5271; literal anchor `bool Md5Digest::finalize(`.
- **A25**: `processor/src/Processor.cpp`, aggregate L6343; literal anchor `if (!calcHash(m_parsers->clientHello->getVersion()))`.

### V2 review correction note

V1 is preserved. V2 clarifies Processor construction uses make_shared, narrows the subscriber-map conflict to runtime insertion allocation rather than treating all deallocation as forbidden, and adds direct anchors for aging, visibility callback types, finalize and version feed. No baseline or production source was modified.

## 14. V3 correction and review record

### 14.1 Correction scope

V2 remains immutable. V3 removes missing future QUIC implementation/deployment evidence as a design blocker throughout status, F01, section 4, gates and handoff. It preserves the source findings, inherited protocol discussion, no-allocation constraint, work model and T01-T20 as future tests. Protocol references are inherited unchanged, not newly revalidated in this focused correction. Earlier V2 review notes above are historical, not the current progression status.

### 14.2 Source identities

- `tls_flow_o07_provider_runtime_review_v2.md`: SHA-256 `879121657e2baf30266bae88356dfd8e67f7d3c4597323cf908646b495826e8f`.
- `TLS-Flow-All-Code.txt`: SHA-256 `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b`.
- `tls_flow_o06_capacity_width_joint_peak_review_v1.md`: SHA-256 `3320e5df3fcb9798280ea62533a16cfe700c4d992dbb0986ba9d6984e7a20efe`.
- `tls_flow_o04_viewers_readers_review_v2.md`: SHA-256 `2517c6bf9bf3efbbd5f5bff6e7e1d6a2eb65eebc1bd04796d96dee5aa027e896`.
- `tls_flow_o05_candidate_disposition_ownership_v2.md`: SHA-256 `a32e7fb96ed7b14623617f7d0eeb0ad33fea5fb0e638a077b977f288da5226c1`.

### 14.3 Review evidence and limits

- Pass 1: stage/authority, required crypto operations, canonical preservation, source-anchor consistency, no-allocation and ownership obligations, failure mapping, and named downstream dependencies checked.
- Pass 2: unchanged candidate checked for contradictory progression wording, D-G01-D-G07 versus V-G01-V-G08 separation, O08 task/limits, preservation of T01-T20 and the source anchor index, and unchanged input hashes.
- Outcome: scoped static design-progression review complete; ready for O08. Provider/API choice and concrete storage fit are not claimed closed. No runtime, implementation, performance or activation PASS.

The external review record stores the final V3 SHA-256 and both check results; the document does not embed its own digest.

## 15. V4 additional double-check and correction

V3 is preserved. This review checks O07 readiness itself and the subsequent O08 design handoff. O07 may be used as the current provider/runtime design analysis; its scoped progression outcome permits starting O08, not activation or final provider/API acceptance.

**First substantive finding:** section 6.1's phrase "any future provider failure after canonical contribution" was broader than the selected-CH failure contract. Read literally, a later packet authentication failure could wrongly retire an unrelated valid pending CH. V4 scopes nonresumability to the actually affected CH evaluation, distinguishes tentative packet output from retained progress, and updates T03/T13 accordingly. It does not introduce a new packet verdict, timeout, provider, state transition or allocation policy.

**Restarted review, pass A:** checked the revised failure boundary against O06 X03/X04 and O01's selected-CH accounting/failure scope; checked no unauthenticated byte admission, no mutated-digest retry, no active/candidate contamination, bounded scratch recovery, full last use, TCP preservation and no invented violation. Other source facts and protocol references remain inherited unchanged.

**Restarted review, pass B:** reread the final candidate for consistency across sections 4, 6.1, 9, T03/T13 and 11. Checked that absent future code is not a design blocker, provider/API selection and numeric storage choices are still open at their proper closure points, and O08 does not assume Finished from CH completion or resource release. Verified unchanged source anchors and input hashes. Both passes are static design review, not executed tests or full primary-requirements alignment.

**Outcome: O07 ANALYSIS COMPLETE; O08 V3 COMPLETION POLICY SELECTED.** No concrete remaining O07 design contradiction was identified that prevents that analysis. Runtime/provider/build validation remains future work. That V4 handoff is superseded by O08 V3: the completion heuristic has been selected. Without its qualifying post-CH evidence, no Finished transition is inferred.

Preserved V3 SHA-256: `a8c6baa0e6fed0984ded10a68fcaff03bd74db16ef86fb57dbb41a5de81d1518`.

## 16. V5 focused R01 coordination

Current completion authority is O08 V3. Sections 13-15 retain historical source/review evidence; references there to starting O08 are historical, not a current completion-selection blocker. No consumer changes or provider/API choice are introduced.
