# O03: TCP Migration, Callback Timing and Malformed-Input Review V7

## V7 focused coordination and review scope

**C-O03-01: coordinated publication boundary.** For the newly supported TCP ClientHello spanning eligible TLS records, contributing physical records are acquisition input, not individually published feedback units. Do not dispatch intermediate TLSRecord/CH callbacks or normal commits for them. Continue safe available acquisition, checking each record header and excluding it from CH bytes; missing input preserves continuation. On full successful CH/FP completion, expose the single D02 completion projection. For cross-record CH its recordLength is the final contributing actual record's declared payload length; for an existing single-record CH preserve its original payload length. This does not turn recordLength into total CH size or a byte offset into the current packet.

**Barrier retained:** once any semantic unit is yielded, finish all applicable callbacks, actual-verdict reduction, permitted commit and every final internal read before processing a later unit or reusing its backing. Do not parse a separate following record ahead of that barrier. An applicable diagnostic/Stop or unrelated-record interruption during acquisition follows O03's exceptional route, not successful completion; deferred success publication does not defer a known blocking error. Existing non-CH TCP routes and retained-Complete callback eligibility remain unchanged. This coordination does not create a QUIC TLSRecord projection, enforcement verdict, automatic Error/drop, new class/API, capacity value or reclaim mechanism.

This is the working projection selected for the user-authorized O04 review, not a claim of legacy cross-record success or migration equivalence. It supersedes the intermediate-record publication premise only for records contributing to this selected CH. It does not authorize processing past an already yielded unit, mixing unrelated records into CH input, or suppressing existing callbacks for separately processed records.

The current user instruction authorizes this focused coordination, restarted O03 review and subsequent O04 review against the selected projection. It is not blanket acceptance of D01-D04 behavior changes. Consolidated V6 CP04 and section 7.3 now express this same rule. Historical statements below about checking that text are discharged for this specific conflict, not for every inherited contract.


**Status: READY TO CONTINUE DESIGN IN O04 under the explicit working dispositions and handoffs below. TCP migration equivalence, implementation and runtime acceptance are NOT established.**

O03 separates existing TCP behavior from the approved arbitrary-byte/cross-record ClientHello extension. The supplied code is the behavioral reference, including surprising callback and error paths. O01 is a proposed shared algorithm, not evidence that those paths are preserved automatically. This review records concrete differences rather than declaring equivalence from matching happy-path fingerprints.

## 1. Scope, authority and evidence limits

V5 preserves V3, V4 and the supplied code without modification. V4 is the revision base so its concrete D01 mappings are not lost. The current user direction is to preserve existing TCP behavior as far as possible and expand QUIC support as fully as possible within the existing approved scope. This does not revoke the previously approved TCP arbitrary-byte/cross-record CH capability, authorize new TCP gap/out-of-order policies, or expand QUIC beyond the established baseline. O01/O02 identities and claims below are inherited evidence only: those files are not available for a fresh cross-document review in this revision. Physical aggregate line numbers in the evidence index refer to the supplied code snapshot.

**Authority discipline:** existing source behavior is CODE FACT; the user's preservation/extension direction is ACCEPTED DIRECTION; new choices below are WORKING DESIGN DISPOSITIONS for progressing design, not claims of existing behavior or blanket product acceptance. A concrete later contradiction must be surfaced, not hidden by this progression status.

- **CODE FACT:** a branch, field, call order or return mapping visible in the supplied source. It does not establish successful execution or absence of undefined behavior.
- **ACCEPTED CONSTRAINT:** TCP CH support may expand to arbitrary byte cuts, including headers and multiple TLS records. No new out-of-order/gap support or automatic overlap/retransmission policy expansion. Startup allocation is allowed; affected Engine runtime must use prepared storage without growth or heap fallback. No maximum CH size is selected.
- **PROPOSAL:** O01's shared operations, checkpoints and accepted-byte accounting; O02's B orchestration and B1 default, with viable B2 composition. Neither architecture is accepted by this review.
- **DESIGN JUDGMENT:** the compatibility boundaries and recommended dispositions here. They are not new accepted product behavior.
- **PLANNED VALIDATION / EVIDENCE GAP:** all differential packet tests, builds, actual listener behavior, runtime allocation and performance evidence. No new implementation is supplied, so old/new execution equivalence cannot be tested here.

O03 covers TCP record/error/CH dispatch timing, migration boundaries, and malformed-input handling. It does not redesign QUIC, choose B1/B2, replace a viewer, allocate pools, close O04-O08, or amend official HLD V10. External RFC claims in O01 are inherited, not revalidated here. No external sources were needed for this source-grounded comparison.

### 1.1 Evidence index

- **E01:** `packet-flow/src/TLSFlowConductor.cpp`, `TLSFlowConductor::process`, aggregate L3428.
- **E02:** `packet-flow/src/TLSFlowConductor.cpp`, `TLSFlowConductor::dispatchStepResult`, aggregate L3486.
- **E03:** `packet-flow/src/TLSFlowConductor.cpp`, `TLSFlowConductor::analyzeProcessResult`, aggregate L3514.
- **E04:** `packet-flow/src/TLSFlowConductor.cpp`, `TLSFlowConductor::updateTLSDataOnClientHello`, aggregate L3820.
- **E05:** `packet-flow/src/TLSFlowConductor.cpp`, `TLSFlowConductor::commitUnsupported`, aggregate L3873.
- **E06:** `packet-flow/src/TLSFlowConductor.hpp`, `TLSFeatureVerdict notify`, aggregate L4008.
- **E07:** `processor/src/Processor.cpp`, `Processor::beginPacket`, aggregate L5410.
- **E08:** `processor/src/Processor.cpp`, `Processor::resumeOrDiscardStepContinuation`, aggregate L5482.
- **E09:** `processor/src/Processor.cpp`, `Processor::processRecord`, aggregate L5564.
- **E10:** `processor/src/Processor.cpp`, `Processor::processSegmentedClientHelloContinuation`, aggregate L5780.
- **E11:** `processor/src/Processor.cpp`, `Processor::processNewRecord`, aggregate L6164.
- **E12:** `processor/src/Processor.cpp`, `Processor::processClientHello`, aggregate L6810.
- **E13:** `processor/src/Processor.cpp`, `Processor::processTlsRecord`, aggregate L6896.
- **E14:** `processor/src/TtlsParser.cpp`, `ClientHelloParser::parse`, aggregate L8544.
- **E15:** `processor/src/TtlsParser.cpp`, `ClientHelloParser::validateLengths`, aggregate L8699.
- **E16:** `processor/src/Processor.cpp`, `Processor::processExtensions`, aggregate L6440.
- **E17:** `processor/src/Processor.cpp`, `Processor::processSegmentedAlertContinuation`, aggregate L5951.
- **E18:** `processor/src/Processor.cpp`, `Processor::processSegmentedOpaqueContinuation`, aggregate L5857.
- **E19:** `processor/src/Processor.cpp`, `Processor::processCiphers`, aggregate L6776.
- **E20:** `processor/src/Processor.cpp`, `Processor::parseAllExtensions`, aggregate L6715.
- **E21:** `processor/src/Processor.cpp`, `Processor::processHandshake`, aggregate L6881.
- **E22:** `processor/src/Processor.cpp`, `Processor::feedExtensionsFP`, aggregate L6575.
- **E23:** `processor/src/Processor.cpp`, `Processor::processSni`, aggregate L6523.

## 2. Existing TCP execution and observable timing

### 2.1 Entry and pull boundary

**CODE FACT [E01, E07-E09].** Conductor handles TCP reset, silent terminal states, empty payload and terminal-payload routing before normal parsing. Normal parsing calls beginPacket, then processRecord with current TLSData. It dispatches each result before pulling again. The loop continues only for RecordParsed while the session remains in an active handshake state. A listener Stop returns before the next pull. There is no endPacket operation in the supplied IProcessor; an eventual cleanup operation is a proposal, not an existing call to preserve verbatim.

beginPacket captures scalar packet/session metadata and sets packet cursors. It can return through continuation/retransmission/gap paths before resetStepState. The reset is not an unconditional per-record reset. Do not infer that all viewer/error fields are cleared on every pull or on every resumed input.

### 2.2 Step-result translation

| Internal/outer result | Existing Conductor behavior | Compatibility consequence |
| --- | --- | --- |
| ContinueProcessing / RecordParsed | analyzeProcessResult | Error, record and eligible CH stages can all run for this unit |
| Segmented / NeedMoreData | Return Continue without record/error/CH dispatch for this step | Buffered partial input is not a record event |
| Error or StopProcessing / Stop | checkForParsingError only; apply actual returned verdict | No automatic record or CH stage and no parser-created violation |
| Unsupported | commitUnsupported using continuation-gap and current state/status | No anomaly callback is dispatched by this branch |
| EndOfPacket | Return Continue | No extra semantic event |

**CODE FACT [E02, E05, E09].** Unsupported does not unconditionally overwrite every state: it sets Unsupported for a continuation gap, PreHandshake, or InProgress with Partial CH. Stop with no actual error produces no anomaly callback. Preserve the difference between parser control result, observer Stop and semantic terminal state.

### 2.3 RecordParsed callback and verdict sequence

**CODE FACT [E03, E06].** The order is anomaly observers if an error exists, then TLSRecord observers, then ClientHello observers whenever the CH viewer status is Complete. The last condition is not restricted to a fresh CH or even to record.isClientHello(). Within each observer list, notify stops at the first Stop and retains bits from listeners already dispatched. Across stages, the Conductor applies a different reduction:

| State at a stage boundary | Remaining stages of this same unit | Final commit / result |
| --- | --- | --- |
| Continue, no accumulated bits | Continue | Normal commit if all stages finish without bits |
| Stop, accumulated bits still zero | Remember Stop; later stages may still run | Normal commit if no bits arrive later; return Stop |
| Stop, accumulated bits nonzero | Immediate return | commitError with accumulated bits; no normal commit |
| Continue, accumulated bits nonzero | Continue | If no Stop occurs, commitViolation only; skip normal commit |
| Earlier zero-mask Stop, later Continue with bits | Later stages still run unless a later Stop triggers immediate reduction | At end commitError because Stop was remembered; skip normal commit |

Thus a zero-mask Stop is not permission to skip all remaining same-record callbacks, and it is not permission to skip the normal commit. It does prevent processing the next record. A prior Continue-with-bits followed by a zero-mask Stop is an immediate Error because accumulated bits are already nonzero. These are preservation requirements, not recommended cleanups.

**CODE FACT [E03, E04].** Normal commit occurs after callbacks. CH count, fingerprint, version mask, early-data flag and timestamp are updated only through the CH record branch and only with Complete status. Callback delivery and CH commit are not interchangeable: a retained Complete can cause a CH callback on a later non-CH record without incrementing clientHelloCount. The timestamp used by TCP commit is session.getLastPacketTimestamp at that commit, not a new first-byte timestamp.

### 2.4 State-sensitive records remain TCP-specific

**CODE FACT [E01, E11, E17, E18].** Handshake parsing is enabled in PreHandshake/InProgress. Encrypted-handshake handling after ChangeCipherSpec, pre-CH application data, selected InProgress application data/CCS and segmented alerts have their own paths. Alert readability depends on state, alignment and length; opaque continuation tracks record completion without CH interpretation. They must not be routed through the shared CH parser just because processing is reorganized.

Existing normal commit can move TCP to ChangeCipherSpec, Finished or TerminatedByAlert. O03 preserves those TCP branches; none becomes a QUIC completion signal. RST and session-aging behavior are outside CH extraction and must not be silently changed.

## 3. Compatibility partition

| Partition | Meaning | Required treatment |
| --- | --- | --- |
| P: preserve | Previously supported TCP inputs and observable behavior outside the approved extension | Preserve error identity, step result, callback sequence, state/counter writes and FP/FullString unless an explicit difference is accepted |
| X: approved capability extension | CH cuts inside headers/prefix/vectors and across records | Implement the accepted capability; legacy rejection is not an oracle for rejecting newly supported valid input |
| D: unresolved behavioral difference | New support requires timing/projection rules not fixed by the capability approval | Record and disposition explicitly; do not call it preserved or already accepted |
| G: evidence gap | Source suggests a defect, stale-state dependency or unsafe access | Do not emulate unsafe memory behavior; no runtime equivalence claim; require bounded implementation and focused validation |

A valid CH split at a formerly unsupported position belongs to X. A malformed CH that used to produce a specific error but now waits or produces another error can belong to D, even if the new collector is safer. Supporting fragmentation does not authorize accepting arbitrary additional handshake messages inside one TCP record, resetting a live CH on every record, changing TLS version policy, or changing retransmission semantics.

## 4. TCP migration scenario matrix

The target column is a preservation obligation or labeled recommendation, not an implemented result. All execution tests remain NOT RUN.

| ID | Input / source trace | Existing behavior | Required target treatment / disposition |
| --- | --- | --- | --- |
| T01 | One complete CH, handshake extent equals record payload [E11,E12,E19,E22] | Body, ciphers, sorted extensions, FP finalize, record push, RecordParsed; then section 2.3 | P: same FP/FullString, counts and order. One shared completion does not bypass record feedback |
| T02 | CH extension block split across consecutive TCP segments [E10,E12,E16] | Initial step NeedMoreData; further partial steps NeedMoreData; completion reconstructs a CH Record and yields RecordParsed | P: do not emit a record callback on the initial partial segment merely because a record header is known. Final callback/commit uses completing packet |
| T03 | Same CH needs three or more extension fragments [E10] | Repeated extension continuation until remaining reaches zero; completion diagnostics depend on segment count | P: preserve repeated continuation and diagnostic meaning for existing cases; no two-segment assumption |
| T04 | CH header cut before all four bytes [E21,E11] | Handshake parser failure becomes MalformedHandshake / Stop on the ordinary CH path | X: collect missing header bytes; no error solely for a valid cut. D01 defines malformed timing boundary |
| T05 | TLS record header cut [E13,E11] | Ordinary non-alert short header becomes MalformedTlsRecord / Stop; a recognized alert first byte has a dedicated continuation path | X for the approved CH path: reconstruct record header. Do not generalize all non-CH record policies without authority |
| T06 | Prefix, session ID, cipher length/element or compression cut [E12,E14] | Some incomplete statuses become Unsupported when segmented; other incomplete field accesses return Failure and become ClientHelloBodyInvalid / Stop | X for valid cuts; D01 for malformed inputs. Failure cannot universally mean NeedInput |
| T07 | CH spans complete TLS handshake records [E11,E12] | Record-level fragmentation can return Unsupported; some mixed segmentation paths instead stop with a length mismatch | X: exclude each record header from CH bytes and keep one message context; D02 defines deferred completion publication and final-record metadata |
| T08 | CH extent smaller than current record payload, including trailing handshake bytes [E11] | HandshakeLengthNotMatchingRecordLength before shared CH body processing | P: retain TCP outer mismatch rejection unless separately accepted. O01 accepted-less-than-offered is not a TCP policy override |
| T09 | Two CH records in one packet [E11] | First can complete; a later CH sees retained Complete and reports MultipleClientHello, provided processing reaches it | P: preserve current check and precedence. No automatic fresh start for TCP merely because O01 permits multiple logical contexts |
| T10 | CH then non-CH record in same packet [E03,E11] | Complete status can remain; another CH callback can occur, without a second CH commit on the non-CH record | P: preserve callback eligibility separately from fresh parsing. Suppressing it as a duplicate is a behavior change, not deduplication |
| T11 | CH then CCS/application-data/alert [E01,E03,E11] | Later record sees committed TLSData; may transition state and stop the outer loop; retained CH callback still follows section 2.3 when eligible | P: no pre-parsing of later record before feedback/commit. Preserve TCP state rules and alert path |
| T12 | Complete record header with unsupported type or oversized payload [E13] | UnsupportedRecordType / Stop or RecordLengthExceedsData / Stop before handshake processing | P: retain error precedence and no record event; do not translate all such cases to Unsupported |
| T13 | Unsupported record-layer version on a CH [E11] | UnsupportedTlsVersion / Stop before CH interpretation; record version check is not a generic all-record check | P: retain TCP check at its boundary. Do not import QUIC's nonblocking applicability behavior into TCP |
| T14 | Declared body inconsistent with vectors/extensions [E14,E15,E12] | Failure becomes ClientHelloBodyInvalid / Stop, subject to prior checks; incomplete statuses depend on actual available bytes | D01: retain exact established complete-input errors; collector must reject impossible declared bounds, not wait forever |
| T15 | Malformed extension, SNI, supported versions/groups or cipher vector [E16,E19,E20,E22,E23] | Specific parser paths set ExtensionParsingFailure, MalformedSniExtension or CipherParsingFailure and Stop; ordering follows parse/extract then sort/feed | P: keep diagnostics and precedence. D04 covers non-error capacity/feed stops and mapping into O01 statuses |
| T16 | No extension field versus explicit zero extension block [E14,E15,E22] | Separate body-parser branches; canonical empty extension delimiters exist. Boundary arithmetic has caveats in section 6 D03 | Preserve supported no-extension semantics; do not claim every empty-block layout already succeeds. D03 remains explicit |
| T17 | Continuation full retransmission, partial overlap, gap [E08,E09] | End <= expected: suppress via NeedMoreData; overlapping prefix trimmed; gap discards pending buffer and yields Unsupported | P: preserve current scoped policy, including serial comparison. No new conflict-byte comparison, out-of-order queue or gap recovery |
| T18 | Segmented opaque or alert record [E17,E18] | NeedMoreData until complete, then one record projection; alert alignment/readability handled separately | P: no fake CH context and no change to record callback timing from CH extraction |
| T19 | Record viewer full or digest/locator capacity exhausted [E10,E11,E19,E20,E22] | Stop paths differ: some set an error; some do not; a cipher non-error stop can fall through to record push | D04: do not replace all outcomes with a generic anomaly or assume no record was published |
| T20 | Session A partial, B processed, A resumed [E07,E10] | Per-session segment backing exists, but several viewer/parser members remain Processor-wide and resume bypasses reset | G: preserve logical association, not accidental cross-session values; O04/O05 must validate exact fields/backing. No blanket interleaving PASS |

### 4.1 Error precedence is part of the contract

For an ordinary CH record in the existing new-record path, the relevant order is record-header parse, payload-size check, supported-record-type check, handshake header, CH record-layer version, handshake-smaller-than-record mismatch, retained-Complete multiple-CH check, CH body processing, remaining record-fragmentation mismatch checks, base/cipher work, extensions and record-viewer capacity. Moving all these checks into a generic post-Complete applicability stage changes which error wins and when it is observable. [E11-E15,E19-E22]

For example, a second CH record with an unsupported record-layer version reports the version error before reaching MultipleClientHello. A CH followed by bytes in the same record fails the outer length check before body parsing. These TCP prechecks can remain in acquisition/orchestration without contaminating the shared CH algorithm.

## 5. Integration obligations independent of O02 packaging

1. **Preserve transport policy outside shared interpretation.** TCP record/type/version checks and permitted sequence handling stay TCP-specific. B1 functions and B2 composition must satisfy the same observable trace. O03 does not resolve the earlier broad-versus-narrow common-orchestration comparison by assumption.
2. **Separate shared completion from callback eligibility.** O01's repeated Complete does not mean reparse/rehash. It also does not authorize removing the existing retained TCP Complete callback path. QUIC duplicate suppression must not be reused as a universal TCP event-suppression rule. [T09,T10]
3. **Do not advance past an emitted feedback boundary.** Acquisition may traverse eligible handshake records continuing the same pending CH, excluding their record headers, without inventing intermediate record events. Once a result requiring dispatch is returned, callbacks, verdict reduction and permitted commit finish before processing the next semantic unit. A later unrelated record is not swallowed as CH continuation. O01 accepted-byte accounting is not the old consumed count. [T01,T02,T07,T11; D02]
4. **Keep current backing until full last use.** O01 sections 4.4/8 require callbacks, verdict reduction, permitted commit and every final internal read to finish before release/reuse. Existing early segment-buffer deallocation/copying is not proof that every borrowed locator or viewer remains valid. O04/O05 own the detailed proof, not a new reclaim mechanism. [T02,T20]
5. **Keep actual verdict authority.** Blocking parser failure stops the affected parse path, not automatically the session. A parser diagnostic is not a TLS-Enforcement violation. A ResourceFailure cannot invent IncompleteHandshake. Preserve the section 2.3 distinction between Stop and Error.
6. **Do not fix unrelated FP behavior during extraction.** Keep cipher iteration, de-GREASE rules, extension ordering, selected content including extension 57, delimiters, byte order and FullString API. O01's one-time contribution discipline is compatible with retaining canonical bytes, not a license for a new fingerprint variant. [E19,E20,E22]
7. **No new runtime allocation.** Collection and continuation use prepared storage; capacities and width migration remain O06, provider/runtime proof O07. Existing numeric constants are source facts, not newly approved CH maxima.

## 6. Concrete differences and proposed dispositions

These findings are the result of the source comparison, not defects silently repaired in the attachments. V5 uses explicit working design dispositions to continue design; it does not infer blanket product acceptance or implementation equivalence. V3 made the D01 proposal precise in D01.1-D01.3. V4 adds concrete branch/diagnostic/packet mappings in D01.4-D01.6 and a restarted closure review in V4 section 8.4 (historical, preserved in V4). Those historical revisions did not implement code or close the migration gate. V5 dispositions below supersede their unresolved progression wording.

### D01: partial-input error timing versus newly supported fragmentation

**CODE FACT [E12,E14,E15].** ClientHelloParser returns IncompleteClientHelloPrefix below its initial minimum, but several later missing-field/vector paths return Failure. processClientHello maps those differently: some incomplete segmented inputs become Unsupported; Failure becomes ClientHelloBodyInvalid. Ordinary short handshake headers fail earlier with MalformedHandshake. O01 instead collects arbitrary cuts and requires impossible declared extents to fail as soon as known.

**Difference:** retaining old error timing for every partial byte sequence is incompatible with accepting the same prefix as a valid fragment. A body that already declares an impossible extent can also fail earlier under O01 than the old minimum-length-first parser. This is not solved by calling every old Failure NeedInput, nor by waiting for the full CH on every case.

**Recommended disposition (refined in V3; PROPOSAL):** distinguish missing eligible bytes from a structural contradiction already established by available bytes and declared bounds. Wait only for potentially valid continuation; stop affected parsing on an established contradiction. Preserve complete-input TCP diagnostic identity and check precedence, with any unavoidable earlier/later malformed-input timing named explicitly. D01.1-D01.3 define the proposed boundary and remaining evidence. No new TLS semantic validation, enforcement action or implicit acceptance is introduced.

**Closure evidence:** source-derived old/target traces for T04-T06/T14, including a common prefix before the distinguishing length is complete, then fitting and impossible continuations. D01.4 supplies concrete cases and diagnostic/callback packets for the single-record domain; D01.6 supplies the current cross-record dispositions. These are static traces, not executed packets. An already impossible complete handshake length cannot also describe a valid CH.

#### D01.1 Proposed partial-input decision boundary

**PROPOSAL, not an accepted implementation API.** Apply these rules only to the approved TCP CH fragmentation expansion and the shared CH parsing boundary. Existing non-CH TCP policies, overlap/retransmission/gap rules and D02 record projection is defined separately below.

1. **Eligible input and precedence first.** TCP retains its outer record/type/version/mismatch/multiple-CH checks at their applicable boundaries and in the source-derived order in section 4.1. A shared body check cannot preempt an earlier applicable TCP check merely because its bytes are available. If an earlier required check is not yet decidable, do not expose a later diagnostic first. An unavailable check cannot be bypassed by reading beyond the supplied region. Where a cross-record projection affects when a check becomes applicable, apply D01.6/D02; do not invent intermediate feedback.
2. **Missing bytes are not a structural failure.** When an allowed header, length field or vector is incomplete and received bytes do not yet prove a contradiction, retain bounded continuation and request more eligible input. A legacy Failure caused only by absent bytes is not automatically a new ParseFailure. Conversely, legacy Failure is not universally translated to NeedInput: the actual cause must be classified.
3. **Established contradictions stop parsing.** Once a complete declared length and known enclosing CH/vector bounds prove that a required field or vector cannot fit, stop the affected parse path at the first eligible decision point, subject to rule 1. Do not wait for bytes outside the declared CH to repair it. An incomplete length field alone is not proof of an oversized vector. Checked arithmetic must establish bounds without overflow, underflow or narrowing. No new supported CH maximum is selected.
4. **Declared end is final for this CH.** If the complete declared CH extent is available but its required structure is incomplete or inconsistent, fail rather than returning NeedInput. Following handshake messages, record headers and unrelated backing bytes cannot complete this CH. D03 still owns the explicit-empty-extension boundary correction; this rule does not silently decide that acceptance question.
5. **Complete-input compatibility.** For previously supported complete input, preserve established diagnostics and precedence, FP/FullString and callback/commit behavior. If incremental detection exposes a structural error earlier than the old parser, retain the corresponding established diagnostic where defined and list the changed decision packet explicitly. Such a case is a named timing delta, not an unqualified equivalence PASS. If no established diagnostic applies, leave a concrete mapping gap rather than inventing a new error or violation.
6. **No forced extra input.** Local NeedInput exhausts the offered eligible region after safe internal work. Continue safe current-input traversal when permitted, but never cross an emitted feedback barrier to obtain more bytes; traversing another eligible record continuing the same pending CH is not itself a feedback event. End of the current packet alone does not prove malformed CH. Pending state remains subject to existing lifecycle and resource limits; waiting is not an unlimited-retention promise or a new timer/reclaim scheme.
7. **Failure is not enforcement.** Processor supplies parsing status, diagnostics and available parsed facts. A blocking structural failure prevents successful completion of that CH and further parsing of the affected path; it does not itself set a violation, drop traffic, set session Error, or generate IncompleteHandshake. Conductor retains existing dispatch/reduction/commit behavior and TLS-Enforcement owns violation decisions through actual verdicts. Nonblocking findings do not become structural failures and do not suppress otherwise valid parsing/FP. This clarification does not convert TCP's existing UnsupportedTlsVersion path into QUIC's nonblocking version behavior.

#### D01.2 Boundary examples and proposed trace oracles

These are symbolic design cases, not generated packet bytes, executed tests or new numeric capacities. P1/P2 denote consecutive input packets; offsets refer to the selected logical CH. Each oracle is subject to earlier TCP checks in D01.1 rule 1.

| ID | Available information | Proposed parsing outcome and observable boundary | Compatibility/evidence |
| --- | --- | --- | --- |
| D01-C01 | P1 ends within an allowed record/handshake header; no established contradiction | Retain missing header bytes; no diagnostic solely for the cut. P2 permits the next applicable check once the required header is available | X; old short-header rejection is not the target oracle. D02 defines deferred completion publication |
| D01-C02 | P1 ends after only part of a vector length field | Wait; do not infer a length from incomplete bytes. When P2 completes the length, classify against known bounds before requesting its payload | X; record the first decision packet |
| D01-C03 | P1 has a complete vector length that fits known CH bounds, but not all vector bytes | Wait for eligible continuation; do not map a missing-field legacy Failure directly to malformed CH | X; valid continuation and malformed continuation may share this same P1 prefix |
| D01-C04 | P2 completes a length field and proves its vector cannot fit the declared CH | Stop affected parsing in P2 once earlier applicable checks permit; do not wait for vector payload outside CH bounds | D01 timing delta; corresponding body diagnostic is ClientHelloBodyInvalid where the existing body-length mapping applies; no fabricated enforcement |
| D01-C05 | P1 supplies the complete handshake length and already proves an impossible body extent | Fail at the eligible P1 decision point, not after an arbitrary future packet or the old parser's minimum-buffer threshold | D01 early-timing delta; D01.4 M05 maps the impossible body to ClientHelloBodyInvalid after the earlier TCP checks; header parse defects remain distinct |
| D01-C06 | Complete declared CH is present but a required field is still incomplete | Fail in the current eligible unit; no NeedInput for bytes beyond the declared end | Preserve existing complete-input diagnostic/precedence; D03 exception remains separate |
| D01-C07 | Same complete CH region is followed physically by unrelated record bytes | Identical CH structural decision whether those unrelated bytes exist or not; never enlarge the CH region to make parsing succeed | Checked accepted frontier; T08 outer mismatch policy and D03 remain unchanged |
| D01-C08 | A previously supported complete malformed CH has multiple detectable defects | Preserve source-derived check order and winning diagnostic; do not move all checks to post-Complete applicability | P unless a specifically named incremental timing delta applies |
| D01-C09 | Current input ends with a potentially valid partial CH and no more safe bytes | Retain continuation; no synthetic malformed/violation/IncompleteHandshake event solely for input end | Existing lifecycle/resource policy applies; no new timeout or capacity |
| D01-C10 | Existing TCP version check or a nonblocking finding is encountered | Preserve the existing TCP version-check routing; keep a genuinely nonblocking finding separate from structural failure | No import of QUIC version enforcement into TCP; no new semantic validation |

For C02/C04, use the same P1 prefix ending before the full length is known, then two P2 variants: one completes a fitting length (leading to C03 if payload remains missing), the other completes an impossible length (C04). Before P2 there is no basis to distinguish the variants. This paired case guards against premature failure as well as indefinite waiting. For C05, the handshake length is already complete; do not claim that a valid CH can have the same impossible declared extent.

The **decision packet** is the packet making the necessary eligible bytes available. The **callback packet/unit** follows the existing TCP step-result/Conductor route and feedback order, not a new callback inside the shared parser. For an affected Stop/Error path, preserve anomaly-only dispatch when an actual diagnostic exists; do not add TLSRecord/ClientHello callbacks or a successful CH commit for that failed step. Prior record units already dispatched are not rolled back. For cross-record input, D01.6/D02 now define the deferred completion or failed-step route; do not add callbacks for intermediate physical records.

#### D01.3 SRS consideration, scope and closure evidence

The user-supplied SRS 4.3.1/4.3.2 is contextual input: fragmented CH acquisition must remain compatible with one shared TCP/QUIC TLSFP algorithm. It is not authority to change TCP error timing, callback eligibility, record projection, version policy or retransmission behavior. This revision does not redesign QUIC header/frame parsing, application-port filtering, statistics/TOP TLSFP, or UDP retransmission handling. It adds no enforcement responsibility to Processor. No external protocol fact is newly asserted or revalidated by this correction.

**Design-level closure:** explicitly disposition D01.1, identify each permitted timing delta, and complete the diagnostic/decision-packet/callback-unit mapping for C01-C10 and T04-T06/T14. A source-grounded preservation design can close a design gap without running packets. Any mapping dependent on D02 must remain a named dependency; it cannot be hidden by declaring all of D01 passed.

**Implementation/runtime acceptance:** paired old/new input traces, exact bytes/segmentation, initial state, listener script, implementation revision, accepted bounds and actual observations remain planned and NOT RUN. D01 refinement does not supply them. No build, fault injection or allocation audit was performed.

#### D01.4 Concrete source-grounded mappings

**Evidence classification:** legacy outcomes below are CODE FACT / static branch traces from the supplied aggregate; target outcomes are DESIGN JUDGMENT / PROPOSAL implementing D01.1. They are not accepted product changes or observed executions. The current instruction authorizes completing this analysis and restarting review, not blanket approval of behavioral deltas. The numbers below describe source-derived field offsets and concrete design examples, not capacities, a supported CH maximum or a generated packet dataset.

**Common conditions for M01-M10 unless overridden:** fresh selected TCP context in PreHandshake, no prior Complete, no gap/overlap/retransmission, successful prepared-storage access, no resource failure, supported record version 0x0303, handshake type ClientHello. One actual TLS record is used unless a cross-record variant is explicitly named. Its payload length R equals the declared handshake body H plus the four-byte handshake header. All header reads in the target are bounded; no old out-of-region load is an oracle. P1 and P2 are ordered packets for the same session. Static old/target comparisons start independently from the same initial state: after legacy Unsupported, do not pretend the old Conductor resumes that CH on P2.

**Field layout F:** a concrete ordinary CH body with H=47, session-ID length 0 at body offset 34, cipher vector length 2 at offsets 35-36, its two bytes at 37-38, compression length 1 at 39 and one method at 40, extension-block length 4 at 41-42, and a four-byte zero-payload padding extension at 43-46. Record payload R=51. Version/random occupy offsets 0-33. Cipher value and other unconstrained bytes are fixed identically across paired cases; no new semantic checks are imposed. This avoids the D03 no-extension/empty-block boundary. The existing cipher loop and canonical FP behavior are preserved, not repaired here. [E14/E15; E19/E20]

**Observable route definitions, derived from E02/E03/E05/E06:**

- **WAIT:** target shared NeedInput, outer NeedMoreData, no new parsing diagnostic from the cut, no error/record/CH dispatch and no normal commit for that step. No assertion about unrelated previously retained viewer fields is implied. In this fresh context, no prior error exists.
- **FAIL(error, P):** parser Error or StopProcessing translates to outer Stop. On packet P, Conductor calls checkForParsingError; registered live anomaly observers receive that exact diagnostic, in existing priority order until an actual Stop. There are no TLSRecord/CH callbacks or normal CH commit for the failed step. With no observers, no callback occurs. Continue/zero mask leaves TLSData unchanged; Stop/zero mask stops dispatch without setting session Error; Continue/nonzero mask records violations without forcing Error; Stop/nonzero mask commits Error. These are actual verdict outcomes, not parser decisions. Earlier successfully dispatched units are not undone. [L3486-3511; L3584-3600; commitError/commitViolation]
- **SUCCESS(P):** after complete parsing and successful resource operations, outer RecordParsed on P follows section 2.3: error stage if an actual error exists, record stage, eligible Complete CH stage, then the existing verdict reduction and permitted normal commit. For the fresh successful case with no violations/Stop, one record callback and one CH callback are eligible, clientHelloCount increments once, timestamp uses P. Counts mean eligible dispatch stages, not guaranteed observer invocations.
- **OLD-UNSUPPORTED(P):** outer Unsupported, no anomaly/record/CH dispatch, commitUnsupported sets this fresh PreHandshake context to Unsupported on P. The actual Processor error field need not be assigned by that branch. [L6818-6824/L6859-6864; L3505-3507; E05]

| Mapping / V3 case | Concrete input boundary | Legacy static branch and packet | Proposed target diagnostic, decision and callback packet | D02 dependence |
| --- | --- | --- | --- | --- |
| M01a / C01, T05 | F, P1 has first 3 bytes of the five-byte TLS header; P2 supplies the rest of this same record | processTlsRecord parse failure -> MalformedTlsRecord -> FAIL on P1 [L6896-6903] | WAIT on P1; after header reconstruction and otherwise valid completion, SUCCESS(P2). No MalformedTlsRecord solely for the cut | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M01b / C01, T04 | F, P1 has full record header and only 2 of 4 handshake-header bytes; P2 completes this same record | processHandshake failure -> MalformedHandshake -> FAIL on P1 [L6881-6888] | WAIT on P1; SUCCESS(P2). A valid cut is not a malformed header. Complete header type/version checks occur before body checks | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M02 / C02, T06 | F, P1 ends after 36 body bytes: only the high cipher-length byte 0x00 is present. P2 supplies low byte 0x02 and the remaining valid F body | bufferLen 36 < source minimum 37 -> IncompleteClientHelloPrefix; segmented record -> OLD-UNSUPPORTED(P1) [L8552-8555; L6859-6864] | WAIT on P1; low byte arrives on P2, vector fits; SUCCESS(P2). No failure for the incomplete length. This high byte does not itself prove impossibility | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M03 / C03, T06 | F, P1 ends after 37 body bytes: complete cipher length 2, no cipher payload yet; P2 completes F | source incOffset(suitesLen=4) from offset 35 cannot reach offset 39 in a 37-byte buffer -> Failure -> ClientHelloBodyInvalid -> FAIL(P1) [L8593-8605; L6869-6870] | WAIT(P1); SUCCESS(P2). The length fits H; missing payload is not an established contradiction. This changes an old anomaly into waiting for a newly supported valid fragment | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M04 / C04, T14 | Same P1 as M02. P2 completes cipher length as 0x0020, supplies no further body bytes; H remains 47, R remains 51 | old path already OLD-UNSUPPORTED(P1), so no old P2 callback in the same lifecycle. Independently offering the accumulated 37-byte prefix reaches incOffset(34) failure -> ClientHelloBodyInvalid; that isolated parser observation is not an old session trace | WAIT(P1); FAIL(ClientHelloBodyInvalid, P2): cipher extent alone ends at body offset 69, outside H=47. No payload outside CH can repair it. No CipherParsingFailure: failure is in the body envelope before processCiphers | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M05a / C05, T14 | P1 has complete TLS and CH headers, H=20, R=24, no body bytes. All earlier checks pass | bodyLen 0 < 37 -> IncompleteClientHelloPrefix; segmented -> OLD-UNSUPPORTED(P1). If the complete 20-byte body had been present initially, incomplete-prefix maps instead to ClientHelloBodyInvalid because the record is not segmented [L8552-8555; L6859-6870] | FAIL(ClientHelloBodyInvalid, P1). H cannot contain even the existing fixed version/random/session-ID-length fields. This is a body-extent contradiction, not MalformedHandshake; no new minimum cipher/compression semantics are added | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M05b / C05 precedence controls | M05a with unsupported record version; separately, supported version but R=25 while H=20 | UnsupportedTlsVersion first [L6279-6284]; otherwise HandshakeLengthNotMatchingRecordLength since H+4 < R [L6287-6298] | Same winning diagnostic on P1: FAIL(UnsupportedTlsVersion, P1), or FAIL(HandshakeLengthNotMatchingRecordLength, P1). Body-invalid must not preempt these checks. A retained Complete is checked after mismatch and before body parsing | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M06 / C06, T14 | Full record in P1, H=39, R=43; body through cipher bytes at 37-38 exists, but no compression-length byte at offset 39 | incOffset(4) from cipher length at 35 fails because offset 39 is one-past-end -> Failure -> FAIL(ClientHelloBodyInvalid, P1) [L8604-8605; L6869-6870] | FAIL(ClientHelloBodyInvalid, P1); declared CH is complete, so no NeedInput for the missing required field. D03 empty-extension cases are excluded | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M07 / C07 | M06 with and without a separate following record physically present after the declared record; offered CH remains exactly H=39 | processNewRecord bounds current payload with min(recordPayloadLen, availablePayloadLength); same body failure before advancing to the following record [E11/E12] | Same FAIL(ClientHelloBodyInvalid, P1) in both cases; no consumption/callback of following record. If extra bytes are instead inside the same declared record, T08 mismatch wins, not this oracle | None for this case; D03 separately owns empty/no-extension accounting |
| M08 / C08 | Complete M06 body in P1, plus unsupported record version and retained Complete; second control uses supported version with H+4 < R and retained Complete; third uses supported version, equality and retained Complete | version -> UnsupportedTlsVersion; otherwise mismatch -> HandshakeLengthNotMatchingRecordLength; otherwise retained Complete -> MultipleClientHello, all before body [L6279-6310] | Preserve the same FAIL diagnostic and P1 for each control; no early body-error exposure or fresh CH publication | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M09 / C09 | P1 contains F through 36 body bytes as M02; no P2 arrives during the observed interval | OLD-UNSUPPORTED(P1), not a successful pending legacy CH | WAIT(P1), no callback/normal commit in that interval. No later parsing diagnostic is scheduled merely because no P2 arrives. Existing lifecycle/resource disposition is separate; no new timer or violation | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |
| M10a / C10, T13 | Full F in P1 with unsupported CH record-layer version, no earlier record error | FAIL(UnsupportedTlsVersion, P1), before body [L6279-6284] | Identical FAIL(UnsupportedTlsVersion, P1); no import of QUIC nonblocking version policy | None for this one-record case |
| M10b / C10 nonblocking control | Complete otherwise valid CH in P1 with a successfully parsed SNI list containing one entry of a non-hostName type; no earlier errors/resources fail | processSni non-host type returns ContinueProcessing without setting an error [L6542-6547]; later normal processing can complete | Preserve this nonblocking path and SUCCESS(P1) when all later stages succeed. No invented diagnostic, no conversion to ParseFailure; SNI availability remains existing viewer behavior | Cross-record variant: apply D01.6/D02 deferred acquisition and completion; no intermediate physical-record feedback. |

**Coverage:** C01 -> M01a/M01b; C02 -> M02; C03 -> M03; C04 -> M04; C05 -> M05a/M05b; C06 -> M06; C07 -> M07; C08 -> M08; C09 -> M09; C10 -> M10a/M10b. T04 -> M01b; T05 -> M01a; T06 -> M02/M03; T14 -> M04-M08. A broad V3 case is not universally proven by one representative: field-class mappings below define the single-record domain; D01.6/D02 define the cross-record working extension.

#### D01.5 Diagnostic classes and named timing deltas

**Body-envelope mapping:** missing version/random/session-ID/cipher/compression bytes that can still fit -> WAIT; complete length/known required-field extent that cannot fit H, odd cipher length already rejected by the existing body parser, or complete declared body missing a required field -> proposed ClientHelloBodyInvalid / outer Stop. The source body parser defaults to Failure and processClientHello maps Failure to that diagnostic. Length-sum mismatch after the complete extension-block length is read also maps to ClientHelloBodyInvalid [L8664-8668; L8699-8721; L6869-6870]. Do not translate a still-missing length field to zero or read beyond the offered region. Partial bytes may sometimes establish a lower-bound contradiction; M02's high byte 0x00 deliberately does not. Checked arithmetic cannot reproduce legacy narrowing/overflow as a compatibility requirement.

**Do not collapse later parser classes into the body mapping:** malformed individual extension envelopes/content remain ExtensionParsingFailure through parseAllExtensions; supported-version/group parser failures remain ExtensionParsingFailure; SNI parse failure is ExtensionParsingFailure while existing SNI count/length/content checks can produce MalformedSniExtension; a failure actually reached in processCiphers is CipherParsingFailure. Their established parse/extract, sort/feed order remains authoritative. An odd cipher length rejected earlier by ClientHelloParser is therefore ClientHelloBodyInvalid, not CipherParsingFailure. A later extension error must not be exposed before an earlier body check is decidable. Capacity/provider failures remain D04, not D01 structural errors. [E14/E16/E19/E20/E22/E23]

| Delta | Explicit old-to-target change | Mapping |
| --- | --- | --- |
| DT01 | Short allowed TLS header: MalformedTlsRecord on P1 -> waiting on P1, success/failure only when eligible information arrives | M01a / T05 |
| DT02 | Short allowed handshake header: MalformedHandshake on P1 -> waiting on P1 | M01b / T04 |
| DT03 | Incomplete prefix: old Unsupported on P1 -> waiting on P1; complete fitting input can succeed on P2 | M02/M09 / T06 |
| DT04 | Incomplete vector payload formerly Failure/ClientHelloBodyInvalid on P1 -> waiting when the declared extent fits | M03 / T06 |
| DT05 | Previously unsupported short prefix followed by impossible vector: target body error on P2; no invented old P2 continuation | M04 / T14 |
| DT06 | Impossible declared body already known from CH header: old segmented Unsupported -> proposed ClientHelloBodyInvalid on P1, without waiting for body | M05a / T14 |
| DT07 | Previously supported complete malformed input: no timing/identity change; outer prechecks retain precedence | M05b/M06/M07/M08/M10a |

These are proposed design dispositions, not user-accepted behavior. DT01-DT06 are not equivalence claims. DT07 excludes D03 boundary fixes and D04 failure changes. New cross-record traces use the D02 deferred-publication convention. These working dispositions permit design progression but do not claim migration equivalence.

#### D01.6 Cross-record dispositions replacing the former XR blockers

The previous XR register assumed that every acquired physical record had to be published before acquisition could continue. That was a proposal, not a source requirement. E10/E02 demonstrate deferred publication for supported TCP segmentation. Cross-record success is new support, so applying that principle here is a working design choice, not a claim that legacy TCP already supports it.

| ID | Working disposition | Remaining detailed work |
| --- | --- | --- |
| D01-XR1 | A record ending inside the CH handshake header is retained as pending acquisition. Do not expose an unknown handshakeType or dispatch a partial record. Continue through eligible handshake-record payloads; return NeedMoreData only when safe current input is exhausted. | O04/O05: bounded saved header state and restoration |
| D01-XR2 | A record ending inside a CH field/vector likewise produces no intermediate TLSRecord/CH event or normal commit. Preserve the existing single-record segmentation trace. | O04: viewer validity; O05: continuation ownership |
| D01-XR3 | Validate each next record header before using its payload. Preserve applicable outer check precedence; an established structural contradiction takes the existing diagnostic/Stop route. No earlier intermediate callback is invented. Do not consume an unrelated non-handshake record as CH bytes; stop acquisition as Unsupported for this newly supported interruption case, with no fabricated violation. | O04: failed-step views; focused future validation for interruption and malformed next header |
| D01-XR4 | On successful completion publish one CH completion projection as defined in D02, then apply existing callback/verdict/commit ordering before any subsequent unit. | O04: projection/viewer contract; O05: last use |

D01 missing-input, unsupported-capability and malformed-input routes remain distinct. The approved fragmentation expansion changes legacy rejection for valid newly supported cuts; it does not convert every legacy Failure to NeedMoreData. Existing complete-input diagnostics and precedence remain preservation obligations. Earlier incremental detection is a named design timing delta, not a general TCP parser cleanup.

### D02: deferred CH completion publication

**CODE FACT [E10,E02,E04].** Existing supported extension continuation returns Segmented/NeedMoreData while incomplete, with no record/CH callbacks or normal commit for that step. Successful completion reconstructs one CH Record, then returns RecordParsed. Normal CH commit is conditional on Complete and the actual verdict reduction. The source stores one physical record payload length; it does not establish a multi-record length convention.

**WORKING DESIGN DISPOSITION:** extend this deferred-publication principle to newly supported CH header/body cuts and cross-record CH. Do not require one callback per contributing physical record. Acquisition may consume several eligible record payloads in one call if available; a physical record boundary alone must not force NeedMoreData while safe current input remains. Each record header is parsed and checked independently and excluded from the logical CH. Non-CH TCP processing retains its existing routes.

**Successful completion projection:**
- recordType = handshakeContent; handshakeType = client_hello.
- For an existing single-record CH, recordLength retains the original record payload length, including when completed in a later TCP segment.
- For a newly supported cross-record CH, recordLength is the declared payload length of the final contributing actual TLS record. It is not the aggregate CH size, not a sum of headers, and not an assertion that the final record physically starts with a new CH header. This is a new working convention, to be checked against actual readers in O04; no width change or synthetic aggregate length is introduced.
- Alert fields remain default zero/false. CH status becomes Complete only after successful parsing and FP completion. Incomplete views are not advertised as complete; no unknown-field placeholder event is needed.
- Exactly one fresh CH completion projection is produced for this acquisition. Existing retained-Complete callback eligibility on later TCP records remains unchanged; this is not a new global callback deduplication rule.
- Callbacks and permitted normal commit use the completing packet. CH count/timestamp are not updated for intermediate fragments. Actual verdicts can prevent normal commit exactly as in section 2.3.
- Preserve the existing prohibition on additional handshake bytes trailing the selected CH within the same final record (T08). Compare against the remaining selected CH extent at that record, not the original total CH size against every contributing record. Do not apply the old one-record equality check in a way that rejects the approved cross-record capability.

**Static trace oracles:**

| Input | Result and observables |
| --- | --- |
| P1 ends inside a record or CH header | Retain bounded acquisition; NeedMoreData; no new diagnostic solely for a supported cut; no record/CH callbacks or CH commit |
| Record A ends inside the CH header/body; B continues in the same packet | Validate B's header and continue the same CH without publishing A; no forced wait for another packet |
| A ends in P1; B completes the CH in P2 | P1 waits; P2 publishes one completion projection with B's declared record payload length, Complete status and existing dispatch/reduction/commit |
| B's complete header is malformed or unsupported by existing record checks | Existing applicable diagnostic and Stop route; no successful CH publication or fabricated violation |
| A pending CH is interrupted by an unrelated non-handshake record | Newly supported acquisition stops as Unsupported; no CH success and no synthetic record callback; no new enforcement decision |
| Listener returns Stop on completion | Existing same-unit reduction applies; no subsequent semantic unit is processed |
| A separate record follows successful CH completion | Process it only after completion feedback; preserve retained-Complete eligibility and state-sensitive TCP handling |

**O04 handoff:** check all Record readers against the final-record-length convention and identify exact valid viewer fields and lifetime. A concrete incompatible reader is a reason to revise this convention in a new version, not permission to silently change existing TCP behavior. This is a bounded design handoff, not an assertion that O04 has passed.

### D03: no-extension and explicitly empty-extension boundary caveats

**CODE FACT [E14,E15].** In the no-extension branch, consumed is getConsumed plus compression length, while getConsumed is offset plus one. At an exact body boundary this can exceed the logical body extent by one. In the explicit-zero-block path, the parser advances by two bytes to the position after the extension length before checking extensionLen == 0. BaseTlsParser::incOffset requires the new position to be within the buffer, not one-past-end. An exact-length empty block can therefore fail while extra backing bytes alter that branch's outcome.

**Static inference, not executed test:** these are concrete boundary/accounting hazards. The existing no-extension branch can return Complete, but its consumed count is not a safe accepted-byte frontier. An explicit empty block is not proven universally supported merely because the code has a zero-length branch. The caller subtracts consumed from a uint16 remaining length; do not preserve unsafe underflow as a compatibility requirement.

**Recommended disposition:** keep O01's checked accepted accounting; preserve successful no-extension CH interpretation and canonical empty delimiters; isolate any changed explicit-empty-block acceptance as a named boundary correction requiring approval and focused regression evidence. Do not widen the offered region with unrelated bytes merely to make the legacy parser succeed. O01's phrase 'where accepted by the applicable parser contract' is a condition, not evidence that this case already works.

**Closure evidence:** exact-length no-extension body, exact-length explicit empty block, each with unrelated following record bytes physically present but excluded from the CH region; verify consumed/accepted bounds and no read beyond the supplied region. This is not permission to change TCP record-length mismatch policy in T08.

### D04: shared failure categories versus legacy soft-stop publication

**CODE FACT [E11,E19-E22].** Many digest append and locator insertion failures return StopProcessing without setting a parsing error. processNewRecord treats a non-error processCiphers result differently from a parser Error: it can fall through, append a record and return StopProcessing. The Conductor Stop branch then dispatches only an anomaly if one exists, not that record. Other capacity failures set RecordsExceedDataViewerCapacity. Thus a record can appear in getAllRecords even when no TLSRecord callback is dispatched for that step.

**Difference:** O01 ResourceFailure invalidates successful CH publication and forbids digest retry, but does not select a TCP step-result/error-viewer/record-list mapping. Mapping all failures to MalformedClientHello, or assuming all failure paths leave the record list untouched, changes behavior. This is relevant even when no runtime allocation is involved.

**Recommended disposition:** preserve established TCP exceptional routing and diagnostics where safe, suppress successful CH publication and never resume a partially modified digest. Define record-list visibility and the error value for each concrete failure site, not one blanket mapping. If an existing soft-stop record-list side effect is intentionally removed, name and approve that difference. O04 validates external readers; O06/O07 validate finite storage/provider behavior.

**Closure evidence:** fault injection at cipher append, extension delimiter/content append, locator insertion, record-vector capacity and segment-storage acquisition. Compare step result, error, record list, callbacks and TLSData. These injections have not been run.

#### D03/D04 working dispositions for design progression

**D03:** preserve existing supported no-extension interpretation and canonical FP delimiters. Never reproduce out-of-bounds reads, consumed underflow or dependence on unrelated backing. Correct accepted-byte accounting in the new bounded path. Any change in explicit-empty-block acceptance remains a separately tracked boundary correction, not silently accepted by migration. O04 may proceed with this exception visible; full migration acceptance remains open until its disposition and focused validation are complete.

**D04 source-derived preservation map [E10-E12,E19-E22]:**

| Failure site | Existing outer result / diagnostic | Record-list and dispatch preservation |
| --- | --- | --- |
| Cipher parser failure | Stop / CipherParsingFailure | No record appended by that branch; anomaly-only dispatch if an error exists |
| Cipher digest append soft stop | Stop / no new diagnostic set by the append failure | New-record caller can append current record; no TLSRecord/CH dispatch on Stop |
| Base-version digest append soft stop | Stop / no new diagnostic set by the append failure | Returns before record append; no successful CH publication |
| Extension delimiter/content digest append or locator insertion | Stop / no new diagnostic set by the failing site | Returns before completion-record append; no successful CH publication; never retry the partially modified digest |
| Record-viewer capacity | Stop / RecordsExceedDataViewerCapacity | Failed push adds no new entry; prior entries remain; no record/CH dispatch for failed step |
| Segment-storage acquisition failure in CH path | Stop / SegmentBufferAllocationFailure | No successful completion record; anomaly route uses actual verdict |

"No new diagnostic" is not a claim that a retained error is always None: resumed entry does not unconditionally reset all fields. Preserve defined association, not stale cross-session values. O04/O05 must audit restoration and external readers. QUIC need not inherit TCP's incidental soft-stop list mutation: use its established failure contract without a fabricated violation. Runtime fault injection remains planned, not a gate for beginning O04.

### 6.1 Findings that remain focused handoffs, not new O03 policy

- **T20 association:** segmented completion falls back to the shared clientHello parser's version when supported_versions was not parsed, even though lastVersion is restored from segment backing. Processor-wide flags/SNI can survive resumed-entry paths. This warrants a concrete O04/O05 interleaving audit; it is not proof of a reproduced corruption or authority to copy stale state. [E07,E10]
- **Backing after completion:** digest copying before segment-buffer release does not by itself prove all locators/current views safe. Preserve O01's last-use rule and prove actual readers in O04/O05. [E10]
- **Malformed memory access:** getDataAtOffsetU24 uses a 32-bit load before shifting. A nominal four-byte handshake header with a length load starting at byte one needs a bounds-safe implementation. Do not treat an out-of-region load as behavior to reproduce. This is a source-level safety concern, not an executed crash claim. [E21; BaseTlsParser::getDataAtOffsetU24]

## 7. Differential validation specification

**PLANNED VALIDATION, NOT RUN.** This is an implementable test specification, not a synthetic packet dataset or an execution report. Existing and proposed implementations must be compared with identical listener scripts and explicit packet/record boundaries. For newly supported cases, compare against the approved target trace, not the old rejection.

Capture at every input and pull: session identity, available region and cursor, CH accepted frontier, step result, error, record-list entries, last record, CH status/eligibility, callback order and payload, actual verdict bits/Stop, TLSData before/after, FP/FullString, and the point backing is released/reused. Keep fresh completion distinct from retained Complete callback eligibility. No raw byte hash-offset assumption.

| Pack | Cases | Required oracle |
| --- | --- | --- |
| V01 | T01-T03, T09-T13, T17-T18 | Source-derived preservation trace, including no event on supported partial extension input and retained Complete callbacks |
| V02 | T04-T07, T14 | D01/D02 dispositions; cut at every byte of record header, handshake header, vector lengths/elements, prefix and extension block; no digest replay |
| V03 | T08-T09 | Preserve mismatch/multiple-CH checks and precedence, with both records in one packet and separated packets; no automatic session-wide two-CH ban |
| V04 | Section 2.3 verdict combinations | Stop at each callback stage and listener priority, with zero/nonzero masks, prior accumulated bits, later bits and expired listeners; assert same-unit versus next-record behavior |
| V05 | T14-T16 | Invalid declared lengths, odd cipher length, extension overruns, SNI failures, supported-version/group failures, absent/empty extensions and physical trailing backing |
| V06 | T19 | D04 fault sites; no fabricated violation or successful CH, no partial digest retry, exact exceptional record-list visibility |
| V07 | T20 and section 6.1 | A/B/A interleaving, supported_versions absent/present, SNI absent/present, early-data flags and backing lifetime; O04/O05 handoff |
| V08 | T11,T17,T18 | CCS/opaque/alert state and alignment paths, serial wrap, existing retransmission/overlap/gap cases; no new overlap policy |

A runtime result must cite the exact input bytes/segmentation, starting state, listener script, implementation revision and actual captured trace. A table of expected outcomes is not evidence of execution. The supplied aggregate does not include the real enforcement/visibility listener implementations or a build/test harness; O03 does not infer their verdicts from callback names.

## 8. V6 static consistency review and progression status

### 8.1 Review scope and results

- Preserved the V4 T01-T20 source columns, evidence index, existing callback/verdict reduction and concrete single-record D01 mappings. Updated only target obligations where the old feedback proposal conflicted with deferred acquisition.
- Rechecked the source routes processRecord, processSegmentedClientHelloContinuation, processNewRecord, processClientHello and Conductor dispatch/commit. NeedMoreData does not dispatch intermediate record/CH callbacks. Stop, Unsupported and malformed diagnostics remain separate.
- Replaced the XR1-XR4 unknown-intermediate-record problem with explicit deferred-publication dispositions. Added a final-record-length convention and interruption policy as working choices, not legacy facts.
- Kept D03 safety/acceptance caveats and supplied D04's concrete source-site preservation map. Neither is erased or mislabeled as runtime PASS.
- O01/O02 contracts are inherited, not independently re-read in V5. No cross-document equivalence PASS is claimed. If their actual text mandates a conflicting per-physical-record barrier, record a focused alignment amendment rather than silently claiming compliance.
- No implementation, packet execution, fault injection, allocation audit or performance measurement was performed. Static review does not establish any of them.

### 8.2 What can progress

**O03 is sufficient to continue the design in O04 with explicit handoffs.** This is not full migration-equivalence approval. The former XR1 publication blocker is removed because its mandatory intermediate-record premise is not retained. Remaining details are assigned to the stage that examines their actual readers, state and backing; they are not waived.

| Handoff | Owner stage | Required outcome |
| --- | --- | --- |
| H01 | O04 | Enumerate actual Record/CH/error viewer readers, valid fields and completion projection compatibility; check final-record-length convention |
| H02 | O04/O05 | A/B/A interleaving, restored version/SNI/flags, borrowed locators and last use before backing reuse |
| H03 | O04, with boundary correction tracked separately | Bounded no-extension/empty-block accounting and explicit record of any changed acceptance |
| H04 | O04/O05 | Preserve D04 source-site outcomes and defined error association; no partial digest retry |
| H05 | O06/O07 | Prepared storage, widths, capacity exhaustion and provider behavior; no invented CH maximum or runtime growth |
| H06 | Implementation validation | Execute V01-V08 and the revised D02 traces; record actual results rather than design expectations |

Stop the next stage on its first concrete correctness contradiction. Do not demand runtime proof before examining interfaces; do not call an unresolved handoff PASS.

### 8.3 TCP/QUIC design direction

1. **TCP:** preserve existing viewers, callback ordering, verdict authority, FP/FullString, state-sensitive non-CH paths and scoped sequence policy wherever possible. Retain the approved arbitrary-byte/cross-record CH extension; do not silently cancel it to simplify migration.
2. **QUIC:** aim for the fullest support within the existing QUIC baseline, reusing the shared CH interpretation/FP and viewer semantics where justified. Do not force TLS record artifacts into QUIC or copy TCP-specific version/record/lifecycle policy into it.
3. **Boundaries remain:** QUIC v1 and the established Initial/CRYPTO/CH scope; client-to-server visibility constraints; no invented Finished signal, new attempt policy, capacity or enforcement responsibility. This O03 correction does not reopen those contracts.
4. **KISS/YAGNI:** a logical responsibility does not require a new class, interface, binding layer, pool or reclaim mechanism. Introduce one only after a concrete need is demonstrated.

## 9. Next design action

Proceed to O04's viewer/interface analysis using H01-H04 and the source-grounded TCP trace. Keep O05-O08 and runtime validation open. Do not restart generic discussion of unknown intermediate handshakeType: no such event is required by the working design. Revisit only a concrete incompatible reader or an actual baseline contradiction.

## 10. Source snapshot identities

- `tls_flow_o01_shared_ch_contract_v4.md`: SHA-256 `53b1c8d6556c5ba57b6bb6f127994d895118334630341abd9f47db9142360a0a`.
- `tls_flow_o02_orchestration_comparison_v2.md`: SHA-256 `21703f9678000b24384669c3ceeaf18da4882238c7a7ffa9204ab4ba7f58aa3f`.
- `TLS-Flow-All-Code.txt`: SHA-256 `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b`.

- `tls_flow_o03_tcp_migration_review_v2.md` (preserved revision input): SHA-256 `6ca22fc41f7df3f83e0322d0ac1c1883e0cfaf1e01c242a63e025048e2283a5e`.

- `tls_flow_o03_tcp_migration_review_v3.md` (preserved V4 revision input): SHA-256 `93c6868e3a1e5ac97bf8a8b4ef8c5d8f34b62483c611b8c7d32298706b663940`.

- `tls_flow_o03_tcp_migration_review_v4.md` (preserved V5 revision base): SHA-256 `83f09d2b864d89897e30d59b851961b0506048d80feef649800b3303818dd960`.

## 11. V6 final document check

V5 is preserved as an intermediate correction snapshot. V6 removes stale cross-record-dependency wording from inherited mapping annotations and fixes the historical section reference. Source input files remain unchanged. The source columns of T01-T20 and M01-M10, the evidence index, and existing callback/verdict description remain preserved. The changed mapping annotations point to D01.6/D02 rather than an unresolved mandatory intermediate-record barrier. The final review checks document consistency and source preservation only; no implementation or runtime PASS is asserted.

- Preserved V5 SHA-256: `19b02b048222d2cdab66c6df1d29cbd3acab02793e5a588d424b61cb730efe5c`.

## 11. Restarted O03 design-progression review after coordination

Review basis: this V7, Consolidated V6, supplied code and unchanged O01 V4/O02 V2. The review was restarted at authority/scope rather than relying on the earlier READY statement. This is a focused static consistency review, not an exhaustive rerun of every inherited code assertion or a migration-equivalence suite.

| Order | Review question | Static result and evidence |
| --- | --- | --- |
| R01 | Are revisions, authority and extension scope explicit? | Checked: V6/V5 inputs retained; C-O03-01 limited to working cross-record CH publication; no architecture/API acceptance |
| R02 | Does D01 distinguish waiting from established contradiction? | Checked against D01.1-D01.6: waiting is not universal; declared-bound contradiction and applicable outer checks retain exceptional routes; no parser-created violation |
| R03 | Is D02 now consistent with consolidated feedback? | Checked: V6 CP04/7.3 now distinguish acquisition records from published units; no intermediate callbacks, one completion projection, barrier before later record |
| R04 | Are D03/D04 corrections and failure views still explicit? | Checked: dispositions and evidence requirements remain inherited, not silently approved; no accepted-byte underflow preservation or generic failure-to-violation mapping |
| R05 | Are actual Conductor semantics preserved? | Checked code L3514-L3625: error/record/eligible CH stages, accumulated bits, zero-mask Stop and post-callback commit; no new immediate same-unit stop rule |
| R06 | Are unresolved reader/ownership/resource details named? | Checked: H01-H04 remain O04/O05 questions; no capacity values, heap fallback, viewer replacement or lifecycle invention |
| R07 | Is progression distinct from migration PASS? | Checked: user-authorized O04 can now start on this projection. No build, runtime, full consumer compatibility or migration PASS |

**Disposition:** the identified cross-document publication conflict is resolved for design progression; proceed to O04 as instructed. This does not close all D01-D04 behavioral approvals, physical lifetime proof, implementation acceptance or runtime validation. A concrete incompatible reader in O04 can require a focused new projection revision; it does not automatically reopen architecture.
