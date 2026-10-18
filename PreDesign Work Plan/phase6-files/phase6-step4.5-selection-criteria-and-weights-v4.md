# Phase 6 Step 4.5: Architecture Selection Criteria and Importance Weights V4

## 1. Purpose and status

**Proposed decision framework for user review. No candidate is scored or selected. Weights are recommendations, not previously accepted requirements.**

A test here is an architecture assessment question, not necessarily an executable unit test. Importance has two forms: mandatory eligibility gates, which cannot be traded away, and weighted preferences among eligible concrete alternatives. A high aggregate score cannot compensate for incorrect behavior.

This step supplements Step 4 V7 before returning to Step 5. It does not overwrite Step 4, the existing Step 5 V9 decision, or any accepted Phase 1-5 contract. Freeze the framework and weights before scoring. Reassessment does not silently revoke the existing decision; Step 5 must explicitly reconcile any different recommendation with its preserved V9 baseline.

### Revision and evidence boundary

V4 is a narrowly corrected copy of V3: M04 now preserves the two-level STOP contract, and the evaluation input is Candidate 4 V4. All weights, gates and other criteria remain unchanged. V3 and its review record remain immutable historical artifacts; the earlier V3 framework PASS does not establish correctness of its superseded M04 wording.

For this correction, Step 4 V7 S04-S05, Candidate 4 V4 Sections 5.1-5.3 and the supplied TLSFlowConductor::analyzeProcessResult implementation were reinspected. Other inherited source locators are not a claim of a fresh full source audit. Internal framework review establishes readiness for acceptance and source-grounded assessment, not candidate eligibility, implementation equivalence, measured performance or Phase 6 completion.

## 2. Authority and fair comparison

Use these preserved sources:

- `phase6-step4-candidate-comparison-v7.md`: Sections 2-8, K01-K08, S01-S15 and H01-H11.
- Current evaluation inputs: Candidate 1 V5 (`candidate1-team-architecture-proposal-v5.md`), Candidate 2 V2 (`Candidate2-V2.md`), Candidate 3 V4 (`candidate3-team-architecture-proposal-v4.md`) and Candidate 4 V4 (`candidate4-team-architecture-proposal-v4.md`). Older summaries do not override these definitions.
- Candidate 4 V4 SHA-256: `c2d5f558e4fcf1ae3722af2ca29e25239c71ed6dd0cbdbe411dad9fbf55700bb`. Step 4 V7 B4 still identifies historical Candidate 4 V3 and its digest. For this assessment only, V4 is the explicit current candidate input; Step 4's semantic contracts remain authority, while its historical B4 identity is not represented as the identity of V4. Record this input substitution in every applicable evaluation record without editing Step 4 or its manifest.
- `phase6-step1-authoritative-comparison-baseline-v2.md`: C01-C30 and accepted-contract/evidence boundaries.
- `phase6-step2-responsibility-and-semantic-sharing-assessment-v3.md`: D01-D28 and semantic-sharing assessment.
- `TLS-Flow-All-Code.txt`: current TCP behavior, not proof of future implementation.
- `phase6-workflow-v1.md`: proportionality, compare-before-select, and Phase 6 workflow.

Source locators below refer to Step 4 V7 unless explicitly stated otherwise. Existing RFC obligations are inherited, not redefined here. No fresh RFC compliance audit or performance measurement was performed to author this framework.

Compare C1, C2, C3, C4-F and C4-P fairly. C4 variants are sharing scopes, not automatically separate complete architectures. A hybrid must state its entry/processing boundary, CH evaluation scope, sole semantic writer, feedback coordinator, physical owners, lifecycle and changed symbols. Examples such as C1 + C4-P and C2 + C4-P are eligible for evaluation, not preselected winners. Do not average parent scores or cherry-pick parent advantages: assess the integrated hybrid and its added seams. Do not enumerate every theoretical combination without a concrete benefit.

## 3. Mandatory gates: absolute importance, no percentage

Gate outcomes are PASS, FAIL or BLOCKED. PASS means a coherent architecture-level mechanism and traceable evidence exist, not that product tests passed. FAIL rejects the concrete realization, not automatically the entire family. BLOCKED means a selection-relevant contradiction or missing mechanism remains; a blocked alternative cannot be declared the winner. Authorized downstream implementation tests and numeric sizing deferrals do not alone cause BLOCKED.

Run G01-G07 in order and stop on the first FAIL/BLOCKED. Correct only in a new version and restart. All inherited requirements remain binding even when not repeated below.

| ID | Mandatory assessment | Required architecture evidence | Source |
|---|---|---|---|
| G01 | Accepted scope and protocol/RFC correctness | Supported protocol/visibility boundary; no invented server observation, fake TLS records, authentication or completion evidence. Distinguish product completion policy from observed TLS Finished. Any concrete RFC conflict is surfaced, not silently resolved by a score. | K01; S09/S11/S15; H01; Step 1 C01-C04/C18-C19 |
| G02 | TCP behavioral equivalence, except explicitly accepted corrections | Preserve callback order, actual masks, retained Complete/U05, staged parsing, terminal/reset/aging exceptions and state timing. Separate current behavior, approved corrections and proposed changes. No requirement to preserve a known defect contrary to accepted corrections. | S01-S05/S12/S14-S15; H02/H04/H09; Step 2 D01-D28 |
| G03 | QUIC attempt and state correctness | One active core, isolated active candidate, qualified terminal reset before CH parsing, no ambiguous/retired revival, same-attempt restart/CH2 and accepted completion ordering. | S06-S09/S12; H05/H10/H11 |
| G04 | One semantic authority and actual feedback | One writer per selected transaction, exact STOP reduction, no speculative later authority, no synthetic observer verdict or automatic drop. | S02/S04-S05; H02-H04 |
| G05 | Ownership, backing and failure safety | Physical owners, valid nested reads, last-use boundaries, admission/rollback, victim-aware retirement, exactly-once release, cause-specific Unsupported before affected-unit dispatch. Resource eviction is not synthetic SessionEnd/Incomplete Handshake. | S07/S10/S13-S15; H06/H08 |
| G06 | Startup-prepared storage and bounded Engine work | No runtime allocation/growth/heap fallback in packet, aging, cleanup or failure paths. Finite prepared resource/work model, checked replacement peak and progress. Numeric capacities remain deferred as authorized; no unavailable CPU/RAM budget is demanded. | Section 6; H07; Section 5.3 |
| G07 | Coherent integration and evidence discipline | Concrete responsibility/changed-symbol map; compatible viewer/consumer/Factory/lifecycle wiring; no unresolved selection-critical contradiction. Distinguish code fact, accepted contract, judgment and unexecuted validation. | K02/K08; Sections 7-10 |

**Why RFC correctness has no percentage:** an incorrect protocol interpretation must not win by being simple or fast. Among correct designs, clarity and localization of protocol rules are assessed under T03/T05, not by awarding extra correctness points.

**Why equivalence appears again below:** G02 tests required behavior; T01 compares how safely and convincingly a conforming refactor can preserve it. These are eligibility versus migration risk, not permission to buy away correctness.

## 4. Weighted preferences: recommended importance totals 100%

Weights represent preference strength, not probability, implementation confidence, test coverage or measured speed. The runtime-sensitive Engine and TCP preservation receive the largest individual weights. DTLS receives a small weight because current scope is QUIC; avoiding unnecessary hard-coding is useful, implementing speculative extensibility is not.

| ID | Assessment criterion | Weight | Why this importance | Step 4 mapping |
|---|---|---:|---|---|
| T01 | TCP equivalence assurance and regression containment | 18% | Existing behavior has subtle feedback, continuation and lifecycle contracts; relocation risk matters even after gate compliance. | K03/K05; S01-S05/S12-S15 |
| T02 | Runtime efficiency | 20% | Evaluate added work across all Engine runtime paths, not only happy-path packet parsing. | K06; Section 6 |
| T03 | KISS: end-to-end simplicity | 12% | Favor understandable complete flows, not merely fewer boxes or shorter files. | K02; Section 7 |
| T04 | YAGNI: necessary delivery scope only | 8% | Avoid frameworks, abstractions and storage with no current requirement. | K08; Sections 7-8 |
| T05 | SOLID: cohesion and contract quality | 8% | Useful boundaries should localize changes without forced interfaces or inheritance. | K02/K07; Section 7 |
| T06 | Minimum necessary existing-code change | 5% | Reduce unnecessary migration, while allowing justified refactoring and required fixes. | K05/K08; Section 8 |
| T07 | Testability and diagnosability | 8% | Enable meaningful trace, failure and provenance checks, not only mocked success. | K07; S01-S15 |
| T08 | Prepared-memory efficiency | 9% | Account for real prepared storage and overlapping lifetimes without assuming shared code means shared backing. | K04/K06; Section 6.2 |
| T09 | Maintainability and semantic-policy drift control | 7% | Make future accepted TCP/QUIC changes safe without forcing unlike evidence into one policy. | K02/K04/K08; Sections 7-8 |
| T10 | DTLS/future-protocol change locality | 2% | Detect avoidable TCP/QUIC hard-coding without paying now for unknown requirements. | Section 7; Step 2 sharing assessment |
| T11 | Delivery integration and reviewability | 3% | Prefer a coherent staged change plan with explicit dependencies and review boundaries. | K08; Section 8 |
| | **Total** | **100%** | | |

The design-principle budget T03-T05 is 28% in total, not three unrestricted opportunities to reward the same abstraction. T01/T06/T11 also assess different effects, as defined below.

### 4.1 Substantive weight reassessment (retained from V3)

These are recommended priorities, not measured optimal weights. They are chosen before candidate scoring, based on the current Engine constraints rather than a preferred architecture.

| Criterion | V1 | V3 | Priority rationale |
|---|---:|---:|---|
| T01 | 18% | 18% | Keep TCP regression containment prominent; mandatory equivalence itself remains a gate. |
| T02 | 18% | 20% | Increase: all Engine runtime paths are sensitive, including aging and failure cleanup. |
| T03 | 12% | 12% | Keep end-to-end comprehensibility central without equating it to class count. |
| T04 | 10% | 8% | Reduce: speculative scope matters, but should not dominate runtime, memory and validation quality; overlaps need discipline. |
| T05 | 8% | 8% | Keep concrete contract/dependency quality; do not reward patterns for their own sake. |
| T06 | 8% | 5% | Reduce: small migration is useful but is a means, not the goal; necessary refactoring must remain competitive. |
| T07 | 7% | 8% | Increase: observable feedback, state and lifetime boundaries are important for this change. |
| T08 | 7% | 9% | Increase: prepared storage and replacement overlap are real architecture costs even while numeric capacities remain deferred. |
| T09 | 6% | 7% | Increase: safe ongoing TCP/QUIC policy changes deserve more weight than source-change minimization. |
| T10 | 3% | 2% | Reduce: future-protocol hard-coding is worth checking, but DTLS is not current delivery scope. |
| T11 | 3% | 3% | Keep staging/review organization useful but secondary to the architecture itself. |

T02+T08 now receive 29%, T03-T05 receive 28%, and T06+T11 receive 8%. This intentionally shifts preference away from incumbent preservation and speculative extensibility toward operational cost and validation. These groups are explanatory budgets, not additional scored criteria. Weight acceptance is still required before scoring.

## 5. Assessment questions, evidence and score anchors

Score each T criterion once, from 0 to 5. Subquestions are evidence prompts, not separately weighted tests. Anchors 0/3/5 below are criterion-specific. Use 1 or 2 for documented intermediate weakness and 4 for documented improvement between 3 and 5; explain the interpolation. A 0 preference score means very weak relative suitability, not necessarily a gate failure. Any actual gate violation is handled by Section 3 regardless of score.

### T01: TCP equivalence assurance and regression containment, 18%

**Questions:** Which callback/state/continuation paths change? Can old/new traces be compared? Are entry/lifecycle exceptions isolated? Are approved corrections separated from relocation?

**Evidence:** changed-symbol and semantic-delta map, the same S01-S15 walkthroughs and common scenario matrix for every alternative, current-code anchors and a differential regression plan. Score architectural exposure and containment under equal evidence expectations, not document length or the polish of a test plan. Do not require unimplemented product tests to claim architecture-level assurance, but do not call that executed equivalence.

**Anchors:** 0 = widespread semantic movement with weak containment; 3 = explicit affected paths and credible regression plan; 5 = strongly localized exposure with precise trace mapping and justified correction boundaries. Few changed lines alone do not earn 5.

### T02: Runtime efficiency, 20%

**Questions:** What additional parsing/hashing, copies/rebases, dispatches, lookups, context trials and cleanup visits occur? Are duplicate/retired/partial/error paths efficient after trustworthy qualification? Does an abstraction require repeated work?

**Evidence:** use the same symbolic work ledger from Step 4 V7 Section 6 for every alternative; identify changed operations and unresolved provider costs. Include aging, retirement and failure paths. No guessed nanoseconds, throughput, virtual-call penalty or claim that composition is automatically free.

**Anchors:** 0 = avoidable repeated expensive work or poorly localized overhead despite finite bounds; 3 = explicit bounded work with justified overhead; 5 = supported reduction/avoidance of material work with no compensating hidden cost. If evidence cannot distinguish alternatives, use equal justified scores or uncertainty, not invented differences. No-allocation compliance alone does not earn efficiency points.

### T03: KISS, 12%

**Questions:** Can a reviewer follow ordinary TCP/QUIC and terminal/replacement flows without hidden control transfers? Can responsibilities merge without losing clarity? Are exception paths explicit?

**Evidence:** minimal responsibility map and short walkthroughs including S05/S07/S14. Count conceptual handoffs only as supporting evidence, not a class-count target.

**Anchors:** 0 = opaque coordination or excessive indirection; 3 = understandable flows with justified boundaries; 5 = minimal coherent control and explicit exceptions with no unnecessary forwarding layers.

### T04: YAGNI, 8%

**Questions:** What current requirement justifies each new interface, base, service, queue, context field and retained store? What can be removed now? Is DTLS being used to justify a speculative framework?

**Evidence:** necessity/removal table: item, present consumer/requirement, simpler alternative and reason to retain.

**Anchors:** 0 = substantial speculative scope; 3 = material additions have current justification, but some necessity trade-offs remain; 5 = every material addition has a present need and speculative work is explicitly deferred. Necessary QUIC support is not penalized for being new. A demonstrably unnecessary addition must be removed in a new candidate version before freeze, or explicitly retained with a documented trade-off and scored as unnecessary scope. An uncertain need is not a proven removable extra. Any changed candidate definition requires reassessment, not silent cleanup after scoring.

### T05: SOLID, 8%

Use these as operational review questions, not a demand for five classes or a generic textbook compliance certificate:

- SRP: do responsibilities have coherent reasons to change, with clear writer versus consumer roles?
- OCP: can a concrete current change be localized without repeatedly editing unrelated policy?
- LSP: where substitution exists, do strategies/adapters honor outcome, lifetime and feedback contracts without fake records or unsupported operations?
- ISP: do consumers receive only capabilities they actually use, without dummy QUIC record methods?
- DIP: are policy dependencies appropriately narrow and testable, without creating interfaces solely for dependency inversion?

**Evidence:** actual dependency/capability map and one current TCP/QUIC change exercise. Where no subtype/interface exists, assess the relevant contract without inventing one; absence of inheritance is not a penalty.

**Anchors:** 0 = high coupling or leaky capabilities; 3 = coherent responsibilities and honest contracts; 5 = focused dependencies and well-localized current changes without unnecessary abstraction. A correctness-breaking substitution fails a gate rather than merely losing points.

### T06: Minimum necessary existing-code change, 5%

**Questions:** Which existing symbols remain, move, generalize or change behavior? Are changes essential for current scope? Can a smaller correct change achieve the same result?

**Evidence:** source-impact map separating pure movement, semantic edits, public API adaptation and common required corrections. Do not rank by lines changed alone or treat existing class boundaries as immutable.

**Anchors:** 0 = broad avoidable rewrite; 3 = bounded justified change set; 5 = no material existing-code change can be removed without weakening current requirements or the coherence of the solution. This anchor does not depend on which competitors are present. Do not reward retaining a defect or defer necessary integration to appear smaller.

### T07: Testability and diagnosability, 8%

**Questions:** Can tests observe callback/state traces, provenance, failure disposition and resource last use? Can real adapters be tested against contracts as well as mocked outcomes? Can a failure be localized without a new production logging framework?

**Evidence:** test-seam map for S01-S15, expected observations, negative cases and diagnostic reuse.

**Anchors:** 0 = tests depend on opaque broad fixtures; 3 = practical seams and observable failure outcomes; 5 = focused tests plus end-to-end conformance traces and clear failure localization. Test count alone is not quality.

### T08: Prepared-memory efficiency, 9%

**Questions:** What storage is actually prepared, shared or duplicated? What overlaps during active/candidate replacement and nested reads? Are ownership and peak accounting understandable?

**Evidence:** symbolic prepared-memory/peak ledger from Step 4 V7 Section 6.2, charged once per backing, with startup capacities left symbolic as authorized. Include inline metadata and temporary claims.

**Anchors:** 0 = material avoidable prepared-storage or peak duplication; 3 = justified backing and metadata with bounded overlap; 5 = demonstrated avoidance of material unnecessary simultaneous backing/metadata without weakening lifetime or support. Transparency is an evidence prerequisite, not a memory-saving benefit. Missing comparative evidence produces U or an interval, not a low efficiency score merely for poor documentation. No RAM-saving claim follows from one evaluator name. Compare symbolic terms at the same workload and capacity assumptions; if authorized capacity deferrals prevent ordering, retain uncertainty rather than invent values.

### T09: Maintainability and policy drift, 7%

**Questions:** How many semantic decision sites must change for one accepted policy correction? If policy remains separate, how is parity checked? Does shared policy preserve different TCP/QUIC evidence predicates?

**Evidence:** change-impact exercise for a shared CH publication rule and a protocol-specific terminal rule; identify policy sites and parity tests.

**Anchors:** 0 = uncontrolled duplication or forced false uniformity; 3 = intentional sharing/separation with parity discipline; 5 = few coherent change sites and explicit protocol qualification. Shared policy is not automatically superior to justified separation.

### T10: DTLS/future-protocol locality, 2%

**Questions:** Which shared names/contracts unnecessarily assume TCP records or QUIC packet types? Could a future protocol add its own qualification without rewriting unrelated shared semantics? What cannot be assessed without future requirements?

**Evidence:** a boundary-level change sketch only. Do not invent DTLS parsing, state, completion, storage, APIs or supported versions. This is not a DTLS design or RFC audit.

**Anchors:** 0 = avoidable protocol assumptions embedded in genuinely shared contracts; 3 = protocol-specific evidence remains explicit with no speculative framework; 5 = a concrete, low-cost locality improvement is demonstrated without extra current runtime/storage burden. Do not award 5 for promises that all protocols fit a universal interface.

### T11: Delivery integration and reviewability, 3%

**Questions:** Can changes be reviewed in coherent stages? Are Factory/viewer/lifecycle dependencies and enablement boundaries explicit? Can a stage be validated without claiming a partial package enables QUIC?

**Evidence:** dependency-ordered delivery slices and validation/enablement conditions. No invented schedule or staffing estimate.

**Anchors:** 0 = tangled migration with unclear integration order; 3 = coherent dependency plan; 5 = small reviewable slices with explicit prerequisites and no hidden integration work. This measures delivery organization, not change volume or correctness.

### 5.1 Common scenario matrix and equal evidence depth

Apply the same scenarios to every complete alternative, including hybrids. This matrix operationalizes existing gates and preferences; it adds no new weighted criterion and does not replace the inherited S01-S15 coverage. The scenario descriptions below are review prompts from the preserved discussion, not newly accepted protocol rules. Resolve their exact expected traces against the authoritative sources before awarding gate PASS.

| ID | Scenario | Required architectural observation | Main mapping |
|---|---|---|---|
| M01 | Ordinary TCP and QUIC CH processing | Entry, parsing, feedback, qualified publication and sole semantic writer are explicit. | G02-G04; T01/T03/T07 |
| M02 | Partial CH with usable input remaining | Continue to the next relevant feedback boundary, a completed ignorable unit or input exhaustion; partial CH alone does not discard remaining input. | G02-G04/G06; T01/T02 |
| M03 | IGNORE | Cursor/work progresses; no stale result, observer dispatch or semantic commit is attributed to the ignored unit. | G04/G06; T02/T07 |
| M04 | Zero-mask observer STOP and two-level feedback | Stop the current event listener list immediately and latch STOP. If the accumulated unit mask remains zero, continue remaining applicable events of that same unit and permit its normal update. Do not process a later unit. If later feedback adds violations, apply the exact current-event and final-mask rules below. Do not fabricate violations or equate STOP with packet DROP. | G02/G04; T01/T07; Step 4 V7 S04-S05; Candidate 4 V4 Sections 5.1-5.3 |
| M05 | Semantic evidence with zero callbacks | Qualified state handling remains possible without a fake TLS record or fabricated observer verdict; use only the accepted completion policy. | G01/G03/G04; T07 |
| M06 | Same-attempt restart/CH2 and new-attempt replacement | Qualification, active/candidate isolation, publication timing and old-attempt retirement are explicit. | G03/G05; T07/T08 |
| M07 | Ambiguous association and retired traffic | No ambiguous merge/reset or retired-attempt revival; identify bounded qualification work. | G03/G06; T02 |
| M08 | Admission/replacement failure | Show failure disposition, rollback, victim effects, peak claims and exactly-once release without synthetic incomplete-handshake violation. | G05/G06; T07/T08 |
| M09 | Nested reads and last use | Identify physical backing, borrowed lifetime and the earliest safe reclaim boundary. | G05; T07/T08 |
| M10 | Terminal, reset, aging and cleanup | Preserve applicable TCP exceptions and QUIC lifecycle rules; show bounded work and no runtime allocation on all paths. | G02/G05/G06; T01/T02 |
| M11 | Shared and protocol-specific policy changes | Exercise the same shared CH publication correction and protocol-specific terminal change; distinguish current dependency quality from future change sites. | T05/T09 |

**M04 ordered trace requirements:** listener-level STOP and unit-level reduction are separate. OR each current event's actual mask into the unit accumulator. Immediate Error/STOP occurs only when the current event returns STOP and the accumulator after OR is nonzero. An earlier zero-mask STOP followed by Continue with violations does not itself cause immediate exit; remaining applicable events run unless a later current STOP triggers that condition. At final reduction, a nonzero mask gives Error if STOP was latched, otherwise violation-only commit; neither permits a normal update. A zero final mask permits normal commit even with latched STOP. Return STOP when latched and suppress the next unit. Preserve context capture and backing lifetime through remaining permitted current-unit work. Parser Stop follows its separate exception route and is not this observer contract. No surrounding SessionTable packet-drop semantics are inferred.

For each row record source/section, preconditions, ordered trace, writer, backing/last use, failure/STOP disposition, symbolic work and remaining uncertainty where applicable. Mark genuinely inapplicable protocol details with a reason, not as missing coverage. Do not require identical internal TCP/QUIC steps or invent wire equivalence. Planned tests remain planned; document walkthroughs are not executed runtime tests.

## 6. Scoring and uncertainty

1. Freeze this framework version and candidate/hybrid definitions before assigning scores. Proposed weights require user acceptance before use as the decision baseline.
2. Resolve gates first. Retain FAIL/BLOCKED alternatives in the evidence register but do not rank them as eligible winners.
3. For each criterion record: score or interval, exact source/section, mechanism, limitation, and evidence kind (code fact, accepted contract, architectural judgment or measured result). Longer documentation is not inherently stronger evidence.
4. Unknown is **U**, not 0, 3 or 5. For an unresolved preference use a justified interval [low, high]; completely unknown is [0,5]. Do not drop its weight or renormalize remaining criteria. Gate-critical unknowns remain BLOCKED.
5. With weights expressed as percentage points, total = sum(weight_i * score_i / 5), from 0 to 100. For intervals compute the corresponding lower and upper totals. This is a preference index, not a measured probability of success. Do not publish an exact winner from overlapping uncertainty as if it were certain.
6. Do not multiply by an invented confidence percentage. Report confidence/limitations separately.
7. Use the same assessment depth and evidence expectations for standalone and hybrid alternatives. A hybrid must pay for integration seams and cannot inherit only its parents' best scores.
8. Do not select solely by total. Explain material disadvantages, unresolved assumptions and trade-offs. There is no invented minimum passing aggregate score.

### Decision-driving evidence gaps

For each uncertainty capable of changing the recommendation, record the disputed mechanism, affected gate/criterion, plausible score range, smallest useful evidence action, expected discriminating observation and responsible downstream stage. Prefer focused source inspection, a contract trace or a bounded prototype over implementing every candidate. Do not invent a timing measurement or force a numerical capacity decision that is explicitly deferred. If evidence remains unavailable, retain the interval/tie and state what judgment is required. Selection-critical missing mechanisms remain BLOCKED, not a preference uncertainty.

### Sensitivity and close decisions

Before recommendation, vary each weight individually by plus/minus 20% of its proposed value, renormalizing all weights to total 100 for that sensitivity case. This is a proposed decision-robustness exercise, not a product requirement. Also inspect plausible score intervals. Before scoring, freeze these additional grouped sensitivity cases: (A) multiply T02/T08 by 1.20; (B) multiply T01/T07 by 1.20; (C) multiply T03/T04/T05/T09 by 1.20. In each case leave other raw weights unchanged, then renormalize the entire vector to 100. Run cases separately, not cumulatively. They test operational-cost, regression/validation and design-quality emphasis; they are not alternative accepted baseline weights. A one-at-a-time sweep alone does not establish robustness to correlated preferences. Report results for every case, including unchanged rankings. Report whether the leading alternative changes; do not tune weights after seeing a preferred winner. If ranking is fragile, state that explicitly and resolve the decision-driving evidence gap or present the trade-off for user judgment. If alternatives remain indistinguishable, prefer the simpler justified scope only when evidence supports that comparison; otherwise retain a tie.

## 7. Prevent double counting

| Potential overlap | Separate assessment boundary |
|---|---|
| KISS / YAGNI / SOLID | T03 = reasoning/control simplicity; T04 = necessity of present scope; T05 = cohesion and capability/dependency contracts. One generic claim of fewer classes cannot earn three improvements. |
| Equivalence / minimum change / delivery | T01 = regression exposure and assurance; T06 = avoidable source/API change surface; T11 = dependency/review organization. Explain distinct effects if one mechanism affects all three. |
| SOLID / maintainability / DTLS | T05 = present dependency quality; T09 = ongoing TCP/QUIC policy change sites; T10 = avoidable future hard-coding only. No extra reward for hypothetical extensibility. |
| Runtime / memory | T02 = operations/work; T08 = prepared bytes and overlapping claims. Fewer copies can affect both only with separate work and storage evidence. |
| Gates / preferences | Gates establish mandatory feasibility/correctness; weighted tests compare compliant mechanisms. Passing a gate is not a bonus score. |

### 7.1 Distinct-effect rule

One mechanism may affect multiple criteria only through separately stated consequences and evidence. Record mechanism ID, affected criteria and a distinct effect for each. A common interface earns no automatic SOLID, maintainability and DTLS bonuses: show capability quality, ongoing policy-change sites and a concrete avoidable hard-coding issue separately. A shorter patch earns no automatic KISS or equivalence bonus. Do not mechanically prohibit legitimate multiple effects, but remove duplicate arguments before aggregation. Evidence completeness is assessed consistently across alternatives and is not an extra preference score.

## 8. Evaluation record template

Use one record per concrete alternative, with a stable definition/version:

- Alternative ID and exact source versions.
- Responsibility/flow/ownership definition and integration delta for a hybrid.
- G01-G07: outcome, evidence and any deferred implementation validation.
- T01-T11: weight, score or interval, source anchor, reason, limitation and overlap explanation where needed.
- Common scenario matrix results and distinct-effect register.
- Decision-driving evidence gaps and minimal discriminating actions.
- Weighted total or range; individual and grouped sensitivity outcomes; principal trade-offs.
- Decision status: eligible for comparison, rejected realization, blocked, tied or recommended for explicit Step 5 reconciliation.

No scores are populated in this document. The existing Step 5 V9 combination receives no bonus for being the incumbent and is not silently revoked by this new framework.

## 9. Review and freeze procedure

Review this framework before applying it:

1. Source identity and baseline preservation.
2. Coverage of every user-proposed topic, including equivalence, minimum change, RFC correctness, runtime, SOLID, KISS, YAGNI and DTLS.
3. Mandatory-versus-preference separation and inherited-contract fidelity.
4. Distinct criterion boundaries and no duplicate weighted subtests.
5. Substantive priority rationale and trade-offs for every weight; positive weights totaling 100; score direction and formula endpoints correct.
6. Criterion-specific questions, evidence and stable score anchors; common scenario coverage and separation of evidence quality from architecture merit.
7. Honest unknown handling, hybrid integration and no predetermined winner.
8. Scope discipline: no invented capacities, performance measurements, RFC conclusions or future protocol implementation.
9. Decision procedure, individual/grouped sensitivity, source locators and evidence limits consistent.
10. Two full internal review passes on identical bytes, with external evidence and no claim of unperformed source or runtime validation.

Stop at the first substantive failure; revise only in a new version and restart. Require two complete review passes on identical document bytes before marking the framework reviewed. Repeated automated arithmetic/string checks are not two independent semantic reviews. Keep review evidence external and identify what actually ran. User acceptance freezes weights for scoring; review alone does not imply acceptance. Any later change to weights or definitions requires a new version and comparable rescoring, not retrospective tuning.
