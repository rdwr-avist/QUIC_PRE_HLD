# Phase 6 Step 4: Evidence-Backed Architecture Comparison V7

## 1. Executive outcome and boundary

**Comparison complete at the static architecture-analysis boundary. No architecture is selected.** C1, C2, C3, C4-F and C4-P are compared on the same eight criteria and fourteen retained scenario topics plus the explicit S15 coverage supplement. No inherent incompatibility is demonstrated for these candidate definitions; specific contract-breaking realizations are rejected in Section 5. Retaining a definition is not implementation approval. Step 5 must choose a proportionate concrete scope and justify departures from shared policy. [E17,E27]

This revision compares the seven current baseline inputs identified in Section 9: four standalone candidate proposals, current code, Step 1 V2 and Step 2 V3. Step 4 V6 is the immediate immutable revision input; V5, V4 and V2 remain preserved ancestry, not additional authority. Candidate 4 V3 is a narrowly corrected new copy of V1, addressing the terminal-candidate ordering defect described in S07. All other current inputs are unchanged. V1 is retained as historical evidence, not silently treated as corrected. The standalone proposals supersede older candidate summaries for candidate placement. No Step 5 selection is imported into this comparison.

Accepted P3-P5 contracts are inherited through Step 1/2; original locators retained for continuity are not a claim of a fresh archive audit. Current code facts are checked against the supplied bundle, while accepted target corrections remain distinct from current behavior.

Evidence labels: **F** = inspected code fact; **A** = accepted contract; **J** = architectural judgment derived from the cited definition and facts; **V** = validation obligation, not executed evidence. Matrix cells are J unless explicitly identified otherwise. No numerical score, cost measurement, target throughput or invented capacity is used. The evidence register distinguishes current sources, inherited contracts and historical locators.

## 2. Candidate normalization and fair comparison

| Candidate | Traversal/feedback authority | Semantic writer and sharing scope | Current definition |
|---|---|---|---|
| C1 | Shared Conductor with explicit protocol branches | Conductor alone commits TLSData; protocol Processors own acquisition/CH evaluation and compose helpers. | B1 Sections 2-6,8-10 |
| C2 | Shared Conductor pulls one result through revised processing contract | Conductor alone commits TLSData; protocol strategies produce qualified facts, not synthetic verdicts. | B2 Sections 2-8,10 |
| C3 | Thin Conductor selects one protocol flow | Selected flow is sole logical writer/reducer; common helpers are primitives, not a second policy owner. | B3 Sections 3-6,9-10 |
| C4-F | Coordinator over protocol extraction and common staged CH evaluation | Common policy alone writes TLSData; common CH evaluator produces facts without writing TLSData. | B4 Sections 3-6,9 |
| C4-P | Same coordinator/common policy, protocol-owned CH evaluation | Common policy alone writes TLSData; protocol evaluators produce qualified facts under common algorithm/output contract. | B4 Sections 3-6,9 |

B1-B7 resolve in Section 9. C4-F/C4-P are evaluator-scope variants of C4, not five independent architectures. C4-P may combine with C1 entry points or C2 strategies. C3 plus common policy is a possible hybrid, not the standalone C3 definition. Logical roles do not require separate classes. Step 2 U/G/P/N classifications are sharing assessments, not mandatory architecture. [E26,E27,E29]

TLS Enforcement retains attempt-wide flow-legality authority in every candidate. A common policy is not a replacement Enforcement authority. In both C4 variants the evaluator never writes TLSData; the coordinator invokes ordinary transaction policy actions after actual feedback. Authorized entry/lifecycle actions are separate: in particular, terminal-candidate reset precedes CH parsing under S07 and does not wait for CH feedback. References below to evaluator/policy describe adjacent logical responsibilities, never two semantic writers. [E26,E27]

All candidates inherit the same accepted constraints: client-only QUIC v1/Initial capabilities, accepted associated client Handshake Long Header completion, TCP U05, actual feedback, single active attempt, candidate isolation, last-use validity, startup-only allocation and authorized numerical deferral. Existing TCP restoration/rollback/no-wrap corrections and QUIC implementation work are common obligations, not optional benefits. [E01-E17,E29]

## 3. Same-criteria comparison matrix

Each row uses the same criterion for all five entries. The evidence column applies to every cell in that row; candidate-specific placement comes from the four standalone proposals in E27. Risks are potential failure modes requiring tests, not findings that the implementation already fails.

| ID / criterion | C1 | C2 | C3 | C4-F | C4-P | Evidence |
|---|---|---|---|---|---|---|
| K01 / Requirement and accepted-contract fit | Fits through explicit TCP/QUIC branches; must preserve exception order rather than reuse record semantics for QUIC. | Fits if revised result contract represents no-projection evidence, partial acquisition and Unsupported separately; a record-only contract is incompatible. | Fits if selected flow is sole logical writer and uses the same accepted policies; two protocol copies require parity checks. | Fits if staged qualified access preserves TCP and common evaluation does not fabricate uniform observability. | Fits if qualified fact/provenance access preserves CH output and actual shared policy; evaluator separation is not itself a contract violation. | E01-E10, E13, E17, E27 |
| K02 / Responsibility clarity | Existing Conductor remains explicit transaction/state owner; protocol branches grow in the same integration point. | Common processing boundary separates traversal from acquisition; outcome/lifetime vocabulary is an added contract to maintain. | Flow owns complete protocol sequence; thin outer entry must not also commit or reduce feedback. | Extraction, semantic evaluation/policy and transaction are logically distinct; over-splitting them into classes is unnecessary. | Common policy is isolated from protocol evaluator mechanics; fact production and policy applicability must not become competing writers. | E06, E18-E24, E26-E27 |
| K03 / Feedback and STOP preservation | Can retain existing reducer with protocol-aware projection eligibility; risk is branching around its exception paths. | One pull boundary exposes feedback gating; risk is precomputing later results or overwriting views inside next-result preparation. | Flow keeps local sequencing; common reducer helps, but each flow must preserve exception and next-unit rules. | Coordinator and common policy make feedback/commit order explicit; no adapter may commit speculatively. | Same coordinator/policy ordering as C4-F; evaluator fact availability must not imply dispatch or commit permission. | E02-E06, E18, E25, E27 |
| K04 / State and attempt isolation | Conductor authorizes replacement; protocol support stays separate. Active candidate evaluation cannot reuse live active backing unsafely. | Long-lived strategies require explicit current-result binding and session/attempt support ownership; interface identity alone gives no isolation. | Flow-local sequence simplifies protocol context, but flow is not a second TLSData owner and must not retain shared workspace across invalidating reuse. | One policy writer reduces policy divergence; shared evaluator increases importance of provenance and active/candidate last-use intervals. | One policy writer with protocol-owned evaluation avoids forcing evaluator migration; qualified facts still need same-attempt provenance and backing validity. | E02-E03, E06, E11-E13, E26-E27 |
| K05 / TCP regression risk | Conductor branches and any extracted CH helpers affect dispatch/commit ordering; unchanged TCP Processor contract can limit one migration surface. | Changes IProcessor/result/viewer adaptation and Conductor loop; continuation/Stop translation and U05 are specific regression targets. | Moves Conductor protocol sequence/state call sites into TCP flow; behavior-preserving relocation still exposes reset/aging/terminal ordering. | Moves intertwined CH/digest/viewer and state logic; staged prefix/restoration/visibility/U05 have direct migration exposure. | Moves state-policy calls and qualified facts while retaining more CH parsing; still exposes state applicability, timestamp provenance and reset/reclaim ordering. | E04-E07, E13 D6-01/02/08, E18-E24, E26-E27 |
| K06 / Runtime cost and boundedness | Adds protocol selection and branches; no inherent event-copy/queue requirement. Branch cost and code footprint are unmeasured. | Adds strategy dispatch/result translation; can be nonallocating and borrowed. Virtual call count/representation/copies depend on actual contract. | Adds flow delegation and shared-helper calls; does not require queue or per-input flow allocation. Duplicate policy code is not automatically duplicate runtime work. | Adds qualified access/coordinator boundaries and may share evaluator workspace; extra copying or repeated parsing is not required but must be checked. | Adds fact-to-policy access; evaluator storage may be shared or separate by live intervals. Less source movement does not prove less RAM or CPU. | E11-E16, E20-E24, E27; Section 6 cost model |
| K07 / Testability | Test existing Conductor with protocol-path inputs and callback spies; protocol branches can couple fixtures. | Inject deterministic strategy outcomes to test reducer/loop; separately test strategy-to-contract conformance, not only mocked happy paths. | Test each flow with common verdict traces; cross-flow policy parity needs explicit tests because helper reuse alone does not ensure parity. | Test common evaluator/policy once for semantic actions, plus both adapters and staged access; shared tests do not replace TCP/QUIC integration. | Test common policy from qualified facts and each protocol evaluator; require same-CH differential outputs and provenance tests across the fact boundary. | E01-E09, E16 V7-03..08/11, E18-E28 |
| K08 / Implementation effort and delivery impact | Implement QUIC path and explicit Conductor branches; helper extraction only where needed. Avoid assuming existing runtime allocations/failure branches are already compliant. | Define revised contract, adapt TCP Processor/viewers and wire both strategies. This contract migration is extra work versus keeping separate entry points, not a measured total-effort ranking. | Extract TCP flow and build QUIC flow; shared delivery helpers reduce duplicate mechanics, while state sequencing may remain duplicated. | Extract common CH evaluation/state policy, preserve staged access and adapt both extraction paths. Broad parser migration is optional only if C4-P is chosen instead. | Extract shared policy and qualify protocol facts; retain evaluator internals where accepted. A smaller CH migration surface is a scope fact, not a guaranteed cheaper delivery. | E13 D6-01/02/08, E16 V7-01..12, E18-E24, E26-E27 |

### 3.1 What the matrix does and does not establish

- **Fit:** each definition has a coherent place for the accepted policies; none is proven compliant merely because its text repeats them. Concrete invalid realizations are rejected below. [E01-E17,E27]
- **Regression exposure:** C4-P retains more existing CH evaluation than C4-F by definition; C2 changes the processing contract, C3 moves sequence ownership, and C1 changes Conductor branching. These are different migration surfaces, not a justified universal ordering of risk or effort. [E18-E24,E26-E27]
- **Sharing:** one implementation of a semantic action can reduce policy drift, but shared code is not evidence of correct applicability. C3 helper reuse does not by itself share the complete state policy; C4-F evaluator reuse does not make TCP and QUIC evidence identical. [E01-E06,E26-E27]
- **No winner:** the matrix identifies the trade-offs to resolve in Step 5. No candidate wins from DRY, naming symmetry, DTLS or an unmeasured claim that virtual calls/copies are cheap or expensive. [E17,E27]

## 4. Common accepted scenarios and candidate-specific consequences

Each scenario has one authority-backed expected behavior and five candidate consequences. These are static walkthroughs, not executed packet tests. Scenario coverage includes TCP and QUIC asymmetry rather than assuming identical wire inputs. Each table cell inherits its scenario authority and the corresponding candidate definition in E27.

### S01: Staged TCP CH and continuation

**Accepted behavior:** Prefix/digest/extension progress is not complete-CH authority; preserve supported opaque/alert modes, mode-specific restoration and no mandatory complete TCP handshake buffer.
**Authority:** P3 Section 5 and FB-F; P4 T03/T11; P5 D6-01 and V7-05/11. [E02-E03,E07,E13,E16,E20-E22]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Keep TCP Processor staging; helper reuse must not reset opaque/alert modes or erase U05. |
| C2 | Adapter exposes partial/ready states and valid borrows; revised contract must not require a complete owned message for every step. |
| C3 | TCP flow preserves original staged sequence and invokes common helpers only at valid milestones. |
| C4-F | Common evaluator needs staged qualified access and digest/locator restoration; reject a contiguous-whole-TCP-message prerequisite. |
| C4-P | Protocol evaluator preserves staging; common policy consumes qualified complete facts only at permitted commit. |

### S02: Multiple units; later unit fails

**Accepted behavior:** Resolve each unit before later authority; earlier committed observations/consumer side effects survive a later failure. No whole-datagram rollback, replay queue or arbitrary tail cutoff.
**Authority:** P3 FB-B/FB-E; P4 T04/Q04/Q20; P5 6.4. [E03,E05,E07-E08,E14,E18]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Conductor loop resolves current unit before next protocol-path call. |
| C2 | Conductor requests next result only after disposition; strategy scanning cannot precommit later semantics. |
| C3 | Flow drives this boundary internally and returns Stop without outer Conductor reprocessing. |
| C4-F | Coordinator finishes evaluation/projections/policy for current unit before later extraction authority. |
| C4-P | Same boundary; separate evaluators cannot publish later facts into active policy prematurely. |

### S03: Fresh versus retained CH; known duplicate

**Accepted behavior:** Retained Complete may still trigger accepted TCP U05 callbacks without a new CH commit; QUIC duplicate-only input creates no new CH callback/count/fingerprint evaluation after trustworthy classification.
**Authority:** P3 Section 5/U05; P4 D4-02/04,T05,Q05; P5 D6-01,7.2. [E02,E04,E07-E08,E13,E15,E18,E21-E22]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Explicit protocol eligibility branch retains TCP exception and suppresses qualified QUIC duplicate observations. |
| C2 | Result provenance separates availability, freshness and dispatch eligibility; Complete alone is insufficient. |
| C3 | TCP/QUIC flows preserve their distinct eligibility; shared callback helper must not impose once-only semantics. |
| C4-F | Common evaluator/policy uses qualified provenance; protocol dispatch eligibility still preserves U05. |
| C4-P | Fact boundary must distinguish retained callback data from newly authoritative CH, without blanket TCP dedup. |

### S04: Zero-mask STOP

**Accepted behavior:** Stop current listener list; latch Stop, allow remaining applicable current-unit projections and permitted normal updates if final mask stays zero; no later unit.
**Authority:** P3 FB-C; P4 Section 4/T12/Q20. [E03,E05,E07-E08,E18,E25]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Use existing actual-mask reducer; protocol branch must not return before required current projections. |
| C2 | Conductor resolves current result then suppresses next pull; strategy receives no automatic replay request. |
| C3 | Flow uses common reducer but must not treat helper Stop as immediate cancellation of all projections. |
| C4-F | Coordinator retains current backing through remaining projections and common-policy disposition. |
| C4-P | Same coordinator behavior; facts remain live through permitted policy commit. |

### S05: STOP and violations; Continue with violations

**Accepted behavior:** Actual current-unit masks are ORed from zero. Immediate Error/Stop requires the **current projection verdict** to be Stop and the accumulator after OR to be nonzero. A previously latched Stop is not substituted for the current verdict in this immediate-return test. Continue with a nonzero mask suppresses normal commit; final disposition also accounts for the latched Stop. A later zero local mask cannot erase earlier violations.

| Ordered actual verdicts | Required current-unit consequence |
|---|---|
| Continue + mask, then Stop + zero | Immediate Error/Stop at the second verdict; suppress remaining projections. |
| Stop + zero, then Continue + mask | Do not return early at the Continue verdict. Run remaining applicable projections unless a later current verdict is Stop with accumulated mask; finalize Error/Stop. |
| Continue + mask, no Stop | Record accumulated violations, no normal commit, return Continue. |
| Stop + zero, final mask zero | Remaining applicable current projections and permitted normal commit; return Stop, no later unit. |

This distinction is directly checked in TLSFlowConductor::analyzeProcessResult/applyVerdict. Parser Stop, Unsupported, RST and lifecycle exceptions remain separate; no outcome implies automatic packet drop. [E18,E25]
**Authority:** P3 FB-C/FB-D; P4 Section 4. [E03,E05,E18,E25]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Conductor owns accumulator and Error/violation action; no path-local second reducer. |
| C2 | Strategy supplies findings, not synthetic observer verdicts; Conductor reduces actual callbacks. |
| C3 | Selected flow is sole reducer/writer; outer entry must not commit again. |
| C4-F | Coordinator reduces; common policy performs permitted Error/mask action only. |
| C4-P | Identical reducer/policy split; fact producer cannot directly mark Error. |

### S06: Terminal gating and late input

**Accepted behavior:** Preserve TCP silent/terminal payload routes. QUIC terminal-candidate classification precedes CH parsing; a qualified new terminal candidate requires the authorized reset before CH parsing, after required preparation. Same-attempt terminal, retired and ambiguous input cannot reopen state. S07 specifies the separate active/terminal sequences.
**Authority:** P3 FB-A/FB-E; P4 T10/Q10/Q19; P5 Section 5. [E03,E07-E08,E13,E18-E19]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Protocol branch precedes any shared terminal shortcut. |
| C2 | Strategy must qualify terminal candidates before returning a CH result; Conductor cannot universally skip all terminal input. |
| C3 | Each flow owns protocol gate; shared terminal predicate is not shared terminal traversal. |
| C4-F | Extraction qualifies terminal candidate first; Conductor/shared policy performs the authorized reset before common CH evaluation. Same-attempt terminal input remains gated. |
| C4-P | Protocol evaluator must not run before terminal qualification; common policy receives accepted replacement only. |

### S07: Complete new attempt and candidate failure

**Accepted behavior:** One active TLSData, with two distinct sequences:

- **Active candidate:** qualify and evaluate in isolated support while preserving unrelated active TLSData; accept/replace only after complete qualifying evidence and required admission, preservation and last uses. Failure before acceptance cannot overwrite or terminalize unrelated active authority.
- **Terminal candidate:** qualify accepted new-attempt evidence without CH parsing; satisfy required support admission, old terminal/retired identity preservation and last uses; authorize and perform the live reset through the candidate's sole semantic writer **before CH parsing**; only then evaluate the new CH and apply actual-feedback disposition. Do not postpone this reset until complete CH facts or callbacks, reset twice, or restore the old terminal attempt merely because post-reset CH evaluation fails.
- **Not a new attempt:** same-attempt terminal, retired or ambiguous evidence grants neither reset nor CH parsing permission. Qualification is not proof of server Retry/HRR. A required-support failure follows its accepted cause-specific disposition, not an invented client violation or automatic drop.
**Authority:** P3 FB-A; P4 D4-06,Q09/Q10; P5 failure matrix,D6-02/05. [E03-E04,E08,E11-E13]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Conductor is the sole writer: active candidate evaluates in isolated protocol support before acceptance; qualified terminal candidate is prepared and reset before CH parsing. |
| C2 | Strategy qualifies and supplies isolated active-candidate results; Conductor authorizes replacement. For a terminal candidate it must authorize/reset before asking the strategy for CH evaluation, not wait for a complete CH result. |
| C3 | QUIC flow coordinates isolated active-candidate evaluation/rollback. Its sole semantic writer performs the qualified terminal reset before CH parsing; table ownership remains unchanged. |
| C4-F | Common evaluator uses isolated active-candidate backing before acceptance. For a terminal candidate, Conductor invokes shared policy for the authorized reset before invoking CH evaluation. V3 Sections 3.3/3.5/6.2/8 make this explicit. |
| C4-P | Protocol evaluator prepares isolated active-candidate facts. Terminal qualification reaches Conductor/shared policy for reset before protocol CH evaluation; policy must not wait for complete CH facts. V3 preserves backing and single-writer rules. |

### S08: Ambiguous/retired evidence and expiry

**Accepted behavior:** No merge/reset from ambiguity; suppress qualified retired input. Missing/expired retired identity alone proves neither retired identity nor newness.
**Authority:** P3 FB-A; P4 Q06/Q19; P5 EV23 and Section 5. [E03,E08,E12-E13]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Keep classification in QUIC path; Conductor acts only on qualified authority. |
| C2 | Contract carries no-authority/ambiguous outcome without mapping it to fresh CH or reset. |
| C3 | QUIC flow keeps bounded identity state and no-merge rules; TCP flow does not inherit it. |
| C4-F | Adapters retain association policy; common semantics cannot infer newness from missing data. |
| C4-P | Same qualified-fact restriction; absence of facts is not a replacement event. |

### S09: Handshake completion and reclaim

**Accepted behavior:** Associated client v1 Handshake Long Header is accepted product completion without payload decryption. Before CH retain bounded evidence; permitted CH can commit InProgress then Finished in the same cycle. No completion callback; reclaim only newly committed Finished after last use.
**Authority:** P3 OP-08/L11,FB-B/FB-F; P4 Q11,G09; P5 2.4/5.0. [E01-E03,E06,E08,E10,E13,E18]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Conductor QUIC branch handles zero projections and ordered transitions; no single-commit restriction. |
| C2 | Common result contract represents completion evidence with no RecordParsed fiction; resolved commit gates next pull. |
| C3 | QUIC flow handles deferred evidence/commit and marking; outer entry cannot mark a second time. |
| C4-F | Common policy consumes qualified evidence under coordinator feedback; adapters do not invent Finished. |
| C4-P | Policy shares completion consequence while protocol path supplies associated/deferred facts; no CH evaluator migration is required for this rule. |

### S10: Resource/support exhaustion and required loss

**Accepted behavior:** Accepted required support loss is Unsupported, including PreHandshake, before same-unit dispatch; no synthetic client violation/drop. Optional retired-history expiry differs. Claim/mark failure is not success; no heap fallback.
**Authority:** P3 FB-D; P4 T09/Q16; P5 Section 4,D6-04/05/08. [E03,E07-E08,E11-E14]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Conductor maps qualified resource cause, not legacy partial-CH predicate blindly to QUIC. |
| C2 | Strategy distinguishes unsupported cause from parser Stop/NeedMoreData; common contract cannot hide failed admission. |
| C3 | Flow coordinates owned rollback and disposition; common helpers do not replace protocol cause qualification. |
| C4-F | Common policy receives qualified required-loss disposition; evaluator reset cannot silently discard active evidence. |
| C4-P | Fact extraction failure and semantic Unsupported remain separate; active core changes only under accepted disposition. |

### S11: Readable close versus opaque traffic

**Accepted behavior:** Associated Initial CRYPTO_ERROR close follows TerminatedByAlert; arbitrary/opaque closes do not. 0-RTT and short-header 1-RTT do not advance state. Opaque Handshake payload does not invalidate visible-header completion.
**Authority:** P3 semantic contract; P4 Q11/Q12/Q17/Q18; P5 2.4/7.2. [E01,E08,E10,E15]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Explicit protocol branch distinguishes close evidence, no-progress input and visible completion. |
| C2 | Result vocabulary must not normalize all encrypted input to one opaque/no-progress outcome. |
| C3 | QUIC flow retains close/completion distinction; shared alert helper accepts only qualified meaning. |
| C4-F | Adapters qualify distinct evidence; shared policy commits termination/completion only on accepted predicates. |
| C4-P | Common policy uses distinct facts, not TCP record types fabricated by evaluator. |

### S12: Additional CH, restart and TCP local guard

**Accepted behavior:** Keep TCP same-input MultipleClientHello guard; no QUIC datagram-local or attempt-wide max-CH parser gate. Additional qualified QUIC CH, including third CH, uses actual Enforcement feedback. Retry-compatible restart/HRR-compatible CH2 are same attempt, not observed server events.
**Authority:** P3 U05/FB-A; P4 T06/Q07/Q08/B01; P5 isolation policy. [E02-E04,E07-E08,E13,E22]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Protocol-local guard stays in TCP path; shared helpers do not reject QUIC by TCP packet history. |
| C2 | Strategy contract preserves protocol-local findings and same-attempt qualification without universal CH-count rejection. |
| C3 | Flows preserve their own input rules; shared CH helper must not contain hidden TCP Complete guard. |
| C4-F | Extract common CH semantics while leaving D27 guard and D28 visibility in TCP qualification. |
| C4-P | Protocol-owned evaluators retain local guards; common policy accepts only qualified additional-CH facts and actual feedback. |

### S13: Nested consumer reads and workspace reuse

**Accepted behavior:** Callbacks see precommit live TLSData. Callback wrapper is not deep-owned backing; full-string/digest/locator access survives nested reads and producer commit. Serial listener interval forbids invalidating reentry; no new locks/generation machinery is required for it.
**Authority:** P3 L01-L14/FB-F and consumer constraints; P4 Section 5/7; P5 D6-01/03. [E02-E03,E06,E09,E11,E13,E28]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Existing viewer access can remain; Conductor must bind correct path/provenance before dispatch. |
| C2 | Current strategy view must remain selected through all reads/commit; no next-result overwrite. |
| C3 | Flow may delegate callbacks synchronously but cannot let helper outlive its owned backing. |
| C4-F | Common evaluator backing is pinned through last use; moved locators/digest must retain correct owner. |
| C4-P | Protocol fact wrappers borrow real backing; policy separation alone gives no deep-copy guarantee. |

### S14: SYN/RST, aging and cleanup exceptions

**Accepted behavior:** Preserve TCP reuse/reset ordering and actual SessionEnd handling; resource pressure reclaim is not synthetic SessionEnd. Detach registrations/clear support safely before index reuse; failed detach cannot publish unsafe reuse.
**Current-code distinction:** TLSFlowConductor::onAging notifies SessionEnd for an existing extension. Independently, SegmentBufferManager::onEvent ages and deallocates TCP continuation slots. New owner-aware QUIC support/extension cleanup must not silently disable or reroute that independent legacy TCP path. Its implementation remains a target obligation, not an existing code fact. C1/C2 place new cleanup coordination in the Conductor, C3 in the selected flow, and C4 in the coordinator plus protocol resource owner; none duplicates legacy TCP cleanup or makes the evaluator a semantic writer. [E18,E19,E30,E26,E27]

**RST exception:** preserve actual TCPReset handling: Stop with a mask commits Error, Continue with a mask records violations, and the current zero-mask path clears TLSData. Do not replace this exception with a normal CH transaction or synthetic QUIC close. [E18,E26: D04]

**Authority:** P3 FB-D/FB-F; P4 T07/T10 and transaction exceptions; P5 D6-02/03 and event matrix. [E03,E05,E07,E11-E13,E18-E19]

| Candidate | Architecture-sensitive consequence / review target |
|---|---|
| C1 | Existing Conductor entry paths remain explicit; accepted cleanup corrections still required. |
| C2 | Common processing contract need not absorb Session lifecycle; Conductor coordinates strategy cleanup at safe boundary. |
| C3 | Flow-local lifecycle plus outer Session entry must have one cleanup authority, not duplicate detach/release. |
| C4-F | Coordinator handles lifecycle, common policy handles semantic reset, resource owners release only after last use. |
| C4-P | Same split with protocol evaluator cleanup; policy extraction must not leave stale reclaim or registrations. |

### S15: No-projection outcomes, captured evidence and plaintext gates

IGNORE, NeedMoreData and Unsupported remain distinct. IGNORE does not implicitly reset/finish an attempt; NeedMoreData retains only permitted bounded acquisition state and grants no premature CH authority; Unsupported follows its accepted no-consumer-dispatch path, not automatic malformed-client violation/drop. Evidence-only outcomes do not fabricate TCP record callbacks. [E26 Sections 3-6; E27; E29 C08-C09,C12-C15]

Capture projection-relevant pre-commit facts while backing is valid. A readable close before CH cannot be retroactively classified using a later CH/version commit. Early associated Handshake-header evidence is bounded deferred evidence, not early Finished authority. Preserve TCP shouldParseHandshake: plaintext handshake parsing requires PreHandshake/InProgress plus handshake content; ChangeCipherSpec completion uses accepted record/state evidence, not plaintext CH parsing. Keep the separate TCP TLS13/early-data ApplicationData rule. QUIC uses qualified Initial CRYPTO and associated client Handshake-header evidence, not those TCP predicates. [E18,E22,E26 D09-D13,D23-D24,D28; E27]

| Candidate | Required placement and review consequence |
|---|---|
| C1 | Explicit protocol branches qualify outcome and capture facts before Conductor commit. |
| C2 | Result contract distinguishes partial/no-projection/evidence-only outcomes; next pull cannot overwrite backing or reinterpret old facts from updated state. |
| C3 | Selected flow qualifies and captures before its own commit; outer entry does not commit again. |
| C4-F | Protocol extraction qualifies visible bytes before common staged evaluation; only common policy writes TLSData. |
| C4-P | Protocol evaluator captures qualified facts; common policy cannot infer fresh CH readiness from retained Complete. |

Participation remains protocol-specific, including QUIC Application Port group admission. Attempt/CID/protection/reassembly metadata and retired suppression are support state, not additional active TLSData contexts. Initial protection verification is not peer authentication. Protected reserved bits, packet-number length/number and short-header key phase require proper visibility/unprotection; do not conflate these with visible QUIC v1 Long Packet Type. C1/C2 keep support under protocol processing, C3 under the selected flow and C4 outside common semantic policy. [E27; E29 C02-C03,C17-C19]

## 5. Hard exclusions, trade-offs and downstream obligations

### 5.1 Reject incompatible realizations before considering advantages

These are rejected realizations, not claims that the current standalone candidate families require the mistake. A favorable score, simpler diagram or lower migration scope cannot compensate for a contract violation. No entire family is rejected because no definition forces one of these failures. [E01-E17,E27]

| ID | Incompatible realization | Reject because / authority | Candidate exposure |
|---|---|---|---|
| H01 | Treat every QUIC unit as a TCP record, CCS/reset or completion callback | Invented observation/event; E01,E08-E10 | C2 common contract, C4 adapters; also any C1/C3 helper that fabricates records |
| H02 | End all current-unit work on zero-mask STOP, or continue to later unit after latched STOP | Breaks actual feedback boundary; E03,E05 | Every coordinator/flow |
| H03 | Commit from strategy/adapter and again from Conductor/policy | Competing semantic authority; E06 | C2/C3/C4 and hybrids |
| H04 | Require one state assignment per input, or rollback earlier committed units after later failure | Rejects accepted deferred completion or per-unit authority; E05,E10,E13-E14 | Every candidate |
| H05 | Apply universal terminal skip before QUIC candidate qualification | Prevents accepted new attempt; E03,E08,E13 | C1/C2 shared loop and C4 common policy |
| H06 | Overwrite active backing during candidate evaluation or recycle borrowed result before last use | Violates isolation/validity; E02-E03,E11-E13 | Every candidate, especially shared workspace designs |
| H07 | Heap fallback, runtime container growth, unbounded context trials/retention, or arbitrary tail replay/cutoff | Violates accepted runtime/support policy; E13-E16 | Every candidate |
| H08 | Normalize Unsupported to malformed client violation/drop, or dispatch consumers before required-loss Unsupported | Violates cause and pre-dispatch contract; E03,E05,E12-E13 | Every result/flow/policy boundary |
| H09 | Require a complete TCP handshake buffer solely because a common evaluator accepts only whole messages | Conflicts with required staged-preservation boundary; E17,E20-E22,E26 | C4-F or over-generalized C2 |
| H10 | Infer a new attempt from missing retired identity/DCID change alone, or impose TCP local CH guard on QUIC | Changes accepted association/count policy; E04,E08,E12-E13 | Every common eligibility/evaluation boundary |
| H11 | Apply active-candidate evaluate-then-reset ordering to a qualified terminal candidate, or let an evaluator reset live TLSData | Terminal reset must precede CH parsing after required preparation; single semantic writer; S06/S07, E03,E08,E11-E13,E26-E27 | Every candidate; corrected C4 V3 makes the branch explicit |

### 5.2 Trade-offs, not hard failures

Extra protocol branches (C1), a common result contract (C2), flow-level policy duplication (C3), broad evaluator extraction (C4-F), and qualified-fact translation (C4-P) are trade-offs to justify with concrete scope. None is intrinsically incompatible. An interface can be allocation-free; a monolithic Conductor can still be incorrect. Fewer classes do not prove less work or safer ownership. [E18-E27; J]

### 5.3 Validation obligations are not missing semantic decisions

| Obligation | Required evidence / stage | Not claimed here |
|---|---|---|
| Actual API/provider and prepared-storage conformance | P5 V7-01/02/06/08 before affected integration/enablement; inspect selected ownership, registration, crypto contexts and rollback | No invented external API, success guarantee or allocation exemption |
| TCP/QUIC semantic and integration correctness | P5 V7-03..08/11; run S01-S15 traces, malformed/partial/duplicate/candidate cases and nested consumers | No packet tests or implementation PASS |
| Runtime allocation and bounded-work characterization | P5 V7-07/09/10; instrument provider/listener/diagnostic/aging paths, not just direct new/malloc | No throughput/latency SLA, fairness or platform-fit claim |
| Numeric capacities and successful-support effects | P5 U8-CAP/V7-12: architect/design approval after HLD presentation as authorized, finite prepared values before enabling runtime | No CPU/RAM budget demand, invented capacities or claim all values preserve traffic support |

Authority: E16. A concrete selected mechanism that cannot satisfy these obligations reopens that mechanism; deferral is not permission to hide unresolved ownership or unbounded work. [E11-E16]

## 6. Runtime and storage comparison without invented measurements

### 6.1 Shared unavoidable work

All candidates still perform accepted participation, protocol qualification, bounded reconstruction, CH evaluation where eligible, actual callbacks and lifecycle work. A known duplicate can bypass new CH evaluation only after trustworthy classification; decryption/range comparison may still be needed. Retired/ambiguous input is not automatically crypto-free. Prepared storage remains resident even on bypass. [E14-E15]

The inherited V2/P5 Section 6.3.1 symbolic ledger is: P = packets visited, F_r = frame/nested visits, N = unlocked logical messages, V = retained entries visited, X = overlap bytes compared, Y = copied/rebased bytes, H = CH bytes parsed/hashed, Z = locator/sort work, T = eligible context trials, K_d = key derivations, L = lifecycle entries, C_b = callbacks. Its symbolic model is:

**W = aP + bF_r + cN + dV + eX + fY + gH + hZ + W_crypto(T,K_d) + W_callbacks(C_b) + W_lifecycle(L).**

Coefficients and service costs are unmeasured. This is a dependency model, not a numeric time bound or a proof from finite packet size. Each candidate adds its actual dispatch/access/translation work to the same ledger; do not assume an added abstraction changes asymptotic work, and do not assume it is free. [E14 Sections 6.3.1/6.4]

| Candidate | Additional work to account for | Storage/liveness question | Validation consequence |
|---|---|---|---|
| C1 | Protocol branch selection and any helper access | Can current result backing be reused after last use? Protocol names alone do not require duplicate evaluator storage. | Count branches/copies and verify no hidden container allocation; E13-E15,E18-E24,E27 |
| C2 | Strategy dispatch, result/provenance adaptation and optional disposition bookkeeping | Borrowed current-result representation need not be an owned event queue; include actual strategy metadata. | Inspect exact contract and instrument dispatch/translation, not an assumed virtual-call penalty; E13-E16,E23-E24,E27 |
| C3 | Flow delegation and shared-helper calls | Long-lived flow metadata can be startup-resident; separate policy code does not require duplicate per-session semantic cores. | Verify one writer/cleanup owner and no per-input flow creation; E11-E16,E18-E19,E27 |
| C4-F | Qualified access, staged evaluator calls and any data/locator rebasing | Common evaluator may reuse workspace under serial last-use rules, but active/candidate backing can overlap. No automatic RAM saving. | Measure actual copies/repeated hashing and test mode restoration/borrows; E02,E11-E16,E20-E22,E27 |
| C4-P | Qualified-fact projection and policy calls | Protocol evaluator backing may be shared or distinct according to live intervals; facts may borrow rather than duplicate CH bodies. | Inspect fact provenance and actual simultaneous backing; smaller source refactor is not proof of lower memory; E02,E11-E16,E27 |

### 6.2 Replacement peak and boundedness

P5 also defines **M_prepared = F + sum_j(C_j * s_j + O_j)**, where C_j is startup slot capacity, s_j charged bytes per slot including inline metadata, O_j class-level overhead, and F fixed Engine-local backing not already charged to a class. Byte backing is charged once; class roles do not imply duplicate physical allocation. These are symbolic layout terms, not selected values or platform-fit proof. [E14 Section 6.3.1]

Preserve P5 admission condition for each prepared resource dimension j: **A_j + K_j + D_j + T_j <= C_j**, with checked arithmetic before mutation/publication. A_j is active usage including any still-live old attempt, K_j is isolated candidate usage, D_j is a separate destination and T_j is other concurrent temporary claims, exactly as defined in P5; do not add old active usage twice if already included. Use the original P5 definitions and accounting when instantiating a representation, not these labels as a new resource schema. [E14 Section 6.3.1]

Serial execution does not eliminate replacement peak: old active backing, isolated candidate and retained terminal identity can have overlapping last-use intervals. Conversely, separate logical roles do not prove two complete evaluator instances are needed. Each candidate must assign actual backing/claims to E11 ownership profiles and E14 bounds. Numeric C_j selection remains deferred; inability to make claims finite or rollback safe is an architecture issue now, not later tuning. [E11-E14,E16]

No arbitrary work cutoff, replay queue, new global Unsupported or automatic drop is added to improve an estimate. Boundedness must come from the accepted input/retained-set progress and checked capacities. If an actual implementation needs a new cutoff/support policy, that requires explicit approval rather than a favorable comparison score. [E14 Section 6.4]

## 7. Abstraction necessity and merge-before-split

| Proposed responsibility/boundary | Current concrete benefit | Simpler merge option | Keep separate only when | Evidence |
|---|---|---|---|---|
| C1 QUIC processing responsibility | Protection, packet/CRYPTO parsing and association differ from TCP records | Implement focused methods/helpers under existing processing ownership; no universal interface needed | Protocol state/lifetime separation or focused testing warrants an object | E01,E08,E11-E13,E22,E27 |
| C2 common processing contract | One pull/eligibility/lifetime boundary for two paths | Explicit C1 calls can avoid contract adaptation if they preserve clarity | Actual outcome/borrow vocabulary stays coherent and the common loop/test seam pays for migration | E02-E06,E23,E27 |
| C3 protocol flow | Owns complete protocol sequencing and exceptions | Keep branches in Conductor if a flow only forwards calls | It owns meaningful traversal/feedback/lifecycle decisions, not a naming wrapper | E05-E08,E18-E19,E27 |
| Common verdict reducer | One actual-mask/STOP algorithm | Keep a shared function in existing Conductor/utility; no VerdictManager | Independent ownership/lifecycle is demonstrated; none is established here | E05,E18,E25 |
| C4 common state policy | One implementation of shared semantic actions and applicability | Extract functions/composition in existing module, not automatically StateMachine class | State access/test boundaries require an object; no competing writer is created | E06,E19,E26-E27 |
| C4-F common CH evaluator | Same fields/fingerprint algorithm across TCP/QUIC | C4-P can share algorithm helpers while retaining staged protocol evaluation | Qualified staged access and actual migration benefit justify moving evaluator orchestration/backing | E01-E02,E09,E13,E21-E22,E27 |
| Qualified-fact/access boundary | Separates protocol evidence from common policy without fake records | Borrow existing viewer fields with explicit provenance if sufficient | A new representation solves actual lifetime/applicability needs without duplicate CH copies | E02,E06,E09,E11,E27-E28 |
| Global allocator/lifecycle service | No new current ownership benefit established | Existing table/Processor/managers keep accepted owners and prepared claims | Concrete missing ownership responsibility appears; not selected now | E11-E13 |

### 7.1 Template Method, correctly understood

The user clarification is adopted: Template Method means a shared algorithm invoking virtual hooks, potentially pure virtual in derived protocol implementations. It is not C++ template programming. The relevant comparison is whether inheritance is useful for an actual stable algorithm, not whether generics are useful.

The ordinary transaction has a stable reducer/order [E03,E05,E18,E25]. That supports a shared function or composed coordinator. It does not by itself establish that the entire packet/session lifecycle is one uniform overridable skeleton: reset, parser Stop, terminal payload, aging, active/terminal candidates and no-projection completion differ [E05,E07-E08,E12-E13]. Exceptions do not make Template Method impossible; they limit which skeleton could be justified.

A narrow Template Method for a revised processing strategy could be considered under C2 if source-level design demonstrates stable hook semantics and benefit over composition. It must not let hooks bypass feedback, mutate TLSData independently or hide ownership. No such need is established by the inspected existing IProcessor/Processor/Conductor split [E18-E25]. Therefore inheritance is not required or ranked as a separate architecture here; this is a proportionality judgment, not a technical rejection of the pattern. [E17,E27; J]

## 8. Trade-off synthesis and Step 5 decision input

- **C1:** preserves explicit existing control and can avoid a new common processing contract, but grows Conductor branching and leaves shared-policy intent less isolated. Select it only with a concrete responsibility map showing branches remain coherent; do not call it zero TCP change. [K02,K05,K08; E18-E24,E27; J]
- **C2:** provides a common processing test/iteration seam at the cost of a new outcome/provenance/lifetime contract and TCP adaptation. Its value must exceed that migration cost; a record-shaped universal result is not an acceptable shortcut. [K01-K03,K05,K07-K08; E01-E06,E23,E27; J]
- **C3:** keeps protocol sequencing cohesive and can preserve the TCP Processor contract, but state-policy sequencing can diverge. Shared helpers alone do not answer which semantic decisions are duplicated and why. A flow/common-policy hybrid is legitimate if it has one writer, not an extra independent family by name. [K02-K05,K08; E05-E08,E18-E19,E27; J]
- **C4-F:** maximizes common evaluator/policy responsibility among the definitions and directly investigates moving TCP state logic. It also touches staged parser/digest/viewer integration. Neither common semantics nor fewer source copies proves a runtime or delivery advantage. [K04-K08; E02,E06,E13,E19-E22,E27; J]
- **C4-P:** tests common policy with a narrower evaluator migration. It must still justify fact provenance/access and same-CH output equivalence. It is not automatically a separate architecture from C1/C2, nor automatically the cheapest or best option. [K02,K04-K08; E01-E02,E06,E13,E19-E22,E27; J]

Step 5 must choose a concrete combination, identify retained/changed/moved/extracted symbols, justify every intentionally separate semantic decision and define the minimum coherent delivery scope. The present evidence does not force full CH evaluator extraction, a new class per logical role, an inheritance pattern or a numeric capacity choice. It does force accepted state/feedback/lifetime/boundedness contracts. No winner is declared here. [E11-E17,E26-E27]

## 9. Evidence register and source integrity

**Current provenance (V7):** E01-E17 are inherited accepted-contract topics resolved through B6 Section 5 and B7 Sections 3-6, with candidate-specific detail in B1-B4. Original P3/P4/P5 locators below are historical navigation retained from V2, not a new original-archive audit. E18-E24/E25 code behavior is checked against B5; E28 consumer evidence is inherited through B6 consumer excerpts/B7, not a fresh second-bundle inspection. E26/E27/E29 use the current inputs below. Any older candidate-section number in retained text is superseded by Section 2's B1-B4 map.


Historical original-document locators below are retained only as inherited navigation through Step 1/2. Current B5 code locators use the supplied bundle; no repository commit or fresh second consumer-bundle audit is inferred. Direct current-code inspection is focused on architecture-sensitive transaction/state, processing/visibility and lifecycle paths, not every parser line or historical test.

| Evidence | File / anchor | Original lines | Role |
|---|---|---:|---|
| E01 | `phase3-semantic-processing-contract-review-v5.md`: 4. Semantic processing contract | 3246-3630 | Semantic operations and supported findings |
| E02 | `phase3-semantic-processing-contract-review-v5.md`: 5. Cardinality, freshness, ownership and lifetime | 3631-3937 | L01-L14 validity, provenance and cardinality |
| E03 | `phase3-semantic-processing-contract-review-v5.md`: 6. Required feedback boundaries | 3938-3953 | FB-A-F association, STOP and last use |
| E04 | `phase4-runtime-orchestration-summary-v4.md`: 3. Consolidated decisions | 34-56 | D4-01..12 accepted behavior |
| E05 | `phase4-runtime-orchestration-summary-v4.md`: 4. Shared transaction and exceptions | 57-87 | Actual verdict reduction and Unsupported pre-dispatch exclusion |
| E06 | `phase4-runtime-orchestration-summary-v4.md`: 5. TLSData authority, timing and gate ownership | 88-117 | R7 writer, field timing, G01-G09 |
| E07 | `phase4-runtime-orchestration-summary-v4.md`: TCP flow catalogue | 122-271 | T01-T12 preserved TCP scenarios |
| E08 | `phase4-runtime-orchestration-summary-v4.md`: QUIC flow catalogue | 272-540 | Q01-Q22 accepted QUIC scenarios |
| E09 | `phase4-runtime-orchestration-summary-v4.md`: 7. Consumer compatibility and common boundary | 541-556 | Actual consumer capability limits |
| E10 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md`: 2.4 Binding Finished Policy Inherited from Phases 2-4 | 83-96 | Accepted completion and reclaim |
| E11 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md`: 3. Resource Inventory and Ownership/Lifecycle Matrix | 97-318 | Sixty resource facets, logical and physical owners |
| E12 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md`: 4. Lifecycle/Event and Allocation-Failure Outcomes | 319-374 | Event/failure dispositions, rollback and context loss |
| E13 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md`: 5. Isolation, Candidate Replacement, Observer Lifetime and Reclaim | 375-474 | Ownership, completion association, D6-01..08 mechanisms |
| E14 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md`: 6. Bound Register and Bounded-Work/Storage Model | 475-573 | Checked bounds, aggregate storage/work and work-tail |
| E15 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md`: 7. Copies, Allocations, Crypto Costs and Fast Bypass Paths | 574-602 | Cost ledger and trustworthy fast bypass |
| E16 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md`: 9. Decisions, Numeric Deferrals and Runtime Feasibility/Validation Obligations | 629-804 | U8-CAP and V7-01..12 downstream validation |
| E17 | `phase5-ownership-lifecycle-runtime-bounds-summary-v7.md`: 10.2 Exact overall Phase 6 definition | 833-882 | Four candidates, criteria and eight exit gates |
| E18 | `TLS-Flow-All-Code.txt`: packet-flow/src/TLSFlowConductor.cpp, extracted L34-362 | 3353-3681 | Entry, stepping, dispatch, verdicts, reclaim and aging |
| E19 | `TLS-Flow-All-Code.txt`: packet-flow/src/TLSFlowConductor.cpp, extracted L421-580 | 3740-3899 | Participation and state helpers |
| E20 | `TLS-Flow-All-Code.txt`: processor/src/Processor.cpp, extracted L50-267 | 5404-5621 | Packet/continuation entry and result translation |
| E21 | `TLS-Flow-All-Code.txt`: processor/src/Processor.cpp, extracted L400-480 | 5754-5834 | Staged CH extension continuation, digest and fallback |
| E22 | `TLS-Flow-All-Code.txt`: processor/src/Processor.cpp, extracted L810-1065 | 6164-6419 | Record framing, opaque/handshake gates and local CH guard |
| E23 | `TLS-Flow-All-Code.txt`: processor/include/IProcessor.hpp, extracted L17-60 | 4434-4477 | Existing TCP-oriented processing interface |
| E24 | `TLS-Flow-All-Code.txt`: context/engine/src/Factory.cpp, extracted L21-113 | 1487-1579 | Initialization and shared Processor/viewer wiring |
| E25 | `TLS-Flow-All-Code.txt`: packet-flow/src/TLSFlowConductor.hpp, extracted L95-122 | 4001-4028 | Ordered live-listener notify and STOP |
| E26 | B7 Step 2 V3, Sections 3-7 and D01-D28 | Current baseline | Responsibility/semantic-sharing assessment, not architecture selection |
| E27 | B1-B4 current standalone candidate proposals; Section 2 maps placement | Current baseline | Candidate definitions; older summaries do not override them |
| E28 | `TLS-Consumers code.txt`: Supplied fragments; repository paths not inferred | 1-706 | Enforcement, Fingerprint and Visibility callbacks; downstream completeness not inferred |
| E29 | B6 Step 1 V2, especially Section 5 C01-C30 and Section 7 | Current baseline | Accepted constraints and evidence limits |
| E30 | B5 SegmentBufferManager::onEvent/::deallocate and TLSFlowConductor::onAging | Exact symbols in current bundle | Independent TCP continuation aging versus SessionEnd notification |

### Current seven-input SHA-256 inventory

| ID | Exact input | SHA-256 of original bytes |
|---|---|---|
| B1 | `candidate1-team-architecture-proposal-v5.md` | `0f964d95ee848ec4b6d825674d0cff5acb6925258264f658f812bea1ec7b692a` |
| B2 | `Candidate2-V2.md` | `71054a49bab1974f766ba2a8297cd10ce13cd786da5073b11c0bc07627cd112b` |
| B3 | `candidate3-team-architecture-proposal-v4.md` | `a2249fb1792c9096512e35adf0d70d67709a398fcc63a0de2b76c21c6a7d5ce1` |
| B4 | `candidate4-team-architecture-proposal-v3.md` | `77f6984d4ac1a78a0855e0ed1e844af265611c48226f311fbda512b80cff1c4f` |
| B5 | `TLS-Flow-All-Code.txt` | `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b` |
| B6 | `phase6-step1-authoritative-comparison-baseline-v2.md` | `836528e0aa952f498dd4ad9b744bb8ea6bc0fd11411965b4ad0c06344e136d94` |
| B7 | `phase6-step2-responsibility-and-semantic-sharing-assessment-v3.md` | `9f9721178ab0ac336949b06fb3cab5a28f3f507d634999d9ac01988561d8341f` |

Immediate immutable revision input: `phase6-step4-candidate-comparison-v6.md`, SHA-256 `01cf4fb9de912b493a743e41aedb2f25e23db3595d9d4a7a08e744ba3766adc9`. Historical V2 ancestor: `phase6-step4-candidate-comparison-v2.md`, SHA-256 `952d09e6072f8edb741d6691a11faec1e814dfdc9bda78b14f2b18efac75ea94`. No baseline input is modified.

## 10. Completion and review boundary

No architecture is selected. The comparison retains all four standalone families and both C4 evaluator scopes for a subsequent explicit decision. Contract-breaking realizations are rejected in Section 5; a possible implementation mistake is not proof that an entire candidate definition is incompatible. [E17,E27,E29]

The accompanying `phase6-step4-v7-review-and-double-check.md` records the V7 provenance corrections and the checks actually executed against the final document digest. Historical V6 review results are retained as historical evidence, not represented as newly executed V7 tests. Static document consistency is not implementation/runtime proof, compilation, packet-trace validation, platform-fit approval or numerical sizing approval. No claim extends beyond the supplied seven inputs and their inherited accepted-contract boundary.

Step 1 C01-C30 and Step 2 D01-D28 remain binding in full. Sections 1-3/5-8 cover comparison scope, proportionality, ownership, runtime and deferrals. S01-S03/S12 cover staged/fresh CH and TCP local guards; S04-S05 cover actual feedback; S06-S09 cover terminal/attempt/deferred completion; S10-S11 cover loss and close; S13-S14 cover lifetime/lifecycle; S15 explicitly covers no-projection outcomes, captured evidence, participation and D28 plaintext eligibility. These are navigation pointers, not replacement requirements.

## 11. V6 correction and review boundary

V4 and V5 are preserved unchanged. V6 also qualifies the Section 2 post-feedback statement so it cannot contradict the terminal pre-parse reset. The concrete failure was Candidate 4 V1 Section 3.3's unqualified evaluate-then-accept/reset sequence: it did not enforce the inherited terminal-candidate reset-before-CH-parsing contract. Candidate 4 V3 is the corrected comparison input for both C4-F and C4-P; V1 is not retrospectively approved. S06/S07 and H11 now expose this distinction across all five compared alternatives. The correction changes ordering, not the accepted attempt classifier, completion signal, feedback reducer, ownership model or deferred numeric capacities.

Static document review and a separate consistency pass cover source identity, candidate normalization, K01-K08, S01-S15, hard exclusions, runtime/storage reasoning, sharing and pattern judgments, evidence provenance and handoff boundaries. Historical V6 review evidence and hashes are external in `phase6-step4-v6-review-and-double-check.md` and `phase6-step4-v6-manifest.json`. The current V7 correction review and hashes are in `phase6-step4-v7-review-and-double-check.md` and `phase6-step4-v7-manifest.json`. The historical V6 passes and current V7 checks are not implementation tests, independent human approval or proof of runtime behavior. No architecture is selected and Step 5 is not rewritten.

## 12. V7 provenance correction and review boundary

V6 and all prior inputs are preserved unchanged. V7 corrects only revision identity, immediate-parent provenance and current review/manifest references. Sections 2-8, the E01-E30 evidence table, the seven-input inventory and the Candidate 4 V3 terminal-reset correction are unchanged. No architecture is selected or reranked. The accompanying V7 report distinguishes newly executed correction/integrity checks from historical V6 review evidence; it does not claim that the historical 45-check suite was rerun.
