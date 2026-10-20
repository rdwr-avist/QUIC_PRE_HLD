# TLS-Flow: Template Method Analysis Stage Summary V1

## 1. Purpose and status

This document summarizes the analysis completed before defining continuation points and their minimum data. It is not an approval of the Template Method base, hook signatures, complete-CH representation, or viewer routing implementation.

The accompanying next-stage artifact is `tls_flow_processing_continuation_points_v1.md`. The term continuation point there includes both internal processing boundaries and returns to Conductor, not only TCP segment continuation.

Official HLD V10 remains unchanged. Strategy Processor Design V2 and Template Method Proposal V1 remain preserved discussion inputs. No documents are merged in this stage.

## 2. Working direction and confirmed constraints

| Topic | Status and meaning |
| --- | --- |
| Processor wrapper and strategies | Working design direction: Conductor sees one wrapper; selection and delegation stay inside it. Exact public API remains a proposal. |
| Template Method | Candidate for sharing actual algorithm ordering, not a reason to force identical transport internals. Base class and hook list are not approved. |
| CH viewer | Each strategy provides the existing viewer, directly or through inherited implementation if a base is selected. No separate allocated viewer is required. |
| endPacket | After callbacks, verdict reduction, permitted commit and every final internal read, current-result availability may be invalidated. No consumer/viewer use of that result follows. Persistent incomplete acquisition is not erased merely because input ended. |
| FullString | Existing API, representation and consumer access remain unchanged. No addition to ClientHelloData is selected. Correct current-result routing is still required. |
| Protocol metadata | Explicit TCP/QUIC metadata in ClientHelloData remains required. Exact enum spelling is a design detail. |
| TCP extension | Support splits at every byte, including headers, and CH across TLS records. This is an approved capability expansion, not a claim of full equivalence to old code. |
| TCP exclusions | No new out-of-order/gap support; no automatic overlap/retransmission policy expansion. |
| Capacity | CH maximum and dependent capacities remain OPEN. Startup preparation only; no runtime allocation or capacity growth. |
| Semantic authority | Conductor applies actual feedback and semantic TLSData updates. Parser findings are not violations or automatic Error. |
| QUIC time | Fresh CH timestamp is the arrival time of the datagram completing the message. |

## 3. Three seams identified by the analysis

### 3.1 Protocol-specific checks after shared CH parsing

**Need:** protocol-specific applicability must use parsed CH facts before the unit becomes consumer-visible. For example, QUIC TLS-version applicability must not become a shared restriction on TCP.

**Recommendation, not approval:** put this logical check after successful shared parsing/FP and before result exposure. Return findings, not enforcement verdicts. Keep nonblocking findings with the CH in one feedback unit.

**Still open:** whether this logical step becomes a protected hook, a direct protocol function, or a protocol-owned invocation of shared parsing. The previous three-hook sketch does not settle that placement.

### 3.2 Complete-CH input and migration from incremental TCP

**Need:** define message boundaries, completeness, valid byte extent, backing lifetime and resource failure before choosing a parser API.

**Recommendation, not approval:** examine a complete-message input, including handshake header and excluding transport framing. Contiguous backing and header inclusion are still proposals. TCP segmentation approval does not approve them automatically.

**Work still required:** map old parser/digest responsibilities into collection versus interpretation; preserve previously supported valid-input FP; identify changed error timing and TCP record/CH event behavior. No maximum CH value is invented.

### 3.3 TCP events and existing viewer access

**Need:** a TCP record boundary and a CH-completion boundary are not the same thing. Retained Complete can permit a callback without authorizing a fresh CH update. The current code also allows callbacks to inspect precommit TLSData.

**Recommendation, not approval:** compare explicit traces before choosing a shared result shape. Keep the existing public viewer contract and route reads to the correct current strategy/result. A wrapper implementing/delegating that interface is one option, not a finalized implementation.

**Work still required:** complete Factory/Application and consumer call-site mapping; handle nested synchronous reads; ensure no old TCP data is visible during a QUIC callback. endPacket lifetime agreement does not itself prove every existing caller complies.

## 4. Source facts checked in this stage

The following are static code facts, not target-design decisions:

- Factory wires one existing TCP Processor into the processing interface and three viewers. See embedded `context/engine/src/Factory.cpp` and `processor/src/Processor.hpp` in the supplied code bundle.
- `TLSFlowConductor::analyzeProcessResult` dispatches parsing error, record, and eligible CH before final reduction. Its current-event Stop and accumulated-mask handling must not be replaced by a generic early return.
- `TLSFlowConductor::onClientHello` constructs callback data from viewer facts while passing precommit TLSData.
- `TLSFlowConductor::updateTLSDataOnClientHello` increments count and copies facts only on the qualified commit path.
- `Processor::beginPacket` and `resumeStepFromSegmentBuffer` follow distinct fresh/resume paths; the Step 1 partial-restoration concern remains unresolved, not silently fixed by a base class.
- The supplied consumer code contains a nested Application CH-viewer FullString read. This confirms a routing requirement, not a change to the FullString API. The inspected call site is not a complete proof of all external caller lifetimes.

## 5. What was completed, and what was not

**Completed:** identified the three design seams; separated user decisions from proposed representations; established the order of analysis; checked relevant existing dispatch/viewer paths.

**Not completed:** final base selection, complete-message contract approval, exhaustive TCP before/after event mapping, final viewer wiring, ownership design, capacity values, implementation or runtime validation.

The previous explanatory answer must not be read as approval of a new post-parse hook or a contiguous CH buffer. The next stage defines required continuation boundaries and minimum information without choosing those mechanisms.

## 6. Handoff to the next stage

For each boundary, define: trigger, information available, information produced, what survives a return, when processing may resume, and what must not be inferred. Keep logical data separate from new structs/classes. Only after these contracts are reviewed should the team decide which boundaries are Template Method hooks and which are ordinary shared functions.

Static document review only. No build, packet test, allocation/performance PASS or Mermaid rendering is claimed.
