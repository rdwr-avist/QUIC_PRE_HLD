# O06: Capacity, Width and Joint-Peak Design Review V2

**Status: bounded-storage design analysis delivered. Numeric capacity selection remains deliberately deferred; storage/layout implementation acceptance is NOT passed. No migration, runtime-allocation or performance PASS.**

The current instruction authorizes O06 design work, not retroactive closure of O04 G01 or O05 E01-E05. O06 makes the finite-storage obligations concrete without inventing a maximum ClientHello, pool sizes, RAM/CPU budgets or architecture. Existing code limits are evidence, not automatically approved QUIC limits.

## 1. Baseline and review scope

Directly read inputs: O01 V4, O02 V2, O03 V7, O04 V2, O05 V2, Consolidated V6 and TLS-Flow-All-Code.txt. Hashes are in the manifest and section 12. Official HLD V10 is not modified; no later HLD/interface draft is promoted to authority. The available O01-O05 contracts are comparison inputs with their recorded proposal/accepted/evidence-gap distinctions, not proof that code implements them.

CODE FACT identifies visible declarations/branches. INHERITED CONTRACT identifies the available design constraints and recorded accepted decisions. DESIGN PROPOSAL identifies this document's symbolic sizing/width recommendations. OPEN PARAMETER is intentionally unselected, not a guessed default. PLANNED VALIDATION is not execution evidence.

The QUIC FRD/SRS and external consumer files remain unavailable locally. Opening their surfaced references failed again. Consequently this is not a fresh full FRD/SRS or consumer audit. QUIC alignment is limited to the available consolidated contracts and recorded accepted constraints. O05's missing-source gate is not silently cleared. No internet protocol research was needed for this source-based capacity review; protocol maxima below are represented symbolically from the existing field model, not asserted as a new supported product limit.

### Invariants retained

- Startup allocation/preparation is allowed. All Engine runtime, including packet processing, callbacks, failure paths and aging, uses prepared storage without allocation, capacity growth or heap fallback.
- QUIC v1 only, eligible Initial processing and shared CH/TLSFP. Protected fields require decoded trusted classification or actual protection removal; no server-side visibility assumption.
- One active TLSData semantic context. Candidate acquisition/evaluation is isolated, not a second active session. Accepted/rejected/deferred disposition is explicit, not inferred from unchanged TLSData.
- Full last use means all applicable callbacks, actual-verdict reduction, permitted commit and every final internal read. Input end does not erase required continuation.
- C-O03-01 deferred cross-record TCP publication remains: no intermediate contributing-record callbacks; one completion projection, then barrier before a separate later record. Final contributing record payload length is not total CH length.
- Resource exhaustion/eviction is not a parser-defined violation, automatic Error/drop, Finished signal or IncompleteHandshake violation. Preserve actual verdict authority and TCP D04 site-specific exceptional visibility.
- Maximum CH and numeric capacities remain open. Do not request unavailable CPU/RAM budgets as a precondition to producing this design analysis. Do not reserve DTLS framework/storage or introduce a new manager/reclaimer.

## 2. Current code capacity inventory

All values in this section are existing-source facts only. They are not newly selected limits, a complete memory budget or proof that all paths are allocation-free.

| ID | Existing source fact | Meaning and migration consequence |
| --- | --- | --- |
| C01 | SegmentBufferSize = 3000; SegmentBuffer.buffer uses that extent | TCP retained segment/extension storage, not maximum logical CH or QUIC CRYPTO offset |
| C02 | MaxNumberOfLocators = 100 | Declared constant; no assumption it controls the actual ExtensionsArray. Inspect concrete instantiation before using it in sizing |
| C03 | MaxNumberOfSegmentBuffers = 1000; FIFO map uses it | Existing session-keyed continuation entries; not QUIC active/candidate/retired pool selection |
| C04 | MaxTlsRecordPayloadSize = 16384 | Existing per-record TCP validation bound. Cross-record CH total is a different quantity |
| C05 | ExtensionsArray.maxSupportedExtensions = 1024, count uint16 | Actual declared locator-array extent; reconcile with C02 rather than silently substituting 100 |
| C06 | Md5Digest array extent = 2870; calculate rejects zero append and aggregate length >= array extent | Strict inequality: exact-fill append fails. FullString/digest input is separate from raw CH storage. No selected CH maximum follows from 2870 |
| C07 | Processor.maxRecords = 128 and CTVector<Record,...> | Existing current record collection; deferred CH acquisition does not require one stored Record per contributing physical record |
| C08 | SegmentBuffer.bufferUsed, nextLength, stepBufferedLength, totalRecordLength and continuation lengths use uint16 | Existing fields are not a general full-CH offset domain; checked adaptation required |
| C09 | PayloadParams.length, packet/record cursors and BaseTlsParser offsets use uint16 | Local region width is not proof full-message width is sufficient |
| C10 | CH parser expectedLength uses uint32; bufferLen/consumed use uint16 | Mixed-width boundary: a wide declared length does not make a narrow buffer or consumed count safe |
| C11 | Processor cipher/extension/compression counts use uint32; ExtensionLocator.length uses uint16 and pValue is a pointer | Count/field-length width and backing lifetime are distinct; a locator pointer is not retained storage |
| C12 | FullString length/index uses size_t; copy clamps bytes but copies original source index | Source-index invariant must be proved before copy/use; a bounded memcpy alone is not index validity |
| C13 | Sni.max_sni_length = 255 | Existing viewer/value representation, not authority for a new silent truncation policy |
| C14 | BaseTlsParser.getConsumed returns offset+1 as uint16; getDataAtOffsetU24 uses a 32-bit load then shift | End-exclusive arithmetic and read width need explicit safety; widening the result alone does not fix over-read or wrapped consumption |

The existing Md5Digest is an accumulation buffer followed by MD5 finalization, not proof of a streaming provider with bounded hidden state. Replacing it with streaming hash alone would lose FullString unless canonical bytes remain available. O07 owns provider/runtime verification; O06 records the retained-byte requirement without selecting a new provider.

### Literal source anchor index

- **K01**: `constexpr static uint64_t SegmentBufferSize = 3000;` at aggregate L4207.
- **K02**: `constexpr static uint64_t MaxNumberOfLocators = 100;` at aggregate L4208.
- **K03**: `constexpr static uint64_t MaxNumberOfSegmentBuffers = 1000;` at aggregate L4209.
- **K04**: `constexpr static uint16_t MaxTlsRecordPayloadSize = 16384;` at aggregate L4218.
- **K05**: `static constexpr uint32_t maxSupportedExtensions = 1024;` at aggregate L4842.
- **K06**: `static constexpr uint32_t s_max_buffer_size = 2870;` at aggregate L5315.
- **K07**: `static constexpr size_t maxRecords = 128;` at aggregate L7151.
- **K08**: `uint16_t bufferUsed;` at aggregate L7745.
- **K09**: `uint16_t m_maxOffset{};` at aggregate L8053.
- **K10**: `uint16_t m_offset{};` at aggregate L8054.
- **K11**: `uint32_t m_numOfCipherSuites = 0;` at aggregate L7137.
- **K12**: `FIFOSimpleMap<SessionId, SegmentBuffer, MaxNumberOfSegmentBuffers> map;` at aggregate L7919.
- **K13**: `uint16_t length;` at aggregate L4775.
- **K14**: `inline uint16_t getConsumed() const { return m_offset + 1; }` at aggregate L8024.
- **K15**: `inline uint32_t getDataAtOffsetU24() const { return (getDataAtOffsetU32()) >> 8; }` at aggregate L8020.
- **K16**: `uint16_t m_lengthLeftToParse{};` at aggregate L7167.
- **K17**: `static constexpr uint16_t max_sni_length = 255;` at aggregate L181.
- **K19**: `Md5Digest::calculate` at aggregate L5259; strict aggregate append guard checked directly.

## 3. Open parameter ledger and units

These are logical quantities, not new classes or configuration keys. Every eventual value must have a source/decision and scope (per message, session, Engine or provider). A zero value, default or legacy constant is not implied by an OPEN entry.

| ID | Symbol / unit | Definition | Status |
| --- | --- | --- | --- |
| P01 | H_body / bytes | Supported maximum CH body extent, excluding handshake header | OPEN product/support decision |
| P02 | H_total / bytes | H_body plus the existing handshake header extent | Derived only after P01; checked addition |
| P03 | B_CH / retained bytes per live CH | Raw bytes that must survive parsing, locators, sorting and readers; may differ from H_total if a proven bounded retention scheme is used | OPEN layout; no automatic whole-CH copy |
| P04 | B_CRYPTO / unique retained bytes per acquisition | Includes out-of-order bytes still needed and excludes exact duplicates; not maximum absolute CRYPTO offset | OPEN |
| P05 | O_CRYPTO / logical offset domain | Highest admitted logical extent/window for the selected message/attempt, checked before allocation/copy | OPEN accepted acquisition policy; not chosen from B_CRYPTO |
| P06 | N_ranges / descriptors per acquisition | Maximum retained disjoint coverage ranges and any required insertion/normalization workspace | OPEN |
| P07 | N_loc / locators per CH | Count needed by actual sorting/extraction/FP behavior, including duplicate extension entries if retained by current semantics | OPEN; C05 is current implementation fact |
| P08 | B_canon / canonical FP bytes per CH | Exact digest/FullString byte stream, delimiters and retained extension contents included | OPEN; derive from actual canonicalization, not CH byte equality |
| P09 | N_CH_live / physical CH states per Engine | Simultaneously retained contexts, including deferred candidates and last-use retention where distinct | OPEN; not equal to active TLSData count |
| P10 | N_assoc / association entries per Engine | Active and pending identities plus required retired suppression and lifecycle bookkeeping | OPEN; identity storage is not a full CH copy |
| P11 | N_candidate / pending candidates | Admitted bounded candidate work per session/Engine under actual accepted policy | OPEN; O05 does not approve an unbounded queue or automatically choose one slot |
| P12 | N_retired / suppression entries | Required retired identities and lifecycle extent | OPEN; do not clear suppression early merely to fit |
| P13 | B_scratch / bytes per Engine | Simultaneously live decrypt/plaintext/parser/sort/normalization temporary storage | OPEN; alias only with proven disjoint lifetimes |
| P14 | N_records / current collection entries | Existing record-list scope plus exceptional D04 effects | Current C07, target fit unproven; not physical contributing-record count |
| P15 | N_crypto_work / prepared provider contexts | Provider contexts needed by actual nested/synchronous call graph | OPEN O07; single-thread alone does not prove one context sufficient |
| P16 | B_identity / bytes per association | Exact admitted identity fields, lifecycle reuse protection and any token/CID backing required by accepted policy | OPEN actual association contract; no arbitrary identity truncation |

Do not derive H_body from TLS record size, current TCP buffer, one datagram, two segments, digest array or locator count. Conversely, selecting H_body alone does not settle ranges, canonical bytes, identities or concurrency of retained states. Protocol-encodable lengths and supported product limits are different.

## 4. Width migration and checked boundaries

### 4.1 Domain separation

**DESIGN PROPOSAL:** use a CH-relative logical extent domain capable of the existing uint24 body length plus header, consistent with O01's uint32 proposal. This is representational sufficiency, not acceptance of every encodable CH. Preserve wider decoded QUIC CRYPTO offset/length values until checked association/window conversion; never narrow them into TCP sequence or CH-relative fields first.

Local input span lengths and physical indexing may use existing narrow types only when their full admitted domain and one-past-end frontier fit. Per-record TCP length remains its existing field domain. Do not globally widen public Record.recordLength to represent CH total: that would change the selected projection and consumers.

| Boundary | Required design treatment | Not permitted |
| --- | --- | --- |
| Declared body plus handshake header | Check representability and selected H_total before retention; distinguish malformed declared structure from unsupported resource extent | uint16 wrap, truncation or treating every oversize as malformed |
| Shared start/advance accepted count | Logical accepted frontier remains CH-relative; accepted <= offered and <= remaining declared extent; no mutation on invalid invocation | Reusing legacy consumed as accepted without bounds proof |
| Legacy parse(bufferLen, consumed) | Keep uint16 only for proven bounded local regions with representable end frontier. Otherwise refactor internal parser offset/length/consumed together or use validated local field slices | Casting a whole large CH to uint16 or splitting it arbitrarily into old stateless parser calls |
| Extension length and locator | Field length can remain uint16 where wire field semantics fit; containing header+value region, absolute offset and cumulative extent need separate checked domain | Assuming a uint16 field length guarantees header+value or one-past-end fits uint16 |
| Packet/record cursors | Validate actual supplied length before conversion; subtract only within current span; preserve TCP serial arithmetic separately | Ordinary numeric CH comparison applied to wrapped TCP sequence values |
| CRYPTO offset + length | Validate decoded wide operands and sum before clipping/association or any copy. Translate only a proven eligible message-relative region | Allocate a buffer up to the largest sparse offset; wrap first then validate |
| Digest append/copy | Validate current index, append length and total in representable domain before writing; match actual provider boundary semantics | Overflowing len+index or assuming exact-fill succeeds in current Md5Digest |
| Collection counts and indices | Count must represent capacity and end position; checked increment and checked index conversion | Wrapping at capacity or using the index's maximum as a sentinel without reserving it |
| Identity/session reuse | Existing lifecycle identity must distinguish stale decisions from current slot/session use | Invented generation API, or assuming same session index means same lifecycle |

**Recommended minimal implementation direction:** widen only shared logical CH extent/checkpoint/consumption arithmetic that must cover H_total; retain bounded TCP record/field representations and current public viewer signatures. For each old parser reused, supply a field-by-field width/read audit. This document does not claim that audit or refactor is already implemented. A selected maximum exceeding a narrow parser's admitted region cannot be accepted until that boundary is corrected.

### 4.2 Safe arithmetic rules

These are mathematical preconditions, not code or a new utility API:

- For a region of length L, start o and requested n: first establish o <= L, then n <= L - o. Validate arithmetic before forming pointers or copying.
- A frontier may equal the region length. An element index may not. Do not force end-exclusive state into an inclusive-last-byte representation without handling empty input and exact end.
- Validate nonzero/empty behavior before subtracting one. The legacy getConsumed offset+1 and minimal-length-first checks require the O03 D01/D03 boundary treatment, not blind compatibility copying.
- Validate wide addition/multiplication before converting to size_t or multiplying pool count by stride. Startup arithmetic can overflow too.
- A nominal three-byte field read must not require an unvalidated fourth byte. Do not add padding to hide a public span contract violation.
- Failure must not silently accept a truncated CH, locator list, canonical stream, identity or suppression set. If a resource limit prevents required work, report the appropriate resource disposition without successful CH publication.

## 5. Joint-peak model: actual simultaneous storage

**DESIGN PROPOSAL:** size by physical backing that is live at the same instant, not by the maximum of isolated happy-path phases. Count each physical block once; two logical roles can alias only when the same bytes/context genuinely serve both and mutation/lifetime permit it.

Let Live(t) be the set of distinct retained backing blocks at processing point t. The required retained capacity covers the maximum, over reachable t, of the sum of their actual aligned extents. This is a sizing condition, not a computed byte budget. For separate pools, apply the condition per pool; spare bytes in one pool do not satisfy a full descriptor pool elsewhere.

An accounting worksheet must include:

- Engine fixed structures, existing session-extension table/storage, existing TCP continuation resources and any newly retained QUIC state. Do not count the same embedded SegmentBuffer/digest twice.
- Each live acquisition's raw CH/CRYPTO backing, ranges, locators, canonical bytes/digest state and scalar CH facts where physically distinct.
- Active semantic facts and active attempt association even when raw CH acquisition is no longer needed.
- Isolated candidate work, including a deferred candidate surviving input end.
- Retired suppression metadata and old backing still borrowed during replacement. Retired identity is not automatically a retained full CH.
- Published views whose backing remains live through callbacks, reduction, permitted commit and every final internal read. Publication is a role, not automatically an extra copy.
- Current datagram/plaintext scratch, provider contexts, sort/range-normalization temporary storage, stack frames, pool metadata/alignment and any required bounded mitigation/reporting handoff data.

| ID | Reachable overlap | Capacity obligation |
| --- | --- | --- |
| J01 | Existing active CH/attempt plus isolated candidate | Preserve active authority/backing while candidate evaluates; do not assume candidate overwrites active slot |
| J02 | Candidate complete/published while old active facts or views remain needed | Candidate backing survives callback/commit; old backing not reusable until its own last use. No automatic third full-CH copy |
| J03 | Deferred candidate, later datagram scratch and active attempt | Deferred bytes cannot live in scratch; new input must not overwrite pending candidate or silently admit another |
| J04 | A partial, B complete/published, then A resumes | Retained A and B state plus current scratch may coexist even on a single-threaded Engine |
| J05 | New attempt accepted, old identity retired | Suppression metadata survives required lifecycle; old full backing retained only while actually needed |
| J06 | Range insertion bridges gaps while old descriptors/locators remain referenced | Preflight descriptor/byte/temporary needs for the actual algorithm; normalized final range count alone is insufficient |
| J07 | CH completion needs sorting/canonical bytes and retained raw extension values | Digest size is not raw CH size; locator sorting may need stable raw backing until canonical feed/final readers end |
| J08 | Resource reclaim or aging occurs during a callback/helper path | Borrowed backing cannot be victim; actual mutation/callback call graph must prove safe reuse |
| J09 | Cross-record TCP CH and later separately published record | No per-fragment Record queue is required, but CH facts retained for later Complete callbacks remain accounted |
| J10 | Error/mitigation/visibility path before final release | Bound any additional scratch/copies and honor D04 failed-step list visibility; no hidden allocation in error reporting |

A/B/A does not mean one workspace is sufficient because execution is sequential. Sequential mutation and simultaneous retained lifetimes are different. Conversely, summing active + candidate + published + retired as four full CH buffers overcounts when roles share the same immutable backing. Resolve aliasing in the concrete layout, not by a new generic pool abstraction.

### 5.1 Finite admission, not unbounded deferred work

No finite pool can promise arbitrary concurrent candidates, ranges, sessions or indefinitely growing suppression history. Define finite admitted populations and the applicable existing lifecycle/resource outcome. A candidate slot limit is not permission to merge candidates, erase active state or guess ambiguous association. If a new candidate cannot fit without violating active/borrowed data, do not begin mutating that protected state.

The existing accepted aging/reclaim path is the integration point. Do not add a new timeout, background collector, heap queue or automatic IncompleteHandshake violation. If the accepted lifecycle cannot bound required retired/pending metadata, that is a concrete unresolved capacity/lifecycle conflict, not a reason to invent an expiry value.

## 6. Sparse CRYPTO, canonical bytes and work bounds

### 6.1 Sparse ranges

A small unique-byte count can still require many disjoint range descriptors. A high offset can be encoded with very few received bytes. Therefore B_CRYPTO, N_ranges and O_CRYPTO are independent checks. Duplicate bytes do not advance CH accepted frontier or hash progress. Retain only data allowed by the accepted acquisition window/message policy; this review does not choose a new window or overlap-conflict policy.

Before mutating coverage, determine whether the actual insertion algorithm can represent the new normalized coverage and any transient split/merge descriptors in prepared storage. Do not allocate a temporary vector merely because final coverage fits. A bounded two-pass approach or prepared workspace may be evaluated in implementation design, but neither is mandated as a new architecture here. On failure, preserve coherent prior association/frontier; never expose partial coverage as if the whole region was accepted.

A ResourceFailure after a digest contribution remains nonresumable under O01; preserving acquisition consistency is not permission to retry a partially modified digest. Prefer preflight for known capacity needs, but do not invent transactional rollback machinery to conceal provider failures.

### 6.2 Canonical FP/FullString

B_canon must be derived from actual append behavior: base fields, cipher contributions, delimiters, sorted extension type/length/content and selected extension content. Do not assume canonical length equals raw CH length, or that removing GREASE always makes it smaller enough for the old array. Include every supported selected extension contribution, including the QUIC-relevant extension path already identified by O03. Preserve FP and FullString semantics; no silent canonical truncation or alternate fingerprint variant.

For a proposed admitted input domain, prove that every successful required canonical append fits the selected provider's actual usable extent. If some CH within H_body cannot fit B_canon/N_loc, either select adequate storage or explicitly disposition that supported-domain restriction; do not claim H_body alone guarantees successful CH evaluation. Current strict digest exact-fill failure is a code fact, not a recommendation to waste a byte in a new provider.

### 6.3 Runtime work is not bounded by memory alone

Finite byte/range/locator capacity bounds retained state, but does not alone bound repeated duplicate frames, packet count, sort comparison cost or per-input traversal. Sorting compares extension contents as well as type/length in the supplied code. O07 must account for bytes inspected, comparisons, repeated validation, provider work and failure/aging paths. Do not invent a frame/packet-count cap that rejects otherwise accepted input merely to claim a CPU bound. A new processing budget would be a product/behavior decision, not an O06 default.

## 7. Startup preparation and runtime exhaustion

### 7.1 Startup

The selected deployment profile must resolve numeric parameters before activating the feature. Compute aligned extents and counts with checked arithmetic; prepare backing, descriptor arrays, canonical storage, association/suppression state, scratch and provider contexts. Verify each required acquisition/initialization result and all dependencies before marking that profile usable. A failed startup preparation must not leave partially initialized runtime state advertised as ready. Exact product activation/failure reporting policy remains existing configuration/lifecycle authority, not invented here.

Startup allocation permission is not permission to grow after the first packet. No runtime lazy initialization, resize/reserve, new provider context, dynamic sort workspace or allocating error formatter may be hidden behind an apparently bounded API. O07 supplies implementation evidence.

### 7.2 Exhaustion disposition matrix

| ID | Exhausted or invalid quantity | Required behavior boundary |
| --- | --- | --- |
| X01 | Declared CH extent exceeds selected supported maximum | Distinguish product/resource limit from structurally impossible declared body. No truncation or successful CH; exact diagnostic/step mapping requires accepted policy |
| X02 | Retained bytes or candidate slot unavailable | Preserve active authority and protected views; no automatic replacement or heap fallback; use explicit applicable resource disposition |
| X03 | Range descriptors/transient normalization space unavailable | Do not publish partial insertion as full acceptance; retain coherent prior state or terminate affected acquisition per contract |
| X04 | Locator/canonical/provider capacity fails | O01 nonresumable failure; no successful FP/CH publication or retry of changed digest. Preserve TCP D04 site-specific routing/list effects |
| X05 | Existing record collection full | Preserve existing TCP exceptional diagnostic/visibility unless explicitly changed; no new per-contributing-record entries just to fill it |
| X06 | Suppression/association storage full | No resurrection, arbitrary identity truncation or ambiguity merge. Exact bounded lifecycle/resource policy remains an explicit open item |
| X07 | Reclaim victim still borrowed | Do not overwrite; use applicable bounded failure/defer path. Existing FIFO call site alone is not proof of victim eligibility |
| X08 | Checked arithmetic/conversion fails | Reject before pointer formation/copy or state mutation where contract requires no mutation; do not reinterpret overflow as NeedInput |
| X09 | Startup preparation incomplete | Profile not usable until required preparation succeeds; no runtime allocation repair |
| X10 | Callback Stop / actual violation coincides with resource outcome | Preserve Conductor actual-verdict reduction and last-use order; resource failure does not manufacture a violation or authorize next pull |

The matrix specifies safety boundaries, not a new universal ParsingError, automatic packet drop or enforcement policy. Exact legacy failure-site behavior remains O03 D04 and O04 reader review. Resource eviction alone does not generate IncompleteHandshake violation or prove handshake completion.

## 8. TCP and QUIC cross-document alignment

| Contract | O06 treatment |
| --- | --- |
| O01 shared algorithm | Wide logical accepted/checkpoint domain; no consumed/accepted conflation, duplicate hash or resume after parser/resource failure |
| O02 orchestration proposals | Same storage/width obligations for B1/B2; no new class/hook or inheritance selection |
| O03 D01/D03 | Missing bytes versus proven contradiction retained; bounded one-past-end arithmetic does not silently accept malformed input |
| O03 C-O03-01/D02 | No intermediate physical-record publication; final record length not used as total CH capacity |
| O03 D04 | Failure-site record-list/diagnostic distinctions preserved; no blanket clear or generic anomaly |
| O04 | Existing viewers remain; FullString/record references borrow backing through full last use; external readers still unverified |
| O05 | Candidate isolation and explicit disposition; one active TLSData, correct A/B/A, terminal/live distinction and existing reclaim integration |
| QUIC Initial/CRYPTO | Separate decoded wide offsets, sparse bytes/ranges and CH-relative frontier; no raw protected-field classification |
| Retry-compatible restart / HRR-compatible CH2 | Same attempt policy; do not reserve an unbounded history of full CHs or infer observed server signals |
| Retired suppression / ambiguity | Bound metadata explicitly without early erasure, merge or reset; exact values/lifecycle remain open |
| D-TIME | Retain scalar completing-datagram arrival timestamp across deferred decision; no callback-time substitution |
| 0-RTT / Finished | O08 V3 selects the associated post-CH client Handshake heuristic, including ACK-only; 0-RTT is not a trigger. After successful completion/publication and permitted commit of the active attempt's CH acquired from eligible Initial CRYPTO data, the attempt can be InProgress. Only a subsequently observed, structurally qualifying, associated client Handshake packet can then move InProgress -> Finished, including ACK-only. A Handshake observed before complete authoritative CH does not establish InProgress or Finished and is not latched for later completion. No Handshake payload decryption or TLS Finished detection is required. Finished is the accepted product heuristic, not cryptographic handshake verification. |
| TLS-Enforcement authority | Nonblocking no-TLS1.3 finding permits full parse/FP; resource/structural failures do not create violations; TCP behavior not automatically changed |
| Mitigation / counters / visibility | Account for required synchronous readers/handoff before release; no mitigation design or new reporting component selected |

This is scoped alignment with available contracts, not a claim all FRD/SRS requirements have been re-read. No numeric maximum or new overlap/retransmission policy is introduced.

## 9. Planned validation and boundary cases

**NOT RUN:** these are required checks for the eventual selected values/layout and implementation, not synthetic execution results.

| ID | Case | Expected proof |
| --- | --- | --- |
| V01 | CH just below, at and above selected H_total | No wrap/truncation; exact supported/resource outcome and full required parse at admitted boundary |
| V02 | Offered span ends inside every header/length field | O01 continuation correct; no out-of-region read, fabricated error solely for cut or duplicate FP contribution |
| V03 | Local field fits uint16 but containing region/frontier does not | Checked wide containing arithmetic; no narrowing before validation |
| V04 | Legacy digest append ends below, at and above array extent; zero append | Exact current strict-boundary oracle distinguished from target provider; no false exact-fill success |
| V05 | Small byte count, many disjoint CRYPTO ranges; high sparse offset | Independent descriptor/window/byte limits; no allocation up to offset |
| V06 | Insertion bridges several ranges with transient workspace pressure | Actual peak fits or safe resource outcome before inconsistent publication |
| V07 | A partial, B complete, A resumed | Retained states coexist; all CH fields and borrowed backing belong to selected context |
| V08 | Active + candidate + published view + retired identity | Count unique physical blocks; no aliasing before last use or automatic four-full-buffer assumption |
| V09 | Deferred candidate plus new input while slots full | No packet borrow, unbounded queue, active overwrite or inferred acceptance |
| V10 | Locator/canonical/provider failure after earlier contribution | No digest retry or successful CH; exact TCP D04 route and actual verdict authority |
| V11 | Reclaim/aging/helper path selects borrowed backing | No overwrite or stale decision after session/slot reuse; no resource-induced IncompleteHandshake violation |
| V12 | Cross-record CH plus later record | Single selected projection and feedback barrier; recordLength not CH total |
| V13 | Startup count*stride/alignment overflow or partial preparation failure | Checked sizing; no usable partially initialized profile or runtime allocation repair |
| V14 | Runtime success/error/mitigation/visibility/aging paths | O07 instrumentation/source proof of no allocation/growth and bounded actual work |
| V15 | Maximum canonical contribution shape within admitted CH domain | B_canon/N_loc adequate or explicit restriction disposition; no alternate FP/FullString |
| V16 | Suppression full and late old-attempt traffic | No silent suppression loss/resurrection; accepted bounded lifecycle policy applied |

## 10. Double-check and disposition

### 10.1 Static checks performed

The first pass mapped source declarations/guards to separate byte/count/offset domains. The second pass checked strict digest exact-fill behavior, actual 1024 locator array versus the separate 100 constant, narrow parser boundaries, unique-block peak accounting, last-use retention, deferred candidates, resource-versus-malformed distinction and O03 publication compatibility. This is scoped static reasoning, not an exhaustive implementation audit.

| Check | Result / scope |
| --- | --- |
| Baseline integrity | Input bytes/hashes preserved; no production or prior document overwritten |
| No invented numeric settings | Current code values labeled facts; P01-P16 remain explicit open parameters |
| Width correctness | O01 uint32 logical proposal retained; no unchecked whole-CH uint16 conversion, no public record-length widening |
| Joint peaks | J01-J10 include candidate/deferred/published/retired/scratch overlap; no double-counted alias assumption |
| Failure semantics | X01-X10 retain actual verdict authority, nonresumable shared failure and D04 distinctions |
| Requirements honesty | Primary-source and consumer gaps remain; no full FRD/SRS or runtime PASS |

Mechanical anchor/count/hash checks accompany the document but do not establish semantic or runtime PASS.

### 10.2 What is complete and what remains open

**O06 analysis delivered:** concrete current-code capacity inventory, symbolic parameter ledger, width migration direction, joint-peak obligations, startup/exhaustion boundaries and validation specification.

**Storage/layout implementation acceptance remains BLOCKED at P01:** no supported maximum CH body extent has been selected. This is an intentionally deferred product/support decision, not a newly discovered code defect and not a request to invent a value. Closure review of concrete layout stops here; remaining numeric parameters, exact physical storage/provider fit and reader/source gaps are inventoried, not passed gates. Even after P01 is selected, B_canon/N_loc/ranges/association/joint peaks and parser conversion proofs still require resolution.

The user's request authorizes this O06 design analysis despite prior evidence gaps; it does not close O04/O05 or authorize implementation. O07 may use this parameterized handoff for focused provider/no-allocation review when instructed, without pretending numeric capacities or O06 implementation acceptance are complete. O08 V3 is the current accepted completion-policy reference; no completion-selection blocker remains. No automatic progression or new architecture decision is made.

## 11. Handoff to O07 without new budgets

O07 receives the exact existing digest guard and FullString requirement, prepared-storage inventory P01-P16, width boundaries, J01-J10 peaks and X01-X10 failure constraints. It should inspect actual provider initialization/update/finalize, scratch, callbacks, sort/range work, failure formatting and aging. It must not require unavailable CPU/RAM budgets to inspect those paths, choose arbitrary capacities, infer allocation-free behavior from fixed-array names, or change fingerprint semantics to fit the old buffer. Missing numeric settings block concrete storage acceptance, not useful source-level provider analysis.

## 12. Source snapshot identities

- `TLS-Flow-All-Code.txt`: SHA-256 `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b`.
- `tls_flow_o01_shared_ch_contract_v4.md`: SHA-256 `53b1c8d6556c5ba57b6bb6f127994d895118334630341abd9f47db9142360a0a`.
- `tls_flow_o02_orchestration_comparison_v2.md`: SHA-256 `21703f9678000b24384669c3ceeaf18da4882238c7a7ffa9204ab4ba7f58aa3f`.
- `tls_flow_o03_tcp_migration_review_v7.md`: SHA-256 `201ebc533aecae65de3cc7d2435b49f9e542fb2699dd0bd0258eec2a1b4397d1`.
- `tls_flow_o04_viewers_readers_review_v2.md`: SHA-256 `2517c6bf9bf3efbbd5f5bff6e7e1d6a2eb65eebc1bd04796d96dee5aa027e896`.
- `tls_flow_o05_candidate_disposition_ownership_v2.md`: SHA-256 `a32e7fb96ed7b14623617f7d0eeb0ad33fea5fb0e638a077b977f288da5226c1`.
- `tls_flow_strategy_template_continuation_consolidated_v6.md`: SHA-256 `8f4748e82a9fa80335cf58adb8218ebd80d524573541dcc5e68f4014648d9590`.

## V2 focused R01 coordination

Only completion-policy references are synchronized. Numeric capacity/layout choices and runtime acceptance remain at their existing closure points, not prerequisites for continuing design consolidation.
