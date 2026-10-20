# O02: CH Orchestration Alternatives and Recommendation V2

**Status: completed comparison and design recommendation for review; not a user-approved implementation decision.**

O01 defines the shared incremental ClientHello algorithm. O02 asks who invokes that algorithm while advancing transport input. These are different decisions: sharing parsing/FP does not by itself justify sharing the TCP and QUIC traversal loop.

## 1. Scope and authority

This document preserves the Consolidated V5 section 11 orchestration comparison: A, base-owned orchestration with a protocol applicability seam; B, protocol-owned calls to shared CH processing. It extends B with a second, independent comparison: B1, minimal shared functions; B2, a composed ClientHelloProcessor that may implement the existing CH viewer. B remains the provisional orchestration recommendation; section 5.1 gives the structural recommendation after comparing B1 and B2. Neither is user-approved. It does not replace official HLD V10, amend O01, change code or approve the surrounding wrapper signatures. Existing protocol selection, viewer and Conductor responsibilities are preserved as working boundaries, not redesigned here.

### Sources and evidence classification

- **O01 proposal baseline:** `tls_flow_o01_shared_ch_contract_v4.md`, sections 3-8 and 10.2. Its logical operations and byte accounting are comparison inputs, not already implemented APIs.
- **Working analysis:** `tls_flow_strategy_template_continuation_consolidated_v5.md`, sections 3-6 and 11; especially CP01-CP09 and the last-use boundary. Its class sketches are proposals.
- **CODE FACT:** `TLS-Flow-All-Code.txt`. Aggregate line anchors: Conductor verdict reduction at L3514, normal commit at L3576-L3584, CH viewer reads/callback preparation at L3605-L3625; Processor packet entry/reset at L5410-L5468; cipher/FP work at L6776-L6809; body-parser entry at L8544-L8571. These show current separation and migration constraints, not an existing QUIC implementation.
- **INHERITED CONSTRAINT:** startup allocation is permitted; all affected Engine runtime, including aging, failures and callbacks, uses prepared storage without allocation, growth or heap fallback. QUIC v1 only. TCP arbitrary-byte/cross-record CH support is an approved extension, not current-code equivalence.
- **DESIGN JUDGMENT / PROPOSAL:** the comparison, recommendation and responsibility placement below. No RFC revalidation, benchmark or runtime proof is claimed.

All source snapshots remain unchanged. No extra product research or capacity values are needed to compare these two control-flow placements or the two internal sharing structures. V2 adds design judgments requested in the current discussion; permission to compare composition is not acceptance of B2. V1 review claims below are inherited static evidence, not newly executed code validation.

## 2. Invariants required of both alternatives

| ID | Required invariant | Authority |
| --- | --- | --- |
| I01 | One shared header/prefix/extensions and canonical FP implementation; no protocol copy of normalization, sorting or digest logic | O01 sections 1, 5, 6 |
| I02 | Only consecutive CH-relative handshake bytes; transport framing and deduplication remain outside shared interpretation | O01 sections 3-5 |
| I03 | Local NeedInput is not automatically public NeedMoreData; continue safe current-input work, but stop at feedback barriers | O01 sections 4.3, 8; V5 CP02-CP04 |
| I04 | Applicability precedes consumer exposure; QUIC nonblocking version finding and completed CH belong to one feedback unit | O01 sections 3, 8; V5 section 5.4 |
| I05 | Conductor retains callbacks, actual verdict reduction and permitted TLSData commits; parser findings do not manufacture violations | O01 section 8; V5 CP06-CP07 |
| I06 | Stable result/backing through callbacks, reduction, permitted commit and every final internal read; input end preserves required continuation | O01 sections 4.4, 8; V5 CP06-CP08 |
| I07 | Session/attempt association and candidate isolation survive pauses; no speculative overwrite or automatic restart | O01 sections 3, 4.2, 7 E07, 8 |
| I08 | Accepted-byte accounting and one-time FP contributions remain O01 semantics, including terminal failure and trailing bytes | O01 sections 4.3, 5-7 |
| I09 | No runtime allocation/growth, invented maximum, generic reassembly object, new reclaim mechanism or automatic Finished trigger | O01 sections 2.1, 4.4, 8, 10.2 |

Synchronous, non-reentrant use of the selected workspace remains the V5 section 3 precondition. A shared Engine scratch area is not persistent per-message continuation. A next pull sees current committed state, not a cached packet-start TLSData reference.

## 3. Alternative A: base-owned orchestration

### Control placement

A common processing base owns the progression from acquisition outcomes to shared CH advance, applicability and a yielded current unit. TCP and QUIC provide their acquisition behavior and protocol-specific applicability. Conductor still handles feedback and commits outside the base. The base resumes only after last use and the required disposition.

To support the actual flows, the base must distinguish at least the following logical outcomes, without assuming these are new enum values or virtual methods:

- More eligible CH bytes for the selected message.
- A non-CH unit requiring Conductor handling, such as an actual TCP record.
- Legitimate end of current traversal, with its reason.
- Failure or resource/lifecycle interruption.
- A completed CH requiring protocol applicability before exposure.

The base cannot assume every acquisition result is CH input or every yielded unit has a fresh CH. TCP record feedback can interrupt CH collection. QUIC frames/packets may produce no CH bytes or a qualified non-CH unit. Candidate disposition is still required where applicable.

### Strengths and costs

**Strength:** the shared progression and applicability-before-exposure ordering have one orchestration location. Repeated status-handling glue can be reduced. A can preserve every invariant in section 2 if its acquisition boundary is sufficiently expressive.

**Cost:** the common loop needs an internal acquisition/yield vocabulary spanning record boundaries, frame traversal, non-CH units and disposition. O01 does not supply this transport contract. Making it explicit is additional design work; hiding it in generic hooks is not simplification. The base must not absorb attempt policy, record semantics or physical resource ownership merely to make the loop uniform.

A is viable, not incorrect. A narrowly scoped shared helper is also possible, but a helper that does not own traversal is not evidence that a common base-owned loop is necessary. No fixed hook count is selected.

## 4. Alternative B: protocol-owned orchestration, shared CH algorithm

### Control placement

TCP and QUIC each own their framing/acquisition traversal and decide when to invoke the same O01 start/advance/release responsibilities. The shared algorithm owns header/prefix/extensions progression and FP contribution discipline. Protocol paths do not implement their own CH field parser or digest schedule.

TCP owns record/sequence boundaries and the existing record projections. QUIC owns v1 packet/frame traversal, verified Initial plaintext, CRYPTO coverage and permitted attempt association. The selected protocol path runs its applicability before exposing a newly completed CH. Conductor remains the sole callback/verdict/commit authority in this scope.

This is logical responsibility placement, not a demand for two new classes, a heap-allocated CH service, an inheritance hierarchy or a new public interface. Shared processing may live in existing code, a minimal shared-function extraction (B1), or a composed ClientHelloProcessor (B2). Section 4.1 compares these structures without changing protocol-owned traversal; exact signatures and physical storage ownership still require implementation design. O01 operation names remain proposed names.

### Strengths and costs

**Strength:** the caller already knows its transport boundary and can stop before the appropriate feedback unit without translating the entire transport traversal into a base contract. CH algorithm reuse is achieved independently of traversal reuse. Non-CH paths remain natural protocol paths.

**Cost:** each protocol call site must correctly handle accepted counts, applicability, exceptional outcomes and feedback suspension. This is a real duplication risk in integration glue, not a reason to duplicate parsing/FP. Use the same contract and focused cross-protocol checks; extract a small common helper only if actual duplicated logic warrants it. Do not add a new coordinator just to police two call sites.

### 4.1 Two decision axes: B1 versus B2

**DESIGN JUDGMENT / PROPOSAL.** A/B selects who owns transport orchestration. B1/B2 selects how the same CH algorithm is packaged. B2 is not a third transport-orchestration option C. It would become a materially different orchestration option only if the processor started pulling transport input, driving strategies or owning the common traversal loop. That is not the proposal evaluated here.

**B1: minimal shared functions.** Both protocol strategies invoke the same CH operations over the correct logical context. Existing viewer routing reads that context through the existing contract. Functions may be internal or existing members; this is not a requirement for free functions or a new utility framework. The benefit is minimal new object wiring. The cost is that operation invocation and viewer/context routing remain separate integration obligations; functions alone do not ensure they select the same context.

**B2: Strategy plus composition.** A separate internal ClientHelloProcessor encapsulates shared CH interpretation, canonical FP progression and, if integration permits, the existing IClientHelloDataViewer implementation. Each protocol strategy holds a borrowed pointer/reference to that prepared collaborator. It invokes processing but retains framing, input traversal, applicability and feedback suspension. The collaborator does not call back into strategies to obtain more transport data. No implementation-bearing common strategy base or Template Method traversal is required. A common strategy interface for selection/delegation may remain; removing a common orchestration base does not mean removing that interface.

The processor's coherent responsibility is interpretation and access to its CH result, not all TLS processing. It does not acquire TCP records, decrypt QUIC packets, manage CRYPTO gaps, admit attempts, issue enforcement verdicts, commit TLSData, decide Finished or own aging. Existing record and parsing-error viewer responsibilities are not automatically moved into it. Shared CH diagnostics remain available to the existing error/feedback path without replacing that path.

| Criterion | B1: shared functions | B2: composed processor, existing CH viewer |
| --- | --- | --- |
| Shared CH/FP implementation | One implementation over selected context | Same implementation encapsulated in one collaborator |
| Traversal and applicability | Protocol-owned | Identical protocol ownership; no hidden common loop |
| CH result access | Existing viewer implementation routes to context separately | Processor can colocate interpretation and existing CH getters; selected-path routing remains necessary |
| Added machinery | No new processor lifetime or collaborator reference required | Prepared processor lifetime, borrowed references and current-context association must be explicit |
| Persistent state | Correct logical context, not function-local scratch | Correct logical context, not an unqualified last-CH member in a shared object |
| Migration risk | Routing and invocation can drift | Moving CH viewer implementation can affect existing callers and status semantics |
| Failure and cleanup | O01 contract and existing lifecycle | Same contract; processor destruction or pointer rebinding is not a substitute for release |
| Performance/resources | No measured advantage or allocation-free proof | No measured advantage or allocation-free proof; no per-packet object allocation |
| Best justification | Existing routing already remains simple and coherent | Interpretation and CH access form a useful cohesive boundary without extra registries/adapters |

Neither structure inherently requires dynamic allocation, a singleton, virtual processing methods, a factory, a new public viewer or a context registry. Both preserve I01-I09. B2 is composition at the processing boundary even if implementing the existing viewer uses interface inheritance.

### 4.2 B2 ownership, association and viewer eligibility

The following are logical constraints on the B2 proposal, not approved API signatures or a physical pool design:

1. **Collaborator lifetime:** the enclosing processing assembly supplies the prepared collaborator; strategy references are borrowed, not evidence of ownership. The collaborator must outlive those references and every permitted viewer read. Exact construction/owner placement remains implementation design; startup allocation is allowed, runtime allocation/growth is not.
2. **Persistent continuation:** accepted frontier, checkpoints, retained facts, backing and digest progress belong to the selected session/attempt/message context. Sharing a processor pointer shares behavior, not one mutable persistent CH slot. Separate processor instances would still have to preserve all interleaved contexts; one per strategy is not a solution to session interleaving by itself. No instance count is fixed here.
3. **Current association:** for the synchronous, non-reentrant interval, processing and viewer reads must resolve to the same eligible context. The association remains stable through callbacks, verdict reduction, permitted commit and final internal reads. It cannot switch to another session or candidate during that interval. A later input reselects the correct context rather than trusting a last-session pointer. No new bind/unbind API or identity field is mandated.
4. **Exposure:** implementing IClientHelloDataViewer does not make shared Complete immediately public. The selected strategy applies applicability first, then the existing selected-path/wrapper routing exposes the eligible current unit. Public None/Partial/Complete meanings, retained TCP Complete eligibility and freshness remain unchanged. Getter access must not bypass that route to reveal a speculative candidate or stale result. No second copied CH payload is introduced.
5. **End and failure:** end of transient input borrowing clears unsafe transient association without erasing required continuation or prematurely invalidating retained TCP results. Release occurs only at the full last-use boundary under the existing lifecycle. A rejected live-context start preserves existing state/result/views; a failed non-live start creates no valid result. Shared parser/resource failure cannot resume a partially modified digest. Invalid calls accept zero before mutation; they do not manufacture a peer violation.
6. **Candidate isolation:** evaluation uses isolated permitted backing; the processor cannot select a candidate as the public active result merely because it processed it. O05 still determines disposition and physical handoff. O04 still verifies every viewer caller and lifetime. Composition closes neither gate.

**Focused CODE FACT:** the supplied code declares IClientHelloDataViewer at aggregate L4286 and Processor implements it alongside IProcessor, IRecordsDataViewer and IParsingErrorViewer at L7054. This establishes an existing CH interface that B2 could implement; it does not establish that extraction is already safe, that all callers are compatible, or that the current Processor is a reusable transport-independent collaborator. No new CH viewer contract is proposed.

## 5. Comparison and recommendation

| Criterion | A: base-owned | B: protocol-owned |
| --- | --- | --- |
| Canonical CH/FP reuse | One shared algorithm | Same shared algorithm; no loss of reuse |
| Common ordering enforcement | Central loop can enforce it | Explicit obligation at each protocol call site |
| TCP record feedback during partial CH | Acquisition must yield a record boundary to the base | TCP stops its own traversal at that boundary |
| QUIC non-CH traversal and candidates | Requires expressive base outcomes and protocol policy separation | Stays in QUIC traversal; O05 handoff still open |
| Added abstraction | Common acquisition/yield contract plus orchestration placement | No common transport acquisition contract required by O02 |
| Migration exposure | More TCP traversal may move into a common skeleton | TCP traversal can remain local while CH algorithm is extracted |
| Runtime performance | Not measured; cannot claim slower or faster | Not measured; cannot claim faster or allocation-free merely by placement |
| Primary risk | Generic loop hides protocol-specific authority or grows hooks | Call-site glue drifts across protocols |

**Recommendation D-O02-01: prefer B, protocol-owned calls to the shared incremental CH algorithm.** This is a design recommendation awaiting acceptance, not an accepted implementation contract.

The deciding factor is the boundary demonstrated by O01: the reusable algorithm begins at consecutive logical CH bytes, whereas transport traversal can yield non-CH units and feedback boundaries. B shares the proven common responsibility without requiring a second, broader common acquisition contract. This is a KISS/YAGNI judgment, not a claim that A cannot work or that B has measured performance advantages.

Reconsider A only if concrete implementation traces reveal substantial identical orchestration that can be centralized without importing record/attempt policy or weakening the barriers. Do not reopen the decision solely for hypothetical DTLS support. Avoid unnecessary TCP/QUIC hard-coding inside the shared CH algorithm, but introduce no DTLS entry, registry, hooks or reserved storage.

### 5.1 Structural recommendation after the B1/B2 comparison

**Recommendation D-O02-02: retain B1 as the minimal default; keep B2 as an explicitly evaluated viable alternative, not an approved component.** B2 has a genuine cohesion benefit: CH interpretation and existing CH data access can live together without a common transport base. However, the supplied evidence does not yet demonstrate that this removes more routing/wiring than it introduces. Both variants still need the same context association, applicability barrier and last-use discipline. Therefore adding an object solely to express composition is not justified by this comparison alone.

This is a completed design-level comparison with a provisional recommendation, not a request to restart architecture research. Prefer B2 instead if a focused integration sketch shows that one internal processor can implement the existing CH viewer and be borrowed by both strategies with less duplicated routing, no parallel viewer state, no extra registry/coordinator and no weakened context/lifetime rules. That is a concrete reason to change the structural preference without reopening A/B. Conversely, if B1 needs repeated CH getter adapters or exposes parser internals merely to avoid a class, B1 is no longer the simpler choice. Neither variant gets a performance preference without measurements.

**Template Method disposition:** under recommended B, no shared base-owned transport Template Method is selected. B1 and B2 both preserve Strategy selection and one shared CH algorithm. This is the decision O02 was intended to examine, not a reason to discard O01 or restart the overall design. Do not retain an empty implementation base just to preserve the pattern name.

D-O02-01 remains provisional B at the orchestration level. D-O02-02 is the provisional B1 default at the packaging level; B2 is not rejected as incorrect. User acceptance of the comparison task does not accept either architecture. O03-O08 remain independent gates, not reasons to invent capacities or block this static comparison.

## 6. Recommended responsibility and state placement

| Responsibility/data | Logical owner under B | Boundary |
| --- | --- | --- |
| Input selection and transient delegation | Existing/proposed wrapper boundary | Selection is fixed for the input; failed begin exposes no stale selection/result |
| TCP record and sequence cursor | TCP path | No new out-of-order/gap or overlap/retransmission policy |
| QUIC packet/frame cursor, CRYPTO ranges and arrival metadata | QUIC path | Only verified supported input; dedup before CH acceptance; no inferred Retry/HRR proof |
| CH accepted frontier, checkpoint, retained facts and digest progress | Correct logical CH context | Prepared backing persists across input and session interleaving; physical owner remains O05/O06 |
| Applicability | Selected protocol path | After fresh shared completion, before exposure; not rerun as a fresh event for retained TCP Complete |
| Current-result routing | Existing viewer contract through selected path | No copied CH return payload or viewer replacement; availability is not freshness |
| Callback order, verdict reduction and permitted TLSData commit | Conductor | Parsing completion alone grants no admission or commit permission |
| Release/reclaim | Existing lifecycle after full last use | No parallel aging/reclaim; pending continuation cannot be discarded by input end |

No new state owner is implied by a call site. Candidate evaluation uses isolated permitted backing and cannot overwrite active state before authorized disposition. O02 neither chooses the handoff API nor assumes TLSData alone conveys accepted/rejected/deferred disposition. If the next design cannot represent that information, O05 blocks final lifecycle signatures rather than being silently bypassed.

## 7. Representative control-flow walkthroughs

These are static design walkthroughs, not packet tests. They exercise the recommendation without fixing C++ hooks.

### F01: complete CH in one eligible region

The protocol path establishes the correct message/context, offers logical handshake bytes and advances its cursor by accepted. Shared processing can complete all checkpoints in one call. The path applies protocol applicability, then yields the stable current unit. Conductor completes callbacks, reduction, permitted commit and final reads. Only then may another semantic unit be processed. Neither A nor B needs a forced return after each CH field.

### F02: partial header/prefix and later input

The path offers consecutive bytes and shared processing returns NeedInput after accepting the offered region. If more safe bytes exist in the same input, the protocol traversal continues. If traversal legitimately ends, it preserves CH checkpoint/backing and protocol acquisition state, then ends transient borrowing. A later input revalidates association before resuming. Neither input entry nor a record/frame boundary restarts the digest.

### F03: TCP CH across records with a feedback barrier

TCP removes record headers and offers only the eligible CH region. If a record unit must be yielded while CH is incomplete, TCP returns that unit rather than requesting a fictitious complete CH. After Conductor feedback and full last use, TCP resumes with current committed state only if permitted. Shared continuation survives. Exact legacy record/error/CH callback timing is O03, not proven by this walkthrough. A would need an explicit acquisition outcome for the same boundary.

### F04: QUIC gap, retransmission and several frames

QUIC retains ranges beyond a gap without calling shared advance out of order. A mixed duplicate/new range offers only the next new consecutive bytes; newly available retained ranges may then advance the same CH within the current input, subject to barriers. No packet-number-based shortcut suppresses genuinely new bytes. A non-CH unit or qualified exceptional outcome can yield without invoking CH. Neither alternative requires a synthetic TLS record.

### F05: complete CH plus trailing bytes and nonblocking finding

Shared Complete accepts only the remaining CH bytes. QUIC applicability attaches the agreed nonblocking version finding while retaining full CH/FP. CH and finding are exposed in the same feedback unit. Trailing bytes remain unprocessed until Conductor feedback and final reads end and any required candidate disposition permits resume. Completion time remains the arrival timestamp of the datagram making the full CH available, not callback time. A retained TCP Complete projection is not a new CH evaluation or new applicability event.

### F06: Stop, failures and input cleanup

A callback Stop prevents the next semantic unit. The Conductor still performs the existing actual-verdict reduction and permitted commit behavior; Stop alone is not a new automatic Error/drop rule. ParseFailure and ResourceFailure end shared continuation without publishing a successful CH or retrying a partially modified digest. ContractFailure is an integration defect, not a peer violation; invalid calls do not mutate the context. A rejected live-context start preserves the previous valid result. Cleanup waits for all last uses; a successful input interval ends even on Stop, while failed begin unwinds itself. Resource eviction does not create Incomplete Handshake violation.

### F07: session interleaving and candidate disposition

Session A pauses, B completes, then A resumes. Each selects its own live context; no mutable last-session scratch supplies A's facts. A candidate cannot replace active backing merely because the protocol path invokes a mutable CH operation. Accepted/rejected/deferred handling remains O05. A next pull is forbidden while disposition or last use is pending; no new acknowledgement API is invented here.

### 7.1 Cross-check of B1 and B2 against the same flows

| Flow | B1 | B2 additional association check |
| --- | --- | --- |
| F01 complete | Shared operations finish; existing viewer reads selected context after applicability | Processor CH getters read that same eligible context; no early exposure |
| F02 partial/resume | Context persists; next call resumes accepted frontier | Borrowed collaborator survives; transient selection is not persistent continuation |
| F03 TCP cross-record | TCP retains record traversal and feedback boundaries | Processor receives no record header and cannot drive TCP past feedback |
| F04 QUIC gap/duplicate/non-CH | QUIC deduplicates and supplies consecutive new bytes | Processor does not own ranges or force a CH call for a non-CH unit |
| F05 trailing bytes/finding | Complete leaves trailing input; applicability and CH are one unit | Viewer implementation does not expose completion before the finding is attached; completing-datagram timestamp remains protocol metadata |
| F06 Stop/failure/cleanup | Existing stop/reduction/commit and O01 failure semantics | No rebind/release before final reads; rejected live start preserves prior views; failed non-live start creates no result |
| F07 A/B/A and candidate | Each invocation selects correct isolated context | One shared pointer cannot mean one persistent CH; no candidate/public-result switch before authorized disposition |

These are static design checks, not executed packet tests. The original F01-F07 remain applicable unchanged; the added checks do not select new APIs.

## 8. Migration boundary and next step

The recommendation changes where shared CH operations are invoked, not the public consumer contract. Preserve valid-input FP/FullString and existing viewer meanings. Do not treat approved TCP fragmentation expansion as permission for unrelated defect fixes or silently changed malformed-input handling.

| Gate | Still required |
| --- | --- |
| O02 acceptance | Review D-O02-01 and D-O02-02 separately: B orchestration and B1 default versus viable B2 packaging. Neither is binding without acceptance |
| O03 | Trace TCP record/error/CH callback timing, cross-record input and malformed previously supported cases; disposition any differences before migration acceptance |
| O04 | Verify existing viewer routing, partial/complete eligibility, freshness and all caller lifetimes |
| O05 | Specify candidate disposition, active/candidate ownership and physical handoff before final lifecycle API |
| O06 | Select maximum CH, finite capacities, widths and joint peaks before storage implementation acceptance; no values invented here |
| O07 | Prove provider fit and allocation-free packet, failure, callback and aging paths before activation |
| O08 | Select/verify qualified completion evidence before state-policy implementation; no Finished trigger inferred here |

The extended O02 comparison is ready for review. The next focused design work is O03 using B and the B1 default as clearly labeled proposals unless accepted; any focused B2 integration sketch must preserve section 4.2 and does not automatically close O04/O05. Do not start implementation or automatically advance through O03-O08 from this document. Official HLD V10 and all input baselines remain unchanged.

## 9. Double-check before delivery

A first static pass compared A and B against I01-I09 and F01-F07. A second consistency pass checked the recommendation against O01 sections 4-8, V5 sections 3-6/11 and the code anchors listed in section 1. Review results below are design-level judgments, not executed tests.

| Check | Evidence / conclusion |
| --- | --- |
| Alternatives compared before recommendation | Sections 3-5 give A a viable central-ordering benefit and B an explicit call-site drift cost; no invented numeric score or benchmark |
| Shared algorithm versus shared traversal | Sections 3-6 keep the same O01 algorithm in both alternatives; B does not fork FP or prescribe new classes |
| Incremental and non-CH paths | F01-F04 preserve internal NeedInput, TCP record barriers, QUIC gaps and units without CH |
| Completion versus exposure/admission | F05 and section 6 keep applicability before exposure, same-unit findings and O05 disposition |
| Stop and failure semantics | F06 preserves Conductor reduction/commit, no automatic Error/drop, nonresumable parser/resource failures and no mutation on invalid calls |
| Lifetime and continuation | Sections 2/6 and F02/F06/F07 preserve backing through callbacks, reduction, permitted commit and final reads; input end does not erase needed continuation |
| Candidate/session isolation | Section 6 and F07 forbid active overwrite, stale scratch and resume before disposition |
| Resources and scope | Sections 1/2/8 preserve startup-only allocation, prepared runtime storage, QUIC v1, no capacity guesses, no DTLS framework and no new reclaim mechanism |
| Authority and closure | Recommendation is not acceptance; O03-O08 and official HLD authority remain explicit |

**Disposition: extended O02 comparison complete; provisional B orchestration and B1 default, with viable B2 composition, ready for user/design review.** No blocking inconsistency was identified within these static checks. No build, unit test, packet execution, measured performance, runtime allocation audit or exhaustive inherited-contract audit was performed. This review does not turn proposed wrapper/API names into approved signatures.

### 9.1 V2 focused double-check and preservation boundary

The V1 review narrative in section 9 is inherited evidence. V2 adds a focused semantic review of sections 4.1-4.2, 5.1 and 7.1 against I01-I09, F01-F07 and the supplied O01 continuation/lifetime contract. It checks both sharing structures rather than treating composition as automatically better.

| Check | Focused static conclusion |
| --- | --- |
| Decision axes | A/B controls traversal; B1/B2 packages the algorithm; no false third orchestration option |
| Fair comparison | B1 has lower wiring cost; B2 has cohesion benefit and explicit migration/association costs; no unsupported performance score |
| Recommendation consistency | Scope, sections 5/5.1, migration gate and final disposition distinguish provisional B from provisional B1; B2 remains viable |
| Shared context and viewer | Section 4.2 and all seven added flow rows preserve association, applicability, freshness and full last use; existing interface implementation is not public-contract replacement |
| Failure/resource boundaries | No runtime allocation/growth, per-packet object, automatic violation/drop/Finished, digest replay, parallel reclaim or active-candidate overwrite |
| Preservation | Original I01-I09, F01-F07, A/B comparison, responsibility table and O03-O08 gate rows remain; only listed scope/recommendation/next-step statements are updated, with additive structural analysis |
| Evidence limits | Focused source declarations were inspected; inherited code anchors and historical review claims are not represented as a new exhaustive audit. No build/runtime/performance PASS |

The delivery check runs preservation and structural assertions twice on unchanged V2 bytes, checks source digests, and reviews the actual diff separately. These mechanical checks supplement, not replace, the semantic review above. V1 and source snapshots are not overwritten. No broader implementation or runtime acceptance is claimed.

## 10. Source snapshot identities

- `tls_flow_o01_shared_ch_contract_v4.md`: SHA-256 `53b1c8d6556c5ba57b6bb6f127994d895118334630341abd9f47db9142360a0a`.
- `tls_flow_strategy_template_continuation_consolidated_v5.md`: SHA-256 `6724bd5d3cf77703b11b456319fdb0713b10460177057798e647425b4298b2c2`.
- `TLS-Flow-All-Code.txt`: SHA-256 `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b`.

- `tls_flow_o02_orchestration_comparison_v1.md` (preserved revision input): SHA-256 `61204b841e7740836b652279914a440c1e2b49143ce0afd0eaa83c2a623cf674`.
