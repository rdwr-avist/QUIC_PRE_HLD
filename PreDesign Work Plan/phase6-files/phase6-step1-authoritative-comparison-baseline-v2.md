# Phase 6 Step 1: Authoritative Comparison Baseline V2

## 1. Outcome and scope

**Step 1 complete at the static source/contract baseline boundary. Ready for Step 2 only.** No architecture candidate has been selected, no component split is accepted, and no implementation/runtime compliance is claimed. The original Phase 1–5 documents and original work-plan are all physically available; no reproduction is substituted for a missing original. The workflow text in the current user instruction is the execution authority for this step.

All eight source inputs are preserved byte-for-byte in the companion archive. Hashes of the seven earlier inputs agree with the historical source map in P5 Section 2; P5 V7 itself is independently hashed here. This checks supplied-snapshot continuity, not repository provenance or correctness of inherited PASS claims.

## 2. Source inventory and authority

| Alias | File | Version / identity | Role | Bytes |
|---|---|---|---|---:|
| PLAN | `tls-flow-quic-pre-design-work-plan-v5.md` | V5 | Phase boundaries, four candidates and eight Phase 6 gates | 33724 |
| P1 | `phase1-copilot-report.md` | Supplied report; no filename revision | Current-behavior research leads; source inspection remains necessary | 17356 |
| P2 | `phase2-state-machine.md` | Supplied accepted state-machine document | State and attempt policy, subject to later accepted clarifications | 28114 |
| P3 | `phase3-semantic-processing-contract-review-v5.md` | V5 | Semantic contract, freshness, feedback and lifetime | 607365 |
| P4 | `phase4-runtime-orchestration-summary-v4.md` | V4 | Orchestration, verdict transaction, state commit and complete flows | 88921 |
| P5 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md` | V7 | Ownership, lifecycle, bounded runtime and accepted capacity deferral | 197870 |
| CODE | `TLS-Flow-All-Code.txt` | Supplied snapshot; repository commit unknown | Current module source evidence, not proof of accepted target behavior | 332903 |
| CONS | `TLS-Consumers code.txt` | Supplied fragments; repository commit unknown | Consumer callback/integration evidence; incomplete repository paths | 33472 |

### SHA-256 of original bytes

- **PLAN** `55ea1536ac4913e3a4ce59cc717fc3f5e8752dcdddf51be66bed34b280c024fa`
- **P1** `9c25e615a26c28f65ddc3a26ee61c9e18ce90212c9ee0f9ca0b0506c8b840a69`
- **P2** `b2d0ae9254ed6e96c643281bfc09302dd8c8360b9672ff16611055820f02de77`
- **P3** `b3278e90d39b6e69396f4b87725e75aeac1aa9baa98f826001964cc16b46c438`
- **P4** `140c2ad934bf30def3f12137556d73f0ce5695b8f038bda3dce46ced9c182769`
- **P5** `8f0c3def3ce18217440fef1a7dfd7d6bea953f385328541afa3d16a9b13beba3`
- **CODE** `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b`
- **CONS** `25719e413bd5aabb2947c79d86bb57ff35d6f75a1971e669697b6481b1af3a9d`

### Authority discipline

- **Current-code fact:** a directly inspected symbol or fragment describes the supplied implementation snapshot only. P1 is a research lead, not substitute source proof.
- **Accepted decision:** P2–P5 contracts and explicit user clarifications govern the target behavior. Later accepted clarifications supersede affected earlier wording only; they do not discard the earlier documents.
- **Candidate proposal:** future placement, sharing, strategy/base choice, component/interface shape and pattern selection remain unselected.
- **Evidence limitation:** missing external implementation or measurement is recorded with its consequence. It is not automatically an architecture blocker.
- P5 V7 Sections 1, 2, 9.2 and 10.2 preserve the accepted completion policy and numerical deferral. An older conversational memory that completion remained blocked does not override these supplied accepted documents.
- PLAN Phase 6 is inspected directly. P5 10.2 is a corroborating reproduction, not the only available authority. Historical Phase 3/4/5 working artifacts cited by the summaries are not all supplied; their archival bytes and original review runs are not claimed inspected.

## 3. Extraction and provenance

The module bundle contains 70 `File:` sections, including the BOM-prefixed first marker. Extracted 48 sections: common semantic data, Engine initialization, extension/storage, packet-flow and processor/parser/viewer infrastructure. This is a focused architecture evidence slice, not a new module design. Controller/IPC, diagnostics implementations and UI are not extracted because Step 1 identifies no candidate-sensitive need to inspect their internals. Their original bytes remain preserved in CODE; follow an actual dependency if later comparison needs them.

Original module-relative paths are retained under `extracted/` in the archive. Each range below is the original bundle content range after its marker through the line preceding the next marker. Separators and whitespace are retained; these are provenance excerpts, not a buildable repository export. Per-excerpt hashes are in the manifest.

| Original module-relative path | CODE content lines |
|---|---:|
| `common/include/Definitions.hpp` | 21-56 |
| `common/include/Fingerprint.hpp` | 58-160 |
| `common/include/Sni.hpp` | 162-271 |
| `common/include/TLSAlert.hpp` | 273-318 |
| `common/include/TLSData.hpp` | 320-492 |
| `common/include/TLSViolation.hpp` | 494-566 |
| `context/engine/include/Application.hpp` | 1104-1182 |
| `context/engine/include/Factory.hpp` | 1184-1311 |
| `context/engine/include/IFactory.hpp` | 1313-1386 |
| `context/engine/src/Application.cpp` | 1388-1465 |
| `context/engine/src/Factory.cpp` | 1467-1586 |
| `extension/include/ITLSSessionExtensionInspector.hpp` | 2597-2656 |
| `extension/include/TLSSessionExtension.hpp` | 2658-2773 |
| `extension/include/TLSSessionExtensionTable.hpp` | 2775-3001 |
| `packet-flow/include/Action.hpp` | 3003-3028 |
| `packet-flow/include/ClientHelloData.hpp` | 3030-3083 |
| `packet-flow/include/ITlsListener.hpp` | 3085-3164 |
| `packet-flow/include/ITlsSubject.hpp` | 3166-3219 |
| `packet-flow/include/TLSFeatureVerdict.hpp` | 3221-3260 |
| `packet-flow/include/TLSPriority.hpp` | 3262-3287 |
| `packet-flow/include/TLSRecordContext.hpp` | 3289-3318 |
| `packet-flow/src/TLSFlowConductor.cpp` | 3320-3905 |
| `packet-flow/src/TLSFlowConductor.hpp` | 3907-4188 |
| `processor/include/Definitions.hpp` | 4190-4223 |
| `processor/include/FullStringData.hpp` | 4225-4265 |
| `processor/include/IClientHelloDataViewer.hpp` | 4267-4392 |
| `processor/include/IParsingErrorViewer.hpp` | 4394-4416 |
| `processor/include/IProcessor.hpp` | 4418-4488 |
| `processor/include/IRecordsDataViewer.hpp` | 4490-4515 |
| `processor/include/ParsingError.hpp` | 4517-4555 |
| `processor/include/ParsingTypes.hpp` | 4557-4600 |
| `processor/include/PayloadParams.hpp` | 4602-4666 |
| `processor/include/Record.hpp` | 4668-4710 |
| `processor/src/ExtensionsSorting.cpp` | 4712-4723 |
| `processor/src/ExtensionsSorting.hpp` | 4725-4852 |
| `processor/src/ISegmentBufferManager.hpp` | 4854-4918 |
| `processor/src/ITLSParser.hpp` | 4920-5237 |
| `processor/src/Md5Digest.cpp` | 5239-5277 |
| `processor/src/Md5Digest.hpp` | 5279-5326 |
| `processor/src/ParsingStatus.hpp` | 5328-5353 |
| `processor/src/Processor.cpp` | 5355-7023 |
| `processor/src/Processor.hpp` | 7025-7479 |
| `processor/src/SegmentBuffer.cpp` | 7481-7607 |
| `processor/src/SegmentBuffer.hpp` | 7609-7773 |
| `processor/src/SegmentBufferManager.cpp` | 7775-7893 |
| `processor/src/SegmentBufferManager.hpp` | 7895-7980 |
| `processor/src/TlsParser.hpp` | 7982-8403 |
| `processor/src/TtlsParser.cpp` | 8405-9137 |

### Consumer excerpts

CONS does not provide reliable `File:` path boundaries; some concatenations join a closing namespace and the next include on the same line. No repository paths are inferred from includes or class names. The six excerpts are byte-preserving line slices, may include boundary comments, and are labeled by observed symbols rather than asserted filenames.

| Synthetic excerpt label | CONS lines | Evidence |
|---|---:|---|
| `enforcement-listener` | 1-166 | TLSEnforcement listener callbacks and verdicts |
| `enforcement-detection-manager` | 167-343 | TLSEnforcement detection Manager |
| `fingerprint-listener` | 344-374 | TLSFingerprint listener callbacks |
| `fingerprint-packet-flow-manager` | 375-549 | TLSFingerprint PacketFlowManager and consumer actions |
| `visibility-listener` | 550-617 | TLSVisibility listener and collector handoff |
| `visibility-monitor` | 618-706 | TLSMonitor::collect and nested field consumption |

## 4. Source/contract integration map

| Concern | Inspected source anchors | Accepted contract anchors |
|---|---|---|
| Entry and orchestration | `TLSFlowConductor::onNewSession`, `prepareInboundExtension`, `process`, `dispatchStepResult`, `buildExtension`, `registerSessionObservers` | P4 4–6; P2 10/12 |
| Processing and parsing | `IProcessor::beginPacket/processRecord`, `Processor.cpp/.hpp`, `ITLSParser.hpp`, `TlsParser.hpp`, `TtlsParser.cpp`, `ParsingStatus.hpp`, `ParsingError.hpp` | P3 4–6; P4 T01–T12/Q01–Q22; P5 D6-06 |
| State and commit | `TLSData.hpp`; `TLSFlowConductor::commitNormalCurrentRecord`, `updateTLSDataOnClientHello`, `updateTLSDataOnChangeCipherSpec`, `updateTLSDataOnApplicationData`, `updateTLSDataOnAlert`, `commitUnsupported`, `commitViolation`, `commitError` | P2 4–14; P4 5; P5 2 |
| Dispatch and verdicts | `ITlsListener.hpp`, `ITlsSubject.hpp`, `TLSFeatureVerdict.hpp`, `TLSPriority.hpp`; `TLSFlowConductor::analyzeProcessResult`, `notify`, `onRecord`, `onClientHello`, `checkForParsingError` | P3 6; P4 4 and 7 |
| Viewers and callback data | `IClientHelloDataViewer.hpp`, `IRecordsDataViewer.hpp`, `IParsingErrorViewer.hpp`, `FullStringData.hpp`, `ClientHelloData.hpp`, `TLSRecordContext.hpp`, `Record.hpp` | P3 5 L01–L14; P5 3 and 5 |
| Storage and lifecycle | `TLSSessionExtension.hpp`, `TLSSessionExtensionTable.hpp`; `SegmentBuffer.cpp/.hpp`, `SegmentBufferManager::allocate/deallocate/access/onEvent`; `TLSFlowConductor::onAging` | P5 3–6; P4 G09 |
| Initialization | Engine `Factory::init/buildProcessor/buildTLSFlowConductor`; Engine `Application::init` and viewer/subject getters | P5 5, 7 and V7-01; startup-only allocation rule |
| Consumers | CONS Enforcement listener/Manager; Fingerprint listener/PacketFlowManager; Visibility listener/TLSMonitor | P3 8; P4 7; P5 3/5 |

This map establishes the candidate-relevant integration surface. It is not an exhaustive parser correctness audit, helper/storage audit, or line-by-line review of every extracted declaration. Extraction alone is not evidence that every line was semantically reviewed. Additional focused source inspection remains part of the later comparison where a proposed sharing boundary requires it.

### Directly checked current-code facts

- `IProcessor` exposes `beginPacket(PayloadParams)` and `processRecord(const TLSData&)`, five step outcomes and continuation modes. These are current TCP-oriented contracts, not mandatory target interfaces (CODE `processor/include/IProcessor.hpp`).
- `TLSFlowConductor::process` uses TCP sequence input and record stepping; its state helpers contain TCP evidence such as CCS/ApplicationData. This is evidence to classify in Step 2, not a reason to keep all state policy protocol-specific.
- `analyzeProcessResult` accumulates actual masks and latches zero-mask STOP; state helpers and `commitUnsupported` show current commit placement. P4/P5 accepted corrections still govern target behavior where the current implementation differs.
- Engine `Factory::buildProcessor` creates parser instances and wires the Processor during initialization. Existing construction proves wiring, not allocation-free behavior of all later paths.
- CONS contains synchronous CH handling and nested field consumption, but does not prove safe asynchronous retention or QUIC correctness of every downstream action.

## 5. Binding comparison constraint register

All entries below are **ACCEPTED** constraints, not architecture proposals. CODE/CONS references identify current integration surfaces only. Pn section references resolve to the exact source files and section ranges in Section 6 and the manifest.

| ID | Concern | Binding constraint | Authority | Comparison consequence |
|---|---|---|---|---|
| C01 | Scope/authority | Compare at least four genuinely different architectures before selection; Candidate 4 must investigate moving/generalizing TCP state logic into common semantic processing. Evaluation is mandatory, selection is not. | PLAN Phase 6; P5 10.2 | No current class boundary is a mandatory target boundary. |
| C02 | Observation boundary | QUIC v1, client-to-server observation and Initial protection processing only. Do not assume server Retry/HRR/Finished, decrypted Handshake, HANDSHAKE_DONE, or server transport parameters. Initial verification is not peer authentication. | P2 2; P4 Q01/Q11; P5 2 | Shared semantics must preserve unequal observable evidence. |
| C03 | Header interpretation | Interpret only visible/invariant fields without header unprotection. Protected reserved bits, packet-number length and packet number require Initial header protection removal; short-header key phase is not state evidence. | P2 2; P5 D6-06 | Do not confuse visible v1 Long Packet Type with protected packet-number fields. |
| C04 | Completion | An unambiguously associated client QUIC v1 Handshake Long Header is accepted product-level completion evidence, not observed TLS Finished. Before authoritative CH, retain only bounded deferred evidence; commit only under the accepted CH/feedback/state conditions. | P2 9; P4 Q11; P5 1 and 2 | Completion is accepted, not a reopened blocker or a proposal. |
| C05 | Semantic processing | Separate supported current-input syntax/encoding/protection findings from attempt-wide flow legality. TLS Enforcement owns flow-legality decisions and associated enforcement; TLS-flow/Processor do not independently drop. | P4 D4-01; P3 4 and 6 | STOP must not become an automatic network discard. |
| C06 | Fingerprint | Preserve the same fingerprint algorithm/output for the same complete CH across TCP and QUIC, including accepted legacy behavior; do not introduce an algorithm correction as part of this feature. | P4 D4-02; P3 4 | Sharing cannot change fingerprint semantics. |
| C07 | Freshness | Available retained result data is not automatically a new eligible observation. Dispatch only qualified fresh observations, with the explicit accepted TCP exception preserved. | P3 5; P4 4 and G05 | Viewer availability alone cannot trigger duplicate callbacks. |
| C08 | Unsupported | Apply the accepted pre-dispatch Unsupported exclusion: no consumer callbacks for the affected Unsupported unit. Support/resource exhaustion follows accepted Unsupported policy, not a synthetic protocol violation. | P4 Unsupported pre-dispatch contract and Q16; P5 D6-04 | Do not replace this with legacy dispatch ordering. |
| C09 | Feedback boundary | Resolve required current-unit feedback before accepting later semantic authority, mutating backing still in use, or proceeding to the next unit. Bounded read-only lookahead does not grant later authority. | P3 6; P4 4 and 5 | No speculative later-unit state commit. |
| C10 | Verdict reduction | Start the current-unit violation accumulator at zero, OR actual callback masks, and do not seed it with historical terminalViolationMask. Observer-list order and first-STOP behavior are preserved. | P4 4; CODE TLSFlowConductor::analyzeProcessResult and notify | Skipped/expired observers produce no synthetic verdict. |
| C11 | STOP without violations | A zero-mask STOP latches traversal stop; remaining applicable current-unit projections may run. A justified normal update remains permitted if the final mask is zero. No later unit is delivered. | P4 4; P3 FB-C | STOP alone is not Error and does not cancel every current-unit projection. |
| C12 | STOP with violations | STOP when the accumulated mask is nonzero causes immediate Error and stops remaining projections. A latched zero-mask STOP followed by a later violation resolves to Error under the accepted transaction rules. | P4 4; P3 FB-C | Preserve accumulated-mask semantics, not just the latest local verdict. |
| C13 | Commit authority | Keep one logical authority for accepted state disposition. Qualified normal commits follow permitted feedback; nonzero violations suppress normal updates. Parsers must not introduce competing state policy. | P4 5 G06-G08; P5 D6-07 | Logical authority does not require a new class. |
| C14 | Participation and terminal gates | Preserve extension/subscriber eligibility, TCP reuse/reset/terminal ordering and protocol-specific next-unit permission. QUIC terminal processing reopens only for an accepted new attempt, not late same-attempt evidence. | P4 5 G01-G04/G08 and T01/T07/T10/Q10/Q19 | Do not import QUIC candidate policy into TCP. |
| C15 | Attempt isolation | Maintain one active TLSData attempt context per DP Session. A qualified complete new attempt replaces the active attempt as session reuse; do not merge candidate and active state. | P2 10 and 14; P4 Q09/Q10; P5 5 | Candidate preparation is not active semantic authority. |
| C16 | Retry and HRR | Retry-compatible acquisition restart and HRR-compatible CH2 remain the same attempt and do not prove observation of server Retry or HRR. | P2 7 and 8; P4 Q07/Q08 | Do not invent observed server events. |
| C17 | Ambiguity and retired input | Retired-attempt suppression and ambiguous-association handling must not merge/reset active state. Missing or expired retired evidence alone proves neither retired identity nor newness. | P2 12; P4 Q06/Q19; P5 EV23 | Do not grow an unlimited history to avoid ambiguity. |
| C18 | 0-RTT and opaque traffic | 0-RTT does not change handshake state. Opaque 1-RTT or close evidence cannot substitute for accepted completion/alert evidence. | P2 11; P4 Q12/Q18; P5 2 | Shared state policy must not fabricate TCP-like records from opaque input. |
| C19 | Alert disposition | A readable associated Initial CRYPTO_ERROR close follows the accepted TerminatedByAlert policy, not Finished. Preserve protocol-specific evidence qualification. | P4 Q17/Q18; P5 2 | No generic synthetic completion from a close. |
| C20 | Ownership | Separate startup physical backing ownership from logical session/attempt/acquisition claims. Preserve the accepted resource ownership profiles; no Engine-global allocator service is preselected. | P5 3 and 5 | Do not turn a logical role into an automatically required component. |
| C21 | Borrow lifetime | Packet bytes, viewer results, nested full-string data and callback projections retain their specific borrow/validity boundaries. A copied wrapper is not owned backing. Finish all current uses before reuse/reset/release. | P3 5 L01-L14 and 8; P5 3 and 5 | Synchronous consumer use does not prove arbitrary downstream retention safe. |
| C22 | Replacement safety | Candidate preparation, acceptance, rollback and cleanup must preserve active authority and backing validity on failure. Bound admission, reservation and replacement peak. | P5 4; D6-02/D6-03/D6-05; 6.3 | Do not hide simultaneous active/candidate storage as routine tuning. |
| C23 | Reclaim | Request reclaim eligibility only after a newly committed Finished transition and current-unit last uses. Marking is not immediate release or proof marking succeeded; preserve required terminal identity and clear stale membership on accepted reuse. | P4 G09; P5 2 and 5 | Repeated/late evidence cannot repeat the transition or authorize new-lifecycle eviction. |
| C24 | Eviction | Resource eviction does not itself produce an Incomplete Handshake violation. Required context loss follows accepted lifecycle/Unsupported policy; it is not synthetic completion. | P2 13; P5 4 and 2 | Distinguish resource loss from observed protocol failure. |
| C25 | Allocation-free runtime | Allocate physical storage at Engine startup only. Packet processing and other Engine runtime work must not allocate dynamically or grow capacity, including indexes, listener slots and auxiliary bookkeeping. | P5 1, 5 and 6; user working rules | Runtime claim/recycle uses prepared storage; existing allocation-looking code is not a target exemption. |
| C26 | Bounded work | Enforce finite checked bounds for parsing, ranges/overlap, acquisition/context trials, reconstruction, retention and cleanup. Use the accepted symbolic storage/work dependencies and explicit exhaustion outcomes. | P5 6; D6-06/D6-08; 7 | No unbounded scan/retry/retention hidden behind a shared interface. |
| C27 | Numeric deferral | Numerical capacities may be selected after HLD presentation and architect decision. They must be finite, approved and prepared before enabling the path. Do not demand unavailable budgets or claim every value preserves identical traffic support. | P5 1 and 9.2 U8-CAP | A value requiring changed ownership, unbounded work or another mechanism reopens the affected design. |
| C28 | Current consumer boundary | Use the supplied Enforcement, Fingerprint and Visibility callbacks as integration evidence. Preserve accepted compatibility boundaries without certifying all helpers/storage/mitigation or QUIC support of existing consumer actions. | P3 8; P4 7; CONS callback fragments | No implicit approval of a QUIC TCP-reset mitigation path. |
| C29 | TCP preservation and fixes | Preserve accepted TCP observable behavior and staged evaluation; distinguish adopted TCP correction obligations from already applied patches. Do not require complete TCP handshake buffering solely for sharing. | P1 2-4; P4 10; P5 1 and V7-11; PLAN Phase 6 | Code differences from target policy are migration obligations, not silent policy replacements. |
| C30 | Proportionality and validation | Every new abstraction needs a current benefit. KISS/YAGNI, merge-before-split and limited DTLS hard-coding checks apply. Validate the selection with available or explicitly planned evidence; document review is not runtime proof. | PLAN Phase 6; P5 10.2; user working rules | No selection, pattern or final component split is made in Step 1. |

## 6. Precise document and symbol locators

Line numbers refer to the original preserved documents or CODE bundle. Section identifiers remain the primary semantic anchors.

### PLAN

- 1. Purpose: L3-12.
- 2. Scope and Authority: L13-46.
- 3. Engineering and Delivery Guardrails: L47-79.
- 4. Phase Review Procedure: L80-93.
- Phase 1: Establish Current Behavior and Required Change: L94-123.
- Phase 2: Investigate Shared TLS State Semantics and Define QUIC Behavior: L124-183.
- Phase 3: Research Semantic Equivalence and Define the Processing Contract: L184-229.
- Phase 4: Define Orchestration, Dispatch, and State Commit: L230-267.
- Phase 5: Define Ownership, Lifecycle, and Runtime Bounds: L268-301.
- Phase 6: Compare and Select a Proportionate Architecture: L302-349.
- Phase 7: Validate, Consolidate, and Freeze the Pre-Design Decisions: L350-397.
- Final Gate: READY FOR HLD: L398-418.
- What Remains for HLD: L419-430.
- Revision Note: L431-452.
### P1

- 1. Current TCP runtime map: L26-140.
- 2. TCP behavior to preserve: L141-151.
- 3. Implementation coupling that may change: L152-163.
- 4. Suspected defects (separate disposition): L164-176.
- 5. External items and their impact on the design: L177-190.
- 6. Where QUIC can't use the current mechanism (RFC): L191-215.
- 7. Requirement map: L216-218.
- 8. Exit gate: L219-228.
- 9. To advance: L229-232.
### P2

- 1. Purpose and Scope: L3-18.
- 2. Observation Boundary: L19-56.
- 3. Responsibility Boundary: L57-66.
- 4. State Meanings: L67-87.
- 5. Shared Semantics and Protocol-Specific Evidence: L88-130.
- 6. QUIC Transition Matrix: L131-156.
- 7. ClientHello and Retransmission Rules: L157-194.
- 8. Retry-Compatible Restart: L195-226.
- 9. Handshake Association and Deferred Evidence: L227-253.
- 10. Complete New Attempt: L254-297.
- 11. 0-RTT: L298-312.
- 12. Terminal and Late Input: L313-330.
- 13. Unsupported Outcomes: L331-346.
- 14. Reuse Decisions: L347-384.
- 15. Exit Gates: L385-401.
- 16. Final Decision: L402-429.
### P3

- Review outcome: L3-22.
- 1. Scope and authority: L23-36.
- 2. Affected-input inventory: L37-1088.
- 3. Evidence-backed equivalence matrix: independently reviewed consolidation: L1089-3245.
- 4. Semantic processing contract: L3246-3630.
- 5. Cardinality, freshness, ownership and lifetime: L3631-3937.
- 6. Required feedback boundaries: L3938-3953.
- 7. Focused scenario review: L3954-4806.
- 8. Decisions, unresolved items and deferred work: L4807-4865.
- 9. KISS/YAGNI concept review: L4866-4882.
- 10. Exit-gate evidence and Phase 4 handoff: L4883-4944.
- 11. Review procedure and preservation: L4945-4995.
- 12. V2 review delta and preservation: L4996-5003.
- V3 publication evidence: L5004-5010.
- Final consistency closure: L5011-5014.
- V5 Phase 3 exit-gate recheck: L5015-5023.
### P4

- 1. Executive decision: L5-14.
- 2. Authority, evidence and scope: L15-33.
- 3. Consolidated decisions: L34-56.
- 4. Shared transaction and exceptions: L57-87.
- 5. TLSData authority, timing and gate ownership: L88-117.
- 6. Complete flow catalogue: L118-540.
- 7. Consumer compatibility and common boundary: L541-556.
- 8. Focused walkthroughs and rechecks: L557-610.
- 9. EG1-EG9 independent exit-gate evaluation: L611-686.
- 10. Closure, candidate decisions and Phase 5 handoff: L687-707.
- 11. Review evidence, source map and preservation: L708-777.
### P5

- 1. Executive Outcome and Exact Readiness Boundary: L3-12.
- 2. Source/Authority Map, Baseline Integrity and Preserved Invariants: L13-96.
- 3. Resource Inventory and Ownership/Lifecycle Matrix: L97-318.
- 4. Lifecycle/Event and Allocation-Failure Outcomes: L319-374.
- 5. Isolation, Candidate Replacement, Observer Lifetime and Reclaim: L375-474.
- 6. Bound Register and Bounded-Work/Storage Model: L475-573.
- 7. Copies, Allocations, Crypto Costs and Fast Bypass Paths: L574-602.
- 8. Focused Walkthrough Evidence: L603-628.
- 9. Decisions, Numeric Deferrals and Runtime Feasibility/Validation Obligations: L629-804.
- 10. Independent EG1-EG6 Results and Phase 6 Handoff: L805-895.

### Core source symbols

- `onNewSession`: CODE `packet-flow/src/TLSFlowConductor.cpp`:48 (bundle L3367).
- `prepareInboundExtension`: CODE `packet-flow/src/TLSFlowConductor.cpp`:74 (bundle L3393).
- `process`: CODE `packet-flow/src/TLSFlowConductor.cpp`:109 (bundle L3428).
- `dispatchStepResult`: CODE `packet-flow/src/TLSFlowConductor.cpp`:167 (bundle L3486).
- `analyzeProcessResult`: CODE `packet-flow/src/TLSFlowConductor.cpp`:195 (bundle L3514).
- `checkForParsingError`: CODE `packet-flow/src/TLSFlowConductor.cpp`:268 (bundle L3587).
- `onClientHello`: CODE `packet-flow/src/TLSFlowConductor.cpp`:286 (bundle L3605).
- `onRecord`: CODE `packet-flow/src/TLSFlowConductor.cpp`:311 (bundle L3630).
- `onAging`: CODE `packet-flow/src/TLSFlowConductor.cpp`:350 (bundle L3669).
- `buildExtension`: CODE `packet-flow/src/TLSFlowConductor.cpp`:421 (bundle L3740).
- `commitNormalCurrentRecord`: CODE `packet-flow/src/TLSFlowConductor.cpp`:469 (bundle L3788).
- `updateTLSDataOnClientHello`: CODE `packet-flow/src/TLSFlowConductor.cpp`:501 (bundle L3820).
- `commitUnsupported`: CODE `packet-flow/src/TLSFlowConductor.cpp`:554 (bundle L3873).
- `commitError`: CODE `packet-flow/src/TLSFlowConductor.cpp`:572 (bundle L3891).
- `buildProcessor`: CODE `context/engine/src/Factory.cpp`:95 (bundle L1561).
- `buildTLSFlowConductor`: CODE `context/engine/src/Factory.cpp`:59 (bundle L1525).
- `allocate`: CODE `processor/src/SegmentBufferManager.cpp`:23 (bundle L7797).
- `deallocate`: CODE `processor/src/SegmentBufferManager.cpp`:72 (bundle L7846).
- `access`: CODE `processor/src/SegmentBufferManager.cpp`:93 (bundle L7867).

## 7. Evidence limitations and disposition

| ID | Limitation | Consequence / disposition |
|---|---|---|
| L01 | Source bundles have no repository commit/build identity. | Comparison uses supplied hashed snapshots. Confirm actual repository/API/provider conformance downstream; not a Step 1 blocker. |
| L02 | Full external SessionTable/DPFrame, upstream classification and crypto/provider implementations are not supplied here. | Preserve P2/P5 qualification contracts. Verify exact APIs when a candidate depends on them; a discovered incompatibility can reopen the affected mechanism. Do not invent an upstream guarantee. |
| L03 | Consumer paths and complete downstream helper/storage/mitigation implementations are not supplied. | Use symbol/range provenance; no complete consumer compliance claim. Targeted integration validation is planned, not executed. |
| L04 | No runtime tests, interoperability runs, benchmark measurements or target CPU/RAM budgets were supplied/executed. | Static contract comparison only. P5 9.3/9.4 V7-01–V7-12 remain downstream validation obligations, not automatic comparison blockers. |
| L05 | Support-sensitive numerical capacities remain intentionally deferred. | Apply U8-CAP. Finite approved startup capacities are required before enablement; do not invent values or demand budgets to compare architectures. |
| L06 | Historical intermediate reports and review logs cited by summaries are not all available. | Accepted final summaries are direct inputs; no claim of independent recovery or reexecution of their historical reviews. |
| L07 | Current TCP code is not proof that accepted corrections were implemented. | Preserve P4/P5 correction obligations and distinguish them from new refactors. No patches are applied in Step 1. |
| L08 | No FRD/SRS originals are included in this step input set. | Use accepted Phases 1–5 as requested. Do not claim independent FRD/SRS verification or add product requirements from recollection. |

No limitation above currently demonstrates an invalid comparison baseline. If later candidate analysis reveals a concrete dependency that can change correctness or selection, record the exact missing evidence and reopen only the affected issue.

## 8. Step 1 completion review

V1 is preserved. A secondary review found one incorrect authority anchor in C02 (P4 D4-12 concerns reclaim); V2 corrects it to P4 Q01/Q11 without changing the accepted constraint. The companion V2 review record reports the checks performed and distinguishes executable integrity checks from static document review. The review is limited to Step 1; it does not award Phase 6 EG1–EG8 PASS before an architecture is selected.

1. All eight required original inputs are registered with versions/identity, byte sizes and hashes; all seven historical hashes match P5 Section 2.
2. Original bytes are preserved in the archive. Module and consumer excerpts reconstruct exactly from the recorded ranges; no invented consumer repository paths.
3. Candidate-sensitive source/contract integration surfaces are mapped with explicit inspection limits.
4. Thirty constraints cover semantics, freshness, feedback/STOP, verdicts, commit, attempts, ownership, lifetime, reclaim and bounded runtime.
5. Accepted completion and capacity-deferral policy is retained; no old code or stale memory silently overrides it.
6. Evidence limitations distinguish downstream obligations from actual architecture-invalidating gaps.
7. No candidate selection, pattern commitment, new API or component split is introduced.

**Disposition: Step 1 PASS at the static baseline boundary. Proceed to Step 2: map current responsibilities and semantic sharing opportunities.** No Step 2 analysis is claimed complete.
