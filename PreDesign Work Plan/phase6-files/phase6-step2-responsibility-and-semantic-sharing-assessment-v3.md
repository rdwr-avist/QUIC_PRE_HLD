# Phase 6 Step 2: Responsibility Map and Semantic-Sharing Assessment V3

## 1. Outcome and authority

**Step 2 complete at the static assessment boundary; ready to define candidates in Step 3.** This document supplies comparison evidence, not a selected architecture, component diagram, API contract, implementation or performance certification.

Inputs are the unchanged Step 1 V2 baseline, manifest and readiness recheck; original P2, P3 V5, P4 V4, P5 V7, PLAN V5, CODE and CONS are identified by the Step 1 hashes. P3 governs semantic/feedback/lifetime contracts, P4 orchestration/commit and accepted TCP exceptions, P5 ownership/bounds and later clarifications. P2 supplies attempt/state semantics. Current code facts do not override accepted target corrections. No external RFC/product requirement is introduced in this step. V1 is preserved; V2 adds the omitted TCP same-input Complete guard as D27 after the first substantive review failure, without changing accepted policy.

### Classification vocabulary

- **X:** protocol-specific extraction, protection processing, framing, reconstruction or association.
- **S:** potentially shared TLS semantic processing or state policy.
- **O:** potentially shared orchestration, dispatch, verdict handling or lifecycle coordination.
- **I:** intentionally protocol-specific behavior, with its shareable consequence separated explicitly.
- **U:** can move unchanged in semantic behavior after its existing preconditions are satisfied; this does not promise literal source-code reuse.
- **G:** needs generalization of evidence, applicability, access or lifecycle boundaries.
- **P:** retain protocol-specific predicate/mechanism. This does not require a separate class or forbid a guarded protocol branch inside shared policy.

All X/S/O/I and U/G/P placements are **assessment proposals** for comparison. The underlying P2–P5 rules are **accepted**; the source descriptions are **current-code facts**.

## 2. Current TCP path: entry through disposition

| Stage | Current code behavior and control return | Accepted boundary / overlay |
|---|---|---|
| 1. Wiring | Engine Factory creates parser/Processor/viewer and Conductor wiring; Session events enter onEvent. | Startup construction is not proof all runtime paths are allocation-free. P5 5/7. |
| 2. New session | A01 calls A11; live listeners determine masks, absent eligibility/allocation returns Continue; successful creation registers inbound/aging observers. | Preserve participation; no semantic parsing without context. P4 G01/T01. |
| 3. Inbound reuse | A02 obtains extension and handles TCP SYN-without-ACK reuse before process; absent extension returns Continue. | TCP trigger retained; QUIC qualification differs. P4 T07/Q09/Q10. |
| 4. Early paths | A03 handles TCP reset, silent terminals, empty payload and terminal payload before normal stepping. | Preserve ordering and exception verdict semantics; QUIC terminal candidate gate precedes new CH parsing. P4 4/T10/Q10/Q19. |
| 5. Begin input | A20 receives payload/session/TCP sequence; prepares packet-local state and resumes/qualifies continuation. | TCP sequence logic remains specific; accepted restoration/isolation fixes are obligations, not applied patches. P5 D6-01. |
| 6. Process unit | A21/A22 select new record, CH extension continuation, opaque continuation or alert continuation; expose step outcome and viewers. | No automatic whole-handshake buffer; current-input validity differs from attempt legality. P3 4/5; P4 T03/T05. |
| 7. Outcome routing | A04 sends RecordParsed to A05; Stop to parsing-error feedback; NeedMoreData/EndOfPacket return; Unsupported commits accepted disposition. | Unsupported has no consumer dispatch. Parser Stop is distinct from observer Stop and drop. P4 4/T08/T09. |
| 8. Ordinary projections | A05 calls parsing-error list, record list, then CH list if Complete; A31 enforces priority/live listeners and within-list Stop. A07 builds callback-local CH data; nested viewer backing remains independently borrowed. | Preserve U05 retained-Complete callback exception for TCP, not a new QUIC CH event. P3 5/8; P4 D4-02/7. |
| 9. Feedback | A05 ORs actual masks; Stop+nonzero immediately commits Error; zero-mask Stop latches and permits remaining applicable current-unit projections. | No rollback of performed consumer side effects; no network drop implied. P4 4. |
| 10. State disposition | Nonzero masks suppress normal commit. Otherwise A12 applies record-specific state policy and A13-A16 field updates. | Const consumer TLSData is precommit live state, not immutable snapshot. P4 5. |
| 11. Reclaim/next | A05 marks newly Finished; A03 loops only on RecordParsed plus updated active state and no returned Stop. | Last uses and required identity must survive; mark failure is not success; no next authority before feedback. P4 G08/G09; P5 5. |
| 12. Lifecycle | A10 notifies SessionEnd then releases; A29 removes reclaim membership and resets/erases extension. SegmentBufferManager separately owns continuation cleanup. | Aging is not the ordinary transaction; resource eviction does not manufacture an Incomplete Handshake violation. P5 4/5. |

## 3. Responsibility map with accepted QUIC overlay

| ID | Responsibility | Current TCP evidence | Accepted QUIC responsibility | Class | Sharing assessment / reason | Contract anchors |
|---|---|---|---|---|---|---|
| R01 | Participation and session admission | A01/A11: gather live listener event masks and allocate an Extension only for participating sessions. | Apply accepted eligibility before semantic QUIC processing; do not infer a new port/profile policy. | O | Common participation intent; current listener masks and startup-prepared storage must be preserved. | P3 8; P4 T01/Q01, G01; P5 3/5 |
| R02 | Input framing and cursor | A03/A20/A23: packet payload, TCP sequence, record headers and continuation cursor. | UDP datagram/coalesced packet bounds, version/header parsing and packet-local protection bounds. | X | Different framing and reconstruction coordinates; a TLS record is not a QUIC packet. | P3 4; P4 T04/Q01/Q04; P5 D6-06 |
| R03 | Protection processing | No QUIC header-unprotection/Initial AEAD path exists in the supplied TCP Processor. | Initial key/context selection, header unprotection, PN reconstruction and AEAD verification within accepted bounds. | X | Protocol-specific cryptographic extraction; Initial verification is not peer authentication. | P2 2; P4 Q14; P5 D6-06 |
| R04 | Continuation and reconstruction | A20-A25: TCP sequence/retransmission handling, extension reassembly, opaque tracking and alert reassembly. | Initial CRYPTO offsets/ranges/gaps/overlap, bounded candidate acquisition and reconstruction. | X | Storage/lifetime discipline can align; byte-ordering and gap semantics cannot be equated. | P3 5; P4 T03/T05/Q03/Q05/Q06; P5 5/6 |
| R05 | Association and attempt qualification | A02: SYN-without-ACK reuse rule based on active state and stored violations. | Same DP tuple plus retained identity/association, retired suppression, ambiguity, Retry-compatible restart and complete-new-attempt qualification. | X | Wire-specific evidence must precede any shared accepted-reset action. | P2 8-10/12/14; P4 T07/Q08-Q10/Q19; P5 4/5 |
| R06 | CH semantic evaluation | A23/A24 plus CH/extensions parsers: fields, supported-version offer, SNI, early-data offer and fingerprint. | Evaluate qualified reconstructed CH with the same accepted algorithm/output and field meaning. | S | Strong sharing opportunity after qualified byte availability; preserve staged TCP access and legacy algorithm behavior. | P3 4/5; P4 D4-02/Q02; P5 D6-01 |
| R07 | Local validity versus flow legality | Processor local syntax/error outputs; CONS Enforcement Manager consumes current facts. | Supported packet/frame/CH encoding findings versus attempt-wide CH legality. | S | Share semantic findings where representable, not fabricated TLS-record errors; Enforcement remains flow-legality owner. | P3 4/6; P4 D4-01/Q15/Q16 |
| R08 | Observation eligibility | A05/A07 use Complete viewer status; accepted TCP U05 permits retained-Complete dispatch. | Fresh qualified observation only; duplicates/retired/ambiguous acquisition do not become new CH events. | S | Generalize eligibility/provenance while explicitly retaining TCP U05, not a universal once-only callback rule. | P3 5/8; P4 D4-02/G05; P5 D6-01 |
| R09 | Projection and observer delivery | A06-A08/A31: parsing error, record and CH lists, priority and weak-listener handling. | Deliver supported semantic projections to existing observers; no fake TCP record/CCS/reset event. | O | Shared delivery mechanics; event applicability and missing QUIC event representations remain explicit. | P3 6/8; P4 4/7 |
| R10 | Verdict transaction | A05/A31: actual masks, within-list Stop, latched zero-mask Stop and final disposition. | Same accepted feedback boundary before next semantic authority. | O | Reducer is shareable; parser Stop and terminal/reset exceptions cannot be flattened into it. | P3 FB-B/FB-C/FB-D; P4 4 |
| R11 | Normal CH state commit | A13: authoritative CH fields, count, timestamp and PreHandshake to InProgress. | Same logical commit for accepted CH; duplicate suppression/CH2/new-attempt qualification occurs first. | S | Share qualified field update and state meaning, not raw viewer availability or packet timestamp sourcing. | P4 5/Q02/Q07; P5 2/5 |
| R12 | Completion evidence and policy | A12/A14/A15: CCS, later handshake record, TLS13 ApplicationData and early-data-offer condition. | Unambiguously associated client v1 Handshake Long Header, possibly bounded deferred evidence before authoritative CH. | I | Evidence is intentionally separate; permitted Finished commit and lifecycle consequence can be shared. | P2 5/6/9; P4 T11/Q11; P5 1/2/5.0 |
| R13 | Alert interpretation | A26/A16: readable TCP alert qualification and terminating-alert policy. | Readable associated Initial CRYPTO_ERROR close follows accepted TerminatedByAlert disposition. | I | Share terminal meaning/commit guard, not plaintext alert framing, encryption assumptions or arbitrary close interpretation. | P4 T11/Q17/Q18; P5 2 |
| R14 | Error/violation/Unsupported disposition | A17-A19: stored mask, Error and current TCP Unsupported conditions. | Accepted resource/support exhaustion and context loss dispositions; Unsupported excludes consumer dispatch. | S | Mask/Error actions can share; cause qualification and TCP-specific Unsupported predicates need generalization. | P3 FB-D; P4 4/5; P5 D6-04/D6-07 |
| R15 | Terminal gating and late input | A03/A09 and TLSData predicates: silent Finished/Unsupported; terminal payload for Error/TerminatedByAlert. | Classify terminal candidates/retired input before new CH parsing; same-attempt terminal input cannot reopen state. | I | Share terminal-state meanings, not unconditional TCP entry/terminal-payload routing. | P2 12; P4 T10/Q10/Q19; P5 4/5 |
| R16 | Reclaim and cleanup | A05/A10/A29/A30: Finished marking, aging callback, extension release and table membership. | Eligible committed Finished plus last use, required terminal identity retention, stale membership clearing and bounded cleanup. | O | Common lifecycle coordination with resource-specific ownership and safe failure handling; no global allocator is implied. | P3 5; P4 G09; P5 3-6 |
| R17 | Reset and replacement | A02/A28/A29: clear TLSData and release/rebuild extension on qualified TCP reuse. | Isolated candidate acceptance, rollback, old/new ownership transfer and retired identity. | S | Default-value reset action can share; accepted replacement is not a blind clear or identical release/rebuild sequence. | P2 10/14; P4 T07/Q09/Q10; P5 D6-02/D6-05 |
| R18 | Startup and runtime bounds | Engine Factory/Application wiring, Extension/table, Processor/SegmentBufferManager ownership. | Prepare bounded keys, ranges, acquisition, canonical CH, result backing and indexes at startup. | O | Shared lifecycle/bound discipline; no runtime allocation/growth and no invented numeric capacities. | P5 3/5/6/9.2 |
| R19 | Consumer side effects | CONS Enforcement, Fingerprint and Visibility callbacks consume current fields and session/profile context. | Preserve accepted CH integration and explicit capability limitations. | I | Consumer mitigation/reporting storage is not automatically transport-neutral; no automatic QUIC TCP-reset action. | P3 8; P4 7; P5 3/5 |

## 4. TCP transition and decision assessment

This inventory includes both writers and read-only state-dependent decisions in the Conductor, Processor and lifecycle path. It is not restricted to functions named state machine. U means semantic relocation is possible, not that callback timing, storage access or function signatures already match a shared interface.

| ID | Existing decision / anchor | Current behavior | Move | Boundary and reason | Accepted authority |
|---|---|---|---|---|---|
| D01 | A28 TLSData defaults/clear | All semantic fields return to defaults. | U | Same value-reset action; authorization and old-resource cleanup are outside this action. | P4 5; P5 5 |
| D02 | A27 active/terminal/silent predicates | PreHandshake/InProgress/ChangeCipherSpec active; Finished/Error/Unsupported/TerminatedByAlert terminal. | G | State meanings may share; ChangeCipherSpec is TCP-only and terminal input routing is protocol-specific. Do not depend on enum ordering as a universal protocol rule. | P2 4/5; P4 G04 |
| D03 | A02 SYN reuse | SYN without ACK rebuilds missing/eligible old extension; active/no-recorded-violation branch retains context. | P | Keep TCP trigger/predicate. QUIC candidate acceptance is different; share only the authorized reset/replacement consequence. | P4 T07; P2 10/14 |
| D04 | A03 TCP RST | Notify TCPReset; Stop+mask Error, Continue+mask stores mask, no-mask clears TLSData. | P | TCP reset event and no-mask clear are not QUIC close semantics. Shared verdict primitives may be reused without changing this exception. | P4 T10; P4 4 exceptions |
| D05 | A03/A09 terminal routes | Finished/Unsupported silent; Error/TerminatedByAlert terminal payload path before normal parsing. | G | Keep TCP behavior; QUIC must qualify new-attempt candidates and suppress retired/ambiguous input before equivalent terminal decisions. | P4 T10/Q10/Q19 |
| D06 | A13 CH completion guard | Commit CH only if current viewer reports Complete. | G | Use qualified authoritative CH readiness/provenance, not generic retained Complete. Preserve U05 callback behavior separately from CH commit. | P3 5/8; P4 5 |
| D07 | A13 CH field updates | Increment CH count, store timestamp/fingerprint/version mask/earlyDataOffered. | G | Field meanings share; valid current backing, duplicate qualification, CH2 and timestamp sourcing must remain correct. D-TIME is not upgraded from proposal to requirement. | P4 5; P5 D6-01 |
| D08 | A13 PreHandshake to InProgress | First permitted CH normal commit starts active handshake. | U | Same state transition after protocol-specific qualification and feedback. No need to keep this policy inside a TCP-only path. | P2 4/6; P4 Q02 |
| D09 | A14 pre-CH CCS | Increment preCcsSeen if CH count is zero. | P | No QUIC CCS surrogate or 0-RTT mapping. | P4 5/T11/Q12 |
| D10 | A14 InProgress to ChangeCipherSpec | After CH, non-TLS13 CCS enters ChangeCipherSpec. | P | TCP evidence and state branch; QUIC does not traverse it. A shared policy may contain a guarded TCP branch, not require separate policy classes. | P2 5; P4 T11 |
| D11 | A12 ChangeCipherSpec to Finished | A later handshake-content record completes this TCP branch. | G | Keep the TCP record/state evidence predicate; generalize only permitted completion disposition, not encrypted payload parsing. | P4 T11; P5 2 |
| D12 | A15 pre-CH ApplicationData | Increment earlyDataCount when CH count is zero. | P | TCP record observation only. QUIC 0-RTT changes no handshake state/counter by analogy. | P4 5/T11/Q12 |
| D13 | A15 TLS13 completion | InProgress + TLS13 ApplicationData + no earlyDataOffered enters Finished. | G | Keep TCP condition intact; QUIC completion uses associated Handshake evidence. Shared Finished action must not infer early-data acceptance or server Finished. | P2 5/9; P4 T11/Q11 |
| D14 | A16 TerminatedByAlert | Active + readable alert + terminating classification enters TerminatedByAlert. | G | Share qualified termination disposition; TCP plaintext visibility/alignment and QUIC associated Initial CRYPTO_ERROR extraction remain separate. | P4 T11/Q17/Q18 |
| D15 | A18 violation accumulation | OR actual violation mask into terminalViolationMask. | U | Same semantic storage action; reset on accepted new lifecycle and keep current accumulator separate. | P4 4/5 |
| D16 | A19 Error | Set Error and OR violations. | U | Same accepted Error action; invocation follows actual verdict contract, not parser Stop or resource exhaustion alone. | P3 FB-C/FB-D; P4 4 |
| D17 | A17 Unsupported | Current code tests continuationGap, PreHandshake, or InProgress with Partial CH. | G | Do not transplant this predicate to QUIC. Apply accepted D6-04 required-context/support exhaustion policy and preserve cause qualification; no callback on Unsupported. | P4 T09/Q16; P5 D6-04 |
| D18 | A05/A31 normal-commit permission | Nonzero accumulated mask suppresses normal commit; Stop+nonzero Error; Stop+zero can still permit normal update. | U | Reducer policy can move unchanged in meaning; adapters and exception paths remain distinct. | P3 FB-C; P4 4 |
| D19 | A03 next-record permission | Continue only on RecordParsed and updated active state; observer Stop returns immediately after resolved current unit. | G | Share feedback-before-next-authority discipline, not RecordParsed as a universal QUIC cursor. | P3 FB-B/FB-E; P4 G08 |
| D20 | A05/A30 Finished reclaim | Compare previous/current state, mark newly Finished; table API returns bool. | G | Share transition-triggered eligibility, honor last uses and marking failure, preserve required identity; marking is not release. | P4 G09; P5 2/5 |
| D21 | A10/A29 aging | Notify SessionEnd, ignore returned verdict at this caller, release extension. | G | Keep current TCP exception; coordinate QUIC cleanup and borrows. Resource eviction is not synthetic SessionEnd/Incomplete Handshake. | P4 4/T10; P5 4/5 |
| D22 | A23 Unsupported parser gate | processNewRecord returns EndOfPacket when state Unsupported. | G | Read-only policy gate may align with shared terminal permission, but must not bypass QUIC terminal candidate classification. | P4 G03/G04; P5 5 |
| D23 | A23 opaque-record selection | State/version plus record type selects opaque continuation for pre-CH ApplicationData, InProgress TLS13 ApplicationData/CCS, and post-CCS handshake. | P | Keep TCP record/visibility selection. QUIC ciphertext is not a record to track by TCP sequence. | P4 T03/T11; P5 D6-01 |
| D24 | A26 and startSegmentedAlert | PreHandshake/InProgress controls plaintext alert visibility and continuation mode. | P | TCP encryption/alignment decision; QUIC Initial readable-close extraction is a different proof. | P4 T11/Q17/Q18 |
| D25 | A20/A21 continuation decisions | Pending continuation sequence/gap/retransmission selects resume, NeedMoreData or Unsupported. | P | TCP gap is not QUIC CRYPTO gap. Share bounded failure vocabulary only where semantics match. | P3 4/5; P4 T05/Q03/Q05 |
| D26 | A04 parser Stop | Parsing-error callbacks and actual verdicts determine disposition; parser Stop itself is not observer Stop. | G | Preserve exception and distinct channels. Do not force a universal normal-record transaction on failed extraction. | P3 FB-D; P4 4/T08/Q15 |
| D27 | A23 same-input Complete guard, Processor.cpp extracted L947-952 | A later CH encountered while current CH status is Complete produces MultipleClientHello and Error parser result. | P | Preserve the accepted legacy TCP local guard. Do not add a QUIC datagram-local CH-count guard or an attempt-wide max-CH gate. Additional qualified QUIC CH, including third CH, remains subject to actual Enforcement feedback, not automatic Processor rejection. | P4 T06/Q07 and B01; P3 U05 |
| D28 | A23 shouldParseHandshake, Processor.cpp extracted L900-907 | Parse a TCP handshake header only in PreHandshake or InProgress and only for a handshake-content record. In ChangeCipherSpec the record is not parsed as plaintext handshake; A12 uses the record/state evidence for the accepted Finished transition. | P | Preserve this TCP plaintext/opaque selection independently of CH semantic evaluation. QUIC CH parsing requires qualified Initial CRYPTO bytes and accepted attempt/terminal gates, not this TCP state/record predicate. A shared evaluator must receive only bytes whose visibility and applicability have already been qualified. | P4 T11/Q02/Q10; P5 D6-01; P3 4/5 |

### QUIC-only policy that must be overlaid, not inferred from TCP

- **Deferred completion:** associate client Handshake Long Header evidence under P2 9/P5 5.0, retain a bounded idempotent fact before authoritative CH, and consume it only under permitted CH/feedback/commit conditions. It is not observed TLS Finished. TCP has no identical deferred-evidence mechanism.
- **Attempt replacement:** active candidate evaluation is isolated until accepted; terminal candidate classification precedes CH parsing. A complete new attempt replaces active state as session reuse. Retry-compatible restart and HRR-compatible CH2 remain the same attempt without proving server Retry/HRR. P2 7–10/14; P4 Q07–Q10; P5 4/5.
- **Ambiguity/retirement:** no merge/reset on ambiguity, retired suppression, or absence/expiry of retired identity alone. No unbounded identity history. P2 12; P4 Q06/Q19; P5 EV23.
- **0-RTT/opaque input:** no state change from 0-RTT, no surrogate CCS, no Finished inferred from opaque 1-RTT. P2 11; P4 Q12.
- **Resource/context loss:** use accepted Unsupported and lifecycle disposition; no synthetic completion, flow violation or incomplete-handshake event. P5 D6-04 and event matrix.

## 5. Semantic equivalence is not wire-evidence equivalence

| Semantic fact/consequence | TCP evidence | QUIC evidence | Safe common boundary | Unsafe equivalence |
|---|---|---|---|---|
| CH facts/fingerprint | Supported staged record/CH parsing and continuation | Qualified Initial CRYPTO reconstruction and complete CH evaluation | Same authoritative fields and accepted fingerprint algorithm/output | A QUIC packet is a TLS record; every retained Complete is a fresh CH |
| InProgress | Permitted complete CH commit from PreHandshake | Permitted authoritative CH commit from PreHandshake | Same state meaning and guarded action | First encrypted packet proves a CH was accepted |
| Finished | TCP CCS/later handshake or accepted TLS13 ApplicationData rule | Unambiguously associated client v1 Handshake Long Header; bounded deferred evidence where permitted | Qualified completion plus feedback/commit/reclaim discipline | Observed TLS Finished, server confirmation, identical early-data predicate |
| TerminatedByAlert | Readable terminating TCP alert under visibility/alignment rules | Readable associated Initial CRYPTO_ERROR close | Qualified terminal semantic disposition | Any QUIC close or opaque ciphertext is a readable TLS alert |
| New attempt | Accepted SYN reuse rule | Accepted complete-new-attempt qualification | Authorized lifecycle reset/replacement | DCID change alone, Retry-compatible restart or CH2 proves new attempt |
| Need more data | Supported TCP continuation | Missing CRYPTO ranges or partial qualified CH acquisition | No premature CH authority or callback | QUIC CRYPTO gap equals TCP continuation-gap Unsupported |
| Unsupported | Accepted TCP support/continuation conditions | Accepted unsupported grammar, support exhaustion or required context loss | No consumer dispatch and no automatic violation/drop | Every extraction failure is Unsupported or a protocol violation |
| STOP | Actual observer feedback, with current-unit accumulator | Same actual feedback contract on supported projections | Same reducer and no later unit after latched Stop | Parser Stop equals observer Stop equals network discard |

## 6. Staged TCP evaluation without whole-handshake buffering

### 6.1 Existing stages to preserve

1. **Packet-local record framing:** A20/A23 operate on the current payload and record cursor. Unsupported/partial outcomes do not require retaining every handshake message.
2. **CH prefix and digest work:** current CH parsing obtains fixed fields/cipher/compression information and incrementally builds fingerprint state. The parser can consume qualified fields before all extension bytes arrive; this is evaluation progress, not an authoritative complete-CH dispatch or TLSData commit.
3. **CH extension continuation:** A24 copies only the supported continuation region into its owned buffer, restores counts/version/digest/locator state, evaluates extensions and finalizes when complete. P5 D6-01 requires mode-specific restoration and current-result isolation; do not treat the existing fallback version read as proof the adopted correction is implemented.
4. **Opaque record tracking:** A25 and A23 retain sequence/remaining-length/type metadata for supported opaque records instead of buffering/decrypting their entire payload. TCP state/version decides applicability. This mechanism is not QUIC CRYPTO reassembly.
5. **Alert continuation:** A26 and alert continuation helpers retain only the supported readable-alert/alignment state. This is not generic handshake buffering.
6. **Feedback and last use:** current views remain valid through callbacks, nested full-string reads and subsequent permitted commit. Reuse/reset/reclaim waits for last use. A callback-local wrapper copy is not an owned deep copy. P3 5 L01–L14; P5 3/5.

### 6.2 Sharing implication

A common semantic evaluator may accept qualified staged field availability or equivalent bounded access to CH data; it need not demand a contiguous complete TCP handshake buffer. QUIC can satisfy its own accepted complete-CH reconstruction requirement without imposing that representation on TCP. This is an admissible comparison boundary, not a selected incremental-parser API.
Fingerprint finalization, complete-CH authority, callback eligibility and TLSData commit remain separate milestones. Staged evaluation cannot expose partial fields as an authoritative complete CH, merge attempts, dispatch a later observation before feedback, or alter the accepted TCP U05 exception. Full-string/digest backing required by consumers remains bounded and independently valid.
Any candidate that requires whole-message TCP buffering solely to fit its abstraction must justify a current requirement and regression/runtime cost; no such requirement is established by Phases 3–5. Conversely, sharing must not eliminate the QUIC reconstruction/association evidence required by those contracts.

## 7. Candidate-facing sharing opportunities and non-goals

### 7.1 Concrete opportunities to evaluate

- **Qualified CH semantics and first-CH transition:** R06/R11 and D06–D08 support moving common field evaluation/commit policy out of an exclusively TCP path, provided readiness/provenance, staged access and accepted callback behavior are preserved.
- **Verdict and commit discipline:** R09/R10/R14 and D15/D16/D18 support common actual-feedback reduction, normal-update suppression and semantic Error actions. Reset, parser-Stop, terminal payload and aging remain explicit exceptions.
- **Qualified completion/termination actions:** R12/R13 and D11/D13/D14 support common state consequences after different evidence qualification. The entire TCP completion predicate cannot move unchanged as QUIC policy.
- **Reset/lifecycle coordination:** R16/R17 and D01/D20/D21 support shared authorized reset and last-use/reclaim coordination without merging physical owners or selecting a global allocator.
- **Intentional TCP branches:** D09/D10/D12/D23/D24/D28 retain CCS, record encryption/visibility and pre-CH record counters. A common policy can still contain explicitly guarded TCP branches; avoiding duplicate state code does not require pretending these observations exist in QUIC.

### 7.2 What is not decided

No common semantic class, strategy interface, base class, protocol-flow class, adapter API, Template Method or state-machine object is selected. Logical responsibility does not imply a component. Candidate 4 must evaluate these opportunities; the other three candidates must be compared against the same evidence. No full TLS endpoint, universal DTLS framework, synthetic record API or automatic consumer mitigation change is authorized.
Allocation at startup only, no Engine runtime growth, bounded replacement peak and numerical deferral remain binding. This assessment supplies no numerical capacities, CPU/RAM budget requirement or measured performance claim.

## 8. Source anchors and provenance

Axx references below identify the original module-relative path, extracted-file line and original CODE bundle line. Extraction provenance/hashes remain in the Step 1 manifest. P2/P3/P4/P5 aliases resolve to the exact original files in the Step 1 inventory. CONS callback fragments use original bundle ranges, not invented repository paths.

| Anchor | Original path | Symbol / token | Extract line | CODE line |
|---|---|---|---:|---:|
| A01 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::onNewSession(` | 48 | 3367 |
| A02 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::prepareInboundExtension(` | 74 | 3393 |
| A03 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::process(` | 109 | 3428 |
| A04 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::dispatchStepResult(` | 167 | 3486 |
| A05 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::analyzeProcessResult(` | 195 | 3514 |
| A06 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::checkForParsingError(` | 268 | 3587 |
| A07 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::onClientHello(` | 286 | 3605 |
| A08 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::onRecord(` | 311 | 3630 |
| A09 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::dispatchTerminalPayload(` | 324 | 3643 |
| A10 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::onAging(` | 350 | 3669 |
| A11 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::buildExtension(` | 421 | 3740 |
| A12 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::commitNormalCurrentRecord(` | 469 | 3788 |
| A13 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::updateTLSDataOnClientHello(` | 501 | 3820 |
| A14 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::updateTLSDataOnChangeCipherSpec(` | 521 | 3840 |
| A15 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::updateTLSDataOnApplicationData(` | 530 | 3849 |
| A16 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::updateTLSDataOnAlert(` | 541 | 3860 |
| A17 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::commitUnsupported(` | 554 | 3873 |
| A18 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::commitViolation(` | 567 | 3886 |
| A19 | `packet-flow/src/TLSFlowConductor.cpp` | `TLSFlowConductor::commitError(` | 572 | 3891 |
| A20 | `processor/src/Processor.cpp` | `void Processor::beginPacket(` | 56 | 5410 |
| A21 | `processor/src/Processor.cpp` | `Processor::processRecord(` | 210 | 5564 |
| A22 | `processor/src/Processor.cpp` | `Processor::processSingleRecord(` | 406 | 5760 |
| A23 | `processor/src/Processor.cpp` | `Processor::processNewRecord(` | 810 | 6164 |
| A24 | `processor/src/Processor.cpp` | `Processor::processSegmentedClientHelloContinuation(` | 426 | 5780 |
| A25 | `processor/src/Processor.cpp` | `Processor::processSegmentedOpaqueContinuation(` | 503 | 5857 |
| A26 | `processor/src/Processor.cpp` | `Processor::processAlert(` | 745 | 6099 |
| A27 | `common/include/TLSData.hpp` | `inline bool isActiveHandshakeState(` | 34 | 353 |
| A28 | `common/include/TLSData.hpp` | `void clear()` | 151 | 470 |
| A29 | `extension/include/TLSSessionExtensionTable.hpp` | `void release(` | 124 | 2898 |
| A30 | `extension/include/TLSSessionExtensionTable.hpp` | `bool markReclaimable(` | 136 | 2910 |
| A31 | `packet-flow/src/TLSFlowConductor.hpp` | `TLSFeatureVerdict notify(` | 102 | 4008 |

Additional source surfaces: `processor/src/ITLSParser.hpp`, `TlsParser.hpp`, `TtlsParser.cpp`, `SegmentBuffer.hpp/.cpp`, `SegmentBufferManager.cpp/.hpp`; `processor/include/IProcessor.hpp`, `IClientHelloDataViewer.hpp`, `IRecordsDataViewer.hpp`, `IParsingErrorViewer.hpp`; `packet-flow/include/ITlsListener.hpp`, `TLSRecordContext.hpp`, `TLSFeatureVerdict.hpp`; Engine `Factory.cpp` and `Application.cpp`. These retain Step 1 exact paths/ranges/hashes.

CONS: Enforcement listener L1–166 and detection Manager L167–343; Fingerprint listener L344–374 and PacketFlowManager L375–549; Visibility listener L550–617 and monitor L618–end. The accepted compatibility limits in P3 8/P4 7 remain; complete downstream compliance is not claimed.

### V3 review correction

V2 remains preserved. The readiness recheck stopped at an omitted explicit classification of `shouldParseHandshake`; D28 now records its TCP visibility gate and why it cannot be reused as QUIC CH eligibility. Exact evidence: CODE `processor/src/Processor.cpp` extracted L900-907, original bundle L6254-6261. The D27 row is also joined to the existing decision table so Markdown renders one continuous inventory. No accepted policy, selected architecture or source code is changed.

## 9. Coverage, review and handoff

| Step 2 requirement | Evidence in this assessment |
|---|---|
| Current TCP path end to end | Section 2, A01–A31 |
| Accepted QUIC overlay without new classes | Section 3 R01–R19; Section 4 QUIC-only overlay |
| Four responsibility categories with reasons | Section 3 classification and rationale columns |
| TCP transition/decision relocation assessment | Section 4 D01–D28, including Processor state reads and lifecycle exceptions |
| Semantic versus observable evidence | Section 5; D03/D09–D14/D23–D25 |
| Staged TCP without complete-handshake buffering | Section 6; A20–A26; P5 D6-01 |

Completion review checks authority integrity, code anchors, responsibility/decision coverage, semantic/evidence separation, staged evaluation, lifetime/bounds and absence of architecture preselection. The companion review record records executed integrity checks and static review separately.

**Disposition: Step 2 PASS at the static assessment boundary. Proceed to Step 3 to define four genuinely different candidates.** No Phase 6 exit gate is declared satisfied by this step alone. Candidate costs, exact interfaces, migration ordering and runtime validation remain later work; no missing numerical capacity is manufactured into a blocker.
