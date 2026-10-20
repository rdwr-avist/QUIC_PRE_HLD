# Phase 7 Pre-Design Decision Package V1

**Status: REVIEWED CANDIDATE at the Step 6 static boundary. Separate Step 7 final review and explicit acceptance/freeze have NOT occurred. No activation is authorized.**

This nine-part package consolidates the active reviewed decisions, not every historical draft. Its evidence closure is identified by exact filenames and SHA-256 in the external Step 6 provenance. The separate Step 6 gate assessment records EG1-EG7; neither that assessment nor this status grants the separate overall final gate. Evidence IDs below are local navigation, not new requirements.

## 1. Scope and relevant requirement mapping

### 1.1 Source authority and active evidence

The original CODE/PLAN/P1-P6 inputs retain the identities in phase7-baseline-intake-03.json. WF V3 governs the focused review procedure and nine-part structure; it explicitly supersedes the older exhaustive intake procedure without claiming it was completed. P2-P5 accepted contracts are not automatically overridden by P6. CODE is the supplied snapshot, not an inferred repository revision or executed target. FRD and SRS are supplementary product sources read together; SRS does not silently erase conflicting FRD requirements.

| Alias | Exact active file | Use / precise locator |
|---|---|---|
| CODE | TLS-Flow-All-Code.txt | Current behavior: Conductor L3393-3682/L3788-3896/L4008-4025; TCP processing L5410-5657/L6301-6438/L6776-6879; table L2898-2925; startup L1561-1586. |
| PLAN | tls-flow-quic-pre-design-work-plan-v5.md | Section 1 scope/source discipline; Phase 7 definition and original seven gates. Not the superseded Phase 7 execution method. |
| P1 | phase1-copilot-report.md | Section 4 SD1-SD8 suspected findings and historical mapping status; dispositions are S4 Section 6, not automatic production bugs. |
| P2 | phase2-state-machine.md | Sections 5-10 accepted state/attempt/CH/restart/association policy; Sections 7-10 govern freshness and replacement. |
| P3 | phase3-semantic-processing-contract-review-v5.md | Accepted semantic/helper/FP/view contracts; tlsVersionMask advertised-versus-negotiated distinction L1293-1301. Inherited evidence retains its limits. |
| P4 | phase4-runtime-orchestration-summary-v4.md | D4-01 legality/action boundary; Section 4 feedback/commit; Section 7 consumer matrix/current-versus-future applicability. |
| P5 | phase5-ownership-lifecycle-runtime-bounds-summary-v7.md | D6-01..08 L417-471 and Section 10.2 handoff; ownership, restoration, serial borrows, support loss, finite startup backing and representation preflight. |
| P6 | phase6-architecture-selection-summary-v1.md | L335 evidence-triggered API/layout check; Sections 7-11 roles/flows, symbol impact, D1-D6 delivery, optional work and downstream evidence. |
| S1 | phase7-validation-baseline-step1-v2.md | Focused P7-VB-01..48 and dependency register; timestamp proposal and product-mapping historical status are qualified by the later explicit overlays below. |
| S2 | phase7_step2_focused_scenario_pack_v12.md | Active oracles: F1-F9, five TCP boundary subcases, U7-PARSE-01/U7-SCOPE-02, NF-01..14 and Section 5 active coverage. |
| S3 | phase7-step3-selected-design-walkthrough-v6.md | Active 84 case-ID headings: expected versus selected-design trace, supporting source anchors, ownership/last use and target-implementation limits. |
| S4 | phase7_step4_traceability_consistency_validation_v9.md | Sections 3-4 product forward/reverse mapping and scope dispositions; Section 5 consistency; Section 6 defects; Sections 7-8 validation/deferred items; Section 10 review. |
| S5 | phase7-step5-kiss-yagni-delivery-review-v1.md | K01-K11 abstraction challenge; Section 3 existing-symbol reconciliation; D1-D6; E01-E07; optional/excluded work and activation limits. |
| WF | phase7-workflow-v3.md | Sections 4-5 nine-part structure and EG1-EG7; Steps 6-7 separate package review from final review/acceptance/freeze. |
| FRD | Quic_FRD.md.txt | FR-01..10, NFR-01..04 and relevant scope/pipeline prose; classified in S4 Section 3. |
| SRS | Quic_SRS.md.txt | Introduction, Sections 1.1/2/3/4.2-4.4.9 and explicit empty/TBD fields; classified in S4 Section 3. |

**Evidence kinds:** CODE FACT = supplied implementation behavior, not execution. ACCEPTED CONTRACT = supplied accepted decision or direct user authority. DESIGN JUDGMENT = chosen minimal realization, not a verbatim user-approved API. INHERITED EVIDENCE = prior inspected/recorded source reasoning, not fresh provider/consumer certification. PLANNED VALIDATION = unexecuted downstream work. No PROPOSAL is silently promoted to an accepted requirement by consolidation.

### 1.2 Accepted overlays and supersession

| Decision | Direct authority and active meaning | Superseded interpretation / unchanged boundary |
|---|---|---|
| U7-TIME-01 | User explicitly chose arrival timestamp of the datagram completing the full CH. Copy before source expiry; write only on permitted normal CH commit. | Closes earlier D-TIME source proposal/B01 only. Not first-byte time or callback/commit wall clock; duplicates do not refresh. |
| U7-PARSE-01 | User requires blocking/nonblocking QUIC findings and full parsing/FP for nonblocking older-only offer; Processor/TLS-Flow do not define violations. TLS-Enforcement owns legality/verdict even if not implemented for QUIC. | Closes B4-02 within this workstream. Does not activate Enforcement or migrate TCP taxonomy. Finite result snapshot, anomaly-before-CH and safe-boundary realization are explicit design judgments in S2, not new product policy. |
| U7-SCOPE-02 | User excludes product profile/port and mitigation-consumer connection-context integration from current design; optional research cannot block it. | Withdraws AD-01..05/CT-01..04 and the assistant's mandatory mitigation-context admission/Unsupported realization. Missing mitigation-only context is not Unsupported. Required inspection association/backing remains. |

Earlier versions, B01/B4-01/B4-02 blocker reports and withdrawn-case snapshots remain preserved, but are not active unresolved requirements. R4-01's blocking-result success guard remains core correctness. R4-02's mandatory external-context addition is superseded by direct scope authority. S1's historical timestamp/mapping entries are read with these explicit later decisions, not overwritten or treated as blanket waivers.

### 1.3 In-scope mapping and exclusions

| Relevant requirement / engineering obligation | Defined coverage | Scope result |
|---|---|---|
| Qualified v1 Initial, fragmented CRYPTO/CH and non-QUIC isolation | CH-Q1/Q2, CR-01..03, RG-01..04, ER-Q1/Q2, CAP-03; S4 PR-01/02, PS-02/08/10 | Covered at parsing/protocol qualification boundary. Existing participation is an input precondition; product port/profile policy is external. |
| Canonical TCP-equivalent FP and no duplicate QUIC observation | CH-T1/CH-Q1, RG-02, AT-02, IX, NF-09/10/11; S4 PR-05, PS-11 | Covered producer/algorithm contract, not actual RTS/statistics implementation. No unrelated FP repair. |
| Older-only TLS offer finding and Enforcement boundary | NF-01..14; S4 PS-09/17; U7-PARSE-01 | Complete nonblocking CH/FP plus finding; no parser violation/Error. Actual eligible observer feedback remains binding. |
| State/attempt/dispatch/lifecycle correctness | AT/FB/LC/CAP/TIME/IX; P2/P4/P5 | Accepted inspection semantics and necessary engineering; no server evidence or negotiated-version inference. |
| Current CH/finding/FP views and finite Engine runtime | IX/NF/CAP/LC; P5; S5 E01-E06 | Qualified provenance, last use, prepared storage and bounded work; actual API/runtime evidence downstream. |
| Performance, platform and TCP preservation | S4 NFR dispositions, PV-01..07/09/11..13; S5 E04-E06 | Defined comparative validation plan; no measured result or invented numeric budget. |
| Profile/port/default/persistence and mitigation-context/action integration | S4 PR-10, PN-04, PS-03/06/07/12/14; PV-08/10; U7-SCOPE-02 | Explicit authorized OUTSIDE CURRENT DESIGN / NONBLOCKING, not product-delivery PASS. No hidden dependent-HLD prerequisite. |
| DTLS, baseline/RTS/reporting/generator implementation | PLAN Section 1; S4 Section 3 | External product work. Shared-versus-separate baseline, DROP/close, REST and on-demand differences remain recorded for their owners, not silently resolved here. |

The complete scoped product index is S4 Section 3: 34 review rows classify all ten FR IDs, four NFR IDs and grouped relevant SRS/prose clauses. S4 Section 4 retains 48 existing-constraint destinations and nine product review items with reverse justification. These are precise references, not a claim that all full-product requirements are delivered. Empty SRS templates are not invented requirements. Relevant workstream coverage is now based on actual product sources plus explicit scope decisions, not P1's historical mapping deferral or numeric-capacity deferral.

## 2. Current behavior and preservation obligations

CODE remains the TCP reference; S3's traces distinguish unchanged behavior from adopted target corrections. Preserve entry/SYN/RST/empty/terminal order, staged CH modes and existing support limits, current callback priority/eligibility/context, exact actual STOP/mask reduction, mode-specific continuation ownership, FP algorithm/format and mark-versus-release meaning. QUIC reuse must not force TCP into full-message buffering or new universal results.

| Suspected item / delta | Current disposition | Evidence and validation boundary |
|---|---|---|
| SD1 stale resumed fields; SD2 resumed legacy-version provenance | AUTHORIZED D6-01 correction: neutralize fresh fields, restore own saved version/counts/digest and recompute extension facts. Not implemented by this review. | P5 L417-421; IX-01/02; S4 Section 6; PV-04/05. |
| SD3 transport admission | Narrow selected protocol routing before TCP-only work. Product feature gates excluded. Production exposure not asserted. | P6 Sections 7-8; EN/CH/MU; PV-02. |
| SD4 unsegmented TCP retransmission/local CH guard | PRESERVE TCP local MultipleClientHello behavior; no broad TCP dedup redesign. | CODE L6301-6306; TCP-GUARD; PV-05. |
| SD5 non-CH segmentation; SD7 encrypted-record length | SEPARATELY DEFERRED investigation, not automatic bug/fix. Preserve existing mode/limits. | S4 Section 6: TCP owner needs concrete required-outcome conflict before change; no current design blocker absent such evidence. |
| SD6 internal errors/eviction | SPLIT: preserve TCP public Stop/actual anomaly verdict; QUIC accepted required-support loss is zero-consumer Unsupported; safe retirement is adopted D6-02. | ER-T1/T2/T3, CAP/LC; PV-06/07. |
| SD8 retained Complete callbacks | PRESERVE U05 callback repetition, not repeated CH normal commit on a non-CH record. | CODE onClientHello/commitNormalCurrentRecord; MU-T1; PV-05. |
| Acquisition/registration/detach failure, stale references | AUTHORIZED D6-02 acquired-only rollback and selected actual-victim retirement integration; not proof existing reset/erase is safe for new support. | P5 L423-431; CAP-04, LC-05/06; PV-07. |
| Unrepresentable semantic count/update | AUTHORIZED D6-08 checked preflight before consumers, explicit local Unsupported and prior-fact preservation; no wrap/saturation. | P5 L469-471; CAP-05; PV-06. |
| Common FP/cipher-feeding repair, broader fragmentation/record-limit rewrite | NOT bundled. Preserve canonical algorithm on both transports; separate authority/evidence needed for a common repair. | P3 USER-FP-01; S4 Section 6; S5 Section 5. |
| markReclaimable failure | Preserve TCP ignored return/control behavior; marking is not release or guaranteed insertion. Required retirement/stale-membership handling is separate. | CODE L2898-2925; LC-06. |
| Broader narrowing / declared-only error producers | Deferred exact API evidence, not a new producer or blanket fix. Required representability behavior remains D6-08; a concrete contradiction reopens affected contract. | S4 Sections 6/8; S5 E01-E03/E06. |

No suspected defect is relabeled a verified production failure solely from P1. No correction above has been applied to CODE. TCP differential validation must use an explicit authorized-delta oracle rather than demand bit-for-bit equality where an accepted correction intentionally changes a result.

## 3. State meanings, transition policy and semantic equivalence

State is inspection-policy state, not proof of endpoint TLS completion. One active TLSData represents one accepted active attempt; support may retain bounded candidates/contexts without another live semantic core. P2 Sections 5-10 remain authoritative with the direct overlays in Section 1.

| Meaning / action | TCP evidence, representation and eligibility | QUIC evidence, representation and eligibility | Lifecycle / authority / evidence |
|---|---|---|---|
| PreHandshake | Existing entry/build/reset semantics; SYN preparation remains TCP-specific. | Participating qualified attempt before authoritative CH; terminal candidate acceptance resets before CH parse. | Sole policy reset; no candidate publishes live facts. AT-06/07, TCP-SYN. |
| InProgress / authoritative CH | Supported parsed CH and permitted normal record commit; existing Complete callback eligibility may persist. | Complete qualified fresh non-retransmitted CH and permitted normal commit; no synthetic Record. | Replace current CH facts/count/FP/offer/time; CH/AT/TIME. Same meaning, different availability evidence. |
| CH timestamp | Existing TCP packet timestamp passed to permitted CH commit. | Arrival timestamp of completing datagram, copied through result lifetime. | Not callback wall clock/first-byte history; TIME-01..03. |
| Additional CH | Preserve local TCP Complete guard and U05 callback behavior. | New accepted offsets/qualified fresh message can be another CH, including third; legality belongs to actual Enforcement. | Same attempt unless complete independent replacement accepted. AT-01/03, TCP-GUARD. |
| ChangeCipherSpec | Existing TCP record-driven state/counters and later handshake completion predicates. | No fabricated CCS transition/record; no TCP heuristic applied to 0-RTT. | Preserve TCP applicability; TCP-CCS-APP, LC-04. |
| Finished | Existing qualified TCP record/state predicates on permitted normal commit. | Only qualified associated client v1 Handshake Long Header under accepted P2 policy after authoritative CH; pre-CH evidence bounded/deferred. | No server Finished/HANDSHAKE_DONE/opaque 1-RTT/0-RTT surrogate. Deferred CH commit must be permitted; mark only newly Finished after last use. LC-03/09/10. |
| TerminatedByAlert | Readable TCP alert predicate using level/description/offer facts. | Associated readable Initial type 0x1c CONNECTION_CLOSE with CRYPTO_ERROR 0x0100..0x01ff, including 0x015a; no synthetic TLS alert callback. | Accepted nonterminal-state policy, including PreHandshake; no inference from failed decryption. TCP-ALERT, LC-04/08. |
| Violation-only metadata | Actual eligible observer mask with no final STOP; normal commit suppressed, not automatic Error. | Same actual reducer, no parser-defined violation. | Local accumulator starts zero; historical masks not current verdict seed. FB-05, NF-05. |
| Error | Actual feedback under current-event immediate or final latched-STOP/nonzero predicate. | Same predicate; finding severity/progress alone cannot set Error. | No automatic packet DROP. FB-02/03/04/07, NF-04/13. |
| Unsupported | Preserve exact TCP gap/PreHandshake/InProgress+Partial guard and public-result loop exit. | Required accepted inspection support loss before ALL affected projections; other accepted cause-specific P2 dispositions retained. | Candidate-only failure isolated; missing mitigation-only context not a cause. ER/CAP/NF-07. |
| Blocking versus nonblocking findings | Existing TCP public Stop/record-loop behavior preserved; no automatic taxonomy migration. | Blocking invalidates affected success; nonblocking older-only offer can coexist with complete CH/FP. Actual finding feedback precedes eligible CH. | Neutral feedback cannot create absent success; NF-01..14. |
| Active/terminal replacement | Preserve existing TCP lifecycle/control behavior, not a universal QUIC attempt algorithm. | Active candidate complete/qualified/admitted before reset; terminal candidate accepted/reset before CH parsing. | One active core; old last use/retired identity before reuse; no post-reset rollback. AT-04..09. |
| Mark/reclaim/aging | Mark result ignored as documented; independent continuation manager owns its aging. | Actual-victim pre-destructive support retirement; aging binding not guessed from current config/packet. | Mark != release; actual SessionEnd only on aging, no resource-eviction violation. LC-02/05/06/07. |

The matrix distinguishes meaning, wire evidence, availability, representation and lifetime. Detailed predicate/branch evidence is S3 under the cited case IDs and P2/P4/P5, not a replacement state machine. Unknown/future advertised version values are not older-only or negotiated-version proof; legacy_version 0x0303 with valid TLS1.3 offer is not the older-only finding (NF-09/10).

## 4. Processor/Conductor contract

**Roles, not required classes:** protocol processing owns acquisition/staged evaluation and qualified facts; Conductor owns invocation/dispatch/feedback/next-unit timing; cohesive shared policy owns live semantic predicates/actions; support owns prepared retained backing; Extension owns one active TLSData and borrows support; table identifies actual victim before destruction; observers supply actual verdicts. No second semantic writer or hidden ownership transfer is introduced.

TCP keeps IProcessor/PayloadParams/TLSStepResult and beginPacket/processRecord modes. QUIC uses its own relevant-result/progress/exhaustion boundary; it is not required to implement a universal record/result interface. Shared canonical helpers preserve current FP algorithm. A CH-only consumer does not imply TLSRecord synthesis or future Enforcement activation.

The current QUIC result distinguishes (a) parse progress/blocking, (b) qualified fresh success availability, (c) finite finding facts, and (d) explicit semantic/support disposition. A finding is not a TLSViolationMask. Complete evaluation/FP precedes publication of a successful current result. Nonblocking older-only offer does not itself suppress CH/FP; a later structural failure removes success, and required support loss suppresses every affected callback. Stable once-per-supported-reason delivery uses finite current storage, not a plugin/registry/history framework. Exact C++ encoding remains downstream; observable behavior is decided now.

Current-producer routing selects the same qualified CH/error/digest/locator backing for nested callback reads and independent commit reads. Callback wrapper lifetime is not backing lifetime. Accepted serial/non-reentrant use and no asynchronous retention protect that interval; any consumer needing longer ownership must satisfy its existing in-scope prepared-storage contract. No new mitigation-context handoff is required. Clear current availability/selectors at producer/result changes so B success/error cannot become A facts (IX/NF-08).

**Input/trust boundary:** only client-to-server visibility; QUIC v1 Initial keys and qualified v1 packet/association facts. No server Retry/HRR/Finished/HANDSHAKE_DONE/transport-parameter visibility is assumed. Packet/sample/varint/extent checks precede unsafe arithmetic or interpretation; no protected packet-number/key-phase inference or later-level payload decryption with Initial keys. A port is not proof of QUIC; product port policy itself is outside this scope. Actual provider conformance is required downstream before relying on its evidence.

## 5. Runtime flows, dispatch and commit semantics

### 5.1 Entry and progress

Preserve TCP SYN/RST/terminal/empty/participation ordering (CODE L3393-3484; EN/TCP-SYN/LC-01). Select protocol before TCP-only work. QUIC walks safely bounded coalesced packets/frames/CRYPTO and yields at the first feedback-dependent relevant result, even inside a packet. Partial CH does not discard remaining usable input. Each traversal advances, yields, completes an ignorable unit or exhausts safe input. IGNORE is not a pending reset/close/Finished/Unsupported with zero projections. No arbitrary tail replay, packet cutoff or ciphertext/CRYPTO resynchronization scan.

### 5.2 Exact actual-feedback reduction

| Situation in current unit | Required action / continuation |
|---|---|
| No live subscribers / expired weak listeners | Neutral actual feedback; no fabricated verdict. |
| Current STOP with accumulated actual mask nonzero | Immediate policy Error; suppress later same-unit projections/normal commit; return STOP. |
| STOP/0 with zero accumulated mask | End current listener list, latch STOP; remaining eligible same-unit events may run. |
| Later Continue/nonzero after earlier STOP/0 | Not immediate Error at that Continue event; remaining eligible events run; final nonzero mask plus latched STOP commits Error. |
| All Continue, final mask nonzero | Violation-only metadata; suppress normal commit, not automatic Error; later unit still requires eligible resulting state. |
| Final mask zero | Normal update only if qualified successful/applicable result exists; latched STOP still forbids a later unit. Blocking result has no CH normal commit. |

Historical stored violations do not seed the current accumulator. Eligible anomaly projections precede CH on the same pre-normal-commit context; QUIC has no Record projection. STOP is not automatic packet DROP. Actual earlier consumer side effects are not rolled back when a later packet trial fails. CODE L3514-3585/L4008-4025; S3 FB-01..07/NF-01..05/13/14.

### 5.3 Findings and failure precedence

For a valid complete older-only CH, finish parsing/FP, expose a nonblocking finding, reduce actual feedback and then deliver/commit CH only when eligible. Without actual Enforcement verdict there is no parser-created violation/Error. A structural blocking result has anomaly-only eligibility, no valid CH/FP, and no normal CH commit even at final zero mask. Required accepted-support loss takes Unsupported before all current-unit consumers, including an internally available finding. Recoverable local trial failure is not accepted required-support loss. NF-06/07/12/13/14 preserve safe boundary and feedback distinctions.

### 5.4 Attempts and semantic-only dispositions

Same-offset/equal accepted bytes are retransmission despite repacketization; no CH/count/time/finding refresh. Fresh accepted offsets can be another CH. Retry-compatible restart and HRR-compatible CH2 remain the same attempt, not observed server Retry/HRR. Compare all retained contexts and retired evidence; ambiguity alone permits neither merge nor reset.

Active replacement evaluates an isolated complete candidate, checks every P2 acceptance condition and joint admission, preserves required old identity/last use, then sole policy resets once before new CH feedback-controlled commit. Candidate failure/expiry returns only candidate claims; eligible partial candidate support may remain without reset. Terminal replacement qualifies/adopts/reset before CH parsing; later malformed/support failure belongs to the new attempt and cannot restore old terminal authority. AT-01..09.

Qualified completion/close can require policy disposition without an observer projection. Deferred pre-CH Handshake evidence is same-attempt bounded evidence; no Finished until authoritative permitted CH commit. Initial CRYPTO_ERROR close is distinct from Finished. 0-RTT/opaque short-header traffic does not progress state. Repeated evidence does not repeat transition/mark. LC-03/04/08..10.

## 6. Ownership, lifecycle, bounds and runtime considerations

| Resource / operation | Owner, last use and safe failure |
|---|---|
| Active TLSData and protocol binding | Extension owns one semantic core; minimal binding borrows prepared support. No candidate owns another live TLSData. |
| QUIC plaintext/ranges/CH/digest/identity | Prepared support owner owns retained bytes/claims; current views borrow. Datagram/frame pointers do not survive input expiry. Keep backing through nested reads, feedback and independent commit last use. |
| Candidate/deferred/retired support | Bounded logical claims in the physical ownership domain, not mandatory classes/stores. Retain only accepted evidence; optional history absence is not required-support loss or positive identity proof. |
| Replacement/admission | Charge simultaneously live old+candidate+destination+temporary physical usage once. Do not free old support early to manufacture admission or double-charge old backing. Denial before active acceptance preserves old authority. |
| Construction/rollback | Publish participation/binding only after successful required acquisition/registration. Unwind acquired owned claims only; borrowed duplicate lookup is not a new owned claim. Stable cleanup keys before invalidating container mutations. |
| Actual-victim retirement | Identify victim B before reset/erase while requester A producer remains valid. Finish last uses and required detach before destructive reuse; failed detach denies unsafe reuse, not a fake cleanup success. Invalidate selectors and clear stale reclaim membership safely. |
| Reclaim mark | Mark after newly committed Finished and last use; not immediate release/guaranteed insertion. Preserve TCP ignored mark return. Repeated cleanup/return is a no-op after ownership is gone. |
| Aging | Actual SessionEnd callback before release with documented ignored aging verdict; protocol binding may be TCP/QUIC/unbound. Resource eviction is not synthetic SessionEnd or IncompleteHandshake. Independent TCP continuation manager retains its own lifecycle. |

Startup allocation is allowed. After startup, every affected Engine packet/aging/retirement/diagnostic/callback/crypto/failure path uses prepared backing only, without dynamic allocation, capacity growth or heap fallback. Logical acquire/recycle does not exempt transitive allocation. Finite work dimensions include packets/frames/messages/ranges/contexts/bytes/locators/reasons/listener visits/lifecycle claims; each has admitted bounds and progress. Checked extent and semantic-representation preflight precedes affected consumers. No numeric capacity or CPU/RAM budget is invented.

Ownership, last use, admission peak, failure disposition and progress are decided now. Exact finite startup values/layout/API fit and actual no-allocation evidence remain at the P5/P6 stages in Section 9, not hidden behavior deferrals. Missing mitigation-only context creates no new backing/admission/Unsupported obligation. Required inspection association still follows P2/P5. Evidence: S3 CAP/AT/IX/LC/NF; P5 D6-01..08; P6 L453-477.

## 7. Selected architecture and existing-code impact map

Preserve Phase 6 R1 / minimum C1 + C4-P. No concrete evidence in the active static traces invalidates that choice. It is not claimed universally optimal, measured faster or implemented. Cohesive policy functions, narrow common helpers, explicit Conductor traversal and current-producer routing are sufficient; logical responsibility does not mandate a class/interface/component. No Flow hierarchy, universal endpoint/record/result, full TCP buffering, DTLS framework, VerdictManager or diagnostic registry is required.

| Existing symbol area | Minimum target change / preservation | Active evidence |
|---|---|---|
| onNewSession/buildExtension/registerSessionObservers | Existing participation/priority integration; acquired-only rollback. No product feature gate work. | EN, TCP-SYN, CAP-04 |
| onInbound/prepareInboundExtension/process | Protocol selection before TCP-only work; preserve control/terminal/loop order. | CH/MU/LC-01/02 |
| dispatchStepResult | Preserve TCP public result; separate QUIC finding/success/disposition routing. | ER/NF/CH-Q3 |
| analyzeProcessResult/notify | Preserve actual reducer and precommit timing; invoke cohesive policy actions. | FB/NF |
| commitNormalCurrentRecord/updateTLSDataOnClientHello | Shared qualified CH actions with protocol applicability, fresh eligibility, timestamp and no-wrap. | CH/TIME/CAP-05 |
| CCS/ApplicationData/Alert updates | Keep TCP predicates; qualified QUIC completion/close separate. | TCP-ALERT/TCP-CCS-APP/LC |
| Unsupported/Violation/Error/RST clear | Sole semantic authority, cause-specific support loss and actual verdicts. | ER/CAP/LC-01/NF |
| IProcessor/PayloadParams/TLSStepResult | Retain TCP contract; no universal migration prerequisite. | CH-T/RG-T/TCP-GUARD |
| TCP beginPacket/processRecord/new-record/continuation methods | Preserve modes/traversal; adopted D6-01 restoration only. | RG-T/IX |
| CH/cipher/extension/FP helpers | Minimum canonical reuse, no unrelated FP repair or full evaluator extraction. | CH/IX/NF-09/10 |
| SegmentBufferManager | Independent existing ownership/aging; no blanket Extension/state-driven free. | LC-01/02/RG-T3/ER-T2 |
| Factory/Application | Startup prepared wiring and current-producer getter routing, not universal facade replacement. | IX/NF-08 |
| TLSData/TLSSessionExtension | One live core and minimal borrowed binding; no embedded full acquisition state. | AT/LC |
| ExtensionTable alloc/release/markReclaimable | Pre-destructive actual-victim retirement, safe failed detach and stale-membership handling; mark not release. | CAP-04/LC-05/06 |
| Existing callbacks | In-scope view/feedback compatibility; no Enforcement activation or external feature integration. | IX/NF; U7-SCOPE-02 |
| QUIC processing/support | v1 Initial, bounded CRYPTO/CH, accepted identity/attempt support, canonical FP and minimal finding capability. | CH-Q/CR/RG/AT/LC/NF |

Precise symbol names and source anchors: P6 Section 8 and S5 Section 3; S5 K01-K11 documents simpler alternatives and why the retained seam is needed. Table retirement is required correctness work, not optional cleanup. Optional universal refactors cannot be smuggled into delivery.

**Minimum delivery order, P6 D1-D6:** D1 reference basis; D2 semantic actions depends D1; D3 support/retirement depends D1; D4 QUIC acquisition/evaluation depends D1 and D3 ownership contract; D5 Conductor/facade integration depends D2-D4; D6 integrated readiness depends D2-D5. D3/D4 may develop against the same fixed ownership contract, neither enables a partial feature. Findings fit D4 result qualification and D5 dispatch, not a new package. Differential TCP evidence precedes operational use of its refactor; no partial QUIC activation. Exact exits and required/optional distinctions are S5 Sections 4-6.

P6 L335's actual R1 coordinator versus R2 adapter sketch check before final API/layout freeze remains a downstream evidence obligation. Concrete repeated policy branches, hidden state, duplicate traversal or demonstrated R1-specific provider/layout disadvantage can reopen the selection under unchanged weights. Do not implement all alternatives or rescore now merely to obtain a PASS. No such concrete invalidating evidence is supplied by this consolidation.

## 8. Scenario review evidence and focused validation plan

### 8.1 Active acceptance set and navigation

S2 V12 and S3 V6 contain 84 matching case IDs: 79 table cases and five TCP boundary subcases. All nine original required families remain. The detailed per-case expected result, selected-design trace, source anchors, lifetime and limitation are reused by exact reference, not replaced by this summary or by historical PASS labels.

| Family | Active case evidence |
|---|---|
| F1 Complete/incomplete CH | CH-T1/T2/T3, CH-Q1/Q2/Q3, TIME-01/02/03, EN-01/02, NF-01/02/06/09/10 |
| F2 Multiple records/coalesced packets | MU-T1/T2, MU-Q1/Q2, NF-03/12 |
| F3 CRYPTO frames/datagrams | CR-01/02/03, CH-Q2, TIME-02/03, NF-08 |
| F4 Reorder/duplicate/gap/overlap | RG-01..04, RG-T1..3, TIME-02, NF-11 |
| F5 Additional CH/attempt reuse | AT-01..09, TCP-GUARD, TCP-SYN, NF-11 |
| F6 Interleaved/resumed | IX-01/02/03, RG-T1/T2, NF-08 |
| F7 Malformed/unsupported/capacity | ER-T1/T2/T3, ER-Q1/Q2/Q3, CAP-01..05, CH-Q3, NF-06/07/10/12/13/14, TCP-CAPACITY |
| F8 STOP with/without violations | FB-01..07, AT-07, TIME-03, NF-01..05/13/14 |
| F9 Terminal/aging/eviction/reclaim | LC-01..10, AT-06/07/08, MU-T2, TCP-ALERT/TCP-CCS-APP |

AD-01..05/CT-01..04 are explicitly withdrawn external integration cases with reserved IDs, not missing active coverage or implemented behavior. No active oracle/trace is rewritten by this package. Active outcome examples and failure branches are in Sections 3-6 and S3; no current material discrepancy was found in the scoped static comparison. Actual provider/consumer conformance and execution are not inferred from that result.

### 8.2 Focused regression/performance plan

All entries are PLANNED VALIDATION, not packet execution or measured performance. S4 Section 7 supplies the detailed risk/method/oracle/evidence/owner/stage rows; S5 E01-E07 preserve blocking points.

| Plan IDs | Risk / focused method | Required evidence / expected boundary |
|---|---|---|
| PV-01/02 | Differential admitted-input TCP traces and existing protocol/non-QUIC routing | Callback/state/FP/return equality except authorized deltas; no UDP-to-TCP fallback; actual comparative throughput/latency. No product gate implementation required. |
| PV-03 | Equivalent TCP/QUIC CH volumes with fragmented/reordered/range/context/fanout variants | Actual cycles/CH, throughput, latency and work ledger under stated comparative NFR; no invented tolerance or benchmark result. |
| PV-04/05 | Interleave/resume nested views and canonical same-CH/duplicate/fresh-offset corpus | Own producer/digest/version/offer/counts; same canonical FP; no duplicate QUIC authority; preserve TCP U05/local guard. |
| PV-06/07 | C-1/C/C+1 per dimension, malformed extents/no-wrap; failure-inject registration/detach/victim reuse | Checked simultaneous peak; zero-consumer required-loss; no unsafe reuse/double return/stale reference; independent TCP ownership. Symbolic C is not a selected number. |
| PV-09 | Startup partial failure, provider/platform audit and transitive allocation instrumentation | Finite approved backing; no runtime allocation/growth/fallback across packet/aging/failure paths; actual platform evidence before enabling affected path. |
| PV-11 | Distinct arrival t1/t2 versus commit t3, reorder, duplicate and feedback branches | U7-TIME-01 exact completion-arrival scalar only on permitted normal commit. |
| PV-12/13 | Older-only/control/unknown-version, blocking/nonblocking/multiple findings, actual feedback and producer switches | NF-01..14 result/commit/feedback; finite current reason snapshot; no parser-created violation/Error or stale/duplicate finding. |
| PV-08/10 | Optional external profile/port/persistence and consumer feature/mitigation integration | OUTSIDE CURRENT DESIGN / NONBLOCKING. No imported current-design/HLD gate or claim of external feature delivery. |

Performance evidence must include actual builds/platform/provider/configuration/workload and comparative samples; placement in functions or a bounded loop is not a speed claim. Relevant FRD NFR criteria are retained. Product baseline/action/RTS algorithms outside scope are not silently implemented to satisfy a parser performance assertion.

## 9. Deferred items, owners and blocking classifications

There is no current unresolved semantic question in this package that is intentionally deferred to implementation. The following are explicit evidence/engineering stages with already defined required behavior, or authorized external scope. A concrete later contradiction reopens the affected contract/oracle/step rather than changing only a gate conclusion.

| ID / item | Rationale and logical owner / decision source | Required closure evidence and stage | Blocking classification |
|---|---|---|---|
| DF01 Actual crypto/header/packet provider | QUIC/provider implementer; required qualified evidence, scratch/failure contract | Exact API/protection/association/bounds before dependent API/layout finalization or D4/D5 provider use. No unavailable qualification may yield Finished. S5 E01/P6 V02. | DOWNSTREAM REQUIRED, before affected use/activation; not executed here. |
| DF02 Current-producer and actual consumer API fit | Facade/Conductor implementer; correct nested CH/FP/SNI/finding views and verdict semantics | Actual getter/borrow/commit last-use mapping before dependent API finalization and D5 exit. No requirement to implement excluded Enforcement or external feature integration. S5 E02. | DOWNSTREAM REQUIRED within scope; incompatible required facts reopen design. |
| DF03 Retirement/container/registration conformance | Table/support implementer; safe actual-victim lifecycle | Exact stable identity/detach failure/last-use/exactly-once API before dependent retirement finalization and D3/D5 exit; no destructive reuse until safe. S5 E03. | DOWNSTREAM REQUIRED; safe-failure outcome already decided. |
| DF04 Finite startup values and replacement peak | Engine/support/layout owner with HLD/architect review | Approved finite capacities/physical charges and checked old+candidate+destination+temporary peak at authorized numeric stage, before D6 activation. Symbolic bounds/ownership decided now. S5 E04/P5 handoff. | DOWNSTREAM REQUIRED; no unavailable CPU/RAM budget or invented number demanded now. |
| DF05 Transitive allocation/work audit | All affected Engine implementers integrated by startup owner | Actual packet/aging/retirement/callback/diagnostic/crypto/failure audit and instrumentation before D6 activation. S5 E05. | DOWNSTREAM REQUIRED; startup allocation allowed, runtime growth/fallback forbidden. |
| DF06 Regression/performance/platform execution | TCP/QUIC validation and affected platform owners | S4 in-scope PV plan, differential authorized-delta oracle and actual evidence at D2/D4/D5 exits/D6 readiness. S5 E06. | DOWNSTREAM REQUIRED; static review is not execution. |
| DF07 Evidence-triggered architecture sketch check | API/layout implementer with architecture review | Actual R1/R2 sketches against same P6 S01-S15 before final API/layout freeze; reconsider only on concrete specified evidence. P6 L335/S5 E07. | DOWNSTREAM REQUIRED at existing freeze point; no current rescore or alternate implementation prerequisite. |
| DF08 SD5/SD7 and unrelated FP/record changes | TCP/FP owner; no current authorized broad fix | Concrete required-outcome failure and separate change authority; preserve current behavior absent that evidence. S4 Section 6. | SEPARATE INVESTIGATION; not current blocker unless a concrete in-scope conflict is established. |
| DF09 Broader narrowing/declared-only errors | API/consumer owner; exact representation evidence | Actual fit before dependent HLD/delivery; D6-08 behavior remains required, no invented producer or silent saturation. S4 Sections 6/8. | DOWNSTREAM API EVIDENCE; a changed required result reopens design. |
| DF10 Profile/port and mitigation-context integration | Direct U7-SCOPE-02; external feature owner in its own scope | No extra current API/retention/admission/action work required. Future explicit scope change needed to import it. | OUTSIDE CURRENT DESIGN / NONBLOCKING, including dependent HLD signoff and Steps 2-7. |
| DF11 DTLS/baseline/RTS/reporting/generator, REST/on-demand conflicts | PLAN scope and product feature authority | External owner resolves its own implementation/release differences; no parser policy selected here. | OUTSIDE CURRENT DESIGN / NONBLOCKING; not a product waiver or delivered feature. |
| DF12 Final package acceptance/freeze | Step 7 procedure and explicit user acceptance | Separate complete final static review of unchanged package, same-byte final-review discipline and explicit acceptance before accepted freeze. | NEXT PHASE-7 GATE; not performed by Step 6 or inferred from assistant PASS. |

**Step 6 boundary:** this reviewed candidate and its evidence closure are ready for the separate Step 7 overall final gate if EG1-EG7 in the companion assessment pass. No user acceptance, accepted freeze, actual API/layout freeze, implementation conformance, runtime/performance PASS or activation authorization is implied. This package adds no new semantic decision or scope expansion. Earlier snapshots remain unchanged; corrections require new versions and reopening any affected earlier step.
