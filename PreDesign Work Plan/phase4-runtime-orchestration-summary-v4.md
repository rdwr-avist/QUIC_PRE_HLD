# Phase 4 Summary: Runtime Orchestration, Dispatch and State Commit V4

Review and delivery date: 2026-10-14. One consolidated Markdown artifact. This is a pre-design static review, not implementation or runtime certification.

## 1. Executive decision

**Phase 4 exit review: EG1-EG9 PASS at the agreed static pre-design boundary. READY FOR PHASE 5 ownership, lifecycle and bounds design.**

All 34 named flows retain their seven stages. The latest explicit user contract closes E04: an Unsupported current unit invokes no consumer, including Anomaly; required evaluation must finish before any consumer dispatch. No same-unit Unsupported/consumer-verdict precedence is needed. Malformed/public Stop remains a different outcome and preserves actual feedback handling.

EG6 is evaluated as the agreed boundary-compatibility analysis, not certification of unchanged QUIC consumer implementations. The user excluded helper/storage audits and made current Fingerprint/Visibility CH-only consumption explicit; broader Enforcement event reuse is not a current implementation requirement. Known TCP-specific actions and unverified wiring remain implementation constraints, not hidden evidence of working QUIC code. Section 7 defines the resulting contract without selecting a new interface/class or changing admission/action behavior.

The nine original workplan conditions are unchanged and evaluated in section 9. PASS authorizes proceeding with Phase 5 design; it is not C++/packet/runtime certification, product release approval, acceptance of every candidate proposal, or proof of downstream ownership/action compliance. V2 and all source baselines are preserved.

## 2. Authority, evidence and scope

- P2: phase2-state-machine.md, accepted state/attempt policy, sections 5-12. It controls QUIC v1, client-only visibility, Initial-key limitations, completion heuristic, same-attempt CH/restart, active/terminal candidate acceptance, retired suppression and ambiguity.
- P3: phase3-semantic-processing-contract-review-v5.md, operations OP-01..10, lifetime and FB-A..F, D01, U04/U05 and section 8 consumer constraints. This review inherits its contract, not a fresh full 131-item inventory audit.
- CODE: TLS-Flow-All-Code.txt, original File: paths and bundle lines. Direct static implementation evidence for TCP and producer/API behavior; not a QUIC implementation.
- P4-2: Step 2 V2 responsibility mapping. R1 orchestration; R2 association/qualification; R3 protection; R4 acquisition/coverage; R5 evaluation/projections; R6 dispatch/feedback; R7 semantic disposition. These are logical roles, not seven components.
- P4-3: Step 3 V1 dispatch/verdict contract and D3-ORDER proposal.
- P4-4: Step 4 V2 gates/commit, with TCP-loop corrections controlled by direct CODE and FLOWS V5. Historical conflicting prose is not a second policy.
- FLOWS: Step 5 V5, complete catalogue, exact TCP anchors, RFC qualifications and explicit E02-E04.
- CONSUMERS: Step 6 V3; six consumer-body findings are preserved prior implementation review from V2, not raw consumer files freshly inspected in this turn. API/producer facts are directly inspected. Contract lifetime is binding; downstream compliance is not inferred.
- COMMON: Step 7 V2, common current dispatch-and-disposition assessment, alternatives and lightweight DTLS counterexamples.
- PLAN: tls-flow-quic-pre-design-work-plan-v5.md, Phase 4 exit gate and Phase 5 scope. The exact nine gate texts are reproduced in section 9.
- USER-U01 (latest conversation, explicit policy): Unsupported invokes no consumer; Fingerprint and Visibility need CH only; broader Enforcement event needs are theoretical/less relevant now. This supersedes E04's earlier hypothetical same-unit mixed outcome, not malformed/public Stop handling or prior-unit effects.
- USER-SCOPE (conversation): helper/storage audits are outside the current scope; Step 6 is four-axis compatibility analysis, not proof all consumers work unchanged. This resolves the analysis boundary for EG6 without claiming missing source was inspected or authorizing a QUIC RST path.

Classification: ACCEPTED = explicit user/inherited policy; CODE = inspected current implementation; PROPOSAL = Phase 4 design recommendation not automatically user-approved; OPEN/BLOCKED = evidence or policy missing. Passing a consistency check does not promote a proposal to accepted product policy.

No final class/interface/result-type architecture, Enforcement implementation, DTLS implementation, TCP refactor or QUIC-only fingerprint repair is selected. No full FRD/SRS coverage claim. No public source lookup can resolve private current consumer wiring/actions; this review uses the supplied RFC-grounded catalogue without claiming a fresh complete RFC/errata audit.

## 3. Consolidated decisions

| ID | Classification | Decision and authority |
|---|---|---|
| D4-01 | ACCEPTED | Neither TLS-flow nor Processor independently drops packets. STOP is existing control feedback, not network discard. TLS-Enforcement owns flow-legality decisions and the enforcement associated with those decisions. Consumer-specific mitigation remains outside TLS-flow and Processor responsibility. Processor reports supported current-input local syntax/encoding/protection findings, not attempt-wide CH-count/first-message legality. User boundary; FLOWS section 1. |
| D4-02 | ACCEPTED | Same complete CH uses the same fingerprint algorithm/output independent of TCP/QUIC. Preserve known legacy algorithm behavior and TCP U05 retained-Complete dispatch; no default bug fix. P3 U04/U05. |
| D4-03 | CODE / ACCEPTED preservation | TCP projections remain parsing-error, record, Complete-CH; one current-record accumulator and normal commit after feedback. Outer loop requires RecordParsed AND updated active state; Stop returns first. CODE 3472-3585. |
| D4-04 | ACCEPTED | Availability, evaluated validity, freshness, delivered observation and accepted authority are distinct. QUIC duplicate-only ranges/retained results do not create new CH callbacks/counts. TCP deduplication is limited to inspected supported continuation, not a general stream promise. P3; FLOWS T05/Q05. |
| D4-05 | ACCEPTED | R7 is the logical semantic writer; R6 gathers actual feedback; observers stay in Extension. Public mutable fields do not prove compile-time enforcement. P4-2/P4-4; CODE TLSData/Extension. |
| D4-06 | ACCEPTED | Active candidate may require isolated complete CH evaluation; terminal candidate acceptance/reset precedes CH parsing. Retry-compatible restart is same attempt; ambiguity cannot merge/reset. One active semantic TLSData context. P2 sections 7-12; P3 FB-A. |
| D4-07 | ACCEPTED | Associated client v1 Handshake Long Header is product completion evidence, not TLS Finished. Pre-CH evidence remains non-authoritative until same-attempt CH authority; no premature reclaim. 0-RTT does not advance state. P2 sections 6/9/11. |
| D4-08 | ACCEPTED | Associated successfully unprotected Initial 0x1c CRYPTO_ERROR close uses P2 terminating-evidence conditions; other/opaque closes are not fabricated TLS alerts. P2 section 9.1; FLOWS Q17/Q18. |
| D4-09 | ACCEPTED | D01 required product-capacity failure -> Unsupported with zero consumer calls in that unit, including Anomaly; no missing/stale successful CH, synthetic violation/drop or replay. Required evaluation precedes all dispatch; E04 closed by USER-U01, not invented Error/Unsupported priority. Historical masks/prior-unit effects remain distinct. P3 D01/FB-D as clarified by USER-U01. |
| D4-10 | PROPOSAL | D3-ORDER: packets/validated frames in incremental encounter order, messages unlocked by CRYPTO in logical stream order; resolve each unit before later semantic delivery. No whole-packet/datagram atomicity or rollback. P4-3; FLOWS Q04/Q15. |
| D4-11 | PROPOSAL assessment | Useful common boundary is current observation/feedback/disposition, not identical wire carrier/inner loop. Verdict reduction alone is useful but insufficient for lifetime/progress. COMMON sections 3-6. No implementation abstraction required. |
| D4-12 | ACCEPTED contract / Phase 5 mechanism | Reclaim eligibility follows a committed eligible Finished transition and last use, not proposal/Stop. Replacement must not inherit stale reclaim membership; inspected release/buildExtension is a lifecycle pattern, not proof of implemented safe QUIC swap. P4-4 V2; CODE table 2829-2926. |

RFC precision retained from FLOWS: v1 long-header type bits are exposed; PN length/PN/reserved bits and short-header key phase are protected. Initial AEAD verification is not peer authentication. HP sample and tag bounds are packet-local; frames are not universal TLVs; CRYPTO gap differs from packet overrun; 1200-byte client Initial minimum applies to UDP payload, not each packet. Endpoint network actions are not this feature's action authority. These are catalogue wire facts, not new product guards.

### B01 clarification retained from FLOWS

TCP's local Complete-status/length guard is preserved as actual source behavior, not a universal RFC one-CH-per-input rule. QUIC packetization and CRYPTO reconstruction can unlock buffered/retransmitted CH bytes and a later logical CH in one datagram without proving when messages were sent or whether HRR intervened. No one-CH-per-datagram guard is added. Phase 2 logical-message eligibility stays unchanged; illegal sequence decisions remain Enforcement. This is a packetization/observation distinction, not proof arbitrary multi-CH sequences are legal. FLOWS section 8 B01; P2 sections 6-8.

## 4. Shared transaction and exceptions

A transaction is a current unit's ordered projection/feedback/disposition interval, not ACID, rollback, whole-input atomicity, an immutable TLSData snapshot or a required new object. A unit can have no callbacks and still justify a qualified policy disposition. Incomplete acquisition need not open a successful CH transaction.

Ordinary unit sequence: qualify current session/attempt/result -> evaluate required current facts -> deliver applicable projections -> reduce actual feedback -> apply permitted normal or violation/Error disposition -> finish backing uses -> resolve next-unit permission/reclaim eligibility. Consumer side effects already performed are not rolled back.

| Ordered feedback | Current disposition | Further delivery |
|---|---|---|
| All Continue, zero accumulated mask | Justified normal update permitted | Updated state/association gate |
| All Continue, nonzero accumulated mask | OR violations, suppress normal update | Only if resolved traversal policy allows |
| Stop while accumulated mask zero | Stop current list, latch Stop; remaining applicable current-unit projections can run | No later unit after current resolution |
| Stop while accumulated mask nonzero | Immediate Error plus accumulated mask | Return Stop, no remaining projection |
| Earlier zero-mask Stop, later Continue adds violation | Remaining applicable current-unit projections still run unless another Stop; final Error | Return Stop |
| Earlier Continue adds violation, later Stop has zero local mask | Immediate Error because accumulator is nonzero | Return Stop |
| Latched Stop and final mask zero | Justified normal update permitted | Return Stop after current unit |

Current accumulator starts zero; stored terminalViolationMask is historical and not its seed. notify ORs only actually called live observers and stops its list on first Stop. No verdict for skipped/expired observers. CODE analyzeProcessResult 3514-3585 and notify 4008-4026; P3 FB-C; COMMON section 6.

Exceptions are not flattened: public NeedMoreData returns Continue without normal CH commit; Unsupported invokes no consumer and uses actual conditional TCP commit or accepted QUIC product disposition; public parser Stop uses actual representable anomaly feedback; reset/terminal payload have distinct branches; actual aging ignores returned verdict and releases after notification. Parser Stop, observer Stop, capability failure and network action remain distinct.

### Unsupported pre-dispatch contract (USER-U01; E04 closure)

For each current semantic unit, finish all evaluation required for its applicable projections before the first consumer call. This includes any required fingerprint/canonical-result evaluation; no Anomaly or other consumer is called early and followed by a required-evaluation Unsupported in that same unit. Do not add whole-packet/datagram prevalidation: a later independent unit can fail after an earlier unit resolved.

- Unsupported: no CH, Anomaly, record or other consumer call for that unit; local consumer accumulator stays zero and no Stop is produced by that unit. R7 applies the existing qualified Unsupported disposition, not Error or a new violation. No normal current-CH facts/count/timestamp are committed; previously valid historical facts/masks remain unless an independently accepted reset already occurred. Existing TCP conditional commitUnsupported is preserved.
- Return/progress: the Unsupported branch returns Continue (no consumer Stop), ends semantic processing of the affected accepted attempt under its terminal gate, and does not replay the input. Safe bounded traversal may only consider independently eligible units/new-attempt candidates under existing Q01/Q10/Q19 rules; it cannot resume ordinary CH delivery for the same Unsupported attempt. An isolated unaccepted active candidate does not make the old active attempt Unsupported merely because its isolated evaluation failed; preserve Q09 isolation and its qualification rules.
- Malformed/public Stop is not Unsupported. Actual representable anomaly feedback remains permitted on its existing path. A resource failure that current TCP maps to public Stop is not silently reclassified to Unsupported.
- Earlier units: already performed consumer side effects, accepted CH authority and historical violation masks are not rolled back. If an earlier unit returned Stop, the later unit is not processed. Actual lifecycle events remain distinct units; the rule does not globally disable actual aging notifications for a session whose stored state is Unsupported.

Proof of E04 exclusion: Unsupported is selected before dispatch, hence no consumer in that unit can contribute a verdict. After dispatch begins, the required evaluation for that unit is complete; any subsequent acquisition/evaluation failure belongs to a later unit, not a retroactive Unsupported verdict on the dispatched unit. This is an accepted ordering invariant to implement, not a claim that an unavailable QUIC implementation already enforces it.

## 5. TLSData authority, timing and gate ownership

| Existing field | Relevant read authority | Logical writer / timing |
|---|---|---|
| hsState | R1 progress, R2 qualification, processor context, R7, const consumers/inspection | R7 accepted reset or resolved normal/Error/Unsupported disposition; not speculative parser completion |
| tlsVersionMask | Existing TCP interpretation/transition, R7, consumers | R7 copies authoritative CH offer at permitted normal CH commit; reset clears |
| clientHelloCount | R7 policy and const consumers/inspection; no unsupported claim of direct Processor count read | R7 authoritative CH normal commit; reset clears; QUIC duplicates do not increment |
| earlyDataCount | Existing TCP context/consumers/inspection as relevant | R7 existing pre-CH ApplicationData update; reset clears; no automatic QUIC 0-RTT mapping |
| earlyDataOffered | Existing TCP completion interpretation, R7, consumers | R7 current authoritative CH offer at normal commit; reset clears; not proof of acceptance |
| preCcsSeen | Existing TCP context/consumers/inspection as relevant | R7 existing pre-CH CCS update; reset clears; no QUIC CCS surrogate |
| chFingerprint | Current authority consumers/inspection, R7 | R7 copies current valid fingerprint at permitted CH commit; reset clears; no stale failed-candidate copy |
| terminalViolationMask | R7, const consumers/inspection | R7 ORs actual transaction violations; accepted new-attempt reset clears; distinct from local accumulator |
| clientHelloTimeStamp | R7, consumers/inspection | R7 uses supplied completion-operation timestamp at permitted CH commit; reset clears; D-TIME remains a proposed contract choice, not a discovered requirement |

This is logical authority, not exhaustive compile-time caller enforcement. TLSData has public mutable fields. R3/R4/R5 may update protection/acquisition/evaluation backing but cannot commit semantic TLSData. Callback-local CH wrappers end at callback return; producer commit backing must independently remain valid until last use. No retention/reentrancy safety is inferred from const or wrapper copies. P3 lifetime and section 8; P4-4 V2 field table; CODE common/include/TLSData.hpp 319-491.

| Gate | Owner/location | Single policy |
|---|---|---|
| G01 participation | R1 input entry | Existing Extension/subscriber eligibility; absent context means no semantic parsing |
| G02 TCP precondition | Existing Conductor branch | Preserve SYN reuse and reset/terminal/payload ordering; no QUIC candidate policy imported |
| G03 QUIC association/candidate | R2 qualification, R7 acceptance before active delivery | Retired suppression, ambiguity no merge/reset; terminal candidate before CH parsing |
| G04 terminal permission | R1 using resolved R7 state | TCP existing silent/terminal paths; QUIC same terminal attempt cannot reopen; accepted replacement is different attempt |
| G05 observation eligibility | R2/R5 before R6 | Fresh eligible result after all required evaluation, not mere availability; Unsupported dispatches none; TCP U05 explicit exception |
| G06 feedback disposition | R6 reduction, R7 commit | One ordinary unit accumulator/latched Stop contract; Unsupported pre-dispatch exclusion USER-U01 closes E04 |
| G07 normal transition | R7 after permitted feedback | Phase 2 protocol-specific conditions; Unsupported disposition before any dispatch; no duplicate parser state policy |
| G08 next unit | R1 after disposition | TCP exact RecordParsed plus updated-active-state predicate; QUIC resolved association/state and Stop before later delivery |
| G09 reclaim request | R7 after eligible committed Finished/last use | Eligibility marker, not immediate release; stale membership cannot authorize new lifecycle eviction |

Gates may defensively check current facts but do not own competing policies. Bounded read-only lookahead cannot accept later authority or overwrite live backing. No cached permission survives reset/commit/context switch. E04 is closed by the pre-dispatch Unsupported exclusion, not by adding a second precedence policy. CODE 3393-3585; P2 sections 7-12; P3 FB-A/B/E; P4-4 V2 with FLOWS TCP correction.

## 6. Complete flow catalogue

The following 12 TCP and 22 QUIC paths are carried from FLOWS V5 sections 4-5. V3 preserves TCP path text and seven-stage coverage, and updates affected QUIC paths to USER-U01; these are explicit current-contract refinements, not a claim of verbatim FLOWS V5 text. Their authority lines, seven stages, conditional outcomes and proposal classifications are retained. TCP is source preservation; QUIC is RFC-qualified acquisition plus inherited product semantics and proposed sequencing. No claim that arbitrary additional CH sequences or different-DCID coalescing are RFC-compliant.

### TCP flow catalogue


All TCP paths are CODE-preservation unless explicitly noted. Each path below has all seven stages; 'none' is intentional, not a missing stage.

#### T01: Participation, empty input and input entry

Authority: CODE onNewSession/buildExtension/onInbound/process; Step 4 V2 G01/G02.

- **Input:** New session or inbound TCP packet.
- **Processing:** Build applicable Extension lists and register session observers; existing prepareInboundExtension first. With no Extension, return; reset and terminal branches precede normal parsing. Empty normal payload returns.
- **Observation:** Participation/no-work outcome, not a CH.
- **Dispatch:** None for absent context/empty normal payload.
- **Feedback:** Neutral; no fabricated verdict.
- **Commit:** Only existing initialization; no normal CH update.
- **Continuation/return:** Return Continue; otherwise select the applicable T02-T12 branch.

#### T02: Successful complete CH

Authority: CODE process/analyzeProcessResult/updateTLSDataOnClientHello; P3 U04/U05.

- **Input:** TCP payload containing supported complete CH in active processing context.
- **Processing:** beginPacket then processRecord; evaluate CH and fingerprint with current TCP algorithm.
- **Observation:** RecordParsed and Complete CH; valid record/CH projections.
- **Dispatch:** Parsing error if present, record list, Complete-CH list in existing order.
- **Feedback:** Shared transaction from section 4 of this summary.
- **Commit:** If permitted, update CH facts/count/time and PreHandshake -> InProgress; violations suppress normal update.
- **Continuation/return:** Next processRecord only when current result is RecordParsed, no observer Stop, AND updated hsState is active. Otherwise return. A commit to Finished/Unsupported/Error/TerminatedByAlert prevents the next record in this packet. Preserve the existing gate, not an invented new one.

#### T03: Partial record or CH and supported continuation

Authority: CODE beginPacket/resumeOrDiscardStepContinuation/processRecord; P3 OP-02.

- **Input:** TCP bytes ending within a specifically supported CH-extension, opaque-record or alert continuation; later packet supplies continuation. Not arbitrary TLS header/CH-prefix reassembly.
- **Processing:** Retain backing and expected sequence only in supported modes. Segmented CH prefix/body truncation is Unsupported. Record-level fragmentation follows the exact processClientHello/processNewRecord branches: the non-segmented recordLevelFragmented path is Unsupported, whereas segmented length mismatch can produce Stop. Do not generalize every handshake/record length mismatch to Unsupported. Partial ordinary TLS header can produce MalformedTlsRecord/Stop; partial alert-header has a special reassembly path.
- **Observation:** NeedMoreData for supported pending continuation; Unsupported for unsupported continuation, or public Stop for local acquisition/parsing failures, including allocation failure where the source produces Stop. Do not generalize all packet-end truncation to NeedMoreData.
- **Dispatch:** No Complete-CH callback while incomplete. Public NeedMoreData has no callback. Public Unsupported follows T09 with no fabricated anomaly. Public Stop follows T08: notify the actual anomaly list only when lastError is non-None and observers exist. Select dispatch from the public processRecord outcome, not a nested status name.
- **Feedback:** NeedMoreData/Unsupported branches are neutral in the current dispatchStepResult. Public Stop uses the actual parsing-error listener verdict, including its Stop and accumulated local mask; no violation is inferred from truncation or allocation failure.
- **Commit:** No incomplete CH authority; continuation acquisition is not semantic commit. Public Unsupported applies the existing conditional commitUnsupported guard (T09). Public Stop applies T08 violation/Error disposition only as actual feedback requires. NeedMoreData alone performs no normal CH/state commit.
- **Continuation/return:** NeedMoreData returns Continue and ends the current packet loop; a later external packet resumes only supported retained continuation. Unsupported also ends the packet loop and returns Continue after its conditional disposition. Public Stop ends the loop and returns the actual observer result (Continue or Stop). When a supported continuation yields RecordParsed, use T02/T04 and the existing updated-active-state loop predicate.

#### T04: Several records in one packet

Authority: CODE process loop and Processor packet-level CH state; P3 U05.

- **Input:** TCP payload with several bounded records.
- **Processing:** Process one record; return to Conductor before the next.
- **Observation:** Current record result; Complete viewer can retain prior CH data.
- **Dispatch:** Existing projection order; retained Complete callback preserved, not treated as proof of new CH.
- **Feedback:** Resolve current record transaction independently.
- **Commit:** Normal update is driven by current record predicates, not merely a retained CH callback.
- **Continuation/return:** Next processRecord only when current result is RecordParsed, no observer Stop, AND updated hsState is active. Otherwise return. A commit to Finished/Unsupported/Error/TerminatedByAlert prevents the next record in this packet. Preserve the existing gate, not an invented new one.

#### T05: Retransmission, overlap and continuation gap

Authority: CODE resumeOrDiscardStepContinuation/processRecord; P3 TCP inventory.

- **Input:** TCP bytes overlapping an existing continuation, retransmission, or sequence gap.
- **Processing:** Use existing sequence/overlap rules, not a new QUIC-style reassembler.
- **Observation:** Existing retransmission NeedMoreData, resumed suffix, or continuation-gap Unsupported.
- **Dispatch:** No invented new CH observation; actual parsed suffix uses existing callbacks.
- **Feedback:** Outcome-specific feedback only.
- **Commit:** Within an existing supported pending continuation, duplicate-only bytes create no new authority; a resumed suffix can complete CH. This is not a general TCP duplicate-CH filter: without pending continuation, beginPacket resets packet-local CH state and the same complete CH bytes in a later payload can be parsed and counted again. No upstream TCP deduplication guarantee is established by this source. Preserve conditional commitUnsupported on gap. TCP skips sequence overlap without byte-equality comparison; QUIC coverage/conflict policy differs.
- **Continuation/return:** Existing NeedMoreData/Unsupported return ends this packet loop; future packet uses retained/discarded continuation per source.

#### T06: Additional CH and legacy same-input rejection

Authority: CODE Processor.cpp 6301-6306; P3 U05; USER-BOUNDARY-01/E01.

- **Input:** Additional CH in a later input, or second CH encountered after Complete within current processing input.
- **Processing:** Parse local CH encoding; existing packet-level Complete guard may produce MultipleClientHello. Do not add an attempt-wide CH-count legality check.
- **Observation:** Actual Complete CH or legacy parser error; neither certifies flow legality.
- **Dispatch:** Complete path uses T02; parser-error path uses T08.
- **Feedback:** Processor reports the actual local MultipleClientHello finding; only actual observer verdicts supply violation bits/Stop. No attempt-wide legality verdict is added.
- **Commit:** Existing permitted normal CH update or feedback disposition; no automatic invalid-attempt state.
- **Continuation/return:** Preserve actual TCP public outcome and return. The local TCP guard remains; B01 explains why no equivalent QUIC datagram-count rejection is inferred from RFC or TCP similarity.

#### T07: Session reuse / new TCP context

Authority: CODE prepareInboundExtension; Step 4 V2 section 7.

- **Input:** TCP SYN without ACK on an existing DP Session.
- **Processing:** Existing prepareInboundExtension tests active state and recorded violations; when applicable release/buildExtension and return from current inbound handling.
- **Observation:** Session reuse lifecycle, not parsed CH.
- **Dispatch:** No CH dispatch for the reuse branch; applicable lists rebuilt via existing registration contract.
- **Feedback:** Neutral; no synthetic sequence violation.
- **Commit:** New context initializes PreHandshake; release removes old reclaim membership.
- **Continuation/return:** Existing branch returns Continue; subsequent packet processes new context. Do not import QUIC candidate rules or force current SYN payload parsing.

#### T08: Local malformed syntax / parser Stop

Authority: CODE processRecord/dispatchStepResult/checkForParsingError; USER-BOUNDARY-01.

- **Input:** Current record/CH bytes with an actual supported local length/encoding failure.
- **Processing:** Trace the actual public outcome. Internal Error/StopProcessing must be followed through processNewRecord/processRecord before selecting dispatch; nested status names alone are not public Stop. No attempt-wide sequence check is added.
- **Observation:** Public Stop and actual lastError for this path; no normal current-record commit. A lastError alone does not establish public Stop: actual RecordParsed paths use the current-record transaction.
- **Dispatch:** Existing anomaly list only when lastError != None and observers exist.
- **Feedback:** Continue/Stop and violation bits come from listeners, not the error itself.
- **Commit:** OR violations or Error only as verdict requires; no normal record commit on parser Stop.
- **Continuation/return:** Return actual observer result; packet loop ends because result was not RecordParsed. Continue does not mean resume guessed bytes.

#### T09: Unsupported and resource outcome

Authority: CODE dispatchStepResult/commitUnsupported; P3 D01 and U05.

- **Input:** Existing TCP unsupported continuation/record or allocation outcome.
- **Processing:** Preserve actual source outcome mapping; not every resource error maps to Unsupported in current TCP.
- **Observation:** Unsupported, or parser Stop with allocation error as actually produced.
- **Dispatch:** Unsupported branch has no fabricated anomaly; actual Stop uses T08.
- **Feedback:** No synthetic violation or drop.
- **Commit:** Preserve conditional TCP Unsupported guard; D01 target semantics do not silently repair legacy TCP.
- **Continuation/return:** End packet loop and return Continue or actual feedback result; document implementation differences rather than changing them.

#### T10: Terminal input, reset and aging

Authority: CODE process/dispatchTerminalPayload/onAging; Step 4 V2.

- **Input:** Input in Finished/Unsupported/Error/TerminatedByAlert; TCP reset; or session aging.
- **Processing:** Reset branch first; silent terminal returns; terminal payload branch as existing; aging accesses existing Extension.
- **Observation:** Reset, terminal payload or session-end observation; no ordinary CH parsing on silent branch.
- **Dispatch:** Actual reset/terminal/session-end lists only.
- **Feedback:** Existing verdict contract; SessionEnd verdict ignored.
- **Commit:** Reset clears only on its existing no-violation branch; terminal verdict may commit mask/Error; aging releases after callbacks.
- **Continuation/return:** Return existing observer result; aging returns Continue. No independent packet drop.

#### T11: Completion, CCS/ApplicationData and readable alert

Authority: CODE commitNormalCurrentRecord/updateTLSDataOn*; P2 section 5.1.

- **Input:** Current parsed record with CCS, ApplicationData, encrypted-handshake boundary or readable alert.
- **Processing:** Parse/track current record according to existing state-dependent interpretation.
- **Observation:** Record facts, offered-version/early-data context and readable terminating-alert fact.
- **Dispatch:** Existing record projection plus U05 Complete projection when triggered.
- **Feedback:** Shared transaction; Enforcement evaluates illegal ordering.
- **Commit:** Permitted existing transition/counter update; terminating alert conditions retained; newly Finished requests reclaim after commit.
- **Continuation/return:** Next processRecord only when current result is RecordParsed, no observer Stop, AND updated hsState is active. Otherwise return. A commit to Finished/Unsupported/Error/TerminatedByAlert prevents the next record in this packet. Preserve the existing gate, not an invented new one.

#### T12: STOP variants

Authority: CODE notify/analyzeProcessResult; Step 3 transaction.

- **Input:** Any T02/T04/T11 current record with listener Stop.
- **Processing:** Finish only permitted current-unit work; never advance to another record after Stop.
- **Observation:** Current unit remains valid through feedback and commit.
- **Dispatch:** Stop current list; later current-unit projections only under zero accumulated mask rule.
- **Feedback:** Latch Stop; OR earlier/later masks; local zero mask never erases accumulated mask.
- **Commit:** Zero final mask permits normal update; nonzero final mask commits Error; no normal update.
- **Continuation/return:** Return STOP through existing observer contract; no TLS-flow-owned network drop, replay or automatic reset.


### QUIC flow catalogue

All QUIC acquisition sequences are PROPOSAL under accepted Phase 2/3 semantics; wire facts are RFC-qualified. No new class or callback API is selected.

#### Q01: Entry, packet boundaries and association

Authority: PROPOSAL; P2 sections 7-12; R0/R2; Step 4 V2 G01/G03.

- **Input:** Client-to-server UDP datagram for participating session, or entry with no participating Extension/empty UDP payload.
- **Processing:** Establish participation and a valid Extension before acquisition. With no participating context or empty input, do no parsing, dispatch or semantic commit and return Continue; do not fabricate an Extension or callback. Otherwise initialize datagram cursor; parse bounded visible version/CIDs and type/version-specific Length where defined. Qualify association without treating current DCID as automatic key basis. Retry in client-to-server input is unexpected/out-of-scope, not observed server Retry; Retry/VN have no frames/ordinary Length and consume their remainder. Later differing-DCID input is not compliant coalescing; bounded candidate inspection is a robustness proposal, not automatic reset.
- **Observation:** Bounded packet metadata and active/candidate/retired/ambiguous classification.
- **Dispatch:** None for acquisition metadata alone.
- **Feedback:** Neutral.
- **Commit:** No CH/state commit from framing; candidate reset only through Q09/Q10.
- **Continuation/return:** Select Q02/Q03/Q11/Q12/Q13/Q14; safe known packet boundary permits later packet traversal unless semantic gate/Stop forbids it.

#### Q02: Successful Initial and complete CH

Authority: PROPOSAL; R1-R3; P3 OP-03/06/07; P2 section 6.

- **Input:** Bounded associated v1 Initial with supported retained key context and CRYPTO completing CH.
- **Processing:** Remove HP; decode/reconstruct PN; verify/decrypt Initial; parse the current frame; insert validated CRYPTO bytes; finish complete ordered CH and required fingerprint/current-result evaluation before any consumer projection. Required evaluation Unsupported selects Q16 without any dispatch, including Anomaly.
- **Observation:** New eligible complete CH with valid provenance; Initial AEAD verification is not peer authentication.
- **Dispatch:** Only after required current-unit evaluation is supported and complete: existing ClientHello list and any genuinely applicable representable local anomaly projection in its established order; no synthetic TLS record. Unsupported dispatches none (USER-U01).
- **Feedback:** Shared transaction, neutral when no applicable listeners.
- **Commit:** Permitted authoritative CH facts/count/time and InProgress; apply retained same-attempt completion only after CH authority.
- **Continuation/return:** Pause before later message/frame/packet; if no Stop and policy permits, resume bounded cursor; datagram end returns Continue.

#### Q03: Partial CH, gaps and several ranges

Authority: PROPOSAL; R1; P2 section 7; P3 OP-04/05.

- **Input:** Initial CRYPTO ranges out of order, partial handshake header/body or gap.
- **Processing:** Parse complete packet/frame encoding; retain bounded nonconflicting ranges by Initial context; deliver only contiguous complete handshake messages.
- **Observation:** Acquisition progress/NeedMoreData; no complete CH observation yet.
- **Dispatch:** None for incomplete CH; no callback per range.
- **Feedback:** Neutral unless separate actual local failure occurs.
- **Commit:** No CH authority; retain coverage only; no violation merely for a gap.
- **Continuation/return:** Continue later frames/packets in same datagram if safe; later ranges may fill gap. At input end return Continue with bounded continuation.

#### Q04: Coalesced packets and multiple messages unlocked together

Authority: PROPOSAL D3-ORDER; R0/R1; P3 FB-B.

- **Input:** Datagram with several packets, or range unlocking several complete handshake messages.
- **Processing:** Packets in bounded encounter order; frames in plaintext order; newly available messages in logical CRYPTO stream order. Finish each unit's required evaluation before its first consumer call; do not prevalidate the whole datagram or batch all messages.
- **Observation:** One current eligible evidence unit at a time, not a datagram-wide batch of accepted CHs.
- **Dispatch:** Applicable projections for current unit only.
- **Feedback:** Resolve feedback before later semantic delivery, even if bytes are already buffered.
- **Commit:** Current-unit disposition only; no precommit of all messages.
- **Continuation/return:** Resume next message/frame/packet only after resolved gate. Stop blocks all later units, including messages unlocked by same range.

#### Q05: Same-context retransmission and identical overlap

Authority: ACCEPTED P2 section 7; PROPOSAL delivery; R1.

- **Input:** Previously accepted CRYPTO offsets/bytes, possibly repacketized with different PN/frame splits.
- **Processing:** Classify same-context duplicate coverage; insert only genuinely new compatible bytes.
- **Observation:** No new logical CH from duplicate bytes; new suffix can yield Q03/Q02.
- **Dispatch:** No replay CH callback from retained available result.
- **Feedback:** Neutral for duplicate-only input.
- **Commit:** No CH count/time/authority refresh solely from retransmission.
- **Continuation/return:** Continue safe independent units or return Continue; packet PN alone is not logical CH identity.

#### Q06: Conflicting overlap or ambiguous association

Authority: ACCEPTED P2 sections 7/9/12; USER-BOUNDARY-01.

- **Input:** Conflicting bytes at accepted CRYPTO offsets, or identity insufficient to choose active context.
- **Processing:** Do not merge conflict or guess association; preserve existing authority and bounded backing.
- **Observation:** Conflict/ambiguity acquisition outcome, not an Enforcement violation or proof of new attempt.
- **Dispatch:** No invented anomaly enum, CH projection or terminal callback.
- **Feedback:** Neutral absent independently applicable real consumer feedback.
- **Commit:** No reset/merge/state transition solely from conflict or ambiguity.
- **Continuation/return:** Skip semantically unusable unit if safely bounded; continue independent eligible units or return without network drop.

#### Q07: Additional CH, including third CH

Authority: ACCEPTED P2 section 6; USER-BOUNDARY-01; R5.

- **Input:** New complete parseable CH at a new logical message position in active attempt.
- **Processing:** Evaluate local CH structure; associate and deduplicate/candidate-classify. No attempt-wide max-CH legality gate. B01 records the resolved RFC distinction; no datagram-local count guard is included.
- **Observation:** Additional CH fact and prior attempt context; HRR-compatible is not proof of HRR.
- **Dispatch:** Existing CH list with valid current facts and precommit TLSData.
- **Feedback:** TLS-Enforcement, if actually participating with applicable contract, decides sequence/count violations. No assumed new QUIC Enforcement implementation.
- **Commit:** No violations permits replacement of authoritative CH and count under policy; verdict can suppress update/Error.
- **Continuation/return:** Continue under resolved state and Stop. Without applicable Enforcement verdict, TLS-flow does not manufacture one.

#### Q08: Retry-compatible acquisition restart

Authority: ACCEPTED P2 section 8; R2/R5.

- **Input:** Distinct retained-compatible Initial context with required token/offset/byte/identity evidence.
- **Processing:** Classify same-context retransmission first. Require a candidate Initial context distinct from every retained context, token evidence, CRYPTO start at offset 0, identical bytes wherever the candidate overlaps active partial/complete CH bytes, no contradictory available attempt identity, and consistent non-zero client SCID when available. Token presence alone is not proof of Retry. Apply P2 section 8; restart Initial/CRYPTO acquisition within the same attempt.
- **Observation:** Retry-compatible restart, not observed server Retry and not new attempt.
- **Dispatch:** No restart callback; a genuinely new eligible CH uses Q02/Q07, duplicate CH does not.
- **Feedback:** Neutral for restart itself.
- **Commit:** Preserve same-attempt semantic authority; no new-attempt reset or false CH count.
- **Continuation/return:** Resume candidate context at safe cursor; retain old contexts as required for association/late-input classification.

#### Q09: Active new-attempt candidate

Authority: ACCEPTED P2 section 10.1; Step 4 V2 section 7; PROPOSAL sequencing.

- **Input:** Candidate while current state is PreHandshake/InProgress with P2 complete-candidate identity evidence.
- **Processing:** Evaluate a complete CH candidate in isolation when needed. Require CRYPTO stream start at offset 0, an Initial context distinct from every retained active context, exclusion of retransmission/Retry-compatible restart, inconsistent available CH identity when known, and no match to available retired identity. Zero-length SCID is neither proof nor rejection; missing retired identity is not itself rejection. Apply all acceptance conditions, not changed DCID alone; preserve old backing until last use.
- **Observation:** Accepted replacement or rejected/ambiguous candidate; isolated parse is not active authority.
- **Dispatch:** Only after acceptance/reset and valid required CH evaluation; no callback exposing two active contexts. An isolated candidate evaluation that is Unsupported calls no consumer and cannot contaminate old active authority; apply only the disposition allowed by actual acceptance/association.
- **Feedback:** New unit accumulator starts at zero; existing traversal Stop is not canceled.
- **Commit:** Retire old identity where available; reset all attempt-local semantics/support; activate PreHandshake; then CH disposition Q02.
- **Continuation/return:** Preserve datagram cursor, not replay. Requalify later units against new context; suppress retired input; no replacement after resolved Stop.

#### Q10: Terminal new-attempt candidate and context lifecycle

Authority: ACCEPTED P2 section 10.2/12; Step 4 V2 section 7/8.

- **Input:** Initial candidate while Finished/Error/Unsupported/TerminatedByAlert.
- **Processing:** Classify attempt-level candidate before CH parsing: require a distinct candidate Initial context, CRYPTO stream start at offset 0, no packet-level duplicate/late evidence, no applicable Retry-compatible restart, and no match to available retired identity. Matching any retained old Initial context remains late. No CH-content identity is required before this terminal reset. On acceptance retire/reset and resolve Extension lifecycle before exposing new active context.
- **Observation:** Accepted new attempt or late/ambiguous input; no CH parse before terminal candidate acceptance.
- **Dispatch:** Re-establish applicable Extension observer lists if release/rebuild used; no stale callback pointers.
- **Feedback:** Neutral reset; candidate CH later uses shared transaction.
- **Commit:** Accepted reset -> PreHandshake before parsing; rejected candidate leaves old state/count/CH intact; stale reclaim eligibility removed by supported lifecycle path.
- **Continuation/return:** Continue current bounded input only with valid new context/backing. Concrete release/rebuild/swap mechanism remains Phase 5 obligation; no SYN requirement for QUIC.

#### Q11: Associated Handshake completion evidence

Authority: ACCEPTED P2 sections 5.2/6/9; R2; PROPOSAL disposition.

- **Input:** Bounded associated QUIC v1 client Handshake Long Header packet.
- **Processing:** Read exposed version/type/CIDs and qualify association; do not decrypt Handshake payload or read protected PN fields without keys.
- **Observation:** Product completion evidence only; ACK-only/encrypted-close payload cannot be distinguished.
- **Dispatch:** No completion callback exists; zero projections, no fake onTLSRecord.
- **Feedback:** Neutral feedback.
- **Commit:** InProgress -> Finished when qualified; before CH retain bounded non-authoritative evidence; authoritative first CH may later finish in same cycle.
- **Continuation/return:** Request reclaim eligibility only after a committed transition newly reaches Finished and current-unit uses are complete. Retaining pre-CH non-authoritative Handshake evidence does not request reclaim and does not enter terminal processing; later eligible acquisition may continue under the active-attempt policy. After committed Finished, subsequent units pass terminal candidate rules. Repeated/late evidence does not create a second Finished transition or a new reclaim request. No claim of TLS Finished or HANDSHAKE_DONE visibility.

#### Q12: 0-RTT and short-header 1-RTT

Authority: ACCEPTED P2 section 11; R0-R2; DEVICE.

- **Input:** Associated 0-RTT long header or short-header remainder.
- **Processing:** Use safely visible framing only; no payload keys or protected key-phase/PN interpretation. Short header does not encode DCID length; full association requires a trusted known length/context. No validation of 0-RTT against current CH early_data offer.
- **Observation:** Opaque noncompletion input; no CH/close/plaintext fact.
- **Dispatch:** None; no invented record callback.
- **Feedback:** Neutral.
- **Commit:** No state advancement, CH count or reset solely from these packets.
- **Continuation/return:** Known 0-RTT Length allows later coalesced packet; short header is final remainder. Return Continue when no further eligible unit.

#### Q13: Non-v1, reserved version and version-negotiation input

Authority: ACCEPTED P2 version policy; R0/R2; DEVICE.

- **Input:** Visible unsupported/non-v1 version, reserved version or version zero.
- **Processing:** Do not apply v1 packet-type/Length grammar to unknown version or assume version zero has ordinary Initial framing. Apply only supported invariant classification.
- **Observation:** Version/association capability outcome, not peer-flow violation.
- **Dispatch:** No invented CH/anomaly event.
- **Feedback:** Neutral absent separate real feedback.
- **Commit:** Associated non-v1 nonreserved nonzero-version long header in InProgress follows P2 Unsupported. Version zero is Version Negotiation, not that transition. Reserved/ambiguous/unassociated input does not inherit it automatically.
- **Continuation/return:** End unbounded remainder safely if no supported packet boundary; otherwise follow established outcome. No traffic drop or version negotiation generation.

#### Q14: Failed Initial unprotection

Authority: RFC R2/R3; PROPOSAL acquisition outcome; USER-BOUNDARY-01.

- **Input:** Bounded Initial that cannot be unprotected under supported candidate/retained context.
- **Processing:** Check HP sample and AEAD/tag extents within the current packet. For v1 AES HP, sample starts at PN offset + 4 and requires 16 bytes. Never borrow following coalesced-packet bytes. Use supported key/PN context; no unverified plaintext parsing or unbounded guessing.
- **Observation:** Protection/evaluation failure; not proof of malformed peer or attack.
- **Dispatch:** No automatic parsing-error-to-violation mapping; use only an existing representable outcome.
- **Feedback:** Neutral for unprotection failure without an actual supported anomaly projection. If the current unit is classified Unsupported, no consumer is called and no verdict can coexist; otherwise only genuinely representable non-Unsupported feedback is allowed.
- **Commit:** No CH/close authority, no automatic Error/Unsupported solely from AEAD failure. Required lost capability separately follows product contract.
- **Continuation/return:** If boundary known, consider later independent packet under gates; otherwise end affected remainder. Inspection exclusion is not network discard.

#### Q15: Malformed packet/frame or complete CH encoding

Authority: RFC R1/R2/R6; P3 OP-06; USER-BOUNDARY-01.

- **Input:** Current packet/frame with truncated declared fields or complete reassembled CH with inconsistent local encoding.
- **Processing:** Validate local bounds, frame grammar, shortest Frame Type encoding, v1 CID lengths, Initial frame admissibility and complete CH vectors. Check CRYPTO offset plus length without overflow against the RFC maximum of 2^62-1. The client Initial minimum uses complete UDP payload size, not each packet; incomplete capture does not prove original wire size. Empty decrypted frame payload is a local packet finding. Ordinary varints are not all canonical-only. Reserved-bit peer-error classification requires HP removal and successful AEAD verification. Packet overrun is not a CH fragment; CRYPTO gap is not malformed CH. Finish required current-unit evaluation before any consumer call: a required Unsupported outcome routes to Q16 with no Anomaly dispatch; a supported malformed/public-Stop outcome retains its actual anomaly path.
- **Observation:** Specific local parsing failure, no successful CH facts.
- **Dispatch:** For a supported malformed/local-error outcome, existing anomaly only if its contract genuinely represents the cause; otherwise internal outcome, not fabricated callback. A required-evaluation Unsupported outcome invokes no consumer at all (Q16), even if a local anomaly was independently available.
- **Feedback:** Actual consumer feedback only; error does not generate violation bits.
- **Commit:** No normal commit for the failed current evaluation; apply feedback disposition if present. Under proposed D3-ORDER, earlier separately resolved units are not retroactively rolled back by a later frame failure. Do not infer whole-packet atomicity or whole-packet RFC validity from a successful earlier CH unit.
- **Continuation/return:** Known outer packet boundary may permit later packet; unsafe frame boundary ends current packet, unsafe packet boundary ends remainder. Never resynchronize by guessing.

#### Q16: Unsupported grammar or resource capacity

Authority: P3 D01; DEVICE; RFC R6; PROPOSAL outcome.

- **Input:** RFC-valid framing requiring unavailable product feature/capacity, or a defined/extension grammar unimplemented by the product. A genuinely unknown protocol frame is a separate local encoding finding, not certified RFC-valid input.
- **Processing:** Distinguish recognized malformed from valid-but-unhandled and unknown grammar; do not assume all unknown frames are skippable TLVs.
- **Observation:** Unsupported/capability outcome, not invalid flow.
- **Dispatch:** None: no consumer, including Anomaly, ClientHello, record or other event, is called for the Unsupported unit (USER-U01). No early callback followed by same-unit required-evaluation Unsupported is allowed.
- **Feedback:** Neutral: local accumulator remains zero and no consumer Stop is generated. Historical masks and earlier-unit effects remain separate, not seeded into a fictitious current verdict.
- **Commit:** For the accepted associated attempt, required evaluation impossible -> R7 Unsupported disposition before any dispatch; preserve last valid historical facts/masks and do not commit current CH/count/time. Isolated unaccepted candidate failure preserves old active authority under Q09. No Error/Unsupported precedence is needed because no same-unit consumer verdict exists.
- **Continuation/return:** Return Continue for this no-consumer branch; end affected-attempt semantic processing, no replay/drop. Safe bounded independent/new-attempt candidate traversal is allowed only under existing association/terminal gates, never ordinary continuation of the same Unsupported attempt. Earlier Stop prevents reaching this unit. Numeric bounds/backing remain Phase 5 work; E04 is closed, not deferred.

#### Q17: Qualified CRYPTO_ERROR close

Authority: ACCEPTED P2 section 9.1/6; R4; PROPOSAL D3-ORDER.

- **Input:** Parsed 0x1c CONNECTION_CLOSE in successfully unprotected associated v1 Initial, code 0x0100..0x01ff.
- **Processing:** Parse current frame locally; qualify retained context/active association and state; includes 0x015a user_canceled.
- **Observation:** Terminating-alert evidence, not TLS Alert record and not automatic drop instruction.
- **Dispatch:** No close callback in inspected listener; no onTCPReset/onTerminalPayload surrogate.
- **Feedback:** Neutral unless separate applicable unit feedback exists.
- **Commit:** Apply P2 permitted terminating transition; do not infer PreHandshake termination merely because local CH reconstruction failed.
- **Continuation/return:** Later same-attempt CH blocked by terminal policy; accepted new-attempt classification remains possible if no Stop. Close order follows D3-ORDER.

#### Q18: Other closes and opaque encrypted close

Authority: ACCEPTED P2 section 9.1; R4; DEVICE.

- **Input:** 0x1c code outside CRYPTO_ERROR range, 0x1d application close where observable, or encrypted Handshake/1-RTT payload.
- **Processing:** Parse only available local encoding; 0x1d in Initial is a packet-local prohibited-frame fact, not an invented TLS alert. Never claim plaintext close from opaque payload.
- **Observation:** Nonqualifying close fact or opaque input; actual QUIC connection close is broader than product TerminatedByAlert.
- **Dispatch:** No new close callback or synthetic alert.
- **Feedback:** Neutral unless representable local anomaly feedback actually exists.
- **Commit:** No automatic TerminatedByAlert for nonqualifying code; opaque Handshake header can still follow Q11 product heuristic.
- **Continuation/return:** Continue/return by accepted state/association outcome; no claim that product state proves network connection still open.

#### Q19: Terminal late and retired input

Authority: ACCEPTED P2 section 12; Step 3 section 6.

- **Input:** Input matching terminal active/retired attempt, or ambiguous late input.
- **Processing:** Candidate classification before any new CH parsing; reject semantic reuse of matching old context.
- **Observation:** Late/retired/ambiguous outcome, not unexpected-flow violation.
- **Dispatch:** No CH replay or TCP terminal-payload callback for QUIC.
- **Feedback:** Neutral.
- **Commit:** Leave terminal state and authoritative CH/count unchanged.
- **Continuation/return:** Inspect only later safely bounded candidate units allowed by policy; otherwise return Continue. Never reopen same attempt.

#### Q20: STOP across messages, frames and packets

Authority: ACCEPTED Step 3 reducer; PROPOSAL QUIC traversal.

- **Input:** Any eligible supported current QUIC CH/anomaly unit with actual listener Stop. Unsupported units have no consumers and cannot enter this path.
- **Processing:** Keep current-unit backing; do not semantically deliver later messages even if already buffered.
- **Observation:** Current observation only.
- **Dispatch:** Stop current list; remaining current-unit projections only under shared zero-mask rule.
- **Feedback:** Latch Stop and OR masks; independent local acquisition outcomes do not manufacture verdicts.
- **Commit:** Final zero mask permits justified normal update; nonzero commits Error and suppresses normal update.
- **Continuation/return:** Return STOP; no later frame/message/packet, no drop/replay/free/reset merely due to Stop. New external input starts new transaction with resolved state.

#### Q21: First observed message is not CH

Authority: USER-BOUNDARY-01; P2 observation policy; R1/R5.

- **Input:** Locally parseable non-CH handshake message or non-CRYPTO frame before any reconstructed CH.
- **Processing:** Identify local type/length and supported framing; do not apply first-message-must-be-CH as Processor flow check.
- **Observation:** Available local message/frame fact; no CH authority.
- **Dispatch:** Only existing applicable supported projections; no invented generic QUIC record event.
- **Feedback:** Enforcement decides illegal sequence only when an actual compatible observation contract exists.
- **Commit:** No CH count or InProgress merely from non-CH; bounded product progress evidence follows its own accepted policy.
- **Continuation/return:** Continue supported acquisition or explicit Unsupported capability outcome; not a flow-validity rejection. Unsupported selects Q16 without any consumer call. Missing broader Enforcement observation capability remains a future-reuse limitation, not a new current event/API requirement.

#### Q22: Session aging and resource eviction

Authority: P3 OP-10; Step 4 V2 lifecycle; DEVICE.

- **Input:** Session aging or resource lifecycle disposition.
- **Processing:** Preserve valid semantic data through last synchronous callback use; release only via supported lifecycle.
- **Observation:** Session end/resource cause, not inferred incomplete-handshake attack.
- **Dispatch:** Existing SessionEnd list for actual aging; eviction does not invent that callback unless its contract provides it.
- **Feedback:** Existing SessionEnd verdict ignored; no manufactured violation.
- **Commit:** Release/reset lifecycle, not network Error or automatic Incomplete Handshake violation.
- **Continuation/return:** Return lifecycle result; subsequent missing context follows participation/acquisition contract, not stale pointer reuse.


## 7. Consumer compatibility and common boundary

| Consumer | Shared sufficient part | Context and capability limitations | Protocol-specific work / evidence status |
|---|---|---|---|
| Fingerprint | Current fingerprint and conditional same-CH canonical full string; unchanged shared algorithm | Policy/profile/server and completion-input statistics; full string is separate viewer dependency, not in ClientHelloData. Neutral listener feedback can coexist with manager side effects | Preserved implementation review records TCP RST/AX/session-aging action path. Current QUIC action/admission implementation unverified; retained as an implementation constraint, not a boundary-analysis blocker under USER-SCOPE. Never approve TCP RST as QUIC action |
| Visibility | Current fingerprint/SNI/projected CH version plus reporting/server context | Shown implementation is CH-only, not lifecycle/alert reporter. Projected version is not proven negotiated. No automatic transport key dimension or deep-copy guarantee | No packet action recorded in shown path; reporting/SNI normalization is shared. Preserve synchronous borrow contract; do not reopen helper storage audit merely for design |
| Enforcement | Current CH facts, precommit TLSData, representable local errors and actual feedback | Actual TCP record/order/alert/reset semantics are not QUIC observations. No generic non-CH/close/completion event. Client-only visibility limits Retry/HRR/server evidence | Flow legality and mitigation remain Enforcement. Future QUIC implementation excluded. Routing is recorded; exact detector field sufficiency/full reuse not certified |

Boundary closure for EG6 (USER-SCOPE/USER-U01): Fingerprint and Visibility consume CH only in the current scope. Both require a supported current CH; Fingerprint additionally has its same-CH full-string dependency. Existing participation is still governed by the actual feature/session mask and applicable port policy, not a new universal gate. The analysis resolves that these contracts do not require a different shared semantic transaction or new interface. Exact wiring and concrete action compliance remain unverified implementation work; no deployment or unchanged-consumer compatibility is approved. Protocol-specific action selection must remain outside the shared transaction and must not send TCP RST as QUIC mitigation. Broader Enforcement events are documented for future reuse, not silently disabled and not a prerequisite for this phase's boundary analysis. No new activation restriction or product action is invented.

Evidence: CONSUMERS sections 3-6; Phase 3 section 8 C01-C06/C-S01..04; CODE ITlsListener 3078-3160 and Conductor projections 3587-3668. Six consumer-body findings are prior review evidence, not fresh raw-source proof. Fingerprint discarded Action, Enforcement neutral alert/reset/end returns and Visibility local collection abort are not automatically TLS-flow Stop/violations. A suppressed normal commit cannot undo their side effects.

Common transaction assessment: reject external input/datagram, universal packet/record carrier, individual callback, and whole attempt as universal semantic commit boundaries. Current dispatch-and-disposition unit is useful; verdict reduction alone is insufficient for freshness/lifetime/progress. This is a design recommendation, not a selected common class/interface/type. TCP and QUIC retain different acquisition loops and projections. COMMON sections 1-7.

Lightweight DTLS counterexamples from COMMON section 8: fragmented/out-of-order CH, handshake retransmission identity distinct from record number, multiple records/datagram, DTLS cookie/version layout, retry CH and protected/invalid record. They show that TCP sequence, QUIC CRYPTO offset, Initial-key model and TLS byte layout must not be universal shared inputs. No DTLS parser/state machine/fingerprint equivalence/support is promised. RFC9147/6347 anchors are inherited from COMMON; no new implementation requirement.

## 8. Focused walkthroughs and rechecks

### 8.1 Executed abstract feedback model

The Python model below was executed through the container as review support. It mirrors inspected notify/analyzeProcessResult sequencing; it is NOT C++ execution, real packets, consumer testing or runtime certification. Verdicts are deliberately synthetic test vectors, not simulated production data. A second event-level formulation was compared over all 4096 two-projection/two-observer combinations using Continue/Stop and masks 0/1/2/3. This checks model consistency only; source fidelity was separately inspected at CODE 3514-3585/4008-4026.

| Case | Input verdict lists (S=Stop, C=Continue) | Observers actually called | Disposition / mask / return / timing | Check |
|---|---|---|---|---|
| M01 | [C:0]; [C:0]; [C:0] | P1.O1, P2.O1, P3.O1 | NormalPermitted / 0 / Continue / final | PASS model check |
| M02 | [C:1]; [C:2] | P1.O1, P2.O1 | ViolationOnly / 3 / Continue / final | PASS model check |
| M03 | [S:0, C:1]; [C:0] | P1.O1, P2.O1 | NormalPermitted / 0 / Stop / final | PASS model check |
| M04 | [S:1]; [C:0] | P1.O1 | Error / 1 / Stop / immediate | PASS model check |
| M05 | [S:0]; [C:1]; [C:2] | P1.O1, P2.O1, P3.O1 | Error / 3 / Stop / final | PASS model check |
| M06 | [C:1]; [S:0]; [C:2] | P1.O1, P2.O1 | Error / 1 / Stop / immediate | PASS model check |
| M07 | [S:0]; [C:1]; [S:0] | P1.O1, P2.O1, P3.O1 | Error / 1 / Stop / immediate | PASS model check |
| M08 | []; [] | none | NormalPermitted / 0 / Continue / final | PASS model check |

M03 proves the skipped second observer in a stopped list contributes no mask. M05 proves an earlier zero-mask Stop does not abort immediately merely because a later Continue adds a violation; the third projection still runs. M06 proves local zero mask does not erase earlier accumulation. M08 does not mean a no-callback unit always changes state; only justified policy disposition is permitted.

### 8.2 Static flow/state/lifetime walkthroughs

| ID | Trace and expected disposition | Evidence | Result |
|---|---|---|---|
| W4-01 | TCP CH -> error/record/CH callbacks read precommit state -> permitted CH update -> next record only if RecordParsed and updated state active | CODE 3472-3585/3820-3838; T02/T04 | PASS static |
| W4-02 | TCP current record commits Finished -> reclaim eligibility -> active-state predicate false -> no next record in same invocation | CODE 3480-3481/3576-3582; T11 | PASS static |
| W4-03 | TCP supported pending continuation -> NeedMoreData -> no callback/normal commit -> Continue; Unsupported uses conditional guard; public Stop uses actual anomaly feedback | CODE 3486-3512/5564-5604/3873-3884; T03/T08/T09 | PASS static |
| W4-04 | Retained TCP Complete callback on later non-CH record -> not new logical CH; normal update uses actual record predicates | CODE 3559-3582/3788-3838; U05; T04 | PASS static |
| W4-05 | Pending TCP duplicate prefix -> existing sequence skip; later complete CH without continuation can be parsed again -> no universal TCP dedup promise | CODE 5410-5562; T05 | PASS static |
| W4-06 | QUIC gap -> bounded ranges retained -> later range completes CH -> one eligible dispatch -> feedback before later message | P2 section 7; P3 FB-B; Q03-Q05 | PASS contract |
| W4-07 | One range unlocks CH A/B -> A Stop -> no B dispatch/authority; third CH alone is not Processor flow verdict | Q04/Q07/Q20; user boundary | PASS contract |
| W4-08 | Pre-CH associated Handshake -> retain non-authoritative evidence, no reclaim -> first permitted authoritative CH can finish same cycle; violations suppress CH/completion | P2 sections 6/9; Q11; P3 FB-C | PASS contract |
| W4-09 | CH then close -> resolve each in proposed encounter order; close then CH -> terminal conditions first; PreHandshake reconstruction failure does not fabricate termination | P2 section 9.1; Q17/Q19; D3-ORDER | PASS under proposal, not accepted order |
| W4-10 | Terminal candidate matches any retained old Initial context -> late input/no CH parse; distinct accepted candidate -> reset before parsing -> requalify later units | P2 sections 10.2/12; Q10/Q19 | PASS contract |
| W4-11 | Active candidate needs complete CH identity -> isolated evaluation -> acceptance/reset before authority; failed candidate cannot contaminate active context | P2 section 10.1; P3 FB-A/D01; Q09 | PASS contract |
| W4-12 | Retry-compatible context restart -> same attempt; byte-equivalent CH not new observation; token/DCID alone insufficient | P2 section 8; Q08 | PASS contract |
| W4-13 | Initial A AEAD fails -> no plaintext authority/automatic violation; known boundary can permit packet B; HP sample cannot borrow B bytes | FLOWS R2/R3/Q14; P3 OP-03 | PASS under acquisition proposal |
| W4-14 | Malformed current CRYPTO frame -> insert none of failed frame; prior separately resolved CH not rolled back; unsafe frame/packet boundary ends affected remainder | Q15/Q16; D3-ORDER | PASS under proposal |
| W4-15 | 0-RTT before CH -> no state change/offer legality check; opaque Handshake/1-RTT cannot yield plaintext close | P2 section 11; Q12/Q18 | PASS contract |
| W4-16 | Finished then new attempt -> stale reclaim marker cannot authorize active lifecycle eviction; release/rebuild pattern preserves registration semantics, not guaranteed same object | P4-4 V2 section 8; CODE 2829-2926/3393-3426; Q10 | PASS semantic obligation; concrete mechanism Phase 5 |
| W4-17 | Aging -> actual SessionEnd notification -> ignore verdict -> release; resource eviction is not invented SessionEnd/Incomplete Handshake violation | CODE 3669-3682; P3 OP-10; T10/Q22 | PASS static/contract |
| W4-18 | Required fingerprint evaluation Unsupported before dispatch -> no consumer including Anomaly; R7 qualified Unsupported, no new CH authority; Continue | USER-U01; D01; Q16 | PASS contract |
| W4-19 | Independently available anomaly plus required-evaluation Unsupported in same unit -> zero consumer calls; no verdict; prior-unit effects/masks retained; no Error precedence needed | USER-U01; section 4 exclusion proof; Q15/Q16; G05-G08 | PASS contract exclusion, not runtime proof |
| W4-20 | FP/VIS CH-only supported result; FP same-CH full string; TCP-specific action remains outside common transaction; wiring/action not certified; broader ENF reuse excluded | USER-SCOPE/USER-U01; section 7; CONSUMERS | PASS boundary analysis; implementation caveat retained |

### 8.3 Corrections applied in this consolidation and affected rechecks

- R4-F01: conflicting older TCP next-record prose is replaced by exact CODE predicate RecordParsed AND updated active state, with Stop first. Rechecked T02/T04/T11, W4-01/02 and G08. No source patch or baseline overwrite.
- R4-F02: public parser outcome, not nested status/lastError alone, selects dispatch. T03 Unsupported/Stop commit and return are explicit; no generic TCP stream reconstruction promise. Rechecked W4-03 and EG1 known-path coverage.
- R4-F03: retained pre-CH completion does not request reclaim; only committed eligible Finished does. Rechecked Q11/W4-08/16 and G09.
- R4-F04 (V2 historical finding, superseded at analysis scope): V3 applies USER-SCOPE and resolves EG6 as the four-axis semantic boundary, retaining exact action/admission compliance as unverified implementation constraints. No missing code is relabeled verified.
- R4-F05 (V2 historical finding, now closed): USER-U01 excludes every consumer, including independent Anomaly, on Unsupported. V3 makes all required evaluation pre-dispatch, updates Q02/Q04/Q09/Q14-Q16/Q20/Q21 and G05-G08, and rechecks W4-18/19. No invented Error/Unsupported priority; malformed/public Stop stays separate.

R4-F01..03 are consolidated text corrections already supported by FLOWS V5 and direct source; R4-F04/05 correct final-gate interpretation. V3 adds explicit user-contract/scope closure rather than claiming prose edits prove implementation. See sections 4/7/9/10.

## 9. EG1-EG9 independent exit-gate evaluation

Gate text is reproduced exactly from PLAN Phase 4 Exit gate, in order. PASS is static pre-design evidence at the stated scope, not implementation/RFC/runtime certification. All nine original conditions are reviewed in order against the latest accepted scope and USER-U01. Per-gate evidence and limits follow; section 11 records repeated mechanical checks separately from semantic review.

### EG1: PASS

**Condition:** Every input has a defined path through processing, dispatch, commit, continuation, and return.

**Evidence and finding:** All 34 seven-stage paths retained. USER-U01 and section 4 exclude every consumer on Unsupported; Q15 distinguishes malformed from Unsupported and Q16 explicitly defines no dispatch, R7 disposition, Continue return and terminal/independent-candidate progress. W4-18/19 now cover independent anomaly availability without dispatch. Active isolated candidate and earlier-unit cases are explicit.

**Scope / closure:** E04 closed by accepted pre-dispatch exclusion, not by structural count alone. This is a defined contract for the proposed QUIC path, not implementation execution proof.

### EG2: PASS

**Condition:** STOP with and without violations preserves the intended transaction behavior.

**Evidence and finding:** Section 4 reducer; executed M01-M08 and 4096 abstract combinations; direct CODE notify 4008-4026/analyzeProcessResult 3514-3585; W4-07/08. Stop with zero/nonzero masks and immediate/final Error timing match source.

**Scope / closure:** PASS covers ordinary reducer and documented exceptional paths. USER-U01 keeps Unsupported outside the reducer; no ordinary Stop timing or historical TCP public outcome is changed.

### EG3: PASS

**Condition:** `TLSData` read/write authority and state-update timing are explicit.

**Evidence and finding:** Section 5 retains all nine fields and R7 as semantic writer. USER-U01 places qualified Unsupported disposition before any dispatch, without normal current-CH writes or new mask/Error; historical facts remain. Candidate isolation/reset timing remains Q09/Q10; malformed/public Stop retains actual feedback. G05-G08 and W4-18/19 are rechecked.

**Scope / closure:** No mixed same-unit Unsupported/verdict exists under the accepted contract. Concrete backing/lifecycle remains Phase 5, not a missing semantic writer/timing decision.

### EG4: PASS

**Condition:** Gates are not duplicated with contradictory policies.

**Evidence and finding:** Section 5 G01-G09; exact CODE TCP loop; P2 terminal candidate before CH parsing; R4-F01; W4-02/10. Defensive checks do not create a competing state writer or gate policy.

**Scope / closure:** PASS for defined gates; USER-U01 is one pre-dispatch eligibility rule, not a competing precedence gate. R7 remains the only semantic writer; source TCP exceptions are preserved.

### EG5: PASS

**Condition:** Shared operations and protocol-specific interpretation are clearly separated.

**Evidence and finding:** Sections 3/4/6/7; TCP records versus QUIC packets/frames/CRYPTO/messages; local syntax versus Enforcement legality; P3 OP families; COMMON sections 3-8.

**Scope / closure:** Shared feedback/freshness/disposition is separated from protocol acquisition, association, completion/close interpretation and action. No universal record facade.

### EG6: PASS

**Condition:** Compatibility with existing feature-consumer contracts has been evaluated; speculative functionality is excluded and architecture-sensitive external contracts are resolved.

**Evidence and finding:** Section 7 and CONSUMERS map actual FP/VIS CH-only field use, same-CH full-string freshness, precommit data, consumer side effects, and action-versus-verdict distinctions. USER-SCOPE excludes helper/storage compliance audits; USER-U01 confirms broader Enforcement events are not a current implementation requirement. The architecture-facing contract is resolved: supported current CH projection, existing participation contract, protocol-specific actions outside shared feedback, no new common API/class. W4-20 checks this boundary, not unavailable action code.

**Scope / closure:** PASS is for the agreed pre-design compatibility analysis, not unchanged QUIC consumer execution, verified wiring, mitigation implementation or release readiness. TCP RST is explicitly not approved for QUIC. Implementation must satisfy these retained constraints; this is not an accepted disabling restriction or an assertion that missing code works. No gate wording is changed.

### EG7: PASS

**Condition:** Enforcement compatibility distinguishes shared semantic facts from protocol context, observation capabilities, and protocol-specific actions. Future reuse does not become a current implementation requirement.

**Evidence and finding:** Section 7 matrix; CONSUMERS sections 4-6; Phase 3 consumer constraints; COMMON section 7. Shared CH/context, protocol-specific records/actions and absent capabilities explicitly distinguished.

**Scope / closure:** PASS as boundary evaluation, not certification of all detectors or future QUIC Enforcement. Missing observations remain explicit; no synthetic callback or new consumer functionality.

### EG8: PASS

**Condition:** A common semantic transaction has been considered explicitly, not dismissed solely because TCP and QUIC have different wire iteration.

**Evidence and finding:** Section 7 common assessment and section 4 sequence; COMMON sections 3-6/8 with six granularity alternatives and DTLS counterexamples.

**Scope / closure:** Common semantic transaction explicitly considered and useful at current unit; different wire loops do not justify dismissing it. No common API/class selected.

### EG9: PASS

**Condition:** No class split is required merely to make the diagrams look uniform.

**Evidence and finding:** Sections 2/3/7; logical R1-R7 roles only; inline orchestration remains allowed; COMMON sections 1/3/5/10.

**Scope / closure:** No class split/interface/type introduced merely for uniform diagrams. Concrete reuse deferred until demonstrated implementation need.

## 10. Closure, candidate decisions and Phase 5 handoff

| Former item | Closure authority | Rechecked evidence | Current status |
|---|---|---|---|
| B4-01 / E04 (EG1/EG3) | USER-U01: Unsupported invokes no consumer, including Anomaly; all required evaluation before dispatch | Section 4 exclusion proof; Q02/Q04/Q09/Q14-Q16/Q20/Q21; G05-G08; W4-18/19 | CLOSED at contract boundary |
| B4-02 / C6-05/C6-06 (EG6) | USER-SCOPE: boundary analysis only; FP/VIS CH-only, future broader ENF reuse not a current implementation requirement | Section 7 actual dependencies/action separation; W4-20; EG6 | CLOSED as analysis blocker; implementation caveats retained |

These closures do not prove private wiring/action/storage compliance, change TCP code, disable Enforcement, or authorize TCP RST on QUIC. No additional helper code is requested. The latest user contract refines older hypothetical E04 prose; all older files remain historical immutable evidence.

### Candidate design carried forward, not falsely accepted

D3-ORDER incremental traversal, the useful common-boundary recommendation and D-TIME remain explicitly PROPOSAL/candidate design. Their consistency is reviewed; this delivery does not manufacture user approval. Phase 5 may evaluate ownership/bounds against these documented candidates and must revisit affected feasibility if a candidate changes. Final HLD/implementation freeze must settle applicable candidates; this does not create another EG or require a common interface/class now.

### Correctly deferred Phase 5 work (E03)

Define ownership/lifecycle for TLSData, Extension observer lists, scratch/viewers, TCP continuation, QUIC CRYPTO/protection contexts and attempt/duplicate/completion identity. Cover create/reset/release/aging/eviction/allocation failure and interleaved session/protocol isolation; same-input replacement, reclaim membership and callback/commit backing last use. Choose/verify bounded work, memory, copies, allocation and crypto costs. Do not invent numeric limits, tokens, generations or pools without need.

Numeric values may remain for HLD only when they cannot change architecture, ownership, supported behavior or feasibility, as PLAN requires. Action/admission implementation constraints from section 7 remain explicit integration requirements, not proof of current compliance and not disguised ownership work. No future QUIC Enforcement or DTLS implementation is added.

**Readiness: READY FOR PHASE 5 design. EG1-EG9 PASS at the agreed static pre-design boundary.** This is not release/implementation certification or a claim that all candidate proposals have been accepted. No E04 precedence remains to be invented.

## 11. Review evidence, source map and preservation

Historical V2 execution: abstract notify/transaction model M01-M08 plus 4096 synthetic combinations. V3 reruns two independent formulations over the same 4096 synthetic combinations, adds Unsupported no-dispatch and cross-unit cases, checks all 34 seven-stage paths and nine original gate conditions, and verifies source manifests/before-after hashes. Detailed results are in the companion V3 review report. Static reasoning: W4-01..20, P2/P3 consistency, exact TCP predicates, field/gate mapping, consumer boundary and common-transaction assessment. Not executed: C++ runtime, packet decryption/reassembly implementation, actual consumer/mitigation tests, full Phase 3 inventory suite or complete RFC/errata compliance audit. Repeated mechanical checks on identical bytes are recorded separately; no two independent full semantic reviews or production tests are claimed.

Exact CODE anchors: prepareInboundExtension 3393-3426; process 3428-3484 (loop 3480-3481); dispatchStepResult 3486-3512; analyzeProcessResult 3514-3585; projections 3587-3668; onAging 3669-3682; commitNormalCurrentRecord 3788-3818; updateTLSDataOnClientHello 3820-3838; commitUnsupported 3873-3884; commitViolation/commitError 3886-3898; notify 4008-4026; beginPacket 5410-5445; resetStepState 5458-5480; resumeOrDiscardStepContinuation 5482-5562; processRecord 5564-5604; processNewRecord 6164-6438; processClientHello 6810-6879. File: paths in the bundle control; line ranges are locators, not executable tests.

RFC provenance retained in FLOWS: RFC9000 sections 8.1/12.2/12.4/12.5/14.1/17.2/19.6/19.19; RFC9001 sections 4/4.7/4.8/5.2/5.4/5.5; RFC8446 sections 4.1.2/5.1. COMMON DTLS counterexamples use RFC9147 sections 4.2/4.3/4.5.2/5.1/5.2/5.3/5.5 and historical RFC6347 sections 4.1.1/4.2.1/4.2.2/4.2.3. Published URLs are recorded in the supplied documents; this turn consolidates those wire facts rather than claiming a new web audit.

### RFC/device ledger retained from FLOWS V5


[R0] RFC 9000 sections 12.2, 17.2: Initial/0-RTT/Handshake have explicit packet Length; short header consumes the remaining datagram and is last. Coalesced packets are independently processed. Safe packet boundaries permit skipping an uninspectable packet without losing later packets. Senders MUST NOT coalesce different connection IDs; receivers SHOULD ignore later differing-DCID packets. Retry/VN have no Length or frames and are not coalesced. A same-datagram replacement trace is not automatically compliant. Endpoint processing requirements are not permission for this feature to drop traffic. Source: https://www.rfc-editor.org/rfc/rfc9000.html#section-12.2

[R1] RFC 9001 sections 4, 4.1.3: CRYPTO carries TLS handshake bytes, not TLS record framing. Encryption levels have distinct byte sequences; CRYPTO is not carried in 0-RTT. Message completion is determined by ordered byte coverage and the handshake message header, not one frame or packet. Sources: https://www.rfc-editor.org/rfc/rfc9001.html#section-4 and https://www.rfc-editor.org/rfc/rfc9000.html#section-19.6

[R2] RFC 9001 sections 5.2-5.4: Initial keys derive from the initial context's client DCID; a later changed DCID is not automatically a new key context. Remove header protection before reading protected PN length/reserved bits/PN; reconstruct PN before AEAD. In v1 LONG headers the packet-type bits are NOT header-protected: only the low four bits of the first byte are masked. Short-header key phase IS protected. Thus associated v1 Handshake header evidence does not require unavailable Handshake keys merely to classify its long-header type. This corrects any broad earlier wording suggesting all packet-type bits are protected; it does not change Phase 2 completion policy. Source: https://www.rfc-editor.org/rfc/rfc9001.html#section-5.4

[R3] RFC 9001 sections 4.9.1, 5.5, 7: Initial AEAD verification is not peer authentication; public Initial secrets permit spoofing. Failure to unprotect does not itself prove a protocol error or attack. Source: https://www.rfc-editor.org/rfc/rfc9001.html#section-5.5

[R4] RFC 9001 section 4.8 and RFC 9000 section 19.19: TLS alert descriptions map to 0x0100+description in transport CONNECTION_CLOSE 0x1c. QUIC closes are not TLS Alert records. Source: https://www.rfc-editor.org/rfc/rfc9001.html#section-4.8

[R5] RFC 8446 sections 4.1.2, 4.1.4, 5.1 and RFC 9001 section 4.7: TLS handshake bytes can be fragmented/coalesced; a second CH can follow HRR. These endpoint sequence requirements are Enforcement concerns here; client-only visibility cannot prove HRR was received. Sources: https://www.rfc-editor.org/rfc/rfc8446.html#section-4.1.2 and https://www.rfc-editor.org/rfc/rfc9001.html#section-4.7

[R6] RFC 9000 sections 12.4-12.5, 19: frames are type-dependent, not a universal TLV. Known supported non-CRYPTO frames must be parsed/skipped using their actual grammar. An unknown frame cannot be skipped by guessing a length. Separate defined-but-product-unimplemented grammar from genuinely unknown protocol frame: RFC 9000 assigns endpoint FRAME_ENCODING_ERROR to the latter. Base Initial permits PADDING, PING, ACK, CRYPTO and transport close 0x1c. Other recognized frame types are packet-local prohibited-frame findings. Frame Type requires shortest varint encoding; ordinary varints do not all require shortest encoding. Source: https://www.rfc-editor.org/rfc/rfc9000.html#section-12.4

[DEVICE] ACCEPTED scope: client-to-server only; QUIC v1 only; Initial keys only; no server Retry/HRR/Finished/transport-parameter visibility; no Handshake/0-RTT/1-RTT plaintext. The accepted associated client Handshake header is a product Finished heuristic, not RFC handshake completion or confirmation. Phase 2 sections 5.2, 6, 9-12 are authoritative. Required fingerprint uses the unchanged shared CH algorithm (P3 U04). Resource inability to evaluate required CH facts follows D01 Unsupported, not a synthetic violation.

[R7] RFC 9000 8.1/14.1: client UDP datagrams carrying Initial require at least 1200 bytes of UDP payload, not 1200 bytes per coalesced packet. Local reporting and endpoint network-discard action remain separate. Source: https://www.rfc-editor.org/rfc/rfc9000.html#section-14.1

Numeric QUIC memory/range/context limits and runtime budgets are NOT supplied by the RFC or frozen device specification here. No values are invented. Phase 5 must choose/verify bounds and backing lifecycle. Engine-runtime sensitivity motivates bounded work and feedback pauses; it does not justify assuming arbitrary numeric budgets or hidden threads.


### Input integrity manifest

Preserved available Markdown inputs and CODE, SHA-256 (freshly computed and checked unchanged after writing):

- `phase1-copilot-report.md`: `9c25e615a26c28f65ddc3a26ee61c9e18ce90212c9ee0f9ca0b0506c8b840a69`.
- `phase2-state-machine.md`: `b2d0ae9254ed6e96c643281bfc09302dd8c8360b9672ff16611055820f02de77`.
- `phase3-semantic-processing-contract-review-v5.md`: `b3278e90d39b6e69396f4b87725e75aeac1aa9baa98f826001964cc16b46c438`.
- `phase4-step1-baseline-and-scope-v1.md`: `8e2f3725cb9da9eee0efefd7fcce3ab7e6a17b0cc5b6222d0c392b8766d67c35`.
- `phase4-step1-baseline-and-scope-v2.md`: `ce953299d320cd7b3fa5f7b865641d651bf273764087584b136e0c2debaa91d6`.
- `phase4-step2-responsibility-and-control-mapping-v1.md`: `4e4eceeac56d13e2fc2fd99cbffa3e1db4b0793fc83df248edaaa6008be9120c`.
- `phase4-step2-responsibility-and-control-mapping-v2.md`: `603537bbd646af232fb783f4a1beb1524f1255e6ca27fa8bcd7a24a51edaaeed`.
- `phase4-step3-dispatch-and-verdict-transaction-v1.md`: `403415ea6e6288f126f8facbdcd8a160379bfc89f12c9566bb7117ecae723c72`.
- `phase4-step3-review-report-v1.md`: `03b95983a84046f4e3457ac1f972c6bf0820256b46652ffe9d51e38216e77c64`.
- `phase4-step4-gates-and-state-commit-v1.md`: `31d73244651202ee2af2d432d4a9a791275773c20c83c854b16a1e17e74ba152`.
- `phase4-step4-gates-and-state-commit-v2.md`: `a0e6751809e3dcbe43d7d2e5a912db7c1240779548a92324a81215fafccc4d00`.
- `phase4-step5-complete-runtime-flows-v1.md`: `d2ad09969213056a9ef9ffdaa0c822f75c08b4b6b454b8aabd2c10458beb4379`.
- `phase4-step5-complete-runtime-flows-v4.md`: `16fe167311f7ebafef57a94a5a175c7123dcf09b7e3ca245a32ad2ad7d5df00a`.
- `phase4-step5-complete-runtime-flows-v5.md`: `b5018bd41e85d6b94a0a62558196ed8082da5522cdfccaa3e4b04bf695e49856`.
- `phase4-step5-review-report-v1.md`: `428d3251313e396cbd81e8b852b6ec7c00d6761e0921ab8d6502751633f3ab22`.
- `phase4-step5-review-report-v4.md`: `77cbb6645b6afca17c681ee034e6e42fd0272d3094c73c1a86db960a15064346`.
- `phase4-step6-consumer-compatibility-v1.md`: `baecd59c32d00ee2b1914312ea4cf94de544ab0e62668e4f527199e924839d2c`.
- `phase4-step6-consumer-compatibility-v2.md`: `720d43d506ed6174cde6bbb03c9a66b20bc3f555ee4e6c672ab8087f2338836f`.
- `phase4-step6-consumer-compatibility-v3.md`: `2b393139d7ae4d5eec25d70ebafbf67c00b75ba3f41371d60e89c0e8dce9ee2f`.
- `phase4-step7-common-semantic-transaction-v1.md`: `b983537d7ce6ce1693780bbbd392e5bad9ae0f2a54fe0847f02a803a5b429324`.
- `phase4-step7-common-semantic-transaction-v2.md`: `8e6f733d2a06267a14ea5f2efa56b14724c06a196abd517e9d69e42fcc9166b4`.
- `phase4-step7-review-v1.md`: `52083a68f8008912c387ca594b089255d68ca37a41c1e4542757597a0a18aaf2`.
- `phase4-step7-review-v2.md`: `8411249710489245dbd8a05b964668869ad09a030e9d85e86a79dca010e01502`.
- `tls-flow-quic-pre-design-work-plan-v5.md`: `55ea1536ac4913e3a4ce59cc717fc3f5e8752dcdddf51be66bed34b280c024fa`.
- `TLS-Flow-All-Code.txt`: `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b`.

Unpublished consolidation draft preserved: `phase4-runtime-orchestration-summary-v1.md`, SHA-256 `1572b29e8d0d7c10e4e2fd318170d6d5c6a7751558f10e77e041349b2ff00b32`. V2 remains preserved. V3 explicitly incorporates USER-U01/USER-SCOPE, closes E04 by no-dispatch ordering, and changes EG1/EG3/EG6 at the agreed analysis boundary. It does not claim the historical V2 statuses were already PASS.


### V3 review delta and authority

The latest user decisions are conversation authority, not RFC requirements or discovered source behavior. No fresh RFC/errata audit was needed for these policy/scope edits; the inherited RFC ledger remains unchanged. All 12 TCP flow bodies remain byte-identical to V2. QUIC flow changes are restricted to the Unsupported dispatch boundary, evaluation timing and affected continuation. Ordinary notify/transaction timing is preserved. No runtime completion-signal issue is newly resolved or strengthened by these changes.

V2 SHA-256: `df0f80c4a9d078b97e8fc2331d8c48c9bb4e8cabbbe0e44a4f9332ed5315017f`.
