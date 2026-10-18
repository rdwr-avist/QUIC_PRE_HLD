# TLS-Flow Architecture with QUIC: Pre-Design Work Plan

## 1. Purpose

Determine the appropriate architectural boundary between transport/protocol-specific TLS extraction and transport-independent TLS semantic processing, including whether the existing TCP/TLS state machine can be reused for QUIC and what refactoring of the current Processor/Conductor is justified to establish that boundary.

Reach a justified **READY FOR HLD** decision for the appropriate TLS-Flow architecture after adding QUIC, reusing existing TLS semantics where meaningful while preserving intended TCP-TLS behavior. The existing `Processor` and `TLSFlowConductor` are investigation starting points, not predetermined final component boundaries. They may remain, be decomposed, or be reorganized when the current feature justifies the change.

This is a decision-making plan, not an implementation plan. Its outcome must be specific enough to write the High-Level Design without reopening fundamental questions about responsibilities, runtime control flow, state ownership, or protocol contracts.

The objective is to deliver the required feature with sound engineering and proportionate changes. Maximum reuse, architectural symmetry, and future extensibility are not objectives in themselves.

## 2. Scope and Authority

### In scope

- Processor and Conductor responsibilities and their interaction.
- TCP-TLS behavior affected by the proposed extension.
- QUIC Initial processing, authenticated frame traversal, CRYPTO reassembly, and ClientHello completion.
- Shared ClientHello parsing and fingerprint evaluation.
- Observer dispatch, verdict handling, state transitions, terminal processing, and cleanup within TLS-Flow.
- Mutable state isolation, resource lifecycle, runtime costs, and regression validation.
- External contracts only where they affect the above decisions.

### Out of scope

- Redesigning baseline learning, RTS infrastructure, configuration, reporting, or statistics.
- Implementing mitigation packet generation.
- Implementing DTLS or a generic UDP/TLS framework.
- Implementing a full QUIC endpoint or decrypting later encryption levels without a current requirement.
- Writing implementation code during pre-design.

Feasibility analysis may identify observations or processing at later encryption levels needed for an architectural capability. This analysis does not authorize their implementation or make them prerequisites for current-release delivery unless an applicable requirement requires that behavior.

An external concern is brought into scope only if it changes a TLS-Flow contract. For example, a consumer's need for connection context may affect the result contract even though its mitigation implementation remains out of scope.

### Source discipline

- Read FRD and SRS together and identify the requirements relevant to this scope.
- Treat the SRS as describing the current QUIC release direction, without silently deleting conflicting FRD requirements or claiming that the SRS automatically overrides them.
- Resolve or obtain an explicit scope disposition only for conflicts that affect Processor/Conductor design. Record unrelated conflicts outside this workstream; do not let them consume this plan.
- Existing code is evidence of current behavior, not a prohibition against refactoring.
- Previous research reports are investigation leads, not independently verified code facts.
- The working decision log records current direction and may be revised with explicit reasoning.
- Label statements as requirement, verified code fact, user-confirmed runtime fact, proposal, accepted decision, or unresolved question.

## 3. Engineering and Delivery Guardrails

### KISS

- Prefer the smallest coherent design that meets current requirements.
- Keep responsibilities and ownership explicit, but do not create a component for every responsibility.
- Prefer private methods or existing components when a new class or interface provides no concrete benefit.
- Accept small, harmless duplication when eliminating it would create an artificial abstraction.
- Do not force TCP Record iteration and QUIC packet/frame iteration into an identical loop.

### YAGNI

- Build only abstractions justified by the current TCP and QUIC paths.
- Use DTLS as a lightweight boundary sanity check, not as a source of speculative requirements.
- Do not require Strategy, inheritance, Template Method, a separate dispatcher, a separate state-machine component, or protocol-flow classes in advance.
- Do not add events, states, stores, or APIs solely because a future feature might need them.
- Preserve extension seams where inexpensive, but do not implement future functionality.

### Delivery discipline

- Every proposed refactor must identify its current benefit, affected behavior, regression exposure, and validation method.
- Preserve intended TCP behavior. Suspected defects are investigated and dispositioned separately; do not silently fix them or automatically turn them into permanent compatibility requirements.
- No synthetic TLS Records for QUIC.
- A common state model does not imply identical protocol triggers or identical observability.
- First investigate whether existing TLS semantic state processing can be preserved across TCP and QUIC. Do not reduce QUIC to ClientHello extraction by default.
- Before rejecting a shared transition or state, identify its intended meaning, required observation, possible QUIC sources, and current-scope availability. Support the conclusion with evidence or explicitly mark the question unresolved.
- Do not fabricate completion evidence, relabel extraction completion as handshake completion, or require a shared state machine to win regardless of correctness and delivery cost.
- Do not interpret parsing failure or unsupported input as a network drop unless an applicable requirement and consumer contract justify that action.
- Bound processing and storage. Do not invent numeric limits.
- Use one evolving decision package, not a collection of unnecessary standalone documents.
- When investigation expands, ask whether it changes a current feature decision. If not, stop and record it as deferred.
- Time-box investigation by agreement with the feature owner. Time pressure may justify a simpler supported design, but not an unsupported PASS or an unresolved architectural blocker.

## 4. Phase Review Procedure

For each phase:

1. Perform the focused work and record evidence.
2. Review the output for correctness, relevant requirement coverage, consistency, KISS, YAGNI, and delivery impact.
3. Evaluate every exit-gate condition explicitly.
4. Mark the phase **PASS** only when all conditions are supported; otherwise mark it **BLOCKED** and identify the missing evidence or decision.
5. Advance only after PASS. If a later decision invalidates an earlier output, reopen only the affected decisions and gates, then revalidate downstream consistency.

An accepted decision records its rationale, supporting evidence, relevant requirement references, and validation method. Unknowns record their impact and whether they block HLD.

---

## Phase 1: Establish Current Behavior and Required Change

### Work

- Map the existing TCP runtime path from Conductor input through Processor results, observer dispatch, verdict accumulation, state commits, and cleanup.
- Inspect only the code necessary to establish the affected contracts. Use the supplied methods as the starting point and request missing evidence selectively.
- Identify initialization, continuation, viewer reset, terminal paths, and interleaved-session state.
- Preserve the actual STOP semantics, including deferred STOP without a violation and immediate STOP with accumulated violations.
- Map relevant FRD/SRS requirements to TLS-Flow responsibilities and acceptance scenarios.
- Identify external assumptions that could change the design, including first-packet delivery, UDP payload boundaries, session identity, and extension reclaim semantics.

### Decisions and output

A concise current-behavior map and scoped requirement map, identifying:

- Behavior that must be preserved.
- Current implementation coupling that may be changed.
- Suspected defects requiring separate disposition.
- External verification items and their architectural impact.

### Exit gate

- The affected TCP control flow and Processor/Conductor feedback points are understood.
- Observer order, verdict accumulation, STOP, and normal commit behavior are documented accurately.
- Relevant requirements are mapped; no requirement is silently omitted or reinterpreted.
- Unknowns are explicit, and none prevents defining the candidate runtime behavior in Phase 2.
- No broad codebase investigation or unrelated feature work has been added without a demonstrated need.

---

## Phase 2: Investigate Shared TLS State Semantics and Define QUIC Behavior

### Work

For each existing state, define its meaning, processing obligations, outputs, transition triggers, and retained state:

- `PreHandshake`
- `InProgress`
- `ChangeCipherSpec`
- `Finished`
- `Error`
- `Unsupported`
- `TerminatedByAlert`

Do not assume that the existing `hsState` enum is the correct abstraction merely because it exists. Determine whether its state meanings can be preserved across protocols while replacing protocol-specific triggers with justified semantic observations. Distinguish directly observed TLS facts, protocol-specific heuristics, and feature-processing lifecycle states. Identify which existing fields and transition rules should remain shared, be extracted or generalized, remain protocol-specific, or be revised.

Distinguish five independent levels of reuse:

1. **Shared state semantics:** the meanings of states.
2. **Shared transition policy:** the rules and evidence that justify transitions.
3. **Shared state representation:** enums, fields, and data structures.
4. **Shared mutable storage:** the actual instances holding runtime state.
5. **Shared implementation:** reusable code implementing evaluation or transitions.

Do not treat reuse at one level as evidence that all five should be shared. The same class or implementation may operate on separate per-session or per-protocol instances. Shared semantic processing does not imply a single mutable state-machine object shared across sessions.

Sharing an enum is not evidence of sharing state semantics or transition policy. For each state and transition, distinguish its intended meaning from its current wire-level trigger. Do not require a new event type, interface, or state-machine class for every distinction; prefer the simplest implementation boundary justified by the current feature.

Investigate preserving a shared semantic state model before proposing separate protocol state policies. Determine which states are shared and which remain TCP-specific within the shared model. Cover ClientHello completion, incomplete input, duplicates, a possible second ClientHello, and a new connection attempt on an existing session.

Separate TLS handshake completion, observed progress, and completion of this feature's processing. Do not conflate them.

### Decisions and output

A state-transition matrix and concise per-state TCP/QUIC flows, including:

- Determine whether the architecture can support the same TLS semantic state progression for QUIC, including `Finished`, and identify what additional observations or protocol processing would be required. Distinguish architectural capability from current-release implementation scope; do not bring later QUIC encryption levels into implementation scope unless an existing requirement explicitly requires them.
- Distinguish observing a TLS Finished message, inferring handshake progress, and completing feature-specific processing. Identify required observations and their availability; do not assume that supporting a state requires decrypting the corresponding TLS message.
- Decide which transitions are justified by observations available in the current release. Architectural support for a future transition does not authorize performing it now without sufficient evidence.
- For each proposed rejection of shared state behavior: the required observation, candidate QUIC source, supporting evidence, and why it is unavailable, inappropriate, or outside the authorized current scope.
- Unknown availability is not evidence of impossibility. Resolve architecture-sensitive unknowns before selecting the architecture.
- What processing remains after completion.
- The current-release policy for additional ClientHellos and attempt reuse.
- The owner of each processing gate and transition decision.

### Exit gate

- Every supported transition has an explicit trigger and meaning.
- The existing enum, fields, and transition rules have been evaluated as abstractions, not accepted merely because they exist. Shared state meanings, protocol-specific heuristics, and feature-processing lifecycle are explicitly distinguished.
- The five levels of reuse are evaluated separately; no shared mutable storage or new state-machine object is implied merely by shared semantics or code.
- Current-release transitions are distinguished from architectural capability. Later encryption-level processing is not introduced as an implementation obligation without an applicable requirement.
- States not used by QUIC are identified without forcing artificial equivalents.
- A no-transition or protocol-specific policy is accepted only after the shared semantic alternative has been investigated and its rejection is supported. An unverified assumption does not pass this gate.
- The investigation is bounded to the observations needed by current behavior and requirements; it does not expand into a full TLS/QUIC implementation.
- Duplicate, second-ClientHello, and attempt-reuse behavior is sufficiently defined for the current release; unsupported cases have an explicit requirement-compatible disposition.
- No unresolved state question prevents defining the result contract.
- No extra state or lifecycle mechanism has been introduced solely for hypothetical future features.

---

## Phase 3: Research Semantic Equivalence and Define the Processing Contract

### Work

Build an explicit equivalence matrix for every TLS semantic input, observer-relevant fact, and state transition used by the affected TCP path. Record:

- Existing TCP source and symbol, current wire trigger, and intended semantic meaning.
- Candidate QUIC source and the extraction/reassembly needed to obtain it.
- Classification as shared, QUIC-applicable with a different trigger/source, TCP-specific, not applicable to the current QUIC scope, or unresolved; additionally record whether the semantic meanings are identical, only analogous, or different.
- Whether the representation differs while the semantic information is the same.
- Whether the current TCP trigger is a heuristic rather than a directly observed TLS message.
- Evidence, required processing cost, and implications for sharing.

Do not assume one TLS Record corresponds to one QUIC packet or one semantic observation. Investigate whether the semantic transaction currently organized around a Record can be generalized independently of wire framing.

Distinguish:

- Processing progress and whether more input or another processing step is needed.
- Newly produced TLS observations, including a newly completed ClientHello.
- Protocol-specific information required by current consumers or state policy.
- Parsing failures, unsupported input, and resource-limit outcomes.
- Observer verdicts, which are not parser outcomes.

Define whether a processing operation can produce zero, one, or multiple observations and where processing can pause for Conductor feedback.

Define result validity, buffer ownership, and lifetime. Distinguish an available ClientHello result from a new completion event.

### Decisions and output

An evidence-backed equivalence matrix and a semantic contract describing inputs, outputs, result lifetime, and protocol-specific extensions. The contract is not required to remain between the existing Processor and Conductor classes; final placement is decided in Phase 6. Decide whether Processor reads `TLSData`, which fields it uses, and whether access is read-only.

Do not create a new context interface merely to hide unused fields if passing existing `TLSData` is sufficient.

### Exit gate

- Every affected TCP semantic input and transition must be classified as shared, QUIC-applicable with a different trigger/source, TCP-specific, not applicable to the current QUIC scope, or unresolved. Each classification must distinguish semantic meaning from wire representation and include supporting evidence or an explicit scope justification. No artificial equivalence is required.
- TCP-specific means that the relevant meaning or mechanism is specific to the TCP path. Not applicable to the current QUIC scope means that relevant information may exist in QUIC but is not acquired or used by this release. A Record-based implementation alone does not justify a TCP-specific classification.
- Unresolved classifications are allowed during investigation, but any classification affecting architecture, contracts, state policy, or required behavior must be resolved before READY FOR HLD. Unverified items are not labeled unavailable.
- Both TCP and QUIC behavior can be expressed without fabricated Records or lost information.
- Result cardinality, freshness, lifetime, and ownership are explicit.
- Protocol-specific outcomes are not misleadingly presented as equivalent.
- The contract supports required STOP and state-feedback points.
- No result type, event, or API exists solely for symmetry or speculative extensibility.

---

## Phase 4: Define Orchestration, Dispatch, and State Commit

### Work

- Define initialization per TCP packet or UDP datagram.
- Assign iteration over TLS Records, QUIC packets, authenticated frames, and CRYPTO ranges.
- Specify when the Conductor regains control and whether it can stop further processing.
- Preserve TCP observer order and verdict transaction semantics.
- Separate protocol-specific interpretation from shared ClientHello facts, violation commits, and justified state updates.
- Decide where terminal gates, normal commits, and reclaim requests occur.
- Keep observer lists on `Extension`; do not conflate them with `TLSData`.

### Decisions and output

Complete TCP and QUIC runtime flows, with responsibilities assigned to logical roles rather than prematurely fixed classes.

Decide whether a common orchestration interface is useful and at what granularity. A common interface must not require a common inner loop.

Check whether the proposed semantic boundary can serve the existing TLS-Fingerprint, TLS-Visibility, and TLS-Enforcement consumers without exposing unnecessary wire framing. Inspect only consumer contracts needed to establish that boundary, within the agreed code-reading scope. Mark external contracts unverified where access is not authorized.

For TLS-Enforcement, distinguish currently required QUIC behavior from future reuse of the semantic boundary. Verify which existing enforcement concepts can consume shared semantic facts without wire-format knowledge, and which require explicit protocol context, observation capabilities, or protocol-specific actions. Do not expand current implementation scope.

Do not implement future QUIC Enforcement or new consumer functionality. This is a boundary compatibility check: identify reusable facts, observation limitations, protocol-specific context, and avoidable coupling. A shared semantic boundary does not require all consumers or enforcement actions to be protocol-agnostic. Use a lightweight future DTLS integration walkthrough to identify avoidable hard-coding, without defining a DTLS implementation.

### Exit gate

- Every input has a defined path through processing, dispatch, commit, continuation, and return.
- STOP with and without violations preserves the intended transaction behavior.
- `TLSData` read/write authority and state-update timing are explicit.
- Gates are not duplicated with contradictory policies.
- Shared operations and protocol-specific interpretation are clearly separated.
- Compatibility with existing feature-consumer contracts has been evaluated; speculative functionality is excluded and architecture-sensitive external contracts are resolved.
- Enforcement compatibility distinguishes shared semantic facts from protocol context, observation capabilities, and protocol-specific actions. Future reuse does not become a current implementation requirement.
- A common semantic transaction has been considered explicitly, not dismissed solely because TCP and QUIC have different wire iteration.
- No class split is required merely to make the diagrams look uniform.

---

## Phase 5: Define Ownership, Lifecycle, and Runtime Bounds

### Work

Map each mutable state item and resource:

- Per-session `TLSData` and Extension observer lists.
- Engine-local parser, evaluator, scratch, and viewer state.
- TCP continuation storage.
- QUIC CRYPTO reassembly and required Initial protection context.
- Completion and duplicate-tracking information.
- Any connection-attempt identity needed by the selected current-release policy.

Define creation, reset, access, release, aging, eviction, and allocation-failure behavior. Validate isolation across interleaved sessions and protocols.

Identify processing and storage bounds, fast bypass paths, copies, allocations, and crypto costs. Reuse infrastructure patterns only where their actual contracts fit.

### Decisions and output

An ownership/lifecycle matrix and bounded-work model. Introduce an attempt store or another component only if the selected behavior actually requires it.

Numeric values may remain for HLD only when their selection cannot change architecture, ownership, supported behavior, or the feasibility of meeting requirements. Otherwise they are blocking decisions.

### Exit gate

- Every mutable state item and resource has an owner, scope, and lifetime.
- No cross-session or cross-protocol state leakage is left unaddressed by the chosen design.
- Incomplete-state eviction, aging, allocation failure, and late arrivals have defined outcomes.
- Required bounds and enforcement locations are specified; architecture-sensitive values are resolved.
- The design does not allocate or retain state without a current need.
- Runtime feasibility is supported by available evidence; unmeasured costs are labeled and assigned a validation method.

---

## Phase 6: Compare and Select a Proportionate Architecture

### Work

Compare at least four concise, genuinely different candidates:

1. A shared Conductor with separate protocol processing paths and composition.
2. Processor strategies behind a revised contract, with shared behavior through composition or a justified base.
3. Protocol flows below a shared Conductor.
4. Transport/protocol-specific extraction adapters feeding common TLS semantic processing and shared state policy, then existing feature observers. In this candidate, current Processor/Conductor responsibilities may be reorganized; existing class boundaries are not constraints.

Candidate 4 must evaluate whether the current TCP state-machine implementation can itself be refactored into that shared semantic layer, rather than assuming that the existing TCP state-machine logic must remain inside the TCP path.

This is a required investigation, not a predetermined refactoring decision. Identify which transition logic can move unchanged, which requires generalization, and which should remain protocol-specific, with justification based on semantics, regression risk, and delivery cost.

Candidate 4 is mandatory to evaluate, not mandatory to select. Its logical layers do not each require a new class. Shared state policy does not require a full TLS endpoint, uniform observability, or buffering complete TCP handshake messages when staged evaluation can preserve behavior.

For each candidate, explicitly compare how much TLS semantic processing and state policy is shared, what remains protocol-specific, and why. Evaluate both benefit and current delivery cost.

Evaluate Template Method only if earlier phases reveal a stable lifecycle with meaningful variable hooks. Do not build complete HLDs for rejected alternatives.

Compare requirement fit, responsibility clarity, feedback and STOP preservation, state isolation, TCP regression risk, runtime cost, testability, implementation effort, and delivery impact.

Use future DTLS only to detect avoidable hard-coding, not to demand a universal abstraction.

### Decisions and output

Select one architecture and record:

- Responsibilities and required components/interfaces.
- Existing symbols to retain, change, move, or extract.
- Shared behavior and intentionally separate behavior.
- Why rejected alternatives are less suitable now.
- The minimum coherent delivery scope and any optional refactors deferred beyond this feature.

### Exit gate

- The selected architecture satisfies the contracts and policies from Phases 1-5.
- The common TLS semantic processing/state candidate has been evaluated with evidence. Selection or rejection is justified by semantics, observable information, regression exposure, and delivery cost, not by attachment to existing classes.
- Any departure from shared state processing identifies exactly which semantics cannot or should not be shared and why.
- Every new component or abstraction has a concrete current responsibility and benefit.
- No selected pattern is justified only by naming symmetry, DRY, or future DTLS.
- Implementation and regression exposure are proportionate to the feature.
- Optional refactors are not prerequisites unless a current requirement or correctness issue makes them necessary.
- The selected design can be validated with the available or explicitly planned evidence.

---

## Phase 7: Validate, Consolidate, and Freeze the Pre-Design Decisions

### Work

Walk the selected design through a focused scenario pack:

- Complete and incomplete ClientHello.
- Multiple TLS Records or coalesced QUIC packets.
- Multiple CRYPTO frames and ranges spanning datagrams.
- Out-of-order data, duplicates, gaps, and overlaps, including conflicting overlaps.
- Additional ClientHello and attempt-reuse cases under the selected policy.
- Interleaved TCP/QUIC sessions and resumed TCP processing.
- Malformed, unsupported, and capacity-limited input.
- Observer STOP with and without violations.
- Terminal processing, aging, eviction, and reclaim.

For each scenario, record the responsible role, output, dispatch behavior, state effect, and cleanup. Verify forward coverage from relevant requirements and reverse justification for proposed behavior.

Perform a final KISS/YAGNI and delivery review. Consolidate the decisions into one pre-design package and freeze its accepted version.

This is a design validation review, not a claim that code has been implemented, executed, or performance-tested.

### Output

The **Pre-Design Decision Package** contains:

1. Scope and relevant requirement mapping.
2. Current behavior and preservation obligations.
3. State meanings, transition policy, and evidence-backed semantic equivalence matrix.
4. Processor/Conductor contract.
5. Runtime flows, dispatch, and commit semantics.
6. Ownership, lifecycle, bounds, and runtime considerations.
7. Selected architecture and existing-code impact map.
8. Scenario review evidence and focused regression/performance validation plan.
9. Deferred items with rationale, owner or required decision source, and blocking classification.

### Exit gate

- Every required scenario has a consistent, defined outcome.
- Relevant requirements are covered or have an explicit authorized scope disposition.
- Proposed behavior has a current requirement or a necessary engineering justification.
- No unresolved question changes component boundaries, contracts, state policy, lifecycle, or required behavior.
- TCP preservation obligations and suspected-defect dispositions are explicit.
- KISS, YAGNI, and delivery reviews pass with documented reasoning.
- The consolidated package is internally consistent and ready for the final gate.

---

## Final Gate: READY FOR HLD

After Phase 7, evaluate this gate explicitly. There is no additional Phase 8.

Declare **READY FOR HLD** only when all conditions below pass:

- **G1: Scope and requirements.** Current-release obligations relevant to TLS-Flow are identified, with no silent resolution of applicable conflicts.
- **G2: Architecture and semantic reuse.** Responsibilities, component boundaries, and the selected reuse approach are decided. The architecture has explicitly evaluated whether the existing TLS semantic state machine is transport-independent and either reuses appropriate semantics/state policy for TCP and QUIC or documents evidence-backed reasons for the non-shared portions. Existing class structure is not the sole justification.
- **G3: Contracts.** Processor inputs, outputs, feedback points, result freshness, and lifetime are defined.
- **G4: State policy and scope.** TCP and QUIC state meanings, transition triggers, terminal gates, and repeat-ClientHello policy are decided. The five levels of state reuse are distinguished. Current-release transitions are supported by available observations; future architectural capability is not confused with implemented observation or authorized scope.
- **G5: Runtime completeness.** Success, incomplete input, failures, STOP, cleanup, and reclaim have complete flows.
- **G6: Ownership and resources.** Mutable state isolation, lifecycle, and architecture-sensitive bounds are resolved.
- **G7: TCP preservation.** Intended behavior and regression obligations are explicit; suspected defects are not silently fixed or blindly preserved.
- **G8: Validation.** Scenario review passes, and implementation regression/performance checks have a focused plan.
- **G9: KISS, YAGNI, and boundary compatibility.** Every abstraction serves a current need; optional refactors and speculative functionality are excluded from the required delivery path. The semantic boundary has been checked against existing Fingerprint, Visibility, and Enforcement consumer needs. The design has been checked for avoidable TCP/QUIC hard-coding that would make future DTLS integration unnecessarily expensive, without introducing DTLS implementation requirements.
- **G10: Delivery feasibility.** The selected scope is proportionate, with no unnecessary architectural work imposed on the feature schedule.
- **G11: Remaining questions.** Only non-blocking details remain. Their resolution will not change fundamental design decisions or weaken required behavior.
- **G12: HLD readiness.** The HLD can explain and formalize the accepted decisions rather than perform unfinished architectural research.

If any condition fails, report **NOT READY FOR HLD**, identify the affected phase and decision, and perform only the necessary follow-up work.

## What Remains for HLD

Once READY FOR HLD is justified, HLD work may elaborate the accepted design through:

- Component and sequence diagrams.
- Concrete interface descriptions and final naming.
- Detailed existing-file/symbol impact.
- Non-architectural capacity values and implementation constraints already classified as non-blocking.
- Integration and validation details consistent with the accepted contracts.

The HLD must not introduce a new architecture, change state semantics, or add speculative functionality without explicitly reopening the relevant pre-design decision.

## Revision Note

This revision corrects the initial plan's bias toward extending existing class boundaries. It makes semantic-equivalence research and evaluation of shared TLS semantic/state processing mandatory while retaining seven phases, explicit exit gates, and a delivery-focused final gate. It does not preselect a shared state machine or authorize scope expansion.

### V3 Changes

- Phase 2 explicitly evaluates the existing enum, state fields, and transition rules as candidate abstractions rather than assuming that reuse of `hsState` establishes shared semantics.
- Phase 3 requires evidence-backed classification instead of forced QUIC mapping and distinguishes TCP-specific behavior from information outside the current QUIC scope.
- Architecture-sensitive unresolved classifications remain blockers for READY FOR HLD. KISS and YAGNI do not require additional event types or components merely to represent the analysis.

### V4 Changes

- Phase 2 separates architectural support for state progression from current-release observation and implementation obligations, including the distinction between TLS Finished observation, inferred progress, and feature-processing completion.
- State semantics, transition policy, representation, mutable storage, and implementation are evaluated as five independent reuse decisions.
- Phase 4 distinguishes current Enforcement obligations from future boundary reuse and separates wire-format-independent semantic facts from protocol context, observation capabilities, and protocol-specific actions.
- Scope and exit/final gates are aligned with these distinctions; later encryption-level processing is not implicitly required.

### V5 Changes

- Candidate 4 explicitly investigates refactoring the existing TCP state-machine implementation into the shared semantic layer, without preselecting that refactor.
- The investigation distinguishes unchanged, generalized, and protocol-specific transition logic and evaluates semantics, regression risk, and delivery cost.
- The final HLD section heading no longer carries obsolete numbering.
