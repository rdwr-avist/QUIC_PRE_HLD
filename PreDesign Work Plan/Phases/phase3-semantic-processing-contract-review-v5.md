# Phase 3: Semantic Processing Contract and Review V5

## Review outcome

**EG1-EG8 PASS at the static semantic-contract boundary. READY FOR PHASE 4: orchestration, dispatch and state commit.** V3 incorporates the user's participation, consumer and preservation decisions; it does not claim implementation compliance, runtime tests, bug repairs or exact archival matrix V2 recovery.

All 131 inventory IDs, 131 reviewed matrix rows, ten operations, 14 lifetime families and 36 focused cases remain. Four consumer-specific contract checks are added in Section 8. Closed U02-U05 are decisions/evidence, not unexplained omissions. Remaining items are bounded implementation obligations or genuine Phase 4 choices, not unknown semantics.

### Contents

1. Scope and authority
2. Affected-input inventory
3. Evidence-backed equivalence matrix and transition coverage
4. Semantic processing contract
5. Cardinality, freshness, ownership and lifetime
6. Required feedback boundaries
7. Focused scenario review
8. Decisions, unresolved items and deferred work
9. KISS/YAGNI concept review
10. Exit-gate evidence and Phase 4 handoff
11. Review procedure, preservation and evidence manifest

## 1. Scope and authority

Scope: semantic equivalence and the processing contract of the affected TLS-flow path for TCP and QUIC v1. Inputs, actual state reads, progress, observations, outcomes, cardinality, view validity and feedback are defined. No classes, traversal owner, full dispatch order, commit mechanism, pools, mitigation, detectors, UI, IPC or unrelated feature design are selected.

Authority precedence: explicit user scope/decisions and workplan; accepted Phase 2 state/attempt policy; verified code for current behavior; prior reports as research leads; proposed semantic guarantees clearly identified. Existing code is not automatically a QUIC requirement. CODE-VERIFIED is static evidence, not runtime validation. PRIOR-REPORT, PHASE2-DECISION, PROPOSAL and OPEN-QUESTION remain distinct.

Inputs used: inventory V5 and exact code bundle; Phase 1/Phase 2/workplan; supplied Step 4 V2, Step 5 V2 and Step 6 V2; historical matrix V1 JSON for supplementary qualitative reasoning. Step 4 V2 has now been supplied in full and matches its preserved publication hash. The supplied matrix V2 was retrieved as document text: its complete 131-row index and available expanded rows inform this review. The local nominal file is still a 57-byte placeholder, and the retrieval truncates during the expanded feedback rows. No exact full-file hash or complete expanded-text parity is certified. Section 3 is an independently reviewed consolidation, not a recovered byte-identical matrix V2.

FRD/SRS are deliberately deferred. Full product requirement mapping is not claimed. Ask only for a focused excerpt/decision when a missing requirement changes this contract. USER-FP-01 is resolved: identical complete CH yields identical fingerprint over TCP/QUIC; packetization/reassembly does not alter it. U04 is closed: preserve the current algorithm/output for both transports. A common fix is a separate future task, not an unresolved transport-equality requirement.

Observation boundary: client-to-server, QUIC v1, Initial keys only. No Handshake/0-RTT/1-RTT payload decryption, server Retry/HRR/Finished/HANDSHAKE_DONE or server parameters are assumed. Visible v1 Long Header type is distinct from protected packet-number/reserved fields. Initial protection success is integrity under public Initial context, not client authentication. Associated client Handshake header is accepted product completion heuristic, not decoded TLS Finished.

D01 is inherited from the supplied Step 6 closure decision: required fingerprint product-capacity exhaustion with reliable evaluation impossible yields Unsupported. It is integrated below, not left as a separate contradictory addendum. It does not certify existing TCP code, redefine malformed syntax, or authorize QUIC-only fingerprint repair.

## 2. Affected-input inventory

All 131 V5 IDs are retained below. Inventory facts and source anchors are preserved, not promoted to mandatory future APIs. Classification appears in Section 3. One-to-one ID coverage is structural evidence only; semantic and reverse traceability review remains separate. Source lines are 1-based in exact extracted files; source hashes are in the manifest.

### I / P3-IN-001: getPayload

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** L7 payload pointer supplied at beginPacket; borrowed packet bytes.
- **Current fact:** uint8_t*: Borrowed packet payload pointer, retained as a pointer for packet processing; bytes are not copied wholesale.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `processor/include/PayloadParams.hpp:43` / `PayloadParams::getPayload`; `processor/src/Processor.cpp:93` / `Processor::captureStepPacketMetadata`.

### I / P3-IN-002: getLength

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Available L7 payload byte count, computed by Conductor from frame length and offsets.
- **Current fact:** uint16_t: Payload length bounds packet-local parsing and participates in expected TCP sequence calculation.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `processor/include/PayloadParams.hpp:47` / `PayloadParams::getLength`; `processor/src/Processor.cpp:93` / `Processor::captureStepPacketMetadata`.

### I / P3-IN-003: getSessionIdx

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Session index supplied by Session; no wire field is a direct substitute.
- **Current fact:** uint32_t: Session index keys continuation lookup/allocation; default constructor argument in the parameterized constructor is 0xffffffff.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `processor/include/PayloadParams.hpp:51` / `PayloadParams::getSessionIdx`; `processor/src/Processor.cpp:93` / `Processor::captureStepPacketMetadata`.

### I / P3-IN-004: getTcpSeq

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** TCP sequence number plus payload length, compared with saved expected sequence using serial arithmetic.
- **Current fact:** uint32_t: TCP sequence participates in exact resume, full retransmission, overlap trimming and gap detection; default parameter is zero.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `processor/include/PayloadParams.hpp:55` / `PayloadParams::getTcpSeq`; `processor/src/Processor.cpp:93` / `Processor::captureStepPacketMetadata`.

### I / P3-IN-005: TLSData input

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** const TLSData& supplied at each processRecord call; no new wire trigger.
- **Current fact:** processRecord accepts const TLSData&. Direct field reads in Processor.cpp are hsState and tlsVersionMask, not all fields of TLSData.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `processor/include/IProcessor.hpp:53` / `IProcessor::processRecord`; `processor/src/Processor.cpp:810` / `Processor::processNewRecord`.

### I / P3-IN-006: hsState

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Stored hsState read during new-record and alert processing; eligibility is state-dependent.
- **Current fact:** Controls Unsupported short-circuit, handshake parsing eligibility, opaque record treatment and plaintext-alert eligibility. Alert continuation captures eligibility when started.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `processor/src/Processor.cpp:810` / `Processor::processNewRecord`; `processor/src/Processor.cpp:745` / `Processor::processAlert`; `processor/src/Processor.cpp:533` / `Processor::startSegmentedAlert`.

### I / P3-IN-007: tlsVersionMask

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Stored advertised tlsVersionMask read for opaque ApplicationData classification.
- **Current fact:** isTLS13(mask) changes ApplicationData opaque-record classification while InProgress. This is an advertised-version mask, not evidence of negotiated version.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `processor/src/Processor.cpp:877` / `Processor::processNewRecord`; `common/include/TLSData.hpp:77` / `isTLS13`.

### I / P3-IN-008: Session and packet identity

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Conductor supplies Session&, DPFrame&, session index and packet timestamp to consumers/state updates. Packet payload length is frame total length minus L7 offset from L2.
- **Current fact:** Conductor supplies Session&, DPFrame&, session index and packet timestamp to consumers/state updates. Packet payload length is frame total length minus L7 offset from L2.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:139` / `TLSFlowConductor::process`.

### I / P3-IN-009: Transport/control observations

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Conductor inspects L4 protocol, SYN-without-ACK and RST-without-ACK before/around processing. These are TCP-specific observations, not QUIC equivalents.
- **Current fact:** Conductor inspects L4 protocol, SYN-without-ACK and RST-without-ACK before/around processing. These are TCP-specific observations, not QUIC equivalents.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:115` / `TLSFlowConductor::process`.

### I / P3-IN-010: State routing

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Finished/Unsupported bypass parsing; Error/TerminatedByAlert use terminal-payload callbacks. Zero payload bypasses processing.
- **Current fact:** Finished/Unsupported bypass parsing; Error/TerminatedByAlert use terminal-payload callbacks. Zero payload bypasses processing.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:134` / `TLSFlowConductor::process`.

### I / P3-IN-011: Session participation

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Subscriber event masks determine extension creation and observer participation; absent extension means no processor invocation.
- **Current fact:** Subscriber event masks determine extension creation and observer participation; absent extension means no processor invocation.
- **Evidence nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:421` / `TLSFlowConductor::buildExtension`; `packet-flow/src/TLSFlowConductor.cpp:59` / `TLSFlowConductor::onInbound`; `packet-flow/include/ITlsListener.hpp:39` / `ITlsListener::getSessionEventMask`.

### I / P3-STATE-001: hsState

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Conductor record/control/unsupported/feedback decisions and reset; current stored value read before normal current-record update.
- **Current fact:** Current accumulated lifecycle state, prior to normal current-record update during record/CH dispatch. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle. Current HsState vocabulary is PreHandshake, InProgress, ChangeCipherSpec, Finished, Error, TerminatedByAlert, Unsupported. isActiveHandshakeState selects the first three; isSilentTerminalState selects Finished/Unsupported; usesTerminalPayloadPath selects Error/TerminatedByAlert. These are current-code predicates, not changes to Phase 2 policy.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:104` / `TLSData::hsState`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`.

### I / P3-STATE-002: tlsVersionMask

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Completed CH normal update replaces mask from CH viewer; reset clears it.
- **Current fact:** Advertised-version mask accumulated from completed CH; also used for alert classification. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:55` / `TLSData::tlsVersionMask`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`; `packet-flow/src/TLSFlowConductor.cpp:501` / `TLSFlowConductor::updateTLSDataOnClientHello`.

### I / P3-STATE-003: clientHelloCount

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Complete CH normal update increments count; reset clears it.
- **Current fact:** uint8_t clientHelloCount increments on a completed CH normal update and is reset with TLSData. It is a bounded stored counter, not an unbounded monotonic count. It gates CCS/ApplicationData handling through the exact zero/nonzero predicate. Exposed through const TLSData&; external field reads are not proven by this bundle.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:106` / `TLSData::clientHelloCount`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`; `packet-flow/src/TLSFlowConductor.cpp:501` / `TLSFlowConductor::updateTLSDataOnClientHello`.

### I / P3-STATE-004: earlyDataCount

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Normal ApplicationData update with clientHelloCount == 0; reset clears uint8_t counter.
- **Current fact:** uint8_t earlyDataCount increments on the normal ApplicationData update when clientHelloCount == 0; it is not independently verified proof that no CH ever occurred. Exposed through const TLSData& to callbacks; external field reads are not proven by this bundle.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:107` / `TLSData::earlyDataCount`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`; `packet-flow/src/TLSFlowConductor.cpp:530` / `TLSFlowConductor::updateTLSDataOnApplicationData`.

### I / P3-STATE-005: earlyDataOffered

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Complete CH normal update copies current CH early_data flag; reset clears it.
- **Current fact:** Committed CH early_data presence; affects TCP TLS13 completion heuristic. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:108` / `TLSData::earlyDataOffered`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`; `packet-flow/src/TLSFlowConductor.cpp:501` / `TLSFlowConductor::updateTLSDataOnClientHello`.

### I / P3-STATE-006: preCcsSeen

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Normal CCS update with clientHelloCount == 0; reset clears uint8_t counter.
- **Current fact:** uint8_t preCcsSeen increments on the normal CCS update when clientHelloCount == 0. Exposed through const TLSData& to callbacks; external field reads are not proven by this bundle.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:109` / `TLSData::preCcsSeen`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`; `packet-flow/src/TLSFlowConductor.cpp:521` / `TLSFlowConductor::updateTLSDataOnChangeCipherSpec`.

### I / P3-STATE-007: chFingerprint

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Complete CH normal update copies fingerprint from viewer; reset defaults it.
- **Current fact:** Value copy of completed CH fingerprint. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:110` / `TLSData::chFingerprint`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`; `packet-flow/src/TLSFlowConductor.cpp:501` / `TLSFlowConductor::updateTLSDataOnClientHello`.

### I / P3-STATE-008: terminalViolationMask

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Existing listener feedback commits OR mask; reset clears it. This is software feedback, not a wire field.
- **Current fact:** Accumulated existing feedback metadata; affects SYN reuse gating. No feature policy is defined here. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:111` / `TLSData::terminalViolationMask`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`; `packet-flow/src/TLSFlowConductor.cpp:567` / `TLSFlowConductor::commitViolation`.

### I / P3-STATE-009: clientHelloTimeStamp

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Complete CH normal update copies session last-packet timestamp; reset clears it.
- **Current fact:** Packet timestamp of last committed completed CH. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:112` / `TLSData::clientHelloTimeStamp`; `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener callback TLSData`; `packet-flow/src/TLSFlowConductor.cpp:501` / `TLSFlowConductor::updateTLSDataOnClientHello`.

### I / P3-STATE-010: TLSData defaults/reset

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** RST no-mask path, extension reset/release/reuse or direct clear/reset operation; no mandatory TLS wire message.
- **Current fact:** clear() restores PreHandshake and zero/false/default values for every field; existing RST path may invoke it. This does not redefine Phase 2 attempt reset. TLSSessionExtension owns TLSData and returns references to it. resetValue replaces it with defaults and clears observer lists. Table release resets and erases the extension; allocation can reclaim an eligible victim. Borrowed callback TLSData references must not be treated as stable snapshots after reset/release/reuse. markReclaimable alone is not immediate release.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `common/include/TLSData.hpp:151` / `TLSData::clear`; `packet-flow/src/TLSFlowConductor.cpp:128` / `TLSFlowConductor::process`; `extension/include/TLSSessionExtension.hpp:49` / `TLSSessionExtension::resetValue`; `extension/include/TLSSessionExtensionTable.hpp:124` / `TLSSessionExtensionTable::release`; `extension/include/TLSSessionExtensionTable.hpp:61` / `TLSSessionExtensionTable::alloc`.

### I / P3-OUT-001: RecordParsed

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** processSingleRecord final ContinueProcessing -> RecordParsed.
- **Current fact:** RecordParsed is the public translation of the final internal ContinueProcessing result. Conductor enters current-record analysis. It does not prove a completed ClientHello, a fresh CH event, successful observer delivery, or a state update; those have separate guards and feedback.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:33` / `IProcessor::TLSStepResult`; `processor/src/Processor.cpp:210` / `Processor::processRecord`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-OUT-002: NeedMoreData

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Final Segmented result or nonempty full-continuation retransmission -> NeedMoreData.
- **Current fact:** Translation of Segmented, or a full continuation retransmission when bytes remain. Conductor returns Continue but stops its record loop because result is not RecordParsed.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:34` / `IProcessor::TLSStepResult`; `processor/src/Processor.cpp:210` / `Processor::processRecord`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-OUT-003: Unsupported

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Continuation gap flag, or final unsupported CH reconstruction outcome -> Unsupported.
- **Current fact:** Translation of unsupported reconstruction or continuation gap. Distinct from ParsingError::UnsupportedRecordType and UnsupportedTlsVersion, which lead to Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:35` / `IProcessor::TLSStepResult`; `processor/src/Processor.cpp:210` / `Processor::processRecord`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-OUT-004: EndOfPacket

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** No remaining packet bytes, or final internal EndOfPacket -> EndOfPacket.
- **Current fact:** No remaining bytes, or internal EndOfPacket (including Unsupported state short-circuit). No current-record dispatch.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:36` / `IProcessor::TLSStepResult`; `processor/src/Processor.cpp:210` / `Processor::processRecord`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-OUT-005: Stop

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Uninitialized packet step, final Error/StopProcessing or default translation -> Stop.
- **Current fact:** Translation of Error/StopProcessing/default or an uninitialized step. Conductor consults getLastError; Stop is not itself an observer Stop or an Error state transition.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:37` / `IProcessor::TLSStepResult`; `processor/src/Processor.cpp:210` / `Processor::processRecord`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-OUT-006: Internal result translation

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** processSingleRecord final return reaches processRecord translation; nested returns may be overridden by caller branches.
- **Current fact:** ContinueProcessing/Segmented/Unsupported/Error/EndOfPacket/StopProcessing are internal RecordProcessResult values. processRecord translates the result returned by processSingleRecord: ContinueProcessing -> RecordParsed; Segmented -> NeedMoreData; Unsupported -> Unsupported; EndOfPacket -> EndOfPacket; Error and StopProcessing -> Stop. This is the final translation, not a guarantee that every nested return propagates unchanged. In particular processNewRecord returns Segmented for an incomplete record before returning its innerProcessResult; see P3-ERR-019 for the cipher soft-stop path.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.hpp:32` / `Processor::RecordProcessResult`; `processor/src/Processor.cpp:210` / `Processor::processRecord`; `processor/src/Processor.cpp:1072` / `Processor::processNewRecord`.

### I / P3-OUT-007: ClientHello status None

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH status reset to None by ordinary packet reset; resume may bypass reset.
- **Current fact:** None is the canonical CH viewer status when no CH has been recognized in the current reset context. It is a stored status, not an independent processing result or wire observation. It is reset by resetStepState; resume paths can bypass that reset.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:25` / `IClientHelloDataViewer::ClientHelloParsingStatus`; `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`; `packet-flow/src/TLSFlowConductor.cpp:554` / `TLSFlowConductor::commitUnsupported`.

### I / P3-OUT-008: ClientHello status Partial

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH recognition or unfinished CH extension continuation sets Partial.
- **Current fact:** Partial is set when CH processing starts and during extension continuation before completion. It can coexist with NeedMoreData, Unsupported or Stop. It is not proof that resumable continuation exists. Conductor uses Partial in the InProgress Unsupported disposition guard.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:26` / `IClientHelloDataViewer::ClientHelloParsingStatus`; `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`; `packet-flow/src/TLSFlowConductor.cpp:554` / `TLSFlowConductor::commitUnsupported`.

### I / P3-OUT-009: ClientHello status Complete

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH digest finalization/completion sets Complete before record-capacity check; no guaranteed callback follows.
- **Current fact:** Explicit viewer status, not an additional public step result or a fresh-event token. Complete is checked for CH callback eligibility and normal state update; Partial is checked in unsupported handling. None is not a completed result. Both ordinary and continuation CH completion set Complete before checking record-viewer capacity. Capacity failure can then produce public Stop while Complete remains visible. Stop dispatch handles the parsing-error channel and does not invoke the normal CH callback. Thus parser completion, successful RecordParsed delivery, and a newly delivered CH observation are distinct.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:27` / `IClientHelloDataViewer::ClientHelloParsingStatus`; `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`; `packet-flow/src/TLSFlowConductor.cpp:554` / `TLSFlowConductor::commitUnsupported`; `processor/src/Processor.cpp:1051` / `Processor::processNewRecord`; `processor/src/Processor.cpp:473` / `Processor::processSegmentedClientHelloContinuation`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-OUT-010: Last error channel

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** getLastError reads canonical error after processing; no new wire trigger from the getter itself.
- **Current fact:** getLastError returns stored m_lastError separately from step result. None suppresses anomaly callback. Error state is not automatic merely because parsing returned Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.cpp:1664` / `Processor::getLastError`; `packet-flow/src/TLSFlowConductor.cpp:268` / `TLSFlowConductor::checkForParsingError`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-ERR-001: None

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Reset/no-error sentinel read; no malformed-wire trigger is implied.
- **Current fact:** No-error sentinel.
- **Evidence nature:** No-error sentinel; not a failure or a fresh observation.
- **Sources:** `processor/include/ParsingError.hpp:14` / `ParsingError::None`; `processor/src/Processor.cpp:120` / `m_lastError assignment None`.

### I / P3-ERR-002: UnsupportedTlsVersion

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for UnsupportedTlsVersion: Assigned in Processor.cpp at extracted-file lines 929. Trigger: a ClientHello record reaches the supported-record-version check and tlsRecord::isSupportedVersion() is false; returns Error/public Stop. This error name is not the public Unsupported result.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 929. Trigger: a ClientHello record reaches the supported-record-version check and tlsRecord::isSupportedVersion() is false; returns Error/public Stop. This error name is not the public Unsupported result.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:15` / `ParsingError::UnsupportedTlsVersion`; `processor/src/Processor.cpp:929` / `m_lastError assignment UnsupportedTlsVersion`.

### I / P3-ERR-003: MalformedTlsRecord

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for MalformedTlsRecord: Assigned in Processor.cpp at extracted-file lines 631, 658, 1548. Triggers: TLS record-header parse is non-Complete (new record or reconstructed alert), or reconstructed alert bytes received exceed the declared total record length (header plus payload). These return Error/public Stop.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 631, 658, 1548. Triggers: TLS record-header parse is non-Complete (new record or reconstructed alert), or reconstructed alert bytes received exceed the declared total record length (header plus payload). These return Error/public Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:16` / `ParsingError::MalformedTlsRecord`; `processor/src/Processor.cpp:631` / `m_lastError assignment MalformedTlsRecord`.

### I / P3-ERR-004: UnsupportedRecordType

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for UnsupportedRecordType: Assigned in Processor.cpp at extracted-file lines 638, 1561. Trigger: the TLS record parser reports an unsupported record type, including reconstructed alert handling. Returns StopProcessing/public Stop, not public Unsupported.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 638, 1561. Trigger: the TLS record parser reports an unsupported record type, including reconstructed alert handling. Returns StopProcessing/public Stop, not public Unsupported.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:17` / `ParsingError::UnsupportedRecordType`; `processor/src/Processor.cpp:638` / `m_lastError assignment UnsupportedRecordType`.

### I / P3-ERR-005: RecordLengthExceedsData

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for RecordLengthExceedsData: Assigned in Processor.cpp at extracted-file lines 646, 1555. Trigger: declared TLS record payload exceeds MaxTlsRecordPayloadSize, including reconstructed alert handling. Returns Error/public Stop; the name does not mean every incomplete packet is an error.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 646, 1555. Trigger: declared TLS record payload exceeds MaxTlsRecordPayloadSize, including reconstructed alert handling. Returns Error/public Stop; the name does not mean every incomplete packet is an error.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:18` / `ParsingError::RecordLengthExceedsData`; `processor/src/Processor.cpp:646` / `m_lastError assignment RecordLengthExceedsData`.

### I / P3-ERR-006: HandshakeLengthNotMatchingRecordLength

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for HandshakeLengthNotMatchingRecordLength: Assigned in Processor.cpp at extracted-file lines 943, 979. Triggers: handshake length plus header is smaller than the TLS record payload; or it remains larger after CH prefix/body parsing without the recordLevelFragmented Unsupported branch. Returns StopProcessing/public Stop. Record-level fragmentation is a distinct Unsupported path.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 943, 979. Triggers: handshake length plus header is smaller than the TLS record payload; or it remains larger after CH prefix/body parsing without the recordLevelFragmented Unsupported branch. Returns StopProcessing/public Stop. Record-level fragmentation is a distinct Unsupported path.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:19` / `ParsingError::HandshakeLengthNotMatchingRecordLength`; `processor/src/Processor.cpp:943` / `m_lastError assignment HandshakeLengthNotMatchingRecordLength`.

### I / P3-ERR-007: MalformedHandshake

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for MalformedHandshake: Assigned in Processor.cpp at extracted-file lines 1533. Trigger: handshake-header parser returns non-Complete; returns Error/public Stop.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 1533. Trigger: handshake-header parser returns non-Complete; returns Error/public Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:20` / `ParsingError::MalformedHandshake`; `processor/src/Processor.cpp:1533` / `m_lastError assignment MalformedHandshake`.

### I / P3-ERR-008: MalformedClientHello

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** No demonstrated runtime producer/wire trigger in supplied Processor; declaration/sentinel only.
- **Current fact:** Declared vocabulary only: no direct assignment in supplied Processor.cpp. Do not invent a reachable producer.
- **Evidence nature:** Declared vocabulary/sentinel only; no demonstrated producer.
- **Sources:** `processor/include/ParsingError.hpp:21` / `ParsingError::MalformedClientHello`.

### I / P3-ERR-009: ClientHelloBodyInvalid

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for ClientHelloBodyInvalid: Assigned in Processor.cpp at extracted-file lines 1515. Trigger: CH parser non-Complete not accepted by the incomplete-status Unsupported or extension-continuation branches; includes Failure. Returns Error/public Stop. See P3-PARSE-002 through P3-PARSE-005.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 1515. Trigger: CH parser non-Complete not accepted by the incomplete-status Unsupported or extension-continuation branches; includes Failure. Returns Error/public Stop. See P3-PARSE-002 through P3-PARSE-005.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:22` / `ParsingError::ClientHelloBodyInvalid`; `processor/src/Processor.cpp:1515` / `m_lastError assignment ClientHelloBodyInvalid`.

### I / P3-ERR-010: MultipleClientHello

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for MultipleClientHello: Assigned in Processor.cpp at extracted-file lines 951. Trigger: a new CH is encountered while the canonical CH status is already Complete in the active processing context; returns Error/public Stop. It is not a session-wide ban; see P3-FB-009.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 951. Trigger: a new CH is encountered while the canonical CH status is already Complete in the active processing context; returns Error/public Stop. It is not a session-wide ban; see P3-FB-009.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:23` / `ParsingError::MultipleClientHello`; `processor/src/Processor.cpp:951` / `m_lastError assignment MultipleClientHello`.

### I / P3-ERR-011: ExtensionsLengthExceedsData

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** No demonstrated runtime producer/wire trigger in supplied Processor; declaration/sentinel only.
- **Current fact:** Declared vocabulary only: no direct assignment in supplied Processor.cpp. Do not invent a reachable producer.
- **Evidence nature:** Declared vocabulary/sentinel only; no demonstrated producer.
- **Sources:** `processor/include/ParsingError.hpp:24` / `ParsingError::ExtensionsLengthExceedsData`.

### I / P3-ERR-012: ExtensionParsingFailure

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for ExtensionParsingFailure: Assigned in Processor.cpp at extracted-file lines 1154, 1176, 1300, 1340, 1376, 1379, 1387. Triggers: invalid processExtensions state; SNI parser non-Complete; supported_versions or supported_groups parser failure; extension parser non-Complete; or consumed-length mismatch. Returns Error/public Stop. Dedicated MalformedSniExtension checks are separate.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 1154, 1176, 1300, 1340, 1376, 1379, 1387. Triggers: invalid processExtensions state; SNI parser non-Complete; supported_versions or supported_groups parser failure; extension parser non-Complete; or consumed-length mismatch. Returns Error/public Stop. Dedicated MalformedSniExtension checks are separate.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:25` / `ParsingError::ExtensionParsingFailure`; `processor/src/Processor.cpp:1154` / `m_lastError assignment ExtensionParsingFailure`.

### I / P3-ERR-013: MalformedSniExtension

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for MalformedSniExtension: processSni assigns MalformedSniExtension when parsed SNI count is zero or greater than two, when the first host_name length is zero or greater than max_sni_length (255), or when strnlen(name, length) differs from the declared length. It returns Error/public Stop. A non-host_name first entry returns ContinueProcessing without setting the parsed-SNI flag; a parser failure instead uses ExtensionParsingFailure.
- **Current fact:** processSni assigns MalformedSniExtension when parsed SNI count is zero or greater than two, when the first host_name length is zero or greater than max_sni_length (255), or when strnlen(name, length) differs from the declared length. It returns Error/public Stop. A non-host_name first entry returns ContinueProcessing without setting the parsed-SNI flag; a parser failure instead uses ExtensionParsingFailure.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:26` / `ParsingError::MalformedSniExtension`; `processor/src/Processor.cpp:1184` / `m_lastError assignment MalformedSniExtension`.

### I / P3-ERR-014: CipherParsingFailure

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for CipherParsingFailure: Assigned in Processor.cpp at extracted-file lines 1433. Trigger: cipher-suite parser returns non-Complete for a nonzero cipher list; returns Error, immediately propagated by processNewRecord and translated to public Stop. Distinct from the cipher soft-stop path in P3-ERR-019.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 1433. Trigger: cipher-suite parser returns non-Complete for a nonzero cipher list; returns Error, immediately propagated by processNewRecord and translated to public Stop. Distinct from the cipher soft-stop path in P3-ERR-019.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:27` / `ParsingError::CipherParsingFailure`; `processor/src/Processor.cpp:1433` / `m_lastError assignment CipherParsingFailure`.

### I / P3-ERR-015: SegmentBufferAllocationFailure

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for SegmentBufferAllocationFailure: Assigned when prepareSegmentBufferForContinuation fails for alert alignment, alert reassembly, opaque tracking or CH extension continuation. Returns StopProcessing/public Stop. Preparation can fail because an existing buffer has a conflicting continuation mode or because allocation/session registration fails; this is not proof of malformed wire data or specifically memory exhaustion.
- **Current fact:** Assigned when prepareSegmentBufferForContinuation fails for alert alignment, alert reassembly, opaque tracking or CH extension continuation. Returns StopProcessing/public Stop. Preparation can fail because an existing buffer has a conflicting continuation mode or because allocation/session registration fails; this is not proof of malformed wire data or specifically memory exhaustion.
- **Evidence nature:** Buffer-preparation failure: resource or conflicting continuation-state outcome, not evidence of malformed TLS.
- **Sources:** `processor/include/ParsingError.hpp:28` / `ParsingError::SegmentBufferAllocationFailure`; `processor/src/Processor.cpp:792` / `m_lastError assignment SegmentBufferAllocationFailure`; `processor/src/Processor.cpp:326` / `Processor::prepareSegmentBufferForContinuation`.

### I / P3-ERR-016: SegmentMismatch

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for SegmentMismatch: Assigned in Processor.cpp at extracted-file lines 1119, 1129. Triggers in FirstSegment extension copying: total extensions length is smaller than continuation nextLength, or calculated bytes to copy exceed m_lengthLeftToParse. The buffer is deallocated and StopProcessing/public Stop is returned.
- **Current fact:** Assigned in Processor.cpp at extracted-file lines 1119, 1129. Triggers in FirstSegment extension copying: total extensions length is smaller than continuation nextLength, or calculated bytes to copy exceed m_lengthLeftToParse. The buffer is deallocated and StopProcessing/public Stop is returned.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/ParsingError.hpp:29` / `ParsingError::SegmentMismatch`; `processor/src/Processor.cpp:1119` / `m_lastError assignment SegmentMismatch`.

### I / P3-ERR-017: RecordsExceedDataViewerCapacity

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for RecordsExceedDataViewerCapacity: Assigned when adding a current/completed record would exceed maxRecords, at Processor.cpp extracted-file lines 483, 524, 736 and 1061; returns StopProcessing, translated to public Stop. CH completion at lines 475 and 1052 precedes its capacity check, so Complete can coexist with this failed current step. Record insertion and normal CH delivery are not guaranteed. This is a result-validity distinction, not a new state policy.
- **Current fact:** Assigned when adding a current/completed record would exceed maxRecords, at Processor.cpp extracted-file lines 483, 524, 736 and 1061; returns StopProcessing, translated to public Stop. CH completion at lines 475 and 1052 precedes its capacity check, so Complete can coexist with this failed current step. Record insertion and normal CH delivery are not guaranteed. This is a result-validity distinction, not a new state policy.
- **Evidence nature:** Record-viewer capacity failure; resource-limit outcome, not malformed TLS.
- **Sources:** `processor/include/ParsingError.hpp:30` / `ParsingError::RecordsExceedDataViewerCapacity`; `processor/src/Processor.cpp:483` / `m_lastError assignment RecordsExceedDataViewerCapacity`; `processor/src/Processor.cpp:473` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:1051` / `Processor::processNewRecord`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-ERR-018: MaxParsingErrorValue

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** No demonstrated runtime producer/wire trigger in supplied Processor; declaration/sentinel only.
- **Current fact:** Enum sentinel, not a runtime error outcome.
- **Evidence nature:** Declared vocabulary/sentinel only; no demonstrated producer.
- **Sources:** `processor/include/ParsingError.hpp:32` / `ParsingError::MaxParsingErrorValue`.

### I / P3-ERR-019: Soft stops without dedicated error

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Condition for Soft stops without dedicated error: Digest append/calculate failure and extension-locator capacity failure can return local StopProcessing without assigning a dedicated ParsingError. Do not equate public Stop with a non-None error. Cipher-specific propagation: processNewRecord immediately propagates only Error from processCiphers; other non-ContinueProcessing results skip extension processing and remain in innerProcessResult. After the record-capacity check and record insertion, an incomplete record returns Segmented (public NeedMoreData), whereas a complete record returns the retained StopProcessing (public Stop). A capacity failure can instead override the outcome with RecordsExceedDataViewerCapacity/Stop. No normal RecordParsed delivery is implied by the cipher soft stop. These are static branch facts, not measured runtime occurrences or approved code fixes.
- **Current fact:** Digest append/calculate failure and extension-locator capacity failure can return local StopProcessing without assigning a dedicated ParsingError. Do not equate public Stop with a non-None error. Cipher-specific propagation: processNewRecord immediately propagates only Error from processCiphers; other non-ContinueProcessing results skip extension processing and remain in innerProcessResult. After the record-capacity check and record insertion, an incomplete record returns Segmented (public NeedMoreData), whereas a complete record returns the retained StopProcessing (public Stop). A capacity failure can instead override the outcome with RecordsExceedDataViewerCapacity/Stop. No normal RecordParsed delivery is implied by the cipher soft stop. These are static branch facts, not measured runtime occurrences or approved code fixes.
- **Evidence nature:** Local capacity/digest soft stop; final progress result depends on caller branches.
- **Sources:** `processor/src/Processor.cpp:1221` / `Processor::feedExtensionsFP`; `processor/src/Processor.cpp:1394` / `Processor::parseAllExtensions`; `processor/src/Md5Digest.cpp:21` / `Md5Digest::calculate`; `processor/src/Processor.cpp:1422` / `Processor::processCiphers`; `processor/src/Processor.cpp:1005` / `Processor::processNewRecord`; `processor/src/Processor.cpp:1060` / `Processor::processNewRecord`; `processor/src/Processor.cpp:210` / `Processor::processRecord`.

### I / P3-CONT-001: None

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Continuation mode None; alignment-only state can still be retained.
- **Current fact:** No explicit reassembly mode; a stored alert alignment offset can still exist.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:23` / `IProcessor::ContinuationMode`; `processor/src/Processor.cpp:406` / `Processor::processSingleRecord`; `processor/src/SegmentBuffer.hpp:23` / `SegmentBuffer::ContinuationMode`.

### I / P3-CONT-002: OpaqueRecordTracking

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Incomplete state-eligible opaque record starts remaining-byte tracking.
- **Current fact:** Tracks remaining record bytes and saved record shape without interpreting opaque content.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:24` / `IProcessor::ContinuationMode`; `processor/src/Processor.cpp:406` / `Processor::processSingleRecord`; `processor/src/SegmentBuffer.hpp:24` / `SegmentBuffer::ContinuationMode`.

### I / P3-CONT-003: ClientHelloExtensionReassembly

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH parser IncompleteExtensionSection, accepted continuation branch and successful preparation.
- **Current fact:** Reconstructs extension section after the CH prefix/body is already available.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:25` / `IProcessor::ContinuationMode`; `processor/src/Processor.cpp:406` / `Processor::processSingleRecord`; `processor/src/SegmentBuffer.hpp:25` / `SegmentBuffer::ContinuationMode`.

### I / P3-CONT-004: AlertRecordReassembly

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Incomplete alert header or alert payload starts alert reassembly.
- **Current fact:** Tracks split alert header/payload and preserves plaintext eligibility/alignment.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/include/IProcessor.hpp:26` / `IProcessor::ContinuationMode`; `processor/src/Processor.cpp:406` / `Processor::processSingleRecord`; `processor/src/SegmentBuffer.hpp:26` / `SegmentBuffer::ContinuationMode`.

### I / P3-CONT-005: Gap flag

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Pending buffer sequence relation reaches the gap branch; isContinuationGap exposes the flag.
- **Current fact:** isContinuationGap exposes packet-local continuation gap. Conductor uses it to decide Unsupported state handling.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.cpp:128` / `Processor::resumeOrDiscardStepContinuation`; `processor/src/Processor.cpp:1086` / `Processor::processExtensions`.

### I / P3-CONT-006: Sequence cases

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** TCP start/end sequence compared with saved expected sequence using serial comparisons.
- **Current fact:** Exact expected sequence resumes; end <= expected under serial arithmetic is full retransmission; overlap crossing expected trims old bytes; remaining case is gap and discards stored continuation.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.cpp:128` / `Processor::resumeOrDiscardStepContinuation`; `processor/src/Processor.cpp:1086` / `Processor::processExtensions`.

### I / P3-CONT-007: Session-scoped continuation

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Session-index lookup selects saved continuation and rebinds Processor storage pointers.
- **Current fact:** Continuation access is keyed by session index, while Processor viewers are instance-local mutable members. Saved state includes remaining lengths, record/handshake types, cipher/compression counts, lastVersion, digest and alert metadata. On CH continuation the code restores cipher/compression counts and lastVersion; it does not restore every canonical CH fact. In particular supported-version mask/parsed flag are not restored here, and the fallback reads the shared parser version. Do not equate session-scoped buffer ownership with complete session-scoped viewer freshness. See P3-VIEW-003 and P3-VIEW-014.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.cpp:265` / `Processor::resumeStepFromSegmentBuffer`; `processor/src/Processor.cpp:1086` / `Processor::processExtensions`; `processor/src/Processor.cpp:432` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:466` / `Processor::processSegmentedClientHelloContinuation`.

### I / P3-CONT-008: Continuation completion

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Saved remaining bytes reach completion; parsing/feeding and capacity guards still determine delivered result.
- **Current fact:** CH/opaque/alert continuation reconstructs current record metadata on completion, but public RecordParsed and observer delivery require subsequent parsing/feeding and record-capacity checks to succeed. A CH can already be Complete when capacity failure produces Stop. Successful completion may leave bytes for another step; finishSegmentBufferContinuation can retain alert alignment state. No unconditional emission or delivery is guaranteed.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.cpp:384` / `Processor::finishSegmentBufferContinuation`; `processor/src/Processor.cpp:1086` / `Processor::processExtensions`.

### I / P3-CONT-009: CH reconstruction boundary

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH parser incomplete status, handshake/record length relation or extension reassembly size bound.
- **Current fact:** Incomplete prefix/body in a segmented CH is Unsupported; incomplete extensions can be continued. Cross-record CH fragmentation is Unsupported; oversized extension reassembly is Unsupported.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.cpp:1456` / `Processor::processClientHello`; `processor/src/Processor.cpp:1086` / `Processor::processExtensions`.

### I / P3-CONT-010: Continuation mode exposure

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** getContinuationMode reads packet-local m_stepContinuationMode; getter has no independent wire trigger.
- **Current fact:** getContinuationMode returns m_stepContinuationMode. Conductor currently consumes isContinuationGap, not getContinuationMode. The field is not proven to be a canonical live-buffer mode at every instant.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.hpp:57` / `Processor::getContinuationMode`; `processor/src/Processor.cpp:56` / `Processor::beginPacket`; `processor/src/Processor.cpp:533` / `Processor::startSegmentedAlert`.

### I / P3-CONT-011: Resource/lifecycle loss

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Continuation allocation/access/deallocation, full-map eviction, session aging or capacity checks; not a unique wire message.
- **Current fact:** SegmentBufferManager may evict oldest entry on full capacity; allocation/access/deallocation have boolean outcomes, aging erases entry. No explicit eviction result is returned through IProcessor. Record as current behavior, not a proposed pool design or violation policy. Existing continuation map capacity is MaxNumberOfSegmentBuffers=1000. SegmentBufferSize=3000 bounds the CH extension reassembly branch; ExtensionsArray::add has its own 1024-locator bound, not the separate declared MaxNumberOfLocators=100. Record viewer capacity is 128 and digest storage is 2870 bytes with a strict append rejection at total >= capacity. These are different limits with different result paths; they are not a proposed QUIC allocation design. Allocation failure does not promise transactional rollback: the manager inserts before checking the new session, and the session-not-found branch returns false without erasing that insertion.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/SegmentBufferManager.cpp:23` / `SegmentBufferManager::allocate`; `processor/src/SegmentBufferManager.cpp:108` / `SegmentBufferManager::onEvent`; `processor/include/Definitions.hpp:18` / `SegmentBufferSize/MaxNumberOfSegmentBuffers`; `processor/src/SegmentBufferManager.hpp:25` / `SegmentBufferManager map capacity`; `processor/src/ExtensionsSorting.hpp:118` / `ExtensionsArray::maxSupportedExtensions`; `processor/src/Processor.hpp:127` / `Processor::maxRecords`; `processor/src/Md5Digest.hpp:37` / `Md5Digest::s_max_buffer_size`.

### I / P3-REC-001: recordType

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** TLS record header type byte, or saved type reconstructed on continuation completion.
- **Current fact:** Raw uint8_t TLS record type; predicates identify handshake/CCS/alert/ApplicationData.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/Record.hpp:10` / `Record::recordType`; `processor/src/Processor.cpp:745` / `Processor::processAlert`.

### I / P3-REC-002: handshakeType

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Handshake header type byte only when handshake parsing runs; otherwise default/saved metadata.
- **Current fact:** Raw uint8_t handshake type, populated only when handshake parsing runs; opaque handshake may retain default/saved value.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/Record.hpp:11` / `Record::handshakeType`; `processor/src/Processor.cpp:745` / `Processor::processAlert`.

### I / P3-REC-003: recordLength

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** TLS record header declared payload length, or saved length reconstructed on continuation completion.
- **Current fact:** uint16_t record payload length, excluding TLS record header.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/Record.hpp:12` / `Record::recordLength`; `processor/src/Processor.cpp:745` / `Processor::processAlert`.

### I / P3-REC-004: alertLevel

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Two-byte readable plaintext alert parser produces level.
- **Current fact:** Alert level, meaningful only with readableAlert.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/Record.hpp:13` / `Record::alertLevel`; `processor/src/Processor.cpp:745` / `Processor::processAlert`.

### I / P3-REC-005: alertDescription

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Two-byte readable plaintext alert parser produces description.
- **Current fact:** Alert description, meaningful only with readableAlert.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/Record.hpp:14` / `Record::alertDescription`; `processor/src/Processor.cpp:745` / `Processor::processAlert`.

### I / P3-REC-006: readableAlert

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Alert record, plaintext eligibility, zero alignment offset and two-byte parse conditions produce readableAlert.
- **Current fact:** Validity discriminator; requires plaintext-eligible state, alignment and exactly two alert bytes in current parsing path.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/Record.hpp:15` / `Record::readableAlert`; `processor/src/Processor.cpp:745` / `Processor::processAlert`.

### I / P3-REC-007: getLastRecord

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** getLastRecord borrows canonical m_record after current step; getter is not a wire event.
- **Current fact:** Returns const reference to mutable canonical m_record; Conductor consumes it per RecordParsed.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/IRecordsDataViewer.hpp:18` / `IRecordsDataViewer::getLastRecord`; `processor/src/Processor.cpp:1640` / `Processor::getLastRecord`.

### I / P3-REC-008: getAllRecords

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** getAllRecords borrows bounded records pushed by processing; getter is not a whole-packet completion event.
- **Current fact:** Returns const reference to bounded m_records collection; capacity 128. Contains records pushed during current/reset-or-resume context, not a guaranteed whole-packet immutable batch.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/IRecordsDataViewer.hpp:16` / `IRecordsDataViewer::getAllRecords`; `processor/src/Processor.cpp:1635` / `Processor::getAllRecords`.

### I / P3-REC-009: getRecordType

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** getRecordType copies canonical current record type; no new wire trigger.
- **Current fact:** Scalar copy of current m_record.recordType.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/IRecordsDataViewer.hpp:19` / `IRecordsDataViewer::getRecordType`; `processor/src/Processor.cpp:1648` / `Processor::getRecordType`.

### I / P3-REC-010: getRecordLength

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** getRecordLength copies canonical current record length; no new wire trigger.
- **Current fact:** Scalar copy of current m_record.recordLength.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `processor/include/IRecordsDataViewer.hpp:20` / `IRecordsDataViewer::getRecordLength`; `processor/src/Processor.cpp:1653` / `Processor::getRecordLength`.

### I / P3-REC-011: TLSRecordContext::record

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Conductor builds current TLSRecordContext borrowing getLastRecord on RecordParsed analysis.
- **Current fact:** Borrowed const Record& valid during current-record dispatch, per header contract.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `packet-flow/include/TLSRecordContext.hpp:13` / `TLSRecordContext::record`; `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`.

### I / P3-REC-012: TLSRecordContext::currentClientHelloSupportedVersionsMask

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Conductor reads supported-version viewer only if current record.isClientHello(); otherwise zero.
- **Current fact:** Current CH mask for CH-shaped record; zero otherwise.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `packet-flow/include/TLSRecordContext.hpp:22` / `TLSRecordContext::currentClientHelloSupportedVersionsMask`; `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`.

### I / P3-REC-013: TLSRecordContext::earlyDataOffered

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Conductor reads early-data viewer only if current record.isClientHello(); otherwise false.
- **Current fact:** Current CH early-data flag for CH-shaped record; false otherwise.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `packet-flow/include/TLSRecordContext.hpp:23` / `TLSRecordContext::earlyDataOffered`; `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`.

### I / P3-REC-014: TLSRecordContext::alertTerminating

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Readable alert record plus stored advertised-version mask invokes isTerminatingAlert.
- **Current fact:** Conductor-derived bool from committed version mask and readable alert fields; defaults false.
- **Evidence nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **Sources:** `packet-flow/include/TLSRecordContext.hpp:24` / `TLSRecordContext::alertTerminating`; `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`.

### I / P3-VIEW-001: getFingerprint

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH digest finalization produces m_fp; getter copies retained value without a fresh-event discriminator.
- **Current fact:** Value copy of canonical m_fp; no per-call success bool.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:35` / `IClientHelloDataViewer::getFingerprint`; `processor/src/Processor.cpp:1591` / `Processor::getFingerprint`.

### I / P3-VIEW-002: getSni

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** SNI extension parsing/validation sets copied Sni and parsed flag; getter tests flag.
- **Current fact:** Copies canonical Sni only if parsed flag is true; returns false otherwise. Conductor ignores bool and supplies default empty Sni. The first host_name is validated and copied into owned Sni storage; the boolean distinguishes unavailable/non-host_name SNI from a successful value. This availability is not represented separately in ClientHelloData.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:43` / `IClientHelloDataViewer::getSni`; `processor/src/Processor.cpp:1596` / `Processor::getSni`; `processor/src/Processor.cpp:1169` / `Processor::processSni`.

### I / P3-VIEW-003: getSupportedVersionBitMask

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** supported_versions feeding ORs version bits, or CH wire-version fallback when parsed flag is false.
- **Current fact:** Advertised mask from supported_versions, with CH wire-version fallback when absent. feedSuppVersionsFP ORs versions into m_supportedVersionsBitMask and sets m_supportedVersionsParsed; ordinary reset clears both, but beginPacket resume bypasses that reset. CH continuation restores lastVersion from its buffer, yet its fallback uses the shared ClientHello parser getVersion() when m_supportedVersionsParsed is false. Session-scoped continuation therefore does not itself prove current-session mask freshness. This is a static interleaving risk, not a verified runtime corruption.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:51` / `IClientHelloDataViewer::getSupportedVersionBitMask`; `processor/src/Processor.cpp:1581` / `Processor::getSupportedVersionBitMask`; `processor/src/Processor.cpp:56` / `Processor::beginPacket`; `processor/src/Processor.cpp:258` / `Processor::clearStepClientHelloValues`; `processor/src/Processor.cpp:432` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:466` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:1294` / `Processor::feedSuppVersionsFP`.

### I / P3-VIEW-004: isEarlyDataOffered

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** parseAllExtensions encounters early_data type; getter reads retained flag.
- **Current fact:** Presence of early_data extension, not observed early-data packets.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:56` / `IClientHelloDataViewer::isEarlyDataOffered`; `processor/src/Processor.cpp:1586` / `Processor::isEarlyDataOffered`.

### I / P3-VIEW-005: getFullString

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Digest append/finalization constructs byte buffer; getter returns borrowed pointer/length, not a new CH event.
- **Current fact:** Borrowed pointer and length into active digest buffer, not an owned string.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:61` / `IClientHelloDataViewer::getFullString`; `processor/src/Processor.cpp:1607` / `Processor::getFullString`.

### I / P3-VIEW-006: getTlsVersion

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Record version before CH, CH wire version, or first non-GREASE supported version updates lastVersion.
- **Current fact:** lastVersion; may start from record version, then CH version, then first non-GREASE supported version.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:70` / `IClientHelloDataViewer::getTlsVersion`; `processor/src/Processor.cpp:1576` / `Processor::getTlsVersion`.

### I / P3-VIEW-007: getCipherCount

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Successful cipher parsing sets count; continuation restores saved count.
- **Current fact:** Parsed cipher count, not necessarily count of bytes actually fed to digest.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:79` / `IClientHelloDataViewer::getCipherCount`; `processor/src/Processor.cpp:1612` / `Processor::getCipherCount`.

### I / P3-VIEW-008: getExtensionCount

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Successful locator collection/sort sets count.
- **Current fact:** Count of collected extension locators.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:89` / `IClientHelloDataViewer::getExtensionCount`; `processor/src/Processor.cpp:1617` / `Processor::getExtensionCount`.

### I / P3-VIEW-009: getCompressionCount

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH body parsing sets compression count; continuation restores saved count.
- **Current fact:** CH compression length/count.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:98` / `IClientHelloDataViewer::getCompressionCount`; `processor/src/Processor.cpp:1622` / `Processor::getCompressionCount`.

### I / P3-VIEW-010: isSegmented

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** ProcessorState changes at continuation start/resume/completion; getter compares it to SingleSegment.
- **Current fact:** Computed as ProcessorState != SingleSegment; completion resets to SingleSegment, so not durable historical segmentation metadata.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:100` / `IClientHelloDataViewer::isSegmented`; `processor/src/Processor.cpp:1627` / `Processor::isSegmented`.

### I / P3-VIEW-011: getClientHelloParsingStatus

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH recognition/continuation/completion or packet reset changes canonical status.
- **Current fact:** Canonical status persists across record steps until reset/updated; it is not a fresh-event discriminator. Complete can remain visible after record-capacity failure returns Stop, since it is set before the capacity check. Normal CH callback delivery requires the RecordParsed analysis path and applicable feedback; the Stop path does not dispatch that callback. See P3-OUT-009 and P3-FB-008.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IClientHelloDataViewer.hpp:109` / `IClientHelloDataViewer::getClientHelloParsingStatus`; `processor/src/Processor.cpp:1659` / `Processor::getClientHelloParsingStatus`; `processor/src/Processor.cpp:1051` / `Processor::processNewRecord`; `processor/src/Processor.cpp:473` / `Processor::processSegmentedClientHelloContinuation`; `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-VIEW-012: Parsing-error viewer

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IParsingErrorViewer getter reads stored error; no independent wire trigger.
- **Current fact:** IParsingErrorViewer::getLastError exposes canonical error by value, not per-record owned result.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/include/IParsingErrorViewer.hpp:16` / `IParsingErrorViewer::getLastError`; `processor/src/Processor.cpp:1664` / `Processor::getLastError`.

### I / P3-VIEW-013: Shared instance wiring

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Factory init injects one Processor instance into all viewer interfaces; software initialization, not a wire event.
- **Current fact:** Factory injects the same Processor instance as IProcessor, CH viewer, record viewer and error viewer, and exposes those interfaces publicly. Results are not per-session immutable viewer objects.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `context/engine/src/Factory.cpp:44` / `Factory::init`; `context/engine/src/Factory.cpp:51` / `Factory::getProcessorViewer`.

### I / P3-VIEW-014: Reset/freshness boundary

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** beginPacket reset versus resumed early-return branch; later processRecord calls do not automatically reset CH facts.
- **Current fact:** beginPacket resets ordinary new-packet CH/viewer/error state through resetStepState. A resumed path returns early and clears only the record collection/current record for non-retransmission/non-gap resume. There is no automatic per-record reset of CH Complete. resumeStepFromSegmentBuffer rebinds buffer/digest/locators and selects continuation mode, but does not restore/reset every canonical CH field. Supported-version mask and parsed flag, early-data/SNI facts and other shared viewer fields must not be assumed session-owned merely because continuation storage is keyed by session. See P3-VIEW-003 for the concrete version-mask/fallback dependency and P3-CONT-007 for the limited saved CH fields.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/src/Processor.cpp:56` / `Processor::beginPacket`; `processor/src/Processor.cpp:104` / `Processor::resetStepState`; `processor/src/Processor.cpp:210` / `Processor::processRecord`; `processor/src/Processor.cpp:265` / `Processor::resumeStepFromSegmentBuffer`; `processor/src/Processor.cpp:432` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:1294` / `Processor::feedSuppVersionsFP`.

### I / P3-VIEW-015: Digest lifetime

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CH digest init/append, continuation digest copy, active-pointer rebinding and later storage reuse.
- **Current fact:** getFullString borrows active digest storage. CH continuation copies digest to initial storage before finishing continuation; cleanup resets active pointers, and later CH digest init/calculate overwrites contents. Pointer identity and freshness are not session ownership.
- **Evidence nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **Sources:** `processor/src/Processor.cpp:1607` / `Processor::getFullString`; `processor/src/Processor.cpp:473` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:305` / `Processor::resetContinuationPointers`; `processor/src/Md5Digest.cpp:15` / `Md5Digest::init`.

### I / P3-FP-001: Existing composition

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Existing composition: CH wire fields or local digest/normalization operation described below.
- **Current fact:** Existing feed path hashes CH wire version, cipher values and sorted extension types; selected extension lengths/content are included. This inventories actual implementation, not a new fingerprint requirement.
- **Evidence nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **Sources:** `processor/src/Processor.cpp:983` / `Processor::processNewRecord`; `processor/src/Processor.cpp:1422` / `Processor::processCiphers`; `processor/src/Processor.cpp:1221` / `Processor::feedExtensionsFP`.

### I / P3-FP-002: QUIC transport parameter content

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** QUIC transport parameter content: CH wire fields or local digest/normalization operation described below.
- **Current fact:** isExtensionContentIncludedInFp explicitly includes quic_transport_parameters and draft type; feedExtensionsFP default branch hashes raw extension content after type/length. This is code evidence, not resolution of the deferred QUIC product requirement.
- **Evidence nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **Sources:** `processor/src/Processor.hpp:234` / `Processor::isExtensionContentIncludedInFp`; `processor/src/Processor.cpp:1274` / `Processor::feedExtensionsFP`.

### I / P3-FP-003: Normalization/sorting

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Normalization/sorting: CH wire fields or local digest/normalization operation described below.
- **Current fact:** Extension type is deGrease-normalized; locators sort with comparator by type/length/value. Supported versions/groups use specialized deGrease feeding. Preserve exact comparator semantics, not comments alone.
- **Evidence nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **Sources:** `processor/src/Processor.cpp:1392` / `Processor::parseAllExtensions`; `processor/src/ExtensionsSorting.hpp:63` / `ExtensionLocator::extensionsSortingComparison`; `processor/src/Processor.cpp:1294` / `Processor::feedSuppVersionsFP`; `processor/src/Processor.cpp:1333` / `Processor::feedSuppGroupsFP`.

### I / P3-FP-004: Digest capacity/failure

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Digest capacity/failure: CH wire fields or local digest/normalization operation described below.
- **Current fact:** Md5Digest.calculate rejects zero length or len + current index >= 2870-byte array size; failure can stop processing without a dedicated error. finalize computes MD5 over accumulated bytes. The local failure must be traced through its caller: cipher feeding can become NeedMoreData on an incomplete record or Stop on a complete record, subject to the record-capacity check; see P3-ERR-019. Other feeding failures must not be assigned that cipher-specific path automatically.
- **Evidence nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **Sources:** `processor/src/Md5Digest.cpp:21` / `Md5Digest::calculate`; `processor/src/Md5Digest.hpp:37` / `Md5Digest::s_max_buffer_size`; `processor/src/Processor.cpp:1005` / `Processor::processNewRecord`; `processor/src/Processor.cpp:1072` / `Processor::processNewRecord`.

### I / P3-FP-005: Cipher feeding loop detail

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Cipher feeding loop detail: CH wire fields or local digest/normalization operation described below.
- **Current fact:** processCiphers uses for (int i = 0; cipherSuitesLen -= 2; i++). For an even nonzero validated length, the last suite is not fed by this loop. Do not replace actual behavior with an assumption that all counted suites are hashed; no repair is made here.
- **Evidence nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **Sources:** `processor/src/Processor.cpp:1440` / `Processor::processCiphers`.

### I / P3-OBS-001: Completed CH

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Completed CH status on a CH-shaped record, successful normal update path, packet timestamp.
- **Current fact:** On the normal-update path for a CH-shaped record, Complete status permits count increment and replacement of timestamp, fingerprint, version mask and early-data flag. PreHandshake becomes InProgress. Complete alone is not sufficient: step delivery, current record shape and observer feedback can prevent this update.
- **Evidence nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:501` / `TLSFlowConductor::updateTLSDataOnClientHello`.

### I / P3-OBS-002: CCS

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** CCS-shaped record plus committed CH count, version mask and hsState.
- **Current fact:** On the normal-update path for a CCS-shaped record, clientHelloCount == 0 increments preCcsSeen; otherwise a non-TLS13 mask with InProgress changes state to ChangeCipherSpec. The zero-count predicate is a stored uint8_t counter check, not independently verified absence of every earlier CH.
- **Evidence nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:521` / `TLSFlowConductor::updateTLSDataOnChangeCipherSpec`.

### I / P3-OBS-003: Handshake after CCS

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Handshake-shaped record while stored hsState is ChangeCipherSpec; no decoded TLS Finished is required by current code.
- **Current fact:** On the normal-update path, a handshake-shaped record while stored hsState is ChangeCipherSpec changes state to Finished. The encrypted TLS Finished content is not decoded to prove this; this is a completion heuristic, subject to feedback allowing normal update.
- **Evidence nature:** Heuristic completion inference from record shape and prior state, not direct observation of TLS Finished.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:469` / `TLSFlowConductor::commitNormalCurrentRecord`.

### I / P3-OBS-004: ApplicationData

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** ApplicationData-shaped record plus CH count, advertised TLS13 bit, InProgress and !earlyDataOffered; no decoded TLS Finished is observed.
- **Current fact:** On the normal-update path for ApplicationData, clientHelloCount == 0 increments earlyDataCount. Otherwise an advertised TLS13 bit with InProgress and !earlyDataOffered changes state to Finished. This is the current TCP completion heuristic, not a decoded Finished observation.
- **Evidence nature:** Heuristic completion inference from record shape and prior state, not direct observation of TLS Finished.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:530` / `TLSFlowConductor::updateTLSDataOnApplicationData`.

### I / P3-OBS-005: Readable terminating alert

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Alert-shaped record, readableAlert, derived alertTerminating and active hsState.
- **Current fact:** Active state + alert record + readableAlert + alertTerminating -> TerminatedByAlert.
- **Evidence nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:541` / `TLSFlowConductor::updateTLSDataOnAlert`.

### I / P3-OBS-006: Unsupported disposition

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Public Unsupported plus continuation-gap flag, hsState and CH Partial status.
- **Current fact:** Gap, PreHandshake, or InProgress with Partial CH -> Unsupported. Public Unsupported alone does not imply unconditional state transition.
- **Evidence nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:554` / `TLSFlowConductor::commitUnsupported`.

### I / P3-OBS-007: TCP reset

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** TCP RST accepted by existing legal-RST predicate with no legal ACK, plus listener feedback.
- **Current fact:** RST without ACK triggers callback; no violation mask clears TLSData even if dispatch Stop. Nonzero mask updates existing feedback state.
- **Evidence nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:109` / `TLSFlowConductor::process`.

### I / P3-OBS-008: TCP session reuse observation

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** TCP SYN-without-ACK predicate plus active-state/violation metadata test.
- **Current fact:** For a TCP SYN-without-ACK predicate, an existing active extension with no recorded violation is preserved. An existing extension failing that test is released and rebuilt; if no extension exists, creation is attempted directly. This is current TCP entry behavior, not Phase 2 QUIC attempt policy.
- **Evidence nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:74` / `TLSFlowConductor::prepareInboundExtension`.

### I / P3-OBS-009: Finished reclamation observation

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Normal-update transition into Finished calls markReclaimable; software lifecycle attempt, not an independent wire event.
- **Current fact:** After a normal current-record state change into Finished, Conductor calls markReclaimable when the extension table exists. The method returns bool, but the caller ignores it; successful marking is not guaranteed by this call. Marking does not immediately release the extension. This is existing lifecycle evidence, not pool design.
- **Evidence nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`; `extension/include/TLSSessionExtensionTable.hpp:136` / `TLSSessionExtensionTable::markReclaimable`.

### I / P3-OBS-010: Alert terminal classifier

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Readable alert level/description plus advertised-version mask; derived terminal classification.
- **Current fact:** Existing helper rejects invalid levels; close_notify is terminal; TLS13-only excludes user_canceled; TLS12-or-less-only uses fatal level; mixed/unknown mask uses fatal except user_canceled. Inventory of helper behavior only, not independent protocol verification.
- **Evidence nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **Sources:** `common/include/TLSAlert.hpp:14` / `isTerminatingAlert`.

### I / P3-CB-001: onTLSRecord

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** RecordParsed analysis invokes onTLSRecord with current record context before normal state update.
- **Current fact:** Receives Session&, DPFrame&, borrowed TLSRecordContext and const TLSData& before normal update.
- **Evidence nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **Sources:** `packet-flow/include/ITlsListener.hpp:64` / `ITlsListener::onTLSRecord`; `packet-flow/src/TLSFlowConductor.cpp:320` / `TLSFlowConductor callback onTLSRecord`.

### I / P3-CB-002: onClientHello

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** RecordParsed analysis and retained CH status Complete, subject to feedback; not necessarily a new CH-shaped record.
- **Current fact:** onClientHello receives Session&, DPFrame&, a stack-local ClientHelloData projection and const TLSData&. ClientHelloData stores const references to the stack-local fingerprint and Sni values created in onClientHello; it does not own copies of those values. Its TLS version is a converted Misc::TLSVersion, and its three counts are uint16_t constructor fields fed by uint32_t viewer getters. These references must not be treated as valid after the synchronous callback returns; retaining the projection by value does not make the referents owned. Callback eligibility is retained Complete status on RecordParsed analysis, not a fresh-event token or necessarily a CH-shaped current record.
- **Evidence nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **Sources:** `packet-flow/include/ITlsListener.hpp:44` / `ITlsListener::onClientHello`; `packet-flow/src/TLSFlowConductor.cpp:307` / `TLSFlowConductor callback onClientHello`; `packet-flow/include/ClientHelloData.hpp:35` / `ClientHelloData constructor and reference fields`.

### I / P3-CB-003: onParsingError

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Non-None stored parsing error and subscribed anomaly listeners on analysis/Stop path.
- **Current fact:** Receives error by value plus Session&, DPFrame&, const TLSData&; called only for non-None error and nonempty anomaly observers.
- **Evidence nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **Sources:** `packet-flow/include/ITlsListener.hpp:49` / `ITlsListener::onParsingError`; `packet-flow/src/TLSFlowConductor.cpp:282` / `TLSFlowConductor callback onParsingError`.

### I / P3-CB-004: onTCPReset

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Observed TCP reset control predicate.
- **Current fact:** Receives Session&, DPFrame&, const TLSData& for observed TCP reset.
- **Evidence nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **Sources:** `packet-flow/include/ITlsListener.hpp:54` / `ITlsListener::onTCPReset`; `packet-flow/src/TLSFlowConductor.cpp:117` / `TLSFlowConductor callback onTCPReset`.

### I / P3-CB-005: onSessionEnd

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Session aging event before extension release; no required wire trigger.
- **Current fact:** Receives Session&, const TLSData& during aging before extension release; notify result is ignored by onAging.
- **Evidence nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **Sources:** `packet-flow/include/ITlsListener.hpp:59` / `ITlsListener::onSessionEnd`; `packet-flow/src/TLSFlowConductor.cpp:359` / `TLSFlowConductor callback onSessionEnd`.

### I / P3-CB-006: onTerminalPayload

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Nonempty payload while stored state uses terminal-payload path; parsing is bypassed.
- **Current fact:** Receives Session&, DPFrame&, const TLSData& without processor parsing in Error/TerminatedByAlert path.
- **Evidence nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **Sources:** `packet-flow/include/ITlsListener.hpp:69` / `ITlsListener::onTerminalPayload`; `packet-flow/src/TLSFlowConductor.cpp:336` / `TLSFlowConductor callback onTerminalPayload`.

### I / P3-FB-001: Step-by-step state feedback

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point Step-by-step state feedback; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** Conductor calls processRecord(tlsData), dispatches current result, then checks updated active state before another call. No explicit accept/commit callback is passed to Processor; feedback is control flow and next-call state.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:109` / `TLSFlowConductor::process`.

### I / P3-FB-002: Observer feedback shape

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point Observer feedback shape; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** TLSFeatureVerdict carries independent Dispatch Continue/Stop and violationMask. These are not parser results, packet-drop instructions or automatic Error transitions.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `packet-flow/include/TLSFeatureVerdict.hpp:12` / `TLSFeatureVerdict`.

### I / P3-FB-003: Within-list STOP

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point Within-list STOP; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** notify accumulates existing mask metadata and stops the current observer list immediately on Dispatch::Stop; expired weak observers are skipped.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `packet-flow/src/TLSFlowConductor.hpp:102` / `TLSFlowConductor::notify`.

### I / P3-FB-004: Across-stage STOP without mask

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point Across-stage STOP without mask; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** analyzeProcessResult sets sawStop and can still run later stages/current normal update when Stop has no accumulated mask. It returns Stop after current processing; next record is not processed.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`.

### I / P3-FB-005: Feedback that suppresses normal update

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point Feedback that suppresses normal update; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** Nonzero accumulated mask suppresses normal current-record update. Combined Stop causes Error; mask without Stop records metadata without state change. Existing boundary behavior only, no policy design.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:195` / `TLSFlowConductor::analyzeProcessResult`.

### I / P3-FB-006: Parser STOP handling

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point Parser STOP handling; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** Public Stop runs parsing-error observers only. With no non-None error or no observers, verdict is default Continue; outer record loop still ends because result is Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:167` / `TLSFlowConductor::dispatchStepResult`.

### I / P3-FB-007: State gate after record

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point State gate after record; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** Even RecordParsed does not guarantee another call: observer Stop returns immediately; non-active updated state ends loop. Remaining bytes can be left unprocessed.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:109` / `TLSFlowConductor::process`.

### I / P3-FB-008: CH callback cardinality coupling

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point CH callback cardinality coupling; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** CH callback condition tests Complete status, not record.isClientHello(). Complete persists across later steps. Thus a later RecordParsed non-CH record in the same packet can trigger another CH callback with retained CH facts, if prior feedback/state permits reaching it. Record context CH fields are still zero/false for that non-CH record. This is reachable code behavior, not an endorsed contract.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `packet-flow/src/TLSFlowConductor.cpp:240` / `TLSFlowConductor::analyzeProcessResult`; `processor/src/Processor.cpp:210` / `Processor::processRecord`; `processor/src/Processor.cpp:104` / `Processor::resetStepState`.

### I / P3-FB-009: Multiple CH scope

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** Software control point Multiple CH scope; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Current fact:** Processor rejects a new CH when its canonical status is already Complete. Ordinary beginPacket resets status; this is not an unconditional prohibition on a second CH across the whole session. Do not import it as Phase 2 attempt policy.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/Processor.cpp:950` / `Processor::processNewRecord`; `processor/src/Processor.cpp:104` / `Processor::resetStepState`.

### I / P3-PARSE-001: IClientHelloParser::Status::Complete

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IClientHelloParser::Status::Complete returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal ClientHello parser Complete means the CH prefix/body parse returned Complete. processClientHello advances its record-local pointer by consumed bytes and returns ContinueProcessing. This is not yet a completed fingerprint or a public RecordParsed result: processNewRecord still applies handshake/record-length checks, version/cipher/extension feeding, completion and capacity checks. Those later operations may yield Unsupported, Stop, or continuation. See P3-OUT-006 and P3-OUT-009.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:60` / `IClientHelloParser::Status`; `processor/src/Processor.cpp:1456` / `Processor::processClientHello`; `processor/src/Processor.cpp:971` / `Processor::processNewRecord`.

### I / P3-PARSE-002: IClientHelloParser::Status::IncompleteClientHelloPrefix

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IClientHelloParser::Status::IncompleteClientHelloPrefix returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal CH parser status. If recordLevelFragmented is true, processClientHello returns Unsupported before starting continuation. Otherwise, if m_currentRecordIsSegmented is true, it returns Unsupported for the incomplete prefix/body. If neither condition holds, it assigns ClientHelloBodyInvalid and returns Error. The caller propagates these outcomes: Unsupported -> public Unsupported; Error -> public Stop. Neither incomplete prefix nor incomplete body is accepted as extension-only continuation.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:60` / `IClientHelloParser::Status`; `processor/src/Processor.cpp:1456` / `Processor::processClientHello`; `processor/src/Processor.cpp:1464` / `Processor::processClientHello`; `processor/src/Processor.cpp:1505` / `Processor::processClientHello`; `processor/src/Processor.cpp:210` / `Processor::processRecord`.

### I / P3-PARSE-003: IClientHelloParser::Status::IncompleteClientHelloBody

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IClientHelloParser::Status::IncompleteClientHelloBody returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal CH parser status. If recordLevelFragmented is true, processClientHello returns Unsupported before starting continuation. Otherwise, if m_currentRecordIsSegmented is true, it returns Unsupported for the incomplete prefix/body. If neither condition holds, it assigns ClientHelloBodyInvalid and returns Error. The caller propagates these outcomes: Unsupported -> public Unsupported; Error -> public Stop. Neither incomplete prefix nor incomplete body is accepted as extension-only continuation.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:60` / `IClientHelloParser::Status`; `processor/src/Processor.cpp:1456` / `Processor::processClientHello`; `processor/src/Processor.cpp:1464` / `Processor::processClientHello`; `processor/src/Processor.cpp:1505` / `Processor::processClientHello`; `processor/src/Processor.cpp:210` / `Processor::processRecord`.

### I / P3-PARSE-004: IClientHelloParser::Status::IncompleteExtensionSection

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IClientHelloParser::Status::IncompleteExtensionSection returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal CH parser status. recordLevelFragmented first yields Unsupported/public Unsupported. Otherwise processClientHello attempts ClientHelloExtensionReassembly allocation; allocation failure sets SegmentBufferAllocationFailure and returns StopProcessing/public Stop. Success selects FirstSegment, records continuation metadata, advances by consumed prefix/body bytes and returns ContinueProcessing locally. The caller still checks handshake/record length and performs feeding; successful partial extension copying returns Segmented/public NeedMoreData. Buffer-bound/consistency or feeding failures may instead yield Unsupported or Stop. Allocation alone is not a completed CH or a guarantee of NeedMoreData.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:60` / `IClientHelloParser::Status`; `processor/src/Processor.cpp:1456` / `Processor::processClientHello`; `processor/src/Processor.cpp:1464` / `Processor::processClientHello`; `processor/src/Processor.cpp:1474` / `Processor::processClientHello`; `processor/src/Processor.cpp:971` / `Processor::processNewRecord`; `processor/src/Processor.cpp:1019` / `Processor::processNewRecord`; `processor/src/Processor.cpp:1101` / `Processor::processExtensions`.

### I / P3-PARSE-005: IClientHelloParser::Status::Failure

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IClientHelloParser::Status::Failure returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal CH parser Failure falls through the non-Complete handling to ClientHelloBodyInvalid/Error: the record-level and segmented-prefix/body exceptions test incomplete statuses, not Failure. processNewRecord propagates Error, translated to public Stop. It does not start CH extension continuation for this status.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:60` / `IClientHelloParser::Status`; `processor/src/Processor.cpp:1456` / `Processor::processClientHello`; `processor/src/Processor.cpp:1512` / `Processor::processClientHello`; `processor/src/Processor.cpp:210` / `Processor::processRecord`.

### I / P3-PARSE-006: IExtensionsParser::Status::Complete

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IExtensionsParser::Status::Complete returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal extension parser Complete is necessary but not sufficient for acceptance. parseAllExtensions also requires consumed bytes to match getSingleExtensionTotalLength(); mismatch becomes ExtensionParsingFailure/Error/public Stop. A valid extension is inserted into the locator array; capacity failure returns local StopProcessing without a dedicated error. Early-data and SNI handling can add facts or fail. Successful iteration advances offset/remaining length; only completion of the entire loop returns ContinueProcessing. Sorting and fingerprint feeding follow and can still fail.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:96` / `IExtensionsParser::Status`; `processor/src/Processor.cpp:1361` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1383` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1392` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1159` / `Processor::processExtensions`.

### I / P3-PARSE-007: IExtensionsParser::Status::IncompleteExtensionTypeField

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IExtensionsParser::Status::IncompleteExtensionTypeField returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal extension parser non-Complete status. parseAllExtensions assigns ExtensionParsingFailure and returns Error for this status; the explicit Failure branch and the other non-Complete branch have the same outcome. There is no per-extension continuation started here. The new-record and CH-continuation callers propagate the error (the latter deallocates its continuation buffer); processRecord translates it to public Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:96` / `IExtensionsParser::Status`; `processor/src/Processor.cpp:1361` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1371` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1159` / `Processor::processExtensions`; `processor/src/Processor.cpp:455` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:210` / `Processor::processRecord`.

### I / P3-PARSE-008: IExtensionsParser::Status::IncompleteExtensionLengthField

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IExtensionsParser::Status::IncompleteExtensionLengthField returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal extension parser non-Complete status. parseAllExtensions assigns ExtensionParsingFailure and returns Error for this status; the explicit Failure branch and the other non-Complete branch have the same outcome. There is no per-extension continuation started here. The new-record and CH-continuation callers propagate the error (the latter deallocates its continuation buffer); processRecord translates it to public Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:96` / `IExtensionsParser::Status`; `processor/src/Processor.cpp:1361` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1371` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1159` / `Processor::processExtensions`; `processor/src/Processor.cpp:455` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:210` / `Processor::processRecord`.

### I / P3-PARSE-009: IExtensionsParser::Status::IncompleteExtensionData

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IExtensionsParser::Status::IncompleteExtensionData returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal extension parser non-Complete status. parseAllExtensions assigns ExtensionParsingFailure and returns Error for this status; the explicit Failure branch and the other non-Complete branch have the same outcome. There is no per-extension continuation started here. The new-record and CH-continuation callers propagate the error (the latter deallocates its continuation buffer); processRecord translates it to public Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:96` / `IExtensionsParser::Status`; `processor/src/Processor.cpp:1361` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1371` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1159` / `Processor::processExtensions`; `processor/src/Processor.cpp:455` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:210` / `Processor::processRecord`.

### I / P3-PARSE-010: IExtensionsParser::Status::Failure

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** IExtensionsParser::Status::Failure returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** Internal extension parser non-Complete status. parseAllExtensions assigns ExtensionParsingFailure and returns Error for this status; the explicit Failure branch and the other non-Complete branch have the same outcome. There is no per-extension continuation started here. The new-record and CH-continuation callers propagate the error (the latter deallocates its continuation buffer); processRecord translates it to public Stop.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ITLSParser.hpp:96` / `IExtensionsParser::Status`; `processor/src/Processor.cpp:1361` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1371` / `Processor::parseAllExtensions`; `processor/src/Processor.cpp:1159` / `Processor::processExtensions`; `processor/src/Processor.cpp:455` / `Processor::processSegmentedClientHelloContinuation`; `processor/src/Processor.cpp:210` / `Processor::processRecord`.

### I / P3-PARSE-011: ParsingStatus vocabulary

- **Authority:** CODE-VERIFIED.
- **TCP producer/trigger:** ParsingStatus vocabulary returned by the corresponding internal parser call; conditional translation is detailed below.
- **Current fact:** ParsingStatus has exactly Complete and Failure. Incomplete outcomes exist in the specialized ClientHello/extension status enums, not this enum. Non-Complete TLS record/handshake/cipher parsing becomes specific stored errors in Processor, not automatically NeedMoreData.
- **Evidence nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **Sources:** `processor/src/ParsingStatus.hpp:17` / `ParsingStatus`; `processor/src/Processor.cpp:1542` / `Processor::processTlsRecord`; `processor/src/Processor.cpp:1527` / `Processor::processHandshake`; `processor/src/Processor.cpp:1422` / `Processor::processCiphers`.

## 3. Evidence-backed equivalence matrix: independently reviewed consolidation

### 3.1 Provenance and interpretation

Every row retains V5 current facts/sources and the carried-forward classification, with a newly explicit semantic comparison boundary and separate acquisition/local-operation costs. The supplied matrix V2 complete index was inspected; its expanded text was available through the beginning of the feedback family, not to the end. The remaining compound rows are independently reviewed against the full V5 and this contract, not claimed as verbatim expanded-V2 parity. EG1/EG2/EG6 assess the workplan's substantive classification/coverage/outcome conditions, not an invented requirement for byte-identical matrix recovery. E01 remains an archival evidence limitation, not a semantic gate failure. Unresolved classifications remain explicit and their architecture impact remains EG3.
Classifications: Shared; QUIC-applicable with different source/trigger; TCP-specific; Not applicable to current QUIC scope; Unresolved. Semantic relation is separate: Identical, Analogous only, Different, or explicitly Not applicable/Unresolved. Record-shaped code alone does not justify TCP-specific. TCP record type/length/CCS/alert level/sequence are real carrier-specific facts; CH type/SNI/offered versions/fingerprint are not. Current-scope unreadable QUIC payload is not declared nonexistent.

Consolidation corrections: P3-ERR-017 preserves matrix V2 Identical at the generic completion-versus-delivery validity boundary; the exact legacy collection mechanism is only analogous and is not a required QUIC limit. P3-REC-008 does not require a QUIC collection/batch API. D01 required fingerprint failure is distinct from completed fingerprint followed by legacy collection failure. These are explicit reviewed boundary qualifications; original inputs remain frozen.

### M / P3-IN-001: getPayload

- **TCP source/trigger:** L7 payload pointer supplied at beginPacket; borrowed packet bytes.
- **Semantic meaning:** uint8_t*: Borrowed packet payload pointer, retained as a pointer for packet processing; bytes are not copied wholesale.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** UDP datagram bytes supply QUIC packet boundaries; Initial plaintext is produced only after header protection removal and AEAD processing. Borrowed ciphertext is not a borrowed CH view.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. UDP datagram bytes supply QUIC packet boundaries; Initial plaintext is produced only after header protection removal and AEAD processing. Borrowed ciphertext is not a borrowed CH view.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. UDP datagram bytes supply QUIC packet boundaries; Initial plaintext is produced only after header protection removal and AEAD processing. Borrowed ciphertext is not a borrowed CH view.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/validate supplied spans, lengths and identity/state. Association may inspect retained context; no fixed complexity is invented.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-01, OP-04; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-001 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-002: getLength

- **TCP source/trigger:** Available L7 payload byte count, computed by Conductor from frame length and offsets.
- **Semantic meaning:** uint16_t: Payload length bounds packet-local parsing and participates in expected TCP sequence calculation.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** Datagram length, each QUIC packet Length and CRYPTO offset/length bound different layers. A TCP uint16 payload length is not a CRYPTO-stream extent.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Datagram length, each QUIC packet Length and CRYPTO offset/length bound different layers. A TCP uint16 payload length is not a CRYPTO-stream extent.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Datagram length, each QUIC packet Length and CRYPTO offset/length bound different layers. A TCP uint16 payload length is not a CRYPTO-stream extent.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/validate supplied spans, lengths and identity/state. Association may inspect retained context; no fixed complexity is invented.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-01, OP-02, OP-04, OP-05; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-002 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-003: getSessionIdx

- **TCP source/trigger:** Session index supplied by Session; no wire field is a direct substitute.
- **Semantic meaning:** uint32_t: Session index keys continuation lookup/allocation; default constructor argument in the parameterized constructor is 0xffffffff.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** DP session identity scopes the active attempt; retained Initial context and attempt association further scope CRYPTO ranges. Session index alone is insufficient.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. DP session identity scopes the active attempt; retained Initial context and attempt association further scope CRYPTO ranges. Session index alone is insufficient.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. DP session identity scopes the active attempt; retained Initial context and attempt association further scope CRYPTO ranges. Session index alone is insufficient.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/validate supplied spans, lengths and identity/state. Association may inspect retained context; no fixed complexity is invented.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-01, OP-03, OP-05; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-003 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-004: getTcpSeq

- **TCP source/trigger:** TCP sequence number plus payload length, compared with saved expected sequence using serial arithmetic.
- **Semantic meaning:** uint32_t: TCP sequence participates in exact resume, full retransmission, overlap trimming and gap detection; default parameter is zero.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** No TCP sequence number exists. CRYPTO offsets track handshake byte positions; packet numbers are separate protected metadata and cannot replace TCP sequence arithmetic.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. No TCP sequence number exists. CRYPTO offsets track handshake byte positions; packet numbers are separate protected metadata and cannot replace TCP sequence arithmetic.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. No TCP sequence number exists. CRYPTO offsets track handshake byte positions; packet numbers are separate protected metadata and cannot replace TCP sequence arithmetic.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-01, OP-02; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-004 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-005: TLSData input

- **TCP source/trigger:** const TLSData& supplied at each processRecord call; no new wire trigger.
- **Semantic meaning:** processRecord accepts const TLSData&. Direct field reads in Processor.cpp are hsState and tlsVersionMask, not all fields of TLSData.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** Current TCP processRecord receives const TLSData& and directly reads hsState and tlsVersionMask, as recorded in V5. Read-only access is the shared access discipline when an operation genuinely needs semantic state; it does not make TLSData a mandatory input to QUIC CH syntax/fingerprint evaluation. Step 4 must identify which operation reads hsState, attempt/context metadata or any other required fact and why. The TCP stored-version use is separately TCP-specific in P3-IN-007. No interface is introduced merely to hide fields.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Current TCP processRecord receives const TLSData& and directly reads hsState and tlsVersionMask, as recorded in V5. Read-only access is the shared access discipline when an operation genuinely needs semantic state; it does not make TLSData a mandatory input to QUIC CH syntax/fingerprint evaluation. Step 4 must identify which operation reads hsState, attempt/context metadata or any other required fact and why. The TCP stored-version use is separately TCP-specific in P3-IN-007. No interface is introduced merely to hide fields.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Read-only semantic state is applicable after association. Preserve only actual required reads; do not assume QUIC needs all TLSData fields or the TCP opaque-record predicates.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/validate supplied spans, lengths and identity/state. Association may inspect retained context; no fixed complexity is invented.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-03, OP-06; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-005 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-006: hsState

- **TCP source/trigger:** Stored hsState read during new-record and alert processing; eligibility is state-dependent.
- **Semantic meaning:** Controls Unsupported short-circuit, handshake parsing eligibility, opaque record treatment and plaintext-alert eligibility. Alert continuation captures eligibility when started.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** Use Phase 2 state and attempt policy to gate QUIC evidence. No QUIC ChangeCipherSpec state; terminal new-attempt classification can precede parsing.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Use Phase 2 state and attempt policy to gate QUIC evidence. No QUIC ChangeCipherSpec state; terminal new-attempt classification can precede parsing.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Use Phase 2 state and attempt policy to gate QUIC evidence. No QUIC ChangeCipherSpec state; terminal new-attempt classification can precede parsing.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/validate supplied spans, lengths and identity/state. Association may inspect retained context; no fixed complexity is invented.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-03, OP-08; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-006 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-007: tlsVersionMask

- **TCP source/trigger:** Stored advertised tlsVersionMask read for opaque ApplicationData classification.
- **Semantic meaning:** isTLS13(mask) changes ApplicationData opaque-record classification while InProgress. This is an advertised-version mask, not evidence of negotiated version.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** This row concerns the existing read of stored tlsVersionMask to classify TCP ApplicationData as opaque while InProgress. That processing use has no current QUIC counterpart and must not be imported. The advertised-version value itself remains a shared CH fact in P3-STATE-002 and P3-VIEW-003. No QUIC read of prior tlsVersionMask is established merely by CH fact availability; Step 4 must justify each actual read by operation.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. This row concerns the existing read of stored tlsVersionMask to classify TCP ApplicationData as opaque while InProgress. That processing use has no current QUIC counterpart and must not be imported. The advertised-version value itself remains a shared CH fact in P3-STATE-002 and P3-VIEW-003. No QUIC read of prior tlsVersionMask is established merely by CH fact availability; Step 4 must justify each actual read by operation.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Advertised CH versions are available from reconstructed CH. They are not negotiated-version proof and do not enable TCP ApplicationData heuristics in QUIC.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06, OP-08; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-007 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-008: Session and packet identity

- **TCP source/trigger:** Conductor supplies Session&, DPFrame&, session index and packet timestamp to consumers/state updates. Packet payload length is frame total length minus L7 offset from L2.
- **Semantic meaning:** Conductor supplies Session&, DPFrame&, session index and packet timestamp to consumers/state updates. Packet payload length is frame total length minus L7 offset from L2.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** Session/packet identity and timestamp remain available, but a datagram can carry several QUIC packets and a CH can span several datagrams. Keep identities distinct.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Session/packet identity and timestamp remain available, but a datagram can carry several QUIC packets and a CH can span several datagrams. Keep identities distinct.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Session/packet identity and timestamp remain available, but a datagram can carry several QUIC packets and a CH can span several datagrams. Keep identities distinct.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/validate supplied spans, lengths and identity/state. Association may inspect retained context; no fixed complexity is invented.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-01, OP-07, OP-09; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-008 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-009: Transport/control observations

- **TCP source/trigger:** Conductor inspects L4 protocol, SYN-without-ACK and RST-without-ACK before/around processing. These are TCP-specific observations, not QUIC equivalents.
- **Semantic meaning:** Conductor inspects L4 protocol, SYN-without-ACK and RST-without-ACK before/around processing. These are TCP-specific observations, not QUIC equivalents.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** SYN/RST flags have no QUIC wire counterpart. New-attempt and close evidence are separate semantics, not translated TCP flags.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. SYN/RST flags have no QUIC wire counterpart. New-attempt and close evidence are separate semantics, not translated TCP flags.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. SYN/RST flags have no QUIC wire counterpart. New-attempt and close evidence are separate semantics, not translated TCP flags.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-10; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-009 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-010: State routing

- **TCP source/trigger:** Finished/Unsupported bypass parsing; Error/TerminatedByAlert use terminal-payload callbacks. Zero payload bypasses processing.
- **Semantic meaning:** Finished/Unsupported bypass parsing; Error/TerminatedByAlert use terminal-payload callbacks. Zero payload bypasses processing.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** Route by Phase 2 semantic state after attempt classification. Do not blindly bypass terminal Initial candidates; they can establish a new attempt under Phase 2 rules.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Route by Phase 2 semantic state after attempt classification. Do not blindly bypass terminal Initial candidates; they can establish a new attempt under Phase 2 rules.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Route by Phase 2 semantic state after attempt classification. Do not blindly bypass terminal Initial candidates; they can establish a new attempt under Phase 2 rules.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/validate supplied spans, lengths and identity/state. Association may inspect retained context; no fixed complexity is invented.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-03, OP-08, OP-10; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-010 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-IN-011: Session participation

- **TCP source/trigger:** Subscriber event masks determine extension creation and observer participation; absent extension means no processor invocation.
- **Semantic meaning:** Subscriber event masks determine extension creation and observer participation; absent extension means no processor invocation.
- **Observed/derived/heuristic nature:** Supplied packet/session/state input; wire data and software metadata must be distinguished.
- **QUIC source, required processing and rationale:** Same feature-driven participation rule for TCP/QUIC: at least one live listener returns nonzero getSessionEventMask(session); successful extension allocation permits inbound/aging observer registration. Zero masks, expired listeners or failed allocation do not participate. User decision plus TLSFlowConductor::onNewSession/buildExtension/registerSessionObservers. Feature-specific port/profile predicates remain feature-owned; no new transport gate or registration architecture.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Same feature-driven participation rule for TCP/QUIC: at least one live listener returns nonzero getSessionEventMask(session); successful extension allocation permits inbound/aging observer registration. Zero masks, expired listeners or failed allocation do not participate. User decision plus TLSFlowConductor::onNewSession/buildExtension/registerSessionObservers. Feature-specific port/profile predicates remain feature-owned; no new transport gate or registration architecture.
- **Representation differences:** Mask/extension participation semantics unchanged; QUIC transport parsing begins only after participation. Nonzero does not require every event bit.
- **Acquisition cost:** Subscriber scan and mask checks; no decryption for the participation decision.
- **Operation cost after evidence is available:** Local access/predicate/callback work only; no repeated Initial decryption per getter.
- **Sharing implications:** Per-session extension/listener registration under existing infrastructure; no new component.
- **Contract trace:** OP-01; sections 2, 3, 8, 9
- **Evidence:** V5 P3-IN-011 source anchors in Section 2; Step 4 V2 Appendix A P3-IN-011; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

- **V3 closure evidence:** Section 8 U02-U05 and phase3-user-consumer-evidence-v1.md; current source anchors retained above.

### M / P3-STATE-001: hsState

- **TCP source/trigger:** Conductor record/control/unsupported/feedback decisions and reset; current stored value read before normal current-record update.
- **Semantic meaning:** Current accumulated lifecycle state, prior to normal current-record update during record/CH dispatch. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle. Current HsState vocabulary is PreHandshake, InProgress, ChangeCipherSpec, Finished, Error, TerminatedByAlert, Unsupported. isActiveHandshakeState selects the first three; isSilentTerminalState selects Finished/Unsupported; usesTerminalPayloadPath selects Error/TerminatedByAlert. These are current-code predicates, not changes to Phase 2 policy.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Same semantic lifecycle vocabulary where applicable; QUIC omits ChangeCipherSpec and uses Phase 2 CH/Handshake/close evidence. State is accumulated policy, not a wire field.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Same semantic lifecycle vocabulary where applicable; QUIC omits ChangeCipherSpec and uses Phase 2 CH/Handshake/close evidence. State is accumulated policy, not a wire field.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Same semantic lifecycle vocabulary where applicable; QUIC omits ChangeCipherSpec and uses Phase 2 CH/Handshake/close evidence. State is accumulated policy, not a wire field.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Scalar read/update or bounded value copy; reset cleanup may depend on retained state. No new wire acquisition for the update itself.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-03, OP-07, OP-08, OP-09; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-001 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-002: tlsVersionMask

- **TCP source/trigger:** Completed CH normal update replaces mask from CH viewer; reset clears it.
- **Semantic meaning:** Advertised-version mask accumulated from completed CH; also used for alert classification. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Parse advertised versions from CH and retain them as CH facts. TLS 1.3 QUIC constraints do not make this mask negotiated-version evidence.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Parse advertised versions from CH and retain them as CH facts. TLS 1.3 QUIC constraints do not make this mask negotiated-version evidence.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Parse advertised versions from CH and retain them as CH facts. TLS 1.3 QUIC constraints do not make this mask negotiated-version evidence.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Scalar read/update or bounded value copy; reset cleanup may depend on retained state. No new wire acquisition for the update itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06, OP-07, OP-08; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-002 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-003: clientHelloCount

- **TCP source/trigger:** Complete CH normal update increments count; reset clears it.
- **Semantic meaning:** uint8_t clientHelloCount increments on a completed CH normal update and is reset with TLSData. It is a bounded stored counter, not an unbounded monotonic count. It gates CCS/ApplicationData handling through the exact zero/nonzero predicate. Exposed through const TLSData&; external field reads are not proven by this bundle.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Count logical accepted CHs under Phase 2 authority/retransmission/attempt rules, not packet or CRYPTO-frame counts. Do not import uint8 overflow as semantic policy.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Count logical accepted CHs under Phase 2 authority/retransmission/attempt rules, not packet or CRYPTO-frame counts. Do not import uint8 overflow as semantic policy.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Count logical accepted CHs under Phase 2 authority/retransmission/attempt rules, not packet or CRYPTO-frame counts. Do not import uint8 overflow as semantic policy.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Scalar read/update or bounded value copy; reset cleanup may depend on retained state. No new wire acquisition for the update itself.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-07, OP-08; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-003 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-004: earlyDataCount

- **TCP source/trigger:** Normal ApplicationData update with clientHelloCount == 0; reset clears uint8_t counter.
- **Semantic meaning:** uint8_t earlyDataCount increments on the normal ApplicationData update when clientHelloCount == 0; it is not independently verified proof that no CH ever occurred. Exposed through const TLSData& to callbacks; external field reads are not proven by this bundle.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** The TCP pre-CH ApplicationData count is not a QUIC 0-RTT count. 0-RTT is state-neutral and encrypted contents are outside current acquisition.
- **Classification:** Not applicable to current QUIC scope
- **Semantic relation:** Not applicable
- **Semantic boundary and rationale:** Current-release exclusion of this exact producer/use, not universal absence in QUIC. The TCP pre-CH ApplicationData count is not a QUIC 0-RTT count. 0-RTT is state-neutral and encrypted contents are outside current acquisition.
- **Representation differences:** Do not add a QUIC event/API for symmetry; this is a scope exclusion, not a claim of universal protocol impossibility. The TCP pre-CH ApplicationData count is not a QUIC 0-RTT count. 0-RTT is state-neutral and encrypted contents are outside current acquisition.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Do not add a QUIC event/API for symmetry; this is a scope exclusion, not a claim of universal protocol impossibility.
- **Contract trace:** OP-08; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-004 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-005: earlyDataOffered

- **TCP source/trigger:** Complete CH normal update copies current CH early_data flag; reset clears it.
- **Semantic meaning:** Committed CH early_data presence; affects TCP TLS13 completion heuristic. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** CH early_data presence is the same offer fact. It does not prove acceptance or observed 0-RTT payload and must not drive the TCP completion heuristic in QUIC.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. CH early_data presence is the same offer fact. It does not prove acceptance or observed 0-RTT payload and must not drive the TCP completion heuristic in QUIC.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. CH early_data presence is the same offer fact. It does not prove acceptance or observed 0-RTT payload and must not drive the TCP completion heuristic in QUIC.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Scalar read/update or bounded value copy; reset cleanup may depend on retained state. No new wire acquisition for the update itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-07, OP-08; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-005 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-006: preCcsSeen

- **TCP source/trigger:** Normal CCS update with clientHelloCount == 0; reset clears uint8_t counter.
- **Semantic meaning:** uint8_t preCcsSeen increments on the normal CCS update when clientHelloCount == 0. Exposed through const TLSData& to callbacks; external field reads are not proven by this bundle.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** QUIC does not carry TLS CCS records; no pre-CCS counter is synthesized.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. QUIC does not carry TLS CCS records; no pre-CCS counter is synthesized.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. QUIC does not carry TLS CCS records; no pre-CCS counter is synthesized.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-08; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-006 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-007: chFingerprint

- **TCP source/trigger:** Complete CH normal update copies fingerprint from viewer; reset defaults it.
- **Semantic meaning:** Value copy of completed CH fingerprint. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Copy the fingerprint of the authoritative reconstructed CH. USER-FP-01 requires equality for the same CH irrespective of transport.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Copy the fingerprint of the authoritative reconstructed CH. USER-FP-01 requires equality for the same CH irrespective of transport.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Copy the fingerprint of the authoritative reconstructed CH. USER-FP-01 requires equality for the same CH irrespective of transport.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Scalar read/update or bounded value copy; reset cleanup may depend on retained state. No new wire acquisition for the update itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-07, OP-10; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-007 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-008: terminalViolationMask

- **TCP source/trigger:** Existing listener feedback commits OR mask; reset clears it. This is software feedback, not a wire field.
- **Semantic meaning:** Accumulated existing feedback metadata; affects SYN reuse gating. No feature policy is defined here. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Existing verdict metadata remains independent of parser outcome and packet disposition. No QUIC violation producers or mitigation policy are inferred.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Existing verdict metadata remains independent of parser outcome and packet disposition. No QUIC violation producers or mitigation policy are inferred.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Existing verdict metadata remains independent of parser outcome and packet disposition. No QUIC violation producers or mitigation policy are inferred.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Scalar read/update or bounded value copy; reset cleanup may depend on retained state. No new wire acquisition for the update itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-09, OP-10; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-008 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-009: clientHelloTimeStamp

- **TCP source/trigger:** Complete CH normal update copies session last-packet timestamp; reset clears it.
- **Semantic meaning:** Packet timestamp of last committed completed CH. Passed through const TLSData& to listener callbacks; not necessarily read by external consumers in this supplied bundle.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Timestamp the accepted CH observation according to the later contract; acquisition can span datagrams. A first-byte versus completion timestamp choice is not settled by TCP storage alone.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Timestamp the accepted CH observation according to the later contract; acquisition can span datagrams. A first-byte versus completion timestamp choice is not settled by TCP storage alone.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Timestamp the accepted CH observation according to the later contract; acquisition can span datagrams. A first-byte versus completion timestamp choice is not settled by TCP storage alone.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Scalar read/update or bounded value copy; reset cleanup may depend on retained state. No new wire acquisition for the update itself.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-07, OP-10; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-009 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-STATE-010: TLSData defaults/reset

- **TCP source/trigger:** RST no-mask path, extension reset/release/reuse or direct clear/reset operation; no mandatory TLS wire message.
- **Semantic meaning:** clear() restores PreHandshake and zero/false/default values for every field; existing RST path may invoke it. This does not redefine Phase 2 attempt reset. TLSSessionExtension owns TLSData and returns references to it. resetValue replaces it with defaults and clears observer lists. Table release resets and erases the extension; allocation can reclaim an eligible victim. Borrowed callback TLSData references must not be treated as stable snapshots after reset/release/reuse. markReclaimable alone is not immediate release.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Phase 2 has two distinct new-attempt acceptance boundaries. For an active attempt, complete parseable CH evidence can establish a complete new attempt; reset prior attempt-local state before accepting its first CH as authoritative. For a terminal attempt, accept a new-attempt candidate from available attempt-level evidence before CH parsing, retire the old identity when available, reset attempt-local state, enter PreHandshake, then parse/reassemble; CH completeness is not a prerequisite for this reset. Retry-compatible restart is the same attempt, not a complete new-attempt reset. Changed DCID alone is insufficient.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Phase 2 has two distinct new-attempt acceptance boundaries. For an active attempt, complete parseable CH evidence can establish a complete new attempt; reset prior attempt-local state before accepting its first CH as authoritative. For a terminal attempt, accept a new-attempt candidate from available attempt-level evidence before CH parsing, retire the old identity when available, reset attempt-local state, enter PreHandshake, then parse/reassemble; CH completeness is not a prerequisite for this reset. Retry-compatible restart is the same attempt, not a complete new-attempt reset. Changed DCID alone is insufficient.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Reset attempt-local semantics on an accepted complete new attempt as Phase 2 requires; Retry-compatible restart remains the same attempt. No blanket reset on changed DCID.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Scalar read/update or bounded value copy; reset cleanup may depend on retained state. No new wire acquisition for the update itself.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-03, OP-07, OP-10; sections 3, 5, 9
- **Evidence:** V5 P3-STATE-010 source anchors in Section 2; Step 4 V2 Appendix A P3-STATE-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-001: RecordParsed

- **TCP source/trigger:** processSingleRecord final ContinueProcessing -> RecordParsed.
- **Semantic meaning:** RecordParsed is the public translation of the final internal ContinueProcessing result. Conductor enters current-record analysis. It does not prove a completed ClientHello, a fresh CH event, successful observer delivery, or a state update; those have separate guards and feedback.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** A successful semantic observation can be CH completion or other acquired evidence. RecordParsed is a TCP carrier name, not a reason to manufacture a QUIC TLS record.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. A successful semantic observation can be CH completion or other acquired evidence. RecordParsed is a TCP carrier name, not a reason to manufacture a QUIC TLS record.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. A successful semantic observation can be CH completion or other acquired evidence. RecordParsed is a TCP carrier name, not a reason to manufacture a QUIC TLS record.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-09; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-001 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-002: NeedMoreData

- **TCP source/trigger:** Final Segmented result or nonempty full-continuation retransmission -> NeedMoreData.
- **Semantic meaning:** Translation of Segmented, or a full continuation retransmission when bytes remain. Conductor returns Continue but stops its record loop because result is not RecordParsed.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Missing contiguous CRYPTO bytes or incomplete CH can require more input. Distinguish waiting from exhaustion of current datagram and from unsupported acquisition.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Missing contiguous CRYPTO bytes or incomplete CH can require more input. Distinguish waiting from exhaustion of current datagram and from unsupported acquisition.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Missing contiguous CRYPTO bytes or incomplete CH can require more input. Distinguish waiting from exhaustion of current datagram and from unsupported acquisition.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-002 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-003: Unsupported

- **TCP source/trigger:** Continuation gap flag, or final unsupported CH reconstruction outcome -> Unsupported.
- **Semantic meaning:** Translation of unsupported reconstruction or continuation gap. Distinct from ParsingError::UnsupportedRecordType and UnsupportedTlsVersion, which lead to Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** An unevaluable required observation can produce Unsupported under Phase 2. Unsupported version/scope, ambiguity, malformed data and resource loss are not automatically the same outcome.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. An unevaluable required observation can produce Unsupported under Phase 2. Unsupported version/scope, ambiguity, malformed data and resource loss are not automatically the same outcome.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. An unevaluable required observation can produce Unsupported under Phase 2. Unsupported version/scope, ambiguity, malformed data and resource loss are not automatically the same outcome.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-03, OP-08, OP-10; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-003 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-004: EndOfPacket

- **TCP source/trigger:** No remaining packet bytes, or final internal EndOfPacket -> EndOfPacket.
- **Semantic meaning:** No remaining bytes, or internal EndOfPacket (including Unsupported state short-circuit). No current-record dispatch.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** End of current QUIC packet/datagram is traversal progress, not end of CRYPTO stream or completion of the handshake.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. End of current QUIC packet/datagram is traversal progress, not end of CRYPTO stream or completion of the handshake.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. End of current QUIC packet/datagram is traversal progress, not end of CRYPTO stream or completion of the handshake.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-01, OP-02, OP-04; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-004 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-005: Stop

- **TCP source/trigger:** Uninitialized packet step, final Error/StopProcessing or default translation -> Stop.
- **Semantic meaning:** Translation of Error/StopProcessing/default or an uninitialized step. Conductor consults getLastError; Stop is not itself an observer Stop or an Error state transition.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Processing Stop remains distinct from observer Stop, error and network drop. QUIC producer reasons need explicit outcome metadata rather than record-specific inference.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Processing Stop remains distinct from observer Stop, error and network drop. QUIC producer reasons need explicit outcome metadata rather than record-specific inference.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Processing Stop remains distinct from observer Stop, error and network drop. QUIC producer reasons need explicit outcome metadata rather than record-specific inference.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-04, OP-09; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-005 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-006: Internal result translation

- **TCP source/trigger:** processSingleRecord final return reaches processRecord translation; nested returns may be overridden by caller branches.
- **Semantic meaning:** ContinueProcessing/Segmented/Unsupported/Error/EndOfPacket/StopProcessing are internal RecordProcessResult values. processRecord translates the result returned by processSingleRecord: ContinueProcessing -> RecordParsed; Segmented -> NeedMoreData; Unsupported -> Unsupported; EndOfPacket -> EndOfPacket; Error and StopProcessing -> Stop. This is the final translation, not a guarantee that every nested return propagates unchanged. In particular processNewRecord returns Segmented for an incomplete record before returning its innerProcessResult; see P3-ERR-019 for the cipher soft-stop path.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Translate QUIC acquisition/parser outcomes to semantic progress without assuming nested results propagate unchanged. No reuse of TCP RecordProcessResult as a QUIC record model.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Translate QUIC acquisition/parser outcomes to semantic progress without assuming nested results propagate unchanged. No reuse of TCP RecordProcessResult as a QUIC record model.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Translate QUIC acquisition/parser outcomes to semantic progress without assuming nested results propagate unchanged. No reuse of TCP RecordProcessResult as a QUIC record model.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-04, OP-09; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-006 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-007: ClientHello status None

- **TCP source/trigger:** CH status reset to None by ordinary packet reset; resume may bypass reset.
- **Semantic meaning:** None is the canonical CH viewer status when no CH has been recognized in the current reset context. It is a stored status, not an independent processing result or wire observation. It is reset by resetStepState; resume paths can bypass that reset.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** None means no CH has been recognized in the current CH parsing/validity context. It is not merely absence of a complete CH: Partial means a recognized but incomplete CH. An authoritative CH from an earlier observation may remain available separately; None in a new operation does not erase it or prove no CH was transmitted. Scope and reset of current parsing status must be explicit.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. None means no CH has been recognized in the current CH parsing/validity context. It is not merely absence of a complete CH: Partial means a recognized but incomplete CH. An authoritative CH from an earlier observation may remain available separately; None in a new operation does not erase it or prove no CH was transmitted. Scope and reset of current parsing status must be explicit.
- **Representation differences:** None means no CH recognized in the current parsing context, not merely no completed CH. Partial is recognized but incomplete; prior authoritative CH availability is separate.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-05, OP-06, OP-10; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-007 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-008: ClientHello status Partial

- **TCP source/trigger:** CH recognition or unfinished CH extension continuation sets Partial.
- **Semantic meaning:** Partial is set when CH processing starts and during extension continuation before completion. It can coexist with NeedMoreData, Unsupported or Stop. It is not proof that resumable continuation exists. Conductor uses Partial in the InProgress Unsupported disposition guard.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** CH reconstruction is partial until contiguous message bytes and parsing are sufficient. QUIC gaps are not TCP record segmentation failures.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. CH reconstruction is partial until contiguous message bytes and parsing are sufficient. QUIC gaps are not TCP record segmentation failures.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. CH reconstruction is partial until contiguous message bytes and parsing are sufficient. QUIC gaps are not TCP record segmentation failures.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-008 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-009: ClientHello status Complete

- **TCP source/trigger:** CH digest finalization/completion sets Complete before record-capacity check; no guaranteed callback follows.
- **Semantic meaning:** Explicit viewer status, not an additional public step result or a fresh-event token. Complete is checked for CH callback eligibility and normal state update; Partial is checked in unsupported handling. None is not a completed result. Both ordinary and continuation CH completion set Complete before checking record-viewer capacity. Capacity failure can then produce public Stop while Complete remains visible. Stop dispatch handles the parsing-error channel and does not invoke the normal CH callback. Thus parser completion, successful RecordParsed delivery, and a newly delivered CH observation are distinct.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Completed CH data, successful observation delivery and fresh completion are distinct. Preserve that distinction; do not promote retained Complete into a new event.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Completed CH data, successful observation delivery and fresh completion are distinct. Preserve that distinction; do not promote retained Complete into a new event.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Completed CH data, successful observation delivery and fresh completion are distinct. Preserve that distinction; do not promote retained Complete into a new event.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-07, OP-09, OP-10; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-009 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OUT-010: Last error channel

- **TCP source/trigger:** getLastError reads canonical error after processing; no new wire trigger from the getter itself.
- **Semantic meaning:** getLastError returns stored m_lastError separately from step result. None suppresses anomaly callback. Error state is not automatic merely because parsing returned Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Error metadata is an independent result channel. Transport-specific reasons stay identifiable; absence of error does not imply successful delivery.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Error metadata is an independent result channel. Transport-specific reasons stay identifiable; absence of error does not imply successful delivery.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Error metadata is an independent result channel. Transport-specific reasons stay identifiable; absence of error does not imply successful delivery.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-04, OP-06, OP-09, OP-10; sections 3, 4, 5, 6
- **Evidence:** V5 P3-OUT-010 source anchors in Section 2; Step 4 V2 Appendix A P3-OUT-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-001: None

- **TCP source/trigger:** Reset/no-error sentinel read; no malformed-wire trigger is implied.
- **Semantic meaning:** No-error sentinel.
- **Observed/derived/heuristic nature:** No-error sentinel; not a failure or a fresh observation.
- **QUIC source, required processing and rationale:** No-error sentinel has the same software meaning; it is not a wire observation or a successful CH guarantee.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. No-error sentinel has the same software meaning; it is not a wire observation or a successful CH guarantee.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. No-error sentinel has the same software meaning; it is not a wire observation or a successful CH guarantee.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06, OP-10; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-001 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-002: UnsupportedTlsVersion

- **TCP source/trigger:** Condition for UnsupportedTlsVersion: Assigned in Processor.cpp at extracted-file lines 929. Trigger: a ClientHello record reaches the supported-record-version check and tlsRecord::isSupportedVersion() is false; returns Error/public Stop. This error name is not the public Unsupported result.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 929. Trigger: a ClientHello record reaches the supported-record-version check and tlsRecord::isSupportedVersion() is false; returns Error/public Stop. This error name is not the public Unsupported result.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** This producer checks the TLS record version. QUIC has no TLS record version; QUIC version and CH TLS-version validation are distinct checks with distinct evidence.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. This producer checks the TLS record version. QUIC has no TLS record version; QUIC version and CH TLS-version validation are distinct checks with distinct evidence.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. This producer checks the TLS record version. QUIC has no TLS record version; QUIC version and CH TLS-version validation are distinct checks with distinct evidence.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-002 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-003: MalformedTlsRecord

- **TCP source/trigger:** Condition for MalformedTlsRecord: Assigned in Processor.cpp at extracted-file lines 631, 658, 1548. Triggers: TLS record-header parse is non-Complete (new record or reconstructed alert), or reconstructed alert bytes received exceed the declared total record length (header plus payload). These return Error/public Stop.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 631, 658, 1548. Triggers: TLS record-header parse is non-Complete (new record or reconstructed alert), or reconstructed alert bytes received exceed the declared total record length (header plus payload). These return Error/public Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** TLS record-header/reconstructed-record malformed conditions do not exist in QUIC. Malformed QUIC packet/frame or CH checks remain applicable under their own reasons.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. TLS record-header/reconstructed-record malformed conditions do not exist in QUIC. Malformed QUIC packet/frame or CH checks remain applicable under their own reasons.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. TLS record-header/reconstructed-record malformed conditions do not exist in QUIC. Malformed QUIC packet/frame or CH checks remain applicable under their own reasons.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-003 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-004: UnsupportedRecordType

- **TCP source/trigger:** Condition for UnsupportedRecordType: Assigned in Processor.cpp at extracted-file lines 638, 1561. Trigger: the TLS record parser reports an unsupported record type, including reconstructed alert handling. Returns StopProcessing/public Stop, not public Unsupported.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 638, 1561. Trigger: the TLS record parser reports an unsupported record type, including reconstructed alert handling. Returns StopProcessing/public Stop, not public Unsupported.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Unsupported TLS record type has no QUIC record counterpart. Unsupported QUIC packet/frame classification is not this error.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. Unsupported TLS record type has no QUIC record counterpart. Unsupported QUIC packet/frame classification is not this error.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. Unsupported TLS record type has no QUIC record counterpart. Unsupported QUIC packet/frame classification is not this error.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-004 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-005: RecordLengthExceedsData

- **TCP source/trigger:** Condition for RecordLengthExceedsData: Assigned in Processor.cpp at extracted-file lines 646, 1555. Trigger: declared TLS record payload exceeds MaxTlsRecordPayloadSize, including reconstructed alert handling. Returns Error/public Stop; the name does not mean every incomplete packet is an error.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 646, 1555. Trigger: declared TLS record payload exceeds MaxTlsRecordPayloadSize, including reconstructed alert handling. Returns Error/public Stop; the name does not mean every incomplete packet is an error.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** TLS record payload limit is not a QUIC packet or CH limit. Preserve bounded lengths at the appropriate QUIC layer, not the TLS-record error meaning.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. TLS record payload limit is not a QUIC packet or CH limit. Preserve bounded lengths at the appropriate QUIC layer, not the TLS-record error meaning.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. TLS record payload limit is not a QUIC packet or CH limit. Preserve bounded lengths at the appropriate QUIC layer, not the TLS-record error meaning.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-005 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-006: HandshakeLengthNotMatchingRecordLength

- **TCP source/trigger:** Condition for HandshakeLengthNotMatchingRecordLength: Assigned in Processor.cpp at extracted-file lines 943, 979. Triggers: handshake length plus header is smaller than the TLS record payload; or it remains larger after CH prefix/body parsing without the recordLevelFragmented Unsupported branch. Returns StopProcessing/public Stop. Record-level fragmentation is a distinct Unsupported path.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 943, 979. Triggers: handshake length plus header is smaller than the TLS record payload; or it remains larger after CH prefix/body parsing without the recordLevelFragmented Unsupported branch. Returns StopProcessing/public Stop. Record-level fragmentation is a distinct Unsupported path.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Handshake-versus-TLS-record length equality is TCP framing-specific. QUIC CH message length must instead fit the reconstructed CRYPTO message, without equality to one packet/frame.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. Handshake-versus-TLS-record length equality is TCP framing-specific. QUIC CH message length must instead fit the reconstructed CRYPTO message, without equality to one packet/frame.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. Handshake-versus-TLS-record length equality is TCP framing-specific. QUIC CH message length must instead fit the reconstructed CRYPTO message, without equality to one packet/frame.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02, OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-006 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-007: MalformedHandshake

- **TCP source/trigger:** Condition for MalformedHandshake: Assigned in Processor.cpp at extracted-file lines 1533. Trigger: handshake-header parser returns non-Complete; returns Error/public Stop.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 1533. Trigger: handshake-header parser returns non-Complete; returns Error/public Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Malformed TLS handshake-message header can be checked on contiguous CRYPTO bytes. Missing bytes first mean partial acquisition, not necessarily malformed input.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Malformed TLS handshake-message header can be checked on contiguous CRYPTO bytes. Missing bytes first mean partial acquisition, not necessarily malformed input.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Malformed TLS handshake-message header can be checked on contiguous CRYPTO bytes. Missing bytes first mean partial acquisition, not necessarily malformed input.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-007 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-008: MalformedClientHello

- **TCP source/trigger:** No demonstrated runtime producer/wire trigger in supplied Processor; declaration/sentinel only.
- **Semantic meaning:** Declared vocabulary only: no direct assignment in supplied Processor.cpp. Do not invent a reachable producer.
- **Observed/derived/heuristic nature:** Declared vocabulary/sentinel only; no demonstrated producer.
- **QUIC source, required processing and rationale:** Declared MalformedClientHello has no verified TCP producer. Do not invent its reachability or a QUIC producer; later error vocabulary may map real CH failures explicitly.
- **Classification:** Unresolved
- **Semantic relation:** Unresolved
- **Semantic boundary and rationale:** Explicit unknown producer, integration or compatibility disposition; no unsupported equivalence is asserted. Declared MalformedClientHello has no verified TCP producer. Do not invent its reachability or a QUIC producer; later error vocabulary may map real CH failures explicitly.
- **Representation differences:** Keep the unresolved item visible; do not select architecture, invent consumers or silently preserve/fix suspected defects. Declared MalformedClientHello has no verified TCP producer. Do not invent its reachability or a QUIC producer; later error vocabulary may map real CH failures explicitly.
- **Acquisition cost:** Depends on the explicit unresolved disposition; no pipeline or cost is invented.
- **Operation cost after evidence is available:** Not established; unresolved is not evidence of expensive processing.
- **Sharing implications:** Keep the unresolved item visible; do not select architecture, invent consumers or silently preserve/fix suspected defects.
- **Contract trace:** OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-008 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-009: ClientHelloBodyInvalid

- **TCP source/trigger:** Condition for ClientHelloBodyInvalid: Assigned in Processor.cpp at extracted-file lines 1515. Trigger: CH parser non-Complete not accepted by the incomplete-status Unsupported or extension-continuation branches; includes Failure. Returns Error/public Stop. See P3-PARSE-002 through P3-PARSE-005.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 1515. Trigger: CH parser non-Complete not accepted by the incomplete-status Unsupported or extension-continuation branches; includes Failure. Returns Error/public Stop. See P3-PARSE-002 through P3-PARSE-005.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Invalid complete CH body is shared TLS syntax; incomplete QUIC CH bytes require reconstruction rather than TCP prefix/body Unsupported shortcuts. Preserve syntax-versus-availability distinction.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Invalid complete CH body is shared TLS syntax; incomplete QUIC CH bytes require reconstruction rather than TCP prefix/body Unsupported shortcuts. Preserve syntax-versus-availability distinction.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Invalid complete CH body is shared TLS syntax; incomplete QUIC CH bytes require reconstruction rather than TCP prefix/body Unsupported shortcuts. Preserve syntax-versus-availability distinction.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-009 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-010: MultipleClientHello

- **TCP source/trigger:** Condition for MultipleClientHello: Assigned in Processor.cpp at extracted-file lines 951. Trigger: a new CH is encountered while the canonical CH status is already Complete in the active processing context; returns Error/public Stop. It is not a session-wide ban; see P3-FB-009.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 951. Trigger: a new CH is encountered while the canonical CH status is already Complete in the active processing context; returns Error/public Stop. It is not a session-wide ban; see P3-FB-009.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Phase 2 decides whether a CH is retransmission, authoritative replacement, same-attempt CH2 or new attempt. TCP retained-Complete rejection cannot define QUIC attempt policy.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Different
- **Semantic boundary and rationale:** The acceptance/disposition predicate differs; generic purpose alone does not make it equivalent. Phase 2 decides whether a CH is retransmission, authoritative replacement, same-attempt CH2 or new attempt. TCP retained-Complete rejection cannot define QUIC attempt policy.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Phase 2 decides whether a CH is retransmission, authoritative replacement, same-attempt CH2 or new attempt. TCP retained-Complete rejection cannot define QUIC attempt policy.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-07; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-010 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-011: ExtensionsLengthExceedsData

- **TCP source/trigger:** No demonstrated runtime producer/wire trigger in supplied Processor; declaration/sentinel only.
- **Semantic meaning:** Declared vocabulary only: no direct assignment in supplied Processor.cpp. Do not invent a reachable producer.
- **Observed/derived/heuristic nature:** Declared vocabulary/sentinel only; no demonstrated producer.
- **QUIC source, required processing and rationale:** Declared ExtensionsLengthExceedsData has no verified TCP producer. A QUIC CH length failure is possible but cannot be assigned this legacy reason without a contract decision.
- **Classification:** Unresolved
- **Semantic relation:** Unresolved
- **Semantic boundary and rationale:** Explicit unknown producer, integration or compatibility disposition; no unsupported equivalence is asserted. Declared ExtensionsLengthExceedsData has no verified TCP producer. A QUIC CH length failure is possible but cannot be assigned this legacy reason without a contract decision.
- **Representation differences:** Keep the unresolved item visible; do not select architecture, invent consumers or silently preserve/fix suspected defects. Declared ExtensionsLengthExceedsData has no verified TCP producer. A QUIC CH length failure is possible but cannot be assigned this legacy reason without a contract decision.
- **Acquisition cost:** Depends on the explicit unresolved disposition; no pipeline or cost is invented.
- **Operation cost after evidence is available:** Not established; unresolved is not evidence of expensive processing.
- **Sharing implications:** Keep the unresolved item visible; do not select architecture, invent consumers or silently preserve/fix suspected defects.
- **Contract trace:** OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-011 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-011; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-012: ExtensionParsingFailure

- **TCP source/trigger:** Condition for ExtensionParsingFailure: Assigned in Processor.cpp at extracted-file lines 1154, 1176, 1300, 1340, 1376, 1379, 1387. Triggers: invalid processExtensions state; SNI parser non-Complete; supported_versions or supported_groups parser failure; extension parser non-Complete; or consumed-length mismatch. Returns Error/public Stop. Dedicated MalformedSniExtension checks are separate.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 1154, 1176, 1300, 1340, 1376, 1379, 1387. Triggers: invalid processExtensions state; SNI parser non-Complete; supported_versions or supported_groups parser failure; extension parser non-Complete; or consumed-length mismatch. Returns Error/public Stop. Dedicated MalformedSniExtension checks are separate.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Extension syntax and consumed-length checks apply to the same reconstructed CH bytes. Missing CRYPTO bytes must not be passed as a supposedly complete extension section.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Extension syntax and consumed-length checks apply to the same reconstructed CH bytes. Missing CRYPTO bytes must not be passed as a supposedly complete extension section.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Extension syntax and consumed-length checks apply to the same reconstructed CH bytes. Missing CRYPTO bytes must not be passed as a supposedly complete extension section.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-012 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-012; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-013: MalformedSniExtension

- **TCP source/trigger:** Condition for MalformedSniExtension: processSni assigns MalformedSniExtension when parsed SNI count is zero or greater than two, when the first host_name length is zero or greater than max_sni_length (255), or when strnlen(name, length) differs from the declared length. It returns Error/public Stop. A non-host_name first entry returns ContinueProcessing without setting the parsed-SNI flag; a parser failure instead uses ExtensionParsingFailure.
- **Semantic meaning:** processSni assigns MalformedSniExtension when parsed SNI count is zero or greater than two, when the first host_name length is zero or greater than max_sni_length (255), or when strnlen(name, length) differs from the declared length. It returns Error/public Stop. A non-host_name first entry returns ContinueProcessing without setting the parsed-SNI flag; a parser failure instead uses ExtensionParsingFailure.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** SNI syntax checks apply to the CH server_name extension independent of carrier. No inference about server identity or negotiated outcome follows.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. SNI syntax checks apply to the CH server_name extension independent of carrier. No inference about server identity or negotiated outcome follows.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. SNI syntax checks apply to the CH server_name extension independent of carrier. No inference about server identity or negotiated outcome follows.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-013 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-013; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-014: CipherParsingFailure

- **TCP source/trigger:** Condition for CipherParsingFailure: Assigned in Processor.cpp at extracted-file lines 1433. Trigger: cipher-suite parser returns non-Complete for a nonzero cipher list; returns Error, immediately propagated by processNewRecord and translated to public Stop. Distinct from the cipher soft-stop path in P3-ERR-019.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 1433. Trigger: cipher-suite parser returns non-Complete for a nonzero cipher list; returns Error, immediately propagated by processNewRecord and translated to public Stop. Distinct from the cipher soft-stop path in P3-ERR-019.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Cipher-suite list syntax is CH content and shares its meaning. Keep syntax failure separate from fingerprint-feeding capacity failure.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Cipher-suite list syntax is CH content and shares its meaning. Keep syntax failure separate from fingerprint-feeding capacity failure.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Cipher-suite list syntax is CH content and shares its meaning. Keep syntax failure separate from fingerprint-feeding capacity failure.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-014 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-014; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-015: SegmentBufferAllocationFailure

- **TCP source/trigger:** Condition for SegmentBufferAllocationFailure: Assigned when prepareSegmentBufferForContinuation fails for alert alignment, alert reassembly, opaque tracking or CH extension continuation. Returns StopProcessing/public Stop. Preparation can fail because an existing buffer has a conflicting continuation mode or because allocation/session registration fails; this is not proof of malformed wire data or specifically memory exhaustion.
- **Semantic meaning:** Assigned when prepareSegmentBufferForContinuation fails for alert alignment, alert reassembly, opaque tracking or CH extension continuation. Returns StopProcessing/public Stop. Preparation can fail because an existing buffer has a conflicting continuation mode or because allocation/session registration fails; this is not proof of malformed wire data or specifically memory exhaustion.
- **Observed/derived/heuristic nature:** Buffer-preparation failure: resource or conflicting continuation-state outcome, not evidence of malformed TLS.
- **QUIC source, required processing and rationale:** QUIC reconstruction/key-context storage can fail; the resource-failure meaning is analogous, not proof of malformed data. No reuse of TCP segment-buffer allocation contract is implied.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. QUIC reconstruction/key-context storage can fail; the resource-failure meaning is analogous, not proof of malformed data. No reuse of TCP segment-buffer allocation contract is implied.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. QUIC reconstruction/key-context storage can fail; the resource-failure meaning is analogous, not proof of malformed data. No reuse of TCP segment-buffer allocation contract is implied.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-04, OP-05; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-015 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-015; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-016: SegmentMismatch

- **TCP source/trigger:** Condition for SegmentMismatch: Assigned in Processor.cpp at extracted-file lines 1119, 1129. Triggers in FirstSegment extension copying: total extensions length is smaller than continuation nextLength, or calculated bytes to copy exceed m_lengthLeftToParse. The buffer is deallocated and StopProcessing/public Stop is returned.
- **Semantic meaning:** Assigned in Processor.cpp at extracted-file lines 1119, 1129. Triggers in FirstSegment extension copying: total extensions length is smaller than continuation nextLength, or calculated bytes to copy exceed m_lengthLeftToParse. The buffer is deallocated and StopProcessing/public Stop is returned.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** The exact TCP FirstSegment nextLength/copy mismatch is absent. CRYPTO offset conflicts and bounds need their own semantics; do not translate a gap into SegmentMismatch.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. The exact TCP FirstSegment nextLength/copy mismatch is absent. CRYPTO offset conflicts and bounds need their own semantics; do not translate a gap into SegmentMismatch.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. The exact TCP FirstSegment nextLength/copy mismatch is absent. CRYPTO offset conflicts and bounds need their own semantics; do not translate a gap into SegmentMismatch.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02, OP-05; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-016 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-016; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-017: RecordsExceedDataViewerCapacity

- **TCP source/trigger:** Condition for RecordsExceedDataViewerCapacity: Assigned when adding a current/completed record would exceed maxRecords, at Processor.cpp extracted-file lines 483, 524, 736 and 1061; returns StopProcessing, translated to public Stop. CH completion at lines 475 and 1052 precedes its capacity check, so Complete can coexist with this failed current step. Record insertion and normal CH delivery are not guaranteed. This is a result-validity distinction, not a new state policy.
- **Semantic meaning:** Assigned when adding a current/completed record would exceed maxRecords, at Processor.cpp extracted-file lines 483, 524, 736 and 1061; returns StopProcessing, translated to public Stop. CH completion at lines 475 and 1052 precedes its capacity check, so Complete can coexist with this failed current step. Record insertion and normal CH delivery are not guaranteed. This is a result-validity distinction, not a new state policy.
- **Observed/derived/heuristic nature:** Record-viewer capacity failure; resource-limit outcome, not malformed TLS.
- **QUIC source, required processing and rationale:** The current TCP producer is a legacy record-collection limit after successful CH evaluation. At the generic result-validity boundary the meaning is identical; exact resource mechanisms can be analogous, but no QUIC observation collection or matching capacity is required. D01 required fingerprint capacity is a different cause/disposition. See S6-28A/28B.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical generic result-validity distinction: completed evaluation does not guarantee successful delivery when a result-capacity limit intervenes. The exact TCP m_records/maxRecords producer is only analogous to a hypothetical QUIC delivery limit; no such QUIC collection, numeric bound or producer is required. D01 fingerprint-building failure is a separate cause and disposition.
- **Representation differences:** TCP m_records/maxRecords is legacy bookkeeping; QUIC current-unit delivery is not a TLS record collection.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share explicit availability/delivery/failure distinctions, not the collection, numeric limit or exact public result path.
- **Contract trace:** OP-02, OP-06, OP-09; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-017 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-017; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-018: MaxParsingErrorValue

- **TCP source/trigger:** No demonstrated runtime producer/wire trigger in supplied Processor; declaration/sentinel only.
- **Semantic meaning:** Enum sentinel, not a runtime error outcome.
- **Observed/derived/heuristic nature:** Declared vocabulary/sentinel only; no demonstrated producer.
- **QUIC source, required processing and rationale:** Enum maximum is a software sentinel, not a produced observation; no QUIC wire mapping or new event is needed.
- **Classification:** Not applicable to current QUIC scope
- **Semantic relation:** Not applicable
- **Semantic boundary and rationale:** Current-release exclusion of this exact producer/use, not universal absence in QUIC. Enum maximum is a software sentinel, not a produced observation; no QUIC wire mapping or new event is needed.
- **Representation differences:** Do not add a QUIC event/API for symmetry; this is a scope exclusion, not a claim of universal protocol impossibility. Enum maximum is a software sentinel, not a produced observation; no QUIC wire mapping or new event is needed.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Do not add a QUIC event/API for symmetry; this is a scope exclusion, not a claim of universal protocol impossibility.
- **Contract trace:** OP-02; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-018 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-018; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-ERR-019: Soft stops without dedicated error

- **TCP source/trigger:** Condition for Soft stops without dedicated error: Digest append/calculate failure and extension-locator capacity failure can return local StopProcessing without assigning a dedicated ParsingError. Do not equate public Stop with a non-None error. Cipher-specific propagation: processNewRecord immediately propagates only Error from processCiphers; other non-ContinueProcessing results skip extension processing and remain in innerProcessResult. After the record-capacity check and record insertion, an incomplete record returns Segmented (public NeedMoreData), whereas a complete record returns the retained StopProcessing (public Stop). A capacity failure can instead override the outcome with RecordsExceedDataViewerCapacity/Stop. No normal RecordParsed delivery is implied by the cipher soft stop. These are static branch facts, not measured runtime occurrences or approved code fixes.
- **Semantic meaning:** Digest append/calculate failure and extension-locator capacity failure can return local StopProcessing without assigning a dedicated ParsingError. Do not equate public Stop with a non-None error. Cipher-specific propagation: processNewRecord immediately propagates only Error from processCiphers; other non-ContinueProcessing results skip extension processing and remain in innerProcessResult. After the record-capacity check and record insertion, an incomplete record returns Segmented (public NeedMoreData), whereas a complete record returns the retained StopProcessing (public Stop). A capacity failure can instead override the outcome with RecordsExceedDataViewerCapacity/Stop. No normal RecordParsed delivery is implied by the cipher soft stop. These are static branch facts, not measured runtime occurrences or approved code fixes.
- **Observed/derived/heuristic nature:** Local capacity/digest soft stop; final progress result depends on caller branches.
- **QUIC source, required processing and rationale:** QUIC can stop without a dedicated parse error, but TCP nested cipher-stop propagation is not automatically a required QUIC behavior. Keep capacity, local return and delivered result distinct.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. QUIC can stop without a dedicated parse error, but TCP nested cipher-stop propagation is not automatically a required QUIC behavior. Keep capacity, local return and delivered result distinct.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. QUIC can stop without a dedicated parse error, but TCP nested cipher-stop propagation is not automatically a required QUIC behavior. Keep capacity, local return and delivered result distinct.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-06, OP-09; sections 3, 6, 10
- **Evidence:** V5 P3-ERR-019 source anchors in Section 2; Step 4 V2 Appendix A P3-ERR-019; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-001: None

- **TCP source/trigger:** Continuation mode None; alignment-only state can still be retained.
- **Semantic meaning:** No explicit reassembly mode; a stored alert alignment offset can still exist.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** No pending reconstruction is a software fact. QUIC must distinguish no pending CRYPTO message from no pending bytes in the current datagram.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. No pending reconstruction is a software fact. QUIC must distinguish no pending CRYPTO message from no pending bytes in the current datagram.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. No pending reconstruction is a software fact. QUIC must distinguish no pending CRYPTO message from no pending bytes in the current datagram.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-001 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-002: OpaqueRecordTracking

- **TCP source/trigger:** Incomplete state-eligible opaque record starts remaining-byte tracking.
- **Semantic meaning:** Tracks remaining record bytes and saved record shape without interpreting opaque content.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Opaque TLS-record byte tracking has no current QUIC requirement: Handshake evidence uses visible associated headers, and encrypted 0-RTT/1-RTT contents are not parsed.
- **Classification:** Not applicable to current QUIC scope
- **Semantic relation:** Not applicable
- **Semantic boundary and rationale:** Current-release exclusion of this exact producer/use, not universal absence in QUIC. Opaque TLS-record byte tracking has no current QUIC requirement: Handshake evidence uses visible associated headers, and encrypted 0-RTT/1-RTT contents are not parsed.
- **Representation differences:** Do not add a QUIC event/API for symmetry; this is a scope exclusion, not a claim of universal protocol impossibility. Opaque TLS-record byte tracking has no current QUIC requirement: Handshake evidence uses visible associated headers, and encrypted 0-RTT/1-RTT contents are not parsed.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Do not add a QUIC event/API for symmetry; this is a scope exclusion, not a claim of universal protocol impossibility.
- **Contract trace:** OP-02; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-002 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-003: ClientHelloExtensionReassembly

- **TCP source/trigger:** CH parser IncompleteExtensionSection, accepted continuation branch and successful preparation.
- **Semantic meaning:** Reconstructs extension section after the CH prefix/body is already available.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Reassemble Initial CRYPTO by offsets to obtain a complete TLS CH, including prefix/body/extensions. Reuse CH parsing semantics, not the TCP extension-only segmentation restriction.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Reassemble Initial CRYPTO by offsets to obtain a complete TLS CH, including prefix/body/extensions. Reuse CH parsing semantics, not the TCP extension-only segmentation restriction.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Reassemble Initial CRYPTO by offsets to obtain a complete TLS CH, including prefix/body/extensions. Reuse CH parsing semantics, not the TCP extension-only segmentation restriction.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-003 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-004: AlertRecordReassembly

- **TCP source/trigger:** Incomplete alert header or alert payload starts alert reassembly.
- **Semantic meaning:** Tracks split alert header/payload and preserves plaintext eligibility/alignment.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** No TLS alert record exists in QUIC. Initial CONNECTION_CLOSE is parsed as a QUIC frame; it is not an alert byte stream needing TCP alignment.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. No TLS alert record exists in QUIC. Initial CONNECTION_CLOSE is parsed as a QUIC frame; it is not an alert byte stream needing TCP alignment.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. No TLS alert record exists in QUIC. Initial CONNECTION_CLOSE is parsed as a QUIC frame; it is not an alert byte stream needing TCP alignment.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-004 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-005: Gap flag

- **TCP source/trigger:** Pending buffer sequence relation reaches the gap branch; isContinuationGap exposes the flag.
- **Semantic meaning:** isContinuationGap exposes packet-local continuation gap. Conductor uses it to decide Unsupported state handling.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** A missing CRYPTO range can wait for out-of-order/retransmitted bytes. A gap alone is not the TCP sequence-gap Unsupported trigger.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Different
- **Semantic boundary and rationale:** The acceptance/disposition predicate differs; generic purpose alone does not make it equivalent. A missing CRYPTO range can wait for out-of-order/retransmitted bytes. A gap alone is not the TCP sequence-gap Unsupported trigger.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. A missing CRYPTO range can wait for out-of-order/retransmitted bytes. A gap alone is not the TCP sequence-gap Unsupported trigger.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-005 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-006: Sequence cases

- **TCP source/trigger:** TCP start/end sequence compared with saved expected sequence using serial comparisons.
- **Semantic meaning:** Exact expected sequence resumes; end <= expected under serial arithmetic is full retransmission; overlap crossing expected trims old bytes; remaining case is gap and discards stored continuation.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Use CRYPTO byte offsets, range coverage and duplicate/conflict evidence after Initial processing. QUIC packet numbers are not byte sequence numbers; retransmitted bytes may arrive in new packets.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Use CRYPTO byte offsets, range coverage and duplicate/conflict evidence after Initial processing. QUIC packet numbers are not byte sequence numbers; retransmitted bytes may arrive in new packets.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Use CRYPTO byte offsets, range coverage and duplicate/conflict evidence after Initial processing. QUIC packet numbers are not byte sequence numbers; retransmitted bytes may arrive in new packets.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-006 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-007: Session-scoped continuation

- **TCP source/trigger:** Session-index lookup selects saved continuation and rebinds Processor storage pointers.
- **Semantic meaning:** Continuation access is keyed by session index, while Processor viewers are instance-local mutable members. Saved state includes remaining lengths, record/handshake types, cipher/compression counts, lastVersion, digest and alert metadata. On CH continuation the code restores cipher/compression counts and lastVersion; it does not restore every canonical CH fact. In particular supported-version mask/parsed flag are not restored here, and the fallback reads the shared parser version. Do not equate session-scoped buffer ownership with complete session-scoped viewer freshness. See P3-VIEW-003 and P3-VIEW-014.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** State belongs to the active attempt and retained Initial contexts within the DP session. Shared mutable viewers do not prove session freshness; retired/ambiguous input must not merge.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. State belongs to the active attempt and retained Initial contexts within the DP session. Shared mutable viewers do not prove session freshness; retired/ambiguous input must not merge.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. State belongs to the active attempt and retained Initial contexts within the DP session. Shared mutable viewers do not prove session freshness; retired/ambiguous input must not merge.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-03, OP-05; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-007 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-008: Continuation completion

- **TCP source/trigger:** Saved remaining bytes reach completion; parsing/feeding and capacity guards still determine delivered result.
- **Semantic meaning:** CH/opaque/alert continuation reconstructs current record metadata on completion, but public RecordParsed and observer delivery require subsequent parsing/feeding and record-capacity checks to succeed. A CH can already be Complete when capacity failure produces Stop. Successful completion may leave bytes for another step; finishSegmentBufferContinuation can retain alert alignment state. No unconditional emission or delivery is guaranteed.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Successful reconstruction and capacity checks can yield a semantic observation. CH completion, opaque-record completion and alert completion are not one common QUIC record event; delivery still depends on feedback.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Successful reconstruction and capacity checks can yield a semantic observation. CH completion, opaque-record completion and alert completion are not one common QUIC record event; delivery still depends on feedback.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Successful reconstruction and capacity checks can yield a semantic observation. CH completion, opaque-record completion and alert completion are not one common QUIC record event; delivery still depends on feedback.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-06, OP-09; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-008 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-009: CH reconstruction boundary

- **TCP source/trigger:** CH parser incomplete status, handshake/record length relation or extension reassembly size bound.
- **Semantic meaning:** Incomplete prefix/body in a segmented CH is Unsupported; incomplete extensions can be continued. Cross-record CH fragmentation is Unsupported; oversized extension reassembly is Unsupported.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** QUIC CH can span CRYPTO frames/packets and require prefix/body reconstruction. TCP extension-only continuation and record-level fragmentation exclusions do not define QUIC support.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Different
- **Semantic boundary and rationale:** The acceptance/disposition predicate differs; generic purpose alone does not make it equivalent. QUIC CH can span CRYPTO frames/packets and require prefix/body reconstruction. TCP extension-only continuation and record-level fragmentation exclusions do not define QUIC support.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. QUIC CH can span CRYPTO frames/packets and require prefix/body reconstruction. TCP extension-only continuation and record-level fragmentation exclusions do not define QUIC support.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-009 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-010: Continuation mode exposure

- **TCP source/trigger:** getContinuationMode reads packet-local m_stepContinuationMode; getter has no independent wire trigger.
- **Semantic meaning:** getContinuationMode returns m_stepContinuationMode. Conductor currently consumes isContinuationGap, not getContinuationMode. The field is not proven to be a canonical live-buffer mode at every instant.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Expose only acquisition status needed by policy/consumers. TCP continuation enum values do not force QUIC modes or an interface/class hierarchy.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Expose only acquisition status needed by policy/consumers. TCP continuation enum values do not force QUIC modes or an interface/class hierarchy.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Expose only acquisition status needed by policy/consumers. TCP continuation enum values do not force QUIC modes or an interface/class hierarchy.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-10; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-010 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CONT-011: Resource/lifecycle loss

- **TCP source/trigger:** Continuation allocation/access/deallocation, full-map eviction, session aging or capacity checks; not a unique wire message.
- **Semantic meaning:** SegmentBufferManager may evict oldest entry on full capacity; allocation/access/deallocation have boolean outcomes, aging erases entry. No explicit eviction result is returned through IProcessor. Record as current behavior, not a proposed pool design or violation policy. Existing continuation map capacity is MaxNumberOfSegmentBuffers=1000. SegmentBufferSize=3000 bounds the CH extension reassembly branch; ExtensionsArray::add has its own 1024-locator bound, not the separate declared MaxNumberOfLocators=100. Record viewer capacity is 128 and digest storage is 2870 bytes with a strict append rejection at total >= capacity. These are different limits with different result paths; they are not a proposed QUIC allocation design. Allocation failure does not promise transactional rollback: the manager inserts before checking the new session, and the session-not-found branch returns false without erasing that insertion.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Resource/lifecycle loss can invalidate future acquisition. Apply Phase 2 Unsupported/disposition rules; eviction is not an Incomplete Handshake violation or wire termination.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Resource/lifecycle loss can invalidate future acquisition. Apply Phase 2 Unsupported/disposition rules; eviction is not an Incomplete Handshake violation or wire termination.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Resource/lifecycle loss can invalidate future acquisition. Apply Phase 2 Unsupported/disposition rules; eviction is not an Incomplete Handshake violation or wire termination.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Coverage, overlap and context bookkeeping depend on tracked ranges/bytes; scalar mode/gap checks do not require another decryption. No data structure is selected.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-04, OP-05, OP-10; sections 3, 4, 5.1, 5.2
- **Evidence:** V5 P3-CONT-011 source anchors in Section 2; Step 4 V2 Appendix A P3-CONT-011; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-001: recordType

- **TCP source/trigger:** TLS record header type byte, or saved type reconstructed on continuation completion.
- **Semantic meaning:** Raw uint8_t TLS record type; predicates identify handshake/CCS/alert/ApplicationData.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** TLS content-type byte has no QUIC record counterpart. Packet type, frame type and handshake message type remain distinct and must not be squeezed into recordType.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. TLS content-type byte has no QUIC record counterpart. Packet type, frame type and handshake message type remain distinct and must not be squeezed into recordType.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. TLS content-type byte has no QUIC record counterpart. Packet type, frame type and handshake message type remain distinct and must not be squeezed into recordType.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-001 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-002: handshakeType

- **TCP source/trigger:** Handshake header type byte only when handshake parsing runs; otherwise default/saved metadata.
- **Semantic meaning:** Raw uint8_t handshake type, populated only when handshake parsing runs; opaque handshake may retain default/saved value.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** TLS handshake-message type exists in CRYPTO message bytes. Initial decryption and contiguous header reconstruction expose ClientHello type without a TLS record wrapper.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. TLS handshake-message type exists in CRYPTO message bytes. Initial decryption and contiguous header reconstruction expose ClientHello type without a TLS record wrapper.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. TLS handshake-message type exists in CRYPTO message bytes. Initial decryption and contiguous header reconstruction expose ClientHello type without a TLS record wrapper.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-002 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-003: recordLength

- **TCP source/trigger:** TLS record header declared payload length, or saved length reconstructed on continuation completion.
- **Semantic meaning:** uint16_t record payload length, excluding TLS record header.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** TLS record length has no QUIC equivalent. QUIC packet Length, CRYPTO length and handshake message length bound different objects.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. TLS record length has no QUIC equivalent. QUIC packet Length, CRYPTO length and handshake message length bound different objects.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. TLS record length has no QUIC equivalent. QUIC packet Length, CRYPTO length and handshake message length bound different objects.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-003 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-004: alertLevel

- **TCP source/trigger:** Two-byte readable plaintext alert parser produces level.
- **Semantic meaning:** Alert level, meaningful only with readableAlert.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** TLS alert level is not carried in QUIC CONNECTION_CLOSE. Do not synthesize warning/fatal levels from CRYPTO_ERROR.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. TLS alert level is not carried in QUIC CONNECTION_CLOSE. Do not synthesize warning/fatal levels from CRYPTO_ERROR.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. TLS alert level is not carried in QUIC CONNECTION_CLOSE. Do not synthesize warning/fatal levels from CRYPTO_ERROR.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02, OP-08; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-004 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-005: alertDescription

- **TCP source/trigger:** Two-byte readable plaintext alert parser produces description.
- **Semantic meaning:** Alert description, meaningful only with readableAlert.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** For an associated Initial transport CONNECTION_CLOSE, CRYPTO_ERROR code minus 0x100 identifies a TLS alert description. Other transport/application errors are not TLS alerts.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. For an associated Initial transport CONNECTION_CLOSE, CRYPTO_ERROR code minus 0x100 identifies a TLS alert description. Other transport/application errors are not TLS alerts.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. For an associated Initial transport CONNECTION_CLOSE, CRYPTO_ERROR code minus 0x100 identifies a TLS alert description. Other transport/application errors are not TLS alerts.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-04, OP-08; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-005 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-006: readableAlert

- **TCP source/trigger:** Alert record, plaintext eligibility, zero alignment offset and two-byte parse conditions produce readableAlert.
- **Semantic meaning:** Validity discriminator; requires plaintext-eligible state, alignment and exactly two alert bytes in current parsing path.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** Successful Initial unprotection and frame parsing provide readable close evidence. An encrypted Handshake/1-RTT close is not readable under Initial-only keys.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Successful Initial unprotection and frame parsing provide readable close evidence. An encrypted Handshake/1-RTT close is not readable under Initial-only keys.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Successful Initial unprotection and frame parsing provide readable close evidence. An encrypted Handshake/1-RTT close is not readable under Initial-only keys.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-04, OP-08; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-006 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-007: getLastRecord

- **TCP source/trigger:** getLastRecord borrows canonical m_record after current step; getter is not a wire event.
- **Semantic meaning:** Returns const reference to mutable canonical m_record; Conductor consumes it per RecordParsed.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** Current observation validity is useful, but getLastRecord is a TCP container contract. QUIC may have no observation or several; retained last data must not imply freshness.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Current observation validity is useful, but getLastRecord is a TCP container contract. QUIC may have no observation or several; retained last data must not imply freshness.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Current observation validity is useful, but getLastRecord is a TCP container contract. QUIC may have no observation or several; retained last data must not imply freshness.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-09, OP-10; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-007 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-008: getAllRecords

- **TCP source/trigger:** getAllRecords borrows bounded records pushed by processing; getter is not a whole-packet completion event.
- **Semantic meaning:** Returns const reference to bounded m_records collection; capacity 128. Contains records pushed during current/reset-or-resume context, not a guaranteed whole-packet immutable batch.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** getAllRecords is current TCP borrowed collection evidence. Multiple QUIC facts are represented by incremental semantic units; no collection, batch API or historical whole-datagram view is required without demonstrated consumer need.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. getAllRecords is current TCP borrowed collection evidence. Multiple QUIC facts are represented by incremental semantic units; no collection, batch API or historical whole-datagram view is required without demonstrated consumer need.
- **Representation differences:** Existing TCP borrowed bounded record collection versus incremental QUIC evidence units. Multiple facts do not require a collection, vector, batch API or immutable datagram history.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** Preserve zero/one/many semantic cardinality and current-unit validity; do not introduce a collection merely for symmetry.
- **Contract trace:** OP-02, OP-09, OP-10; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-008 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-009: getRecordType

- **TCP source/trigger:** getRecordType copies canonical current record type; no new wire trigger.
- **Semantic meaning:** Scalar copy of current m_record.recordType.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** The TLS record-type getter cannot represent QUIC evidence. Shared semantic classification is possible separately; no fabricated record getter is required.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. The TLS record-type getter cannot represent QUIC evidence. Shared semantic classification is possible separately; no fabricated record getter is required.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. The TLS record-type getter cannot represent QUIC evidence. Shared semantic classification is possible separately; no fabricated record getter is required.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02, OP-10; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-009 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-010: getRecordLength

- **TCP source/trigger:** getRecordLength copies canonical current record length; no new wire trigger.
- **Semantic meaning:** Scalar copy of current m_record.recordLength.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** The TLS record-length getter has no QUIC wire object. Retain proper packet/frame/message length bounds rather than a surrogate TLS length.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. The TLS record-length getter has no QUIC wire object. Retain proper packet/frame/message length bounds rather than a surrogate TLS length.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. The TLS record-length getter has no QUIC wire object. Retain proper packet/frame/message length bounds rather than a surrogate TLS length.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-02, OP-10; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-010 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-011: TLSRecordContext::record

- **TCP source/trigger:** Conductor builds current TLSRecordContext borrowing getLastRecord on RecordParsed analysis.
- **Semantic meaning:** Borrowed const Record& valid during current-record dispatch, per header contract.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** The semantic current-observation context can carry CH or close evidence, but TLSRecordContext::record itself is TCP representation. Share only actual consumer facts.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. The semantic current-observation context can carry CH or close evidence, but TLSRecordContext::record itself is TCP representation. Share only actual consumer facts.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. The semantic current-observation context can carry CH or close evidence, but TLSRecordContext::record itself is TCP representation. Share only actual consumer facts.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-09, OP-10; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-011 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-011; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-012: TLSRecordContext::currentClientHelloSupportedVersionsMask

- **TCP source/trigger:** Conductor reads supported-version viewer only if current record.isClientHello(); otherwise zero.
- **Semantic meaning:** Current CH mask for CH-shaped record; zero otherwise.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** Current CH advertised-version mask comes from the reconstructed CH and is valid only for that observation. Do not leak previous CH facts into a non-CH observation.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Current CH advertised-version mask comes from the reconstructed CH and is valid only for that observation. Do not leak previous CH facts into a non-CH observation.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Current CH advertised-version mask comes from the reconstructed CH and is valid only for that observation. Do not leak previous CH facts into a non-CH observation.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-09; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-012 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-012; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-013: TLSRecordContext::earlyDataOffered

- **TCP source/trigger:** Conductor reads early-data viewer only if current record.isClientHello(); otherwise false.
- **Semantic meaning:** Current CH early-data flag for CH-shaped record; false otherwise.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** Current CH early_data presence is identical content semantics. For other observations it is not a new CH offer fact.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Current CH early_data presence is identical content semantics. For other observations it is not a new CH offer fact.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Current CH early_data presence is identical content semantics. For other observations it is not a new CH offer fact.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-09; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-013 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-013; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-REC-014: TLSRecordContext::alertTerminating

- **TCP source/trigger:** Readable alert record plus stored advertised-version mask invokes isTerminatingAlert.
- **Semantic meaning:** Conductor-derived bool from committed version mask and readable alert fields; defaults false.
- **Observed/derived/heuristic nature:** Observed wire metadata or software projection; record shape is representation, not a transport-sharing verdict.
- **QUIC source, required processing and rationale:** Phase 2 Initial close association plus CRYPTO_ERROR range establishes terminating-alert evidence. No QUIC alert level is needed; TCP classifier rules are not copied blindly.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Phase 2 Initial close association plus CRYPTO_ERROR range establishes terminating-alert evidence. No QUIC alert level is needed; TCP classifier rules are not copied blindly.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Phase 2 Initial close association plus CRYPTO_ERROR range establishes terminating-alert evidence. No QUIC alert level is needed; TCP classifier rules are not copied blindly.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Project only current valid fields or borrow existing backing. Any collection traversal depends on actual entries and is not a required QUIC API.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-08, OP-09; sections 3, 5.2, 7, 8
- **Evidence:** V5 P3-REC-014 source anchors in Section 2; Step 4 V2 Appendix A P3-REC-014; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-001: getFingerprint

- **TCP source/trigger:** CH digest finalization produces m_fp; getter copies retained value without a fresh-event discriminator.
- **Semantic meaning:** Value copy of canonical m_fp; no per-call success bool.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Fingerprint is the same function of the same complete CH under USER-FP-01. It is not a function of TCP sequence, QUIC DCID or transport packetization.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Fingerprint is the same function of the same complete CH under USER-FP-01. It is not a function of TCP sequence, QUIC DCID or transport packetization.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Fingerprint is the same function of the same complete CH under USER-FP-01. It is not a function of TCP sequence, QUIC DCID or transport packetization.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-09, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-001 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-002: getSni

- **TCP source/trigger:** SNI extension parsing/validation sets copied Sni and parsed flag; getter tests flag.
- **Semantic meaning:** Copies canonical Sni only if parsed flag is true; returns false otherwise. Conductor ignores bool and supplies default empty Sni. The first host_name is validated and copied into owned Sni storage; the boolean distinguishes unavailable/non-host_name SNI from a successful value. This availability is not represented separately in ClientHelloData.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** SNI value and validity come from the same CH extension. Borrowed versus copied representation must preserve the value without assuming retention by external consumers.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. SNI value and validity come from the same CH extension. Borrowed versus copied representation must preserve the value without assuming retention by external consumers.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. SNI value and validity come from the same CH extension. Borrowed versus copied representation must preserve the value without assuming retention by external consumers.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-09, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-002 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-003: getSupportedVersionBitMask

- **TCP source/trigger:** supported_versions feeding ORs version bits, or CH wire-version fallback when parsed flag is false.
- **Semantic meaning:** Advertised mask from supported_versions, with CH wire-version fallback when absent. feedSuppVersionsFP ORs versions into m_supportedVersionsBitMask and sets m_supportedVersionsParsed; ordinary reset clears both, but beginPacket resume bypasses that reset. CH continuation restores lastVersion from its buffer, yet its fallback uses the shared ClientHello parser getVersion() when m_supportedVersionsParsed is false. Session-scoped continuation therefore does not itself prove current-session mask freshness. This is a static interleaving risk, not a verified runtime corruption.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Advertised versions come from CH supported_versions/fallback under the shared interpretation. Existing TCP shared-parser freshness risks are not approved QUIC behavior.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Advertised versions come from CH supported_versions/fallback under the shared interpretation. Existing TCP shared-parser freshness risks are not approved QUIC behavior.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Advertised versions come from CH supported_versions/fallback under the shared interpretation. Existing TCP shared-parser freshness risks are not approved QUIC behavior.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-003 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-004: isEarlyDataOffered

- **TCP source/trigger:** parseAllExtensions encounters early_data type; getter reads retained flag.
- **Semantic meaning:** Presence of early_data extension, not observed early-data packets.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** early_data extension presence is the same offer fact. QUIC 0-RTT packets neither prove acceptance nor change semantic state under Phase 2.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. early_data extension presence is the same offer fact. QUIC 0-RTT packets neither prove acceptance nor change semantic state under Phase 2.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. early_data extension presence is the same offer fact. QUIC 0-RTT packets neither prove acceptance nor change semantic state under Phase 2.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-004 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-005: getFullString

- **TCP source/trigger:** Digest append/finalization constructs byte buffer; getter returns borrowed pointer/length, not a new CH event.
- **Semantic meaning:** Borrowed pointer and length into active digest buffer, not an owned string.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Fingerprint full-string composition can be transport-independent for the same CH; its borrowed storage lifetime is not thereby shared or guaranteed.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Fingerprint full-string composition can be transport-independent for the same CH; its borrowed storage lifetime is not thereby shared or guaranteed.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Fingerprint full-string composition can be transport-independent for the same CH; its borrowed storage lifetime is not thereby shared or guaranteed.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-005 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-006: getTlsVersion

- **TCP source/trigger:** Record version before CH, CH wire version, or first non-GREASE supported version updates lastVersion.
- **Semantic meaning:** lastVersion; may start from record version, then CH version, then first non-GREASE supported version.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** The viewer's CH version selection is derived from CH bytes, not negotiated version or QUIC version. Use the same interpretation for identical CH.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. The viewer's CH version selection is derived from CH bytes, not negotiated version or QUIC version. Use the same interpretation for identical CH.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. The viewer's CH version selection is derived from CH bytes, not negotiated version or QUIC version. Use the same interpretation for identical CH.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-006 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-007: getCipherCount

- **TCP source/trigger:** Successful cipher parsing sets count; continuation restores saved count.
- **Semantic meaning:** Parsed cipher count, not necessarily count of bytes actually fed to digest.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Cipher-suite count is CH content. Preserve the count separately from the suspected cipher-feeding omission in the digest loop.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Cipher-suite count is CH content. Preserve the count separately from the suspected cipher-feeding omission in the digest loop.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Cipher-suite count is CH content. Preserve the count separately from the suspected cipher-feeding omission in the digest loop.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-007 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-008: getExtensionCount

- **TCP source/trigger:** Successful locator collection/sort sets count.
- **Semantic meaning:** Count of collected extension locators.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Extension count is CH content after parsing. CRYPTO-frame count and QUIC packet count are unrelated.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Extension count is CH content after parsing. CRYPTO-frame count and QUIC packet count are unrelated.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Extension count is CH content after parsing. CRYPTO-frame count and QUIC packet count are unrelated.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-008 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-009: getCompressionCount

- **TCP source/trigger:** CH body parsing sets compression count; continuation restores saved count.
- **Semantic meaning:** CH compression length/count.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Compression-list count is a CH field. QUIC TLS 1.3 constraints do not justify deleting it from shared CH evaluation or inventing a record fact.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Compression-list count is a CH field. QUIC TLS 1.3 constraints do not justify deleting it from shared CH evaluation or inventing a record fact.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Compression-list count is a CH field. QUIC TLS 1.3 constraints do not justify deleting it from shared CH evaluation or inventing a record fact.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-009 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-010: isSegmented

- **TCP source/trigger:** ProcessorState changes at continuation start/resume/completion; getter compares it to SingleSegment.
- **Semantic meaning:** Computed as ProcessorState != SingleSegment; completion resets to SingleSegment, so not durable historical segmentation metadata.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Acquisition fragmentedness can be meaningful, but QUIC ranges/frames/packets do not equal TCP ProcessorState. Do not share isSegmented semantics without defining the unit.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Acquisition fragmentedness can be meaningful, but QUIC ranges/frames/packets do not equal TCP ProcessorState. Do not share isSegmented semantics without defining the unit.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Acquisition fragmentedness can be meaningful, but QUIC ranges/frames/packets do not equal TCP ProcessorState. Do not share isSegmented semantics without defining the unit.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-010 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-011: getClientHelloParsingStatus

- **TCP source/trigger:** CH recognition/continuation/completion or packet reset changes canonical status.
- **Semantic meaning:** Canonical status persists across record steps until reset/updated; it is not a fresh-event discriminator. Complete can remain visible after record-capacity failure returns Stop, since it is set before the capacity check. Normal CH callback delivery requires the RecordParsed analysis path and applicable feedback; the Stop path does not dispatch that callback. See P3-OUT-009 and P3-FB-008.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** CH validity status is software metadata; it is not a new-event signal. Complete can coexist with failed delivery, so freshness needs a separate guarantee.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. CH validity status is software metadata; it is not a new-event signal. Complete can coexist with failed delivery, so freshness needs a separate guarantee.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. CH validity status is software metadata; it is not a new-event signal. Complete can coexist with failed delivery, so freshness needs a separate guarantee.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06, OP-07, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-011 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-011; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-012: Parsing-error viewer

- **TCP source/trigger:** IParsingErrorViewer getter reads stored error; no independent wire trigger.
- **Semantic meaning:** IParsingErrorViewer::getLastError exposes canonical error by value, not per-record owned result.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Expose actual parser/acquisition error metadata with protocol origin. Do not equate no error, successful observation and continuation status.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Expose actual parser/acquisition error metadata with protocol origin. Do not equate no error, successful observation and continuation status.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Expose actual parser/acquisition error metadata with protocol origin. Do not equate no error, successful observation and continuation status.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-06, OP-09, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-012 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-012; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-013: Shared instance wiring

- **TCP source/trigger:** Factory init injects one Processor instance into all viewer interfaces; software initialization, not a wire event.
- **Semantic meaning:** Factory injects the same Processor instance as IProcessor, CH viewer, record viewer and error viewer, and exposes those interfaces publicly. Results are not per-session immutable viewer objects.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Consumers read immutable current results through const-reference callbacks. Supplied TLS Fingerprint consumer additionally reads full-string from the shared viewer synchronously inside onClientHello. Enforcement forwards CH/record pointers into detectors and immediately invokes detect. Preserve current backing through these nested current-unit uses; prohibit expired-pointer dereference, scratch rebinding or later-unit precommit while borrows are live. Concrete backing choice remains Phase 4; no claim that const implies lifetime or no packet/session side effects.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Consumers read immutable current results through const-reference callbacks. Supplied TLS Fingerprint consumer additionally reads full-string from the shared viewer synchronously inside onClientHello. Enforcement forwards CH/record pointers into detectors and immediately invokes detect. Preserve current backing through these nested current-unit uses; prohibit expired-pointer dereference, scratch rebinding or later-unit precommit while borrows are live. Concrete backing choice remains Phase 4; no claim that const implies lifetime or no packet/session side effects.
- **Representation differences:** Const callback projection and borrowed shared full-string are different access paths. A pointer setter does not prove asynchronous use; it also does not prove ownership.
- **Acquisition cost:** No extra acquisition from getter access; current CH digest backing is already available.
- **Operation cost after evidence is available:** Local access/predicate/callback work only; no repeated Initial decryption per getter.
- **Sharing implications:** Stable current-unit backing through callback and nested calls; no speculative asynchronous snapshot API.
- **Contract trace:** OP-09, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-013 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-013; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

- **V3 closure evidence:** Section 8 U02-U05 and phase3-user-consumer-evidence-v1.md; current source anchors retained above.

### M / P3-VIEW-014: Reset/freshness boundary

- **TCP source/trigger:** beginPacket reset versus resumed early-return branch; later processRecord calls do not automatically reset CH facts.
- **Semantic meaning:** beginPacket resets ordinary new-packet CH/viewer/error state through resetStepState. A resumed path returns early and clears only the record collection/current record for non-retransmission/non-gap resume. There is no automatic per-record reset of CH Complete. resumeStepFromSegmentBuffer rebinds buffer/digest/locators and selects continuation mode, but does not restore/reset every canonical CH field. Supported-version mask and parsed flag, early-data/SNI facts and other shared viewer fields must not be assumed session-owned merely because continuation storage is keyed by session. See P3-VIEW-003 for the concrete version-mask/fallback dependency and P3-CONT-007 for the limited saved CH fields.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Freshness must be scoped to observation/attempt; QUIC retransmission/restart must not expose previous session facts. TCP reset/resume shortcuts are evidence of risk, not reusable guarantees.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Freshness must be scoped to observation/attempt; QUIC retransmission/restart must not expose previous session facts. TCP reset/resume shortcuts are evidence of risk, not reusable guarantees.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Freshness must be scoped to observation/attempt; QUIC retransmission/restart must not expose previous session facts. TCP reset/resume shortcuts are evidence of risk, not reusable guarantees.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-014 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-014; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-VIEW-015: Digest lifetime

- **TCP source/trigger:** CH digest init/append, continuation digest copy, active-pointer rebinding and later storage reuse.
- **Semantic meaning:** getFullString borrows active digest storage. CH continuation copies digest to initial storage before finishing continuation; cleanup resets active pointers, and later CH digest init/calculate overwrites contents. Pointer identity and freshness are not session ownership.
- **Observed/derived/heuristic nature:** Stored/exposed software fact; freshness and ownership must be assessed separately.
- **QUIC source, required processing and rationale:** Digest/CH backing storage can outlive acquisition or be released; borrowed full-string views need explicit validity. Do not select storage ownership or pooling in this matrix.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Digest/CH backing storage can outlive acquisition or be released; borrowed full-string views need explicit validity. Do not select storage ownership or pooling in this matrix.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Digest/CH backing storage can outlive acquisition or be released; borrowed full-string views need explicit validity. Do not select storage ownership or pooling in this matrix.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Read/copy current valid scalar/reference/span metadata; SNI or backing-byte copies scale with copied bytes. No new AEAD, CH parse or sorting at getter time.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-06, OP-10; sections 3, 5.1, 5.2, 10
- **Evidence:** V5 P3-VIEW-015 source anchors in Section 2; Step 4 V2 Appendix A P3-VIEW-015; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FP-001: Existing composition

- **TCP source/trigger:** Existing composition: CH wire fields or local digest/normalization operation described below.
- **Semantic meaning:** Existing feed path hashes CH wire version, cipher values and sorted extension types; selected extension lengths/content are included. This inventories actual implementation, not a new fingerprint requirement.
- **Observed/derived/heuristic nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **QUIC source, required processing and rationale:** Apply the same CH composition and selected extension-content rules after transport-independent CH reconstruction. USER-FP-01 forbids transport-dependent fingerprint differences.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Apply the same CH composition and selected extension-content rules after transport-independent CH reconstruction. USER-FP-01 forbids transport-dependent fingerprint differences.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Apply the same CH composition and selected extension-content rules after transport-independent CH reconstruction. USER-FP-01 forbids transport-dependent fingerprint differences.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Digest feeding/finalization scales with fed bytes; extension sorting/comparison belongs to P3-FP-003 and depends on locator count and compared content. Bounds/failure checks are separate.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-FP-001 source anchors in Section 2; Step 4 V2 Appendix A P3-FP-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FP-002: QUIC transport parameter content

- **TCP source/trigger:** QUIC transport parameter content: CH wire fields or local digest/normalization operation described below.
- **Semantic meaning:** isExtensionContentIncludedInFp explicitly includes quic_transport_parameters and draft type; feedExtensionsFP default branch hashes raw extension content after type/length. This is code evidence, not resolution of the deferred QUIC product requirement.
- **Observed/derived/heuristic nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **QUIC source, required processing and rationale:** quic_transport_parameters is CH extension content; its existing inclusion is based on extension type, not packet carrier. Identical CH gets identical inclusion over TCP and QUIC; requirement is resolved by USER-FP-01.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. quic_transport_parameters is CH extension content; its existing inclusion is based on extension type, not packet carrier. Identical CH gets identical inclusion over TCP and QUIC; requirement is resolved by USER-FP-01.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. quic_transport_parameters is CH extension content; its existing inclusion is based on extension type, not packet carrier. Identical CH gets identical inclusion over TCP and QUIC; requirement is resolved by USER-FP-01.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Digest feeding/finalization scales with fed bytes; extension sorting/comparison belongs to P3-FP-003 and depends on locator count and compared content. Bounds/failure checks are separate.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-FP-002 source anchors in Section 2; Step 4 V2 Appendix A P3-FP-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FP-003: Normalization/sorting

- **TCP source/trigger:** Normalization/sorting: CH wire fields or local digest/normalization operation described below.
- **Semantic meaning:** Extension type is deGrease-normalized; locators sort with comparator by type/length/value. Supported versions/groups use specialized deGrease feeding. Preserve exact comparator semantics, not comments alone.
- **Observed/derived/heuristic nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **QUIC source, required processing and rationale:** Use the same sorting/normalization rules for identical CH. Do not introduce QUIC-only parameter normalization or transport identity into the digest.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Use the same sorting/normalization rules for identical CH. Do not introduce QUIC-only parameter normalization or transport identity into the digest.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Use the same sorting/normalization rules for identical CH. Do not introduce QUIC-only parameter normalization or transport identity into the digest.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Digest feeding/finalization scales with fed bytes; extension sorting/comparison belongs to P3-FP-003 and depends on locator count and compared content. Bounds/failure checks are separate.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-FP-003 source anchors in Section 2; Step 4 V2 Appendix A P3-FP-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FP-004: Digest capacity/failure

- **TCP source/trigger:** Digest capacity/failure: CH wire fields or local digest/normalization operation described below.
- **Semantic meaning:** Md5Digest.calculate rejects zero length or len + current index >= 2870-byte array size; failure can stop processing without a dedicated error. finalize computes MD5 over accumulated bytes. The local failure must be traced through its caller: cipher feeding can become NeedMoreData on an incomplete record or Stop on a complete record, subject to the record-capacity check; see P3-ERR-019. Other feeding failures must not be assigned that cipher-specific path automatically.
- **Observed/derived/heuristic nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **QUIC source, required processing and rationale:** The CH digest builder's capacity/failure semantics are carrier-independent. Acquisition cost differs; failure must not silently yield a valid fingerprint or imply packet drop.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. The CH digest builder's capacity/failure semantics are carrier-independent. Acquisition cost differs; failure must not silently yield a valid fingerprint or imply packet drop.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. The CH digest builder's capacity/failure semantics are carrier-independent. Acquisition cost differs; failure must not silently yield a valid fingerprint or imply packet drop.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Digest feeding/finalization scales with fed bytes; extension sorting/comparison belongs to P3-FP-003 and depends on locator count and compared content. Bounds/failure checks are separate.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-FP-004 source anchors in Section 2; Step 4 V2 Appendix A P3-FP-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FP-005: Cipher feeding loop detail

- **TCP source/trigger:** Cipher feeding loop detail: CH wire fields or local digest/normalization operation described below.
- **Semantic meaning:** processCiphers uses for (int i = 0; cipherSuitesLen -= 2; i++). For an even nonzero validated length, the last suite is not fed by this loop. Do not replace actual behavior with an assumption that all counted suites are hashed; no repair is made here.
- **Observed/derived/heuristic nature:** Derived fingerprint computation over observed CH bytes; current implementation, not a product requirement.
- **QUIC source, required processing and rationale:** User explicitly selects preservation of existing fingerprint algorithm/output for both transports in this task. processCiphers decrements byte length before loop body, feeding suites 0..N-2 and omitting N-1 for valid N suites. This verified legacy defect is documented, not repaired in QUIC alone. Any common fix is a separate approved compatibility task, not an open Phase 3 choice.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** User explicitly selects preservation of existing fingerprint algorithm/output for both transports in this task. processCiphers decrements byte length before loop body, feeding suites 0..N-2 and omitting N-1 for valid N suites. This verified legacy defect is documented, not repaired in QUIC alone. Any common fix is a separate approved compatibility task, not an open Phase 3 choice.
- **Representation differences:** Same existing algorithm after reconstruction; transport framing is not digest content. Preservation does not claim algorithm correctness.
- **Acquisition cost:** Shared evaluator work; no transport-dependent repair or extra sorting merely for symmetry.
- **Operation cost after evidence is available:** Local access/predicate/callback work only; no repeated Initial decryption per getter.
- **Sharing implications:** Preserve outputs in both transports; isolate any later common fix from this task.
- **Contract trace:** OP-06; sections 3, 6, 10
- **Evidence:** V5 P3-FP-005 source anchors in Section 2; Step 4 V2 Appendix A P3-FP-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

- **V3 closure evidence:** Section 8 U02-U05 and phase3-user-consumer-evidence-v1.md; current source anchors retained above.

### M / P3-OBS-001: Completed CH

- **TCP source/trigger:** Completed CH status on a CH-shaped record, successful normal update path, packet timestamp.
- **Semantic meaning:** On the normal-update path for a CH-shaped record, Complete status permits count increment and replacement of timestamp, fingerprint, version mask and early-data flag. PreHandshake becomes InProgress. Complete alone is not sufficient: step delivery, current record shape and observer feedback can prevent this update.
- **Observed/derived/heuristic nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **QUIC source, required processing and rationale:** A complete parseable authoritative CH is the same semantic fact. Phase 2 determines whether a candidate is retransmission/replacement/new attempt before it becomes authoritative.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. A complete parseable authoritative CH is the same semantic fact. Phase 2 determines whether a candidate is retransmission/replacement/new attempt before it becomes authoritative.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. A complete parseable authoritative CH is the same semantic fact. Phase 2 determines whether a candidate is retransmission/replacement/new attempt before it becomes authoritative.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-07, OP-09; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-001 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-002: CCS

- **TCP source/trigger:** CCS-shaped record plus committed CH count, version mask and hsState.
- **Semantic meaning:** On the normal-update path for a CCS-shaped record, clientHelloCount == 0 increments preCcsSeen; otherwise a non-TLS13 mask with InProgress changes state to ChangeCipherSpec. The zero-count predicate is a stored uint8_t counter check, not independently verified absence of every earlier CH.
- **Observed/derived/heuristic nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **QUIC source, required processing and rationale:** TLS CCS wire observation does not exist in QUIC. Do not invent CCS from a Handshake packet.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. TLS CCS wire observation does not exist in QUIC. Do not invent CCS from a Handshake packet.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. TLS CCS wire observation does not exist in QUIC. Do not invent CCS from a Handshake packet.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-08; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-002 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-003: Handshake after CCS

- **TCP source/trigger:** Handshake-shaped record while stored hsState is ChangeCipherSpec; no decoded TLS Finished is required by current code.
- **Semantic meaning:** On the normal-update path, a handshake-shaped record while stored hsState is ChangeCipherSpec changes state to Finished. The encrypted TLS Finished content is not decoded to prove this; this is a completion heuristic, subject to feedback allowing normal update.
- **Observed/derived/heuristic nature:** Heuristic completion inference from record shape and prior state, not direct observation of TLS Finished.
- **QUIC source, required processing and rationale:** TCP handshake-after-CCS is a completion heuristic; QUIC uses associated client v1 Handshake Long Header evidence under Phase 2. These are analogous product completion signals, not the same TLS wire fact.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. TCP handshake-after-CCS is a completion heuristic; QUIC uses associated client v1 Handshake Long Header evidence under Phase 2. These are analogous product completion signals, not the same TLS wire fact.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. TCP handshake-after-CCS is a completion heuristic; QUIC uses associated client v1 Handshake Long Header evidence under Phase 2. These are analogous product completion signals, not the same TLS wire fact.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-08; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-003 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-004: ApplicationData

- **TCP source/trigger:** ApplicationData-shaped record plus CH count, advertised TLS13 bit, InProgress and !earlyDataOffered; no decoded TLS Finished is observed.
- **Semantic meaning:** On the normal-update path for ApplicationData, clientHelloCount == 0 increments earlyDataCount. Otherwise an advertised TLS13 bit with InProgress and !earlyDataOffered changes state to Finished. This is the current TCP completion heuristic, not a decoded Finished observation.
- **Observed/derived/heuristic nature:** Heuristic completion inference from record shape and prior state, not direct observation of TLS Finished.
- **QUIC source, required processing and rationale:** TCP ApplicationData completion heuristic has no direct QUIC counterpart. Associated client Handshake header is the accepted completion signal; 0-RTT/short-header traffic does not substitute.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. TCP ApplicationData completion heuristic has no direct QUIC counterpart. Associated client Handshake header is the accepted completion signal; 0-RTT/short-header traffic does not substitute.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. TCP ApplicationData completion heuristic has no direct QUIC counterpart. Associated client Handshake header is the accepted completion signal; 0-RTT/short-header traffic does not substitute.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-08; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-004 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-005: Readable terminating alert

- **TCP source/trigger:** Alert-shaped record, readableAlert, derived alertTerminating and active hsState.
- **Semantic meaning:** Active state + alert record + readableAlert + alertTerminating -> TerminatedByAlert.
- **Observed/derived/heuristic nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **QUIC source, required processing and rationale:** Associated successfully parsed Initial CONNECTION_CLOSE type 0x1c with CRYPTO_ERROR 0x100..0x1ff gives Phase 2 terminal-alert evidence. Includes user_canceled 0x15a; excludes other close reasons.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Associated successfully parsed Initial CONNECTION_CLOSE type 0x1c with CRYPTO_ERROR 0x100..0x1ff gives Phase 2 terminal-alert evidence. Includes user_canceled 0x15a; excludes other close reasons.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Associated successfully parsed Initial CONNECTION_CLOSE type 0x1c with CRYPTO_ERROR 0x100..0x1ff gives Phase 2 terminal-alert evidence. Includes user_canceled 0x15a; excludes other close reasons.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-08; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-005 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-006: Unsupported disposition

- **TCP source/trigger:** Public Unsupported plus continuation-gap flag, hsState and CH Partial status.
- **Semantic meaning:** Gap, PreHandshake, or InProgress with Partial CH -> Unsupported. Public Unsupported alone does not imply unconditional state transition.
- **Observed/derived/heuristic nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **QUIC source, required processing and rationale:** Unsupported means required current-scope evaluation cannot continue under Phase 2 conditions. Keep parser errors, out-of-scope input, ambiguity and eviction distinctions.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Unsupported means required current-scope evaluation cannot continue under Phase 2 conditions. Keep parser errors, out-of-scope input, ambiguity and eviction distinctions.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Unsupported means required current-scope evaluation cannot continue under Phase 2 conditions. Keep parser errors, out-of-scope input, ambiguity and eviction distinctions.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-03, OP-08, OP-10; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-006 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-007: TCP reset

- **TCP source/trigger:** TCP RST accepted by existing legal-RST predicate with no legal ACK, plus listener feedback.
- **Semantic meaning:** RST without ACK triggers callback; no violation mask clears TLSData even if dispatch Stop. Nonzero mask updates existing feedback state.
- **Observed/derived/heuristic nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **QUIC source, required processing and rationale:** TCP RST is absent. QUIC stateless reset cannot be assumed observable/validated with Initial-only context; no automatic RST mapping.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. TCP RST is absent. QUIC stateless reset cannot be assumed observable/validated with Initial-only context; no automatic RST mapping.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. TCP RST is absent. QUIC stateless reset cannot be assumed observable/validated with Initial-only context; no automatic RST mapping.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-10; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-007 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-008: TCP session reuse observation

- **TCP source/trigger:** TCP SYN-without-ACK predicate plus active-state/violation metadata test.
- **Semantic meaning:** For a TCP SYN-without-ACK predicate, an existing active extension with no recorded violation is preserved. An existing extension failing that test is released and rebuilt; if no extension exists, creation is attempted directly. This is current TCP entry behavior, not Phase 2 QUIC attempt policy.
- **Observed/derived/heuristic nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **QUIC source, required processing and rationale:** Complete new-attempt classification within the same DP session replaces active attempt under Phase 2. Changed DCID alone is not proof; Retry-compatible restart is not session reuse.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Complete new-attempt classification within the same DP session replaces active attempt under Phase 2. Changed DCID alone is not proof; Retry-compatible restart is not session reuse.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Complete new-attempt classification within the same DP session replaces active attempt under Phase 2. Changed DCID alone is not proof; Retry-compatible restart is not session reuse.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-03, OP-07, OP-10; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-008 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-009: Finished reclamation observation

- **TCP source/trigger:** Normal-update transition into Finished calls markReclaimable; software lifecycle attempt, not an independent wire event.
- **Semantic meaning:** After a normal current-record state change into Finished, Conductor calls markReclaimable when the extension table exists. The method returns bool, but the caller ignores it; successful marking is not guaranteed by this call. Marking does not immediately release the extension. This is existing lifecycle evidence, not pool design.
- **Observed/derived/heuristic nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **QUIC source, required processing and rationale:** Product Finished may permit later reclamation, but retained identity and late-input suppression still matter. Do not choose resource release timing or treat eviction as completion.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Product Finished may permit later reclamation, but retained identity and late-input suppression still matter. Do not choose resource release timing or treat eviction as completion.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Product Finished may permit later reclamation, but retained identity and late-input suppression still matter. Do not choose resource release timing or treat eviction as completion.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-08, OP-10; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-009 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-OBS-010: Alert terminal classifier

- **TCP source/trigger:** Readable alert level/description plus advertised-version mask; derived terminal classification.
- **Semantic meaning:** Existing helper rejects invalid levels; close_notify is terminal; TLS13-only excludes user_canceled; TLS12-or-less-only uses fatal level; mixed/unknown mask uses fatal except user_canceled. Inventory of helper behavior only, not independent protocol verification.
- **Observed/derived/heuristic nature:** Derived state/control decision from observed input and stored context; exact guards apply.
- **QUIC source, required processing and rationale:** QUIC close classification uses error-code range and active-attempt association, not TCP alert-level/version classifier. user_canceled has different terminal significance because the close terminates the connection.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Different
- **Semantic boundary and rationale:** The acceptance/disposition predicate differs; generic purpose alone does not make it equivalent. QUIC close classification uses error-code range and active-attempt association, not TCP alert-level/version classifier. user_canceled has different terminal significance because the close terminates the connection.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. QUIC close classification uses error-code range and active-attempt association, not TCP alert-level/version classifier. user_canceled has different terminal significance because the close terminates the connection.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Translate the concrete current outcome/reason or evaluate its exact guard. Resource preparation and syntax checks retain their own costs; a result read is not a second parse.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-08; sections 3, 6, 7
- **Evidence:** V5 P3-OBS-010 source anchors in Section 2; Step 4 V2 Appendix A P3-OBS-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CB-001: onTLSRecord

- **TCP source/trigger:** RecordParsed analysis invokes onTLSRecord with current record context before normal state update.
- **Semantic meaning:** Receives Session&, DPFrame&, borrowed TLSRecordContext and const TLSData& before normal update.
- **Observed/derived/heuristic nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **QUIC source, required processing and rationale:** Consumers may need acquired semantic evidence, but onTLSRecord is a TCP callback shape. CH/close/header evidence are not synthetic TLS records; exact future callback shape remains later work.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Consumers may need acquired semantic evidence, but onTLSRecord is a TCP callback shape. CH/close/header evidence are not synthetic TLS records; exact future callback shape remains later work.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Consumers may need acquired semantic evidence, but onTLSRecord is a TCP callback shape. CH/close/header evidence are not synthetic TLS records; exact future callback shape remains later work.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Construct the current projection and invoke eligible listeners; cost depends on copied fields and invoked listeners. Listener-internal work is unmeasured.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-09; sections 3, 5.1, 5.2, 7
- **Evidence:** V5 P3-CB-001 source anchors in Section 2; Step 4 V2 Appendix A P3-CB-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CB-002: onClientHello

- **TCP source/trigger:** RecordParsed analysis and retained CH status Complete, subject to feedback; not necessarily a new CH-shaped record.
- **Semantic meaning:** onClientHello receives Session&, DPFrame&, a stack-local ClientHelloData projection and const TLSData&. ClientHelloData stores const references to the stack-local fingerprint and Sni values created in onClientHello; it does not own copies of those values. Its TLS version is a converted Misc::TLSVersion, and its three counts are uint16_t constructor fields fed by uint32_t viewer getters. These references must not be treated as valid after the synchronous callback returns; retaining the projection by value does not make the referents owned. Callback eligibility is retained Complete status on RecordParsed analysis, not a fresh-event token or necessarily a CH-shaped current record.
- **Observed/derived/heuristic nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **QUIC source, required processing and rationale:** Reconstruct a complete parseable CH from associated Initial CRYPTO bytes, then apply Phase 2 authority/retransmission/attempt rules. The CH field values are shared for the same CH. The current TCP callback is eligible on retained Complete status during RecordParsed analysis, whereas a new QUIC CH observation must be identified by logical accepted CH production, not a retained viewer flag. Same bytes at accepted offsets are retransmission; a complete CH at new offsets can be a new logical CH even with identical contents. P3-FB-008/U05 explicitly preserve existing TCP callback behavior for this task. Do not silently fix it or reproduce it as a new QUIC logical CH event.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Reconstruct a complete parseable CH from associated Initial CRYPTO bytes, then apply Phase 2 authority/retransmission/attempt rules. The CH field values are shared for the same CH. The current TCP callback is eligible on retained Complete status during RecordParsed analysis, whereas a new QUIC CH observation must be identified by logical accepted CH production, not a retained viewer flag. Same bytes at accepted offsets are retransmission; a complete CH at new offsets can be a new logical CH even with identical contents. P3-FB-008/U05 explicitly preserve existing TCP callback behavior for this task. Do not silently fix it or reproduce it as a new QUIC logical CH event.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Deliver an authoritative newly completed CH observation with valid shared CH facts. Retransmission or retained Complete must not automatically generate a new completion event.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Construct the current projection and invoke eligible listeners; cost depends on copied fields and invoked listeners. Listener-internal work is unmeasured.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-07, OP-09, OP-10; sections 3, 5.1, 5.2, 7
- **Evidence:** V5 P3-CB-002 source anchors in Section 2; Step 4 V2 Appendix A P3-CB-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CB-003: onParsingError

- **TCP source/trigger:** Non-None stored parsing error and subscribed anomaly listeners on analysis/Stop path.
- **Semantic meaning:** Receives error by value plus Session&, DPFrame&, const TLSData&; called only for non-None error and nonempty anomaly observers.
- **Observed/derived/heuristic nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **QUIC source, required processing and rationale:** Report actual parsing/acquisition failure distinctly from observer verdict and Unsupported. QUIC-specific origin/reason must not be erased by legacy names.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Report actual parsing/acquisition failure distinctly from observer verdict and Unsupported. QUIC-specific origin/reason must not be erased by legacy names.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Report actual parsing/acquisition failure distinctly from observer verdict and Unsupported. QUIC-specific origin/reason must not be erased by legacy names.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Construct the current projection and invoke eligible listeners; cost depends on copied fields and invoked listeners. Listener-internal work is unmeasured.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-09; sections 3, 5.1, 5.2, 7
- **Evidence:** V5 P3-CB-003 source anchors in Section 2; Step 4 V2 Appendix A P3-CB-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CB-004: onTCPReset

- **TCP source/trigger:** Observed TCP reset control predicate.
- **Semantic meaning:** Receives Session&, DPFrame&, const TLSData& for observed TCP reset.
- **Observed/derived/heuristic nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **QUIC source, required processing and rationale:** No TCPReset callback is implied by QUIC close or unsupported stateless-reset input. Keep transport-specific reset behavior separate.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Exact carrier-specific field, predicate or mechanism; related QUIC semantics are not aliases. No TCPReset callback is implied by QUIC close or unsupported stateless-reset input. Keep transport-specific reset behavior separate.
- **Representation differences:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately. No TCPReset callback is implied by QUIC close or unsupported stateless-reset input. Keep transport-specific reset behavior separate.
- **Acquisition cost:** No QUIC producer for this exact item. Related acquisition belongs to its own operation.
- **Operation cost after evidence is available:** No proposed QUIC local operation for this exact item.
- **Sharing implications:** Retain TCP behavior; do not manufacture a QUIC TLS record. Related QUIC semantics are described separately.
- **Contract trace:** OP-10; sections 3, 5.1, 5.2, 7
- **Evidence:** V5 P3-CB-004 source anchors in Section 2; Step 4 V2 Appendix A P3-CB-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CB-005: onSessionEnd

- **TCP source/trigger:** Session aging event before extension release; no required wire trigger.
- **Semantic meaning:** Receives Session&, const TLSData& during aging before extension release; notify result is ignored by onAging.
- **Observed/derived/heuristic nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **QUIC source, required processing and rationale:** DP session end is a lifecycle event independent of transport, but attempt association and retained identity cleanup differ. No proof of TLS completion follows from session aging.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. DP session end is a lifecycle event independent of transport, but attempt association and retained identity cleanup differ. No proof of TLS completion follows from session aging.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. DP session end is a lifecycle event independent of transport, but attempt association and retained identity cleanup differ. No proof of TLS completion follows from session aging.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Construct the current projection and invoke eligible listeners; cost depends on copied fields and invoked listeners. Listener-internal work is unmeasured.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-10; sections 3, 5.1, 5.2, 7
- **Evidence:** V5 P3-CB-005 source anchors in Section 2; Step 4 V2 Appendix A P3-CB-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-CB-006: onTerminalPayload

- **TCP source/trigger:** Nonempty payload while stored state uses terminal-payload path; parsing is bypassed.
- **Semantic meaning:** Receives Session&, DPFrame&, const TLSData& without processor parsing in Error/TerminatedByAlert path.
- **Observed/derived/heuristic nature:** Delivered callback observation; delivery cardinality is not automatically wire-event cardinality.
- **QUIC source, required processing and rationale:** Terminal-state payload routing may be useful, but Phase 2 new-attempt classification must precede blanket terminal routing. Encrypted QUIC payload cannot be represented as decrypted TLS content.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Analogous only
- **Semantic boundary and rationale:** Comparable observation/access/control role, not identical producer, eligibility, cardinality or borrowing contract. Terminal-state payload routing may be useful, but Phase 2 new-attempt classification must precede blanket terminal routing. Encrypted QUIC payload cannot be represented as decrypted TLS content.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Terminal-state payload routing may be useful, but Phase 2 new-attempt classification must precede blanket terminal routing. Encrypted QUIC payload cannot be represented as decrypted TLS content.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Construct the current projection and invoke eligible listeners; cost depends on copied fields and invoked listeners. Listener-internal work is unmeasured.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-03, OP-10; sections 3, 5.1, 5.2, 7
- **Evidence:** V5 P3-CB-006 source anchors in Section 2; Step 4 V2 Appendix A P3-CB-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FB-001: Step-by-step state feedback

- **TCP source/trigger:** Software control point Step-by-step state feedback; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** Conductor calls processRecord(tlsData), dispatches current result, then checks updated active state before another call. No explicit accept/commit callback is passed to Processor; feedback is control flow and next-call state.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Further acquisition must observe applicable state/feedback decisions. This is a semantic feedback requirement, not a choice of traversal owner or full dispatch order.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Further acquisition must observe applicable state/feedback decisions. This is a semantic feedback requirement, not a choice of traversal owner or full dispatch order.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Further acquisition must observe applicable state/feedback decisions. This is a semantic feedback requirement, not a choice of traversal owner or full dispatch order.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Current-unit verdict/state checks and listener accumulation; no acquisition is performed by feedback itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-001 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FB-002: Observer feedback shape

- **TCP source/trigger:** Software control point Observer feedback shape; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** TLSFeatureVerdict carries independent Dispatch Continue/Stop and violationMask. These are not parser results, packet-drop instructions or automatic Error transitions.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Continue/Stop and violation metadata stay independent. No automatic network drop, new detector or Error transition is introduced by QUIC support.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Continue/Stop and violation metadata stay independent. No automatic network drop, new detector or Error transition is introduced by QUIC support.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Continue/Stop and violation metadata stay independent. No automatic network drop, new detector or Error transition is introduced by QUIC support.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Current-unit verdict/state checks and listener accumulation; no acquisition is performed by feedback itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-002 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FB-003: Within-list STOP

- **TCP source/trigger:** Software control point Within-list STOP; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** notify accumulates existing mask metadata and stops the current observer list immediately on Dispatch::Stop; expired weak observers are skipped.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Stop within the current listener list remains a feedback boundary. Expired observers are software lifecycle facts, not protocol evidence.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Stop within the current listener list remains a feedback boundary. Expired observers are software lifecycle facts, not protocol evidence.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Stop within the current listener list remains a feedback boundary. Expired observers are software lifecycle facts, not protocol evidence.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Current-unit verdict/state checks and listener accumulation; no acquisition is performed by feedback itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-003 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FB-004: Across-stage STOP without mask

- **TCP source/trigger:** Software control point Across-stage STOP without mask; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** analyzeProcessResult sets sawStop and can still run later stages/current normal update when Stop has no accumulated mask. It returns Stop after current processing; next record is not processed.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Existing soft observer Stop permits current-stage work according to the TCP boundary while preventing later processing. QUIC contract must preserve required semantics without inventing full dispatch order here.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Existing soft observer Stop permits current-stage work according to the TCP boundary while preventing later processing. QUIC contract must preserve required semantics without inventing full dispatch order here.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Existing soft observer Stop permits current-stage work according to the TCP boundary while preventing later processing. QUIC contract must preserve required semantics without inventing full dispatch order here.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Current-unit verdict/state checks and listener accumulation; no acquisition is performed by feedback itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-004 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FB-005: Feedback that suppresses normal update

- **TCP source/trigger:** Software control point Feedback that suppresses normal update; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** Nonzero accumulated mask suppresses normal current-record update. Combined Stop causes Error; mask without Stop records metadata without state change. Existing boundary behavior only, no policy design.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Existing feedback can suppress normal update independently of parser success. Share the boundary requirement, not mitigation or violation policy design.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Existing feedback can suppress normal update independently of parser success. Share the boundary requirement, not mitigation or violation policy design.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Existing feedback can suppress normal update independently of parser success. Share the boundary requirement, not mitigation or violation policy design.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Current-unit verdict/state checks and listener accumulation; no acquisition is performed by feedback itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-07, OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-005 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FB-006: Parser STOP handling

- **TCP source/trigger:** Software control point Parser STOP handling; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** Public Stop runs parsing-error observers only. With no non-None error or no observers, verdict is default Continue; outer record loop still ends because result is Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** A processing stop need not have a parsing error or observer Stop. QUIC must preserve the distinction between traversal ending and the verdict returned to the outer consumer.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. A processing stop need not have a parsing error or observer Stop. QUIC must preserve the distinction between traversal ending and the verdict returned to the outer consumer.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. A processing stop need not have a parsing error or observer Stop. QUIC must preserve the distinction between traversal ending and the verdict returned to the outer consumer.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Current-unit verdict/state checks and listener accumulation; no acquisition is performed by feedback itself.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-006 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FB-007: State gate after record

- **TCP source/trigger:** Software control point State gate after record; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** Even RecordParsed does not guarantee another call: observer Stop returns immediately; non-active updated state ends loop. Remaining bytes can be left unprocessed.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** State/observer feedback can end further processing even after a successful observation. Remaining datagram packets/frames must not imply unconditional continued dispatch.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. State/observer feedback can end further processing even after a successful observation. Remaining datagram packets/frames must not imply unconditional continued dispatch.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. State/observer feedback can end further processing even after a successful observation. Remaining datagram packets/frames must not imply unconditional continued dispatch.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Current-unit verdict/state checks and listener accumulation; no acquisition is performed by feedback itself.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-08, OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-007 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-FB-008: CH callback cardinality coupling

- **TCP source/trigger:** Software control point CH callback cardinality coupling; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** CH callback condition tests Complete status, not record.isClientHello(). Complete persists across later steps. Thus a later RecordParsed non-CH record in the same packet can trigger another CH callback with retained CH facts, if prior feedback/state permits reaching it. Record context CH fields are still zero/false for that non-CH record. This is reachable code behavior, not an endorsed contract.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Retained Complete can cause repeated current TCP CH notification if feedback/state permits. User selects no TCP behavior change in this task. This legacy notification predicate is not a new QUIC event requirement. QUIC uses newly eligible logical CH identity, not retained Complete. A proven bug may be addressed separately with explicit correction scope; no automatic repair is authorized here.
- **Classification:** TCP-specific
- **Semantic relation:** Different
- **Semantic boundary and rationale:** Retained Complete can cause repeated current TCP CH notification if feedback/state permits. User selects no TCP behavior change in this task. This legacy notification predicate is not a new QUIC event requirement. QUIC uses newly eligible logical CH identity, not retained Complete. A proven bug may be addressed separately with explicit correction scope; no automatic repair is authorized here.
- **Representation differences:** TCP retained canonical Complete versus QUIC logical CH eligibility; intended available/fresh distinction is shared but exact legacy repeated-notification behavior is not.
- **Acquisition cost:** No additional acquisition; predicate uses current stored status.
- **Operation cost after evidence is available:** Local access/predicate/callback work only; no repeated Initial decryption per getter.
- **Sharing implications:** Preserve existing TCP eligibility and order; do not copy suspected duplicate behavior into QUIC.
- **Contract trace:** OP-02, OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-008 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

- **V3 closure evidence:** Section 8 U02-U05 and phase3-user-consumer-evidence-v1.md; current source anchors retained above.

### M / P3-FB-009: Multiple CH scope

- **TCP source/trigger:** Software control point Multiple CH scope; exact listener/step/state condition is detailed below. No independent wire trigger.
- **Semantic meaning:** Processor rejects a new CH when its canonical status is already Complete. Ordinary beginPacket resets status; this is not an unconditional prohibition on a second CH across the whole session. Do not import it as Phase 2 attempt policy.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** TCP Complete-status guard is processing-context scoped. QUIC duplicate/retransmission/CH2/new-attempt classification follows Phase 2, not a session-wide ban.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Different
- **Semantic boundary and rationale:** The acceptance/disposition predicate differs; generic purpose alone does not make it equivalent. TCP Complete-status guard is processing-context scoped. QUIC duplicate/retransmission/CH2/new-attempt classification follows Phase 2, not a session-wide ban.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. TCP Complete-status guard is processing-context scoped. QUIC duplicate/retransmission/CH2/new-attempt classification follows Phase 2, not a session-wide ban.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Current-unit verdict/state checks and listener accumulation; no acquisition is performed by feedback itself.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-07, OP-09; sections 3, 4, 5.1, 7
- **Evidence:** V5 P3-FB-009 source anchors in Section 2; Step 4 V2 Appendix A P3-FB-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-001: IClientHelloParser::Status::Complete

- **TCP source/trigger:** IClientHelloParser::Status::Complete returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal ClientHello parser Complete means the CH prefix/body parse returned Complete. processClientHello advances its record-local pointer by consumed bytes and returns ContinueProcessing. This is not yet a completed fingerprint or a public RecordParsed result: processNewRecord still applies handshake/record-length checks, version/cipher/extension feeding, completion and capacity checks. Those later operations may yield Unsupported, Stop, or continuation. See P3-OUT-006 and P3-OUT-009.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Complete CH prefix/body parsing is the same TLS syntax result, not necessarily completed fingerprint or delivered observation. Later validation/feeding can still fail.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Complete CH prefix/body parsing is the same TLS syntax result, not necessarily completed fingerprint or delivered observation. Later validation/feeding can still fail.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Complete CH prefix/body parsing is the same TLS syntax result, not necessarily completed fingerprint or delivered observation. Later validation/feeding can still fail.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-001 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-001; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-002: IClientHelloParser::Status::IncompleteClientHelloPrefix

- **TCP source/trigger:** IClientHelloParser::Status::IncompleteClientHelloPrefix returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal CH parser status. If recordLevelFragmented is true, processClientHello returns Unsupported before starting continuation. Otherwise, if m_currentRecordIsSegmented is true, it returns Unsupported for the incomplete prefix/body. If neither condition holds, it assigns ClientHelloBodyInvalid and returns Error. The caller propagates these outcomes: Unsupported -> public Unsupported; Error -> public Stop. Neither incomplete prefix nor incomplete body is accepted as extension-only continuation.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Missing CH prefix bytes in Initial CRYPTO require more contiguous input; TCP segmented-prefix Unsupported shortcut does not define QUIC support. Invalid complete prefix remains a syntax failure.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Missing CH prefix bytes in Initial CRYPTO require more contiguous input; TCP segmented-prefix Unsupported shortcut does not define QUIC support. Invalid complete prefix remains a syntax failure.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Missing CH prefix bytes in Initial CRYPTO require more contiguous input; TCP segmented-prefix Unsupported shortcut does not define QUIC support. Invalid complete prefix remains a syntax failure.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-002 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-002; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-003: IClientHelloParser::Status::IncompleteClientHelloBody

- **TCP source/trigger:** IClientHelloParser::Status::IncompleteClientHelloBody returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal CH parser status. If recordLevelFragmented is true, processClientHello returns Unsupported before starting continuation. Otherwise, if m_currentRecordIsSegmented is true, it returns Unsupported for the incomplete prefix/body. If neither condition holds, it assigns ClientHelloBodyInvalid and returns Error. The caller propagates these outcomes: Unsupported -> public Unsupported; Error -> public Stop. Neither incomplete prefix nor incomplete body is accepted as extension-only continuation.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Missing CH body bytes require CRYPTO reconstruction, not the TCP early-body Unsupported shortcut. Completeness and validity remain separate.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Missing CH body bytes require CRYPTO reconstruction, not the TCP early-body Unsupported shortcut. Completeness and validity remain separate.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Missing CH body bytes require CRYPTO reconstruction, not the TCP early-body Unsupported shortcut. Completeness and validity remain separate.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-003 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-003; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-004: IClientHelloParser::Status::IncompleteExtensionSection

- **TCP source/trigger:** IClientHelloParser::Status::IncompleteExtensionSection returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal CH parser status. recordLevelFragmented first yields Unsupported/public Unsupported. Otherwise processClientHello attempts ClientHelloExtensionReassembly allocation; allocation failure sets SegmentBufferAllocationFailure and returns StopProcessing/public Stop. Success selects FirstSegment, records continuation metadata, advances by consumed prefix/body bytes and returns ContinueProcessing locally. The caller still checks handshake/record length and performs feeding; successful partial extension copying returns Segmented/public NeedMoreData. Buffer-bound/consistency or feeding failures may instead yield Unsupported or Stop. Allocation alone is not a completed CH or a guarantee of NeedMoreData.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Missing extension-section bytes require offset-based CH reconstruction. QUIC is not restricted to TCP extension-only continuation or one-record equality.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Missing extension-section bytes require offset-based CH reconstruction. QUIC is not restricted to TCP extension-only continuation or one-record equality.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Missing extension-section bytes require offset-based CH reconstruction. QUIC is not restricted to TCP extension-only continuation or one-record equality.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-004 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-004; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-005: IClientHelloParser::Status::Failure

- **TCP source/trigger:** IClientHelloParser::Status::Failure returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal CH parser Failure falls through the non-Complete handling to ClientHelloBodyInvalid/Error: the record-level and segmented-prefix/body exceptions test incomplete statuses, not Failure. processNewRecord propagates Error, translated to public Stop. It does not start CH extension continuation for this status.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Failure on sufficient complete CH bytes is a shared syntax failure. Do not feed incomplete CRYPTO coverage as if it were a complete malformed CH.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Failure on sufficient complete CH bytes is a shared syntax failure. Do not feed incomplete CRYPTO coverage as if it were a complete malformed CH.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Failure on sufficient complete CH bytes is a shared syntax failure. Do not feed incomplete CRYPTO coverage as if it were a complete malformed CH.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-005 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-005; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-006: IExtensionsParser::Status::Complete

- **TCP source/trigger:** IExtensionsParser::Status::Complete returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal extension parser Complete is necessary but not sufficient for acceptance. parseAllExtensions also requires consumed bytes to match getSingleExtensionTotalLength(); mismatch becomes ExtensionParsingFailure/Error/public Stop. A valid extension is inserted into the locator array; capacity failure returns local StopProcessing without a dedicated error. Early-data and SNI handling can add facts or fail. Successful iteration advances offset/remaining length; only completion of the entire loop returns ContinueProcessing. Sorting and fingerprint feeding follow and can still fail.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Complete extension parsing still requires consumed-length consistency and subsequent semantic/feeding checks. The same CH bytes imply the same checks.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Complete extension parsing still requires consumed-length consistency and subsequent semantic/feeding checks. The same CH bytes imply the same checks.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Complete extension parsing still requires consumed-length consistency and subsequent semantic/feeding checks. The same CH bytes imply the same checks.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-006 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-006; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-007: IExtensionsParser::Status::IncompleteExtensionTypeField

- **TCP source/trigger:** IExtensionsParser::Status::IncompleteExtensionTypeField returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal extension parser non-Complete status. parseAllExtensions assigns ExtensionParsingFailure and returns Error for this status; the explicit Failure branch and the other non-Complete branch have the same outcome. There is no per-extension continuation started here. The new-record and CH-continuation callers propagate the error (the latter deallocates its continuation buffer); processRecord translates it to public Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Incomplete extension type in a partial CH waits for CRYPTO bytes; if the declared complete CH section is truncated it is malformed. TCP parseAllExtensions collapse alone cannot decide acquisition state.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Incomplete extension type in a partial CH waits for CRYPTO bytes; if the declared complete CH section is truncated it is malformed. TCP parseAllExtensions collapse alone cannot decide acquisition state.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Incomplete extension type in a partial CH waits for CRYPTO bytes; if the declared complete CH section is truncated it is malformed. TCP parseAllExtensions collapse alone cannot decide acquisition state.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-05, OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-007 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-007; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-008: IExtensionsParser::Status::IncompleteExtensionLengthField

- **TCP source/trigger:** IExtensionsParser::Status::IncompleteExtensionLengthField returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal extension parser non-Complete status. parseAllExtensions assigns ExtensionParsingFailure and returns Error for this status; the explicit Failure branch and the other non-Complete branch have the same outcome. There is no per-extension continuation started here. The new-record and CH-continuation callers propagate the error (the latter deallocates its continuation buffer); processRecord translates it to public Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Incomplete extension length has the same partial-versus-invalid distinction after CRYPTO reconstruction. Do not classify packet fragmentation as a syntax error.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Incomplete extension length has the same partial-versus-invalid distinction after CRYPTO reconstruction. Do not classify packet fragmentation as a syntax error.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Incomplete extension length has the same partial-versus-invalid distinction after CRYPTO reconstruction. Do not classify packet fragmentation as a syntax error.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-05, OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-008 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-008; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-009: IExtensionsParser::Status::IncompleteExtensionData

- **TCP source/trigger:** IExtensionsParser::Status::IncompleteExtensionData returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal extension parser non-Complete status. parseAllExtensions assigns ExtensionParsingFailure and returns Error for this status; the explicit Failure branch and the other non-Complete branch have the same outcome. There is no per-extension continuation started here. The new-record and CH-continuation callers propagate the error (the latter deallocates its continuation buffer); processRecord translates it to public Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Incomplete extension data can wait for missing CRYPTO bytes; a complete CH whose declared section exceeds its message bounds is invalid. Same syntax, different availability boundary.
- **Classification:** QUIC-applicable with different source/trigger
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Incomplete extension data can wait for missing CRYPTO bytes; a complete CH whose declared section exceeds its message bounds is invalid. Same syntax, different availability boundary.
- **Representation differences:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing. Incomplete extension data can wait for missing CRYPTO bytes; a complete CH whose declared section exceeds its message bounds is invalid. Same syntax, different availability boundary.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** Share the stated semantic fact/guarantee only; keep packet/frame/CRYPTO acquisition distinct from TLS-record framing.
- **Contract trace:** OP-05, OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-009 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-009; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-010: IExtensionsParser::Status::Failure

- **TCP source/trigger:** IExtensionsParser::Status::Failure returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** Internal extension parser non-Complete status. parseAllExtensions assigns ExtensionParsingFailure and returns Error for this status; the explicit Failure branch and the other non-Complete branch have the same outcome. There is no per-extension continuation started here. The new-record and CH-continuation callers propagate the error (the latter deallocates its continuation buffer); processRecord translates it to public Stop.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Extension Failure on a complete CH section is a shared syntax failure. Error reason and delivered progress remain separate channels.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Extension Failure on a complete CH section is a shared syntax failure. Error reason and delivered progress remain separate channels.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Extension Failure on a complete CH section is a shared syntax failure. Error reason and delivered progress remain separate channels.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-010 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-010; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### M / P3-PARSE-011: ParsingStatus vocabulary

- **TCP source/trigger:** ParsingStatus vocabulary returned by the corresponding internal parser call; conditional translation is detailed below.
- **Semantic meaning:** ParsingStatus has exactly Complete and Failure. Incomplete outcomes exist in the specialized ClientHello/extension status enums, not this enum. Non-Complete TLS record/handshake/cipher parsing becomes specific stored errors in Processor, not automatically NeedMoreData.
- **Observed/derived/heuristic nature:** Software outcome/control or declared vocabulary; not an independent observed TLS message.
- **QUIC source, required processing and rationale:** Complete/Failure are parser-local vocabulary, not wire facts or public progress events. QUIC acquisition waiting is separate and must not invent an Incomplete enumerator here.
- **Classification:** Shared
- **Semantic relation:** Identical
- **Semantic boundary and rationale:** Identical only at the stated value or semantic guarantee; current TCP branch behavior, representation, acquisition and consumer use are not thereby identical. Complete/Failure are parser-local vocabulary, not wire facts or public progress events. QUIC acquisition waiting is separate and must not invent an Incomplete enumerator here.
- **Representation differences:** CH content or transport-independent software fact; reuse does not imply a shared object/API. Complete/Failure are parser-local vocabulary, not wire facts or public progress events. QUIC acquisition waiting is separate and must not invent an Incomplete enumerator here.
- **Acquisition cost:** Relevant evidence is acquired upstream once by OP-01..05 or OP-08 as applicable. Initial cryptography and CRYPTO reconstruction are not repeated for a getter, scalar update or callback.
- **Operation cost after evidence is available:** Validate only the relevant current message/header/list/extension bytes. Missing coverage is handled by acquisition; sorting is not charged to each parser status.
- **Sharing implications:** CH content or transport-independent software fact; reuse does not imply a shared object/API.
- **Contract trace:** OP-02, OP-05, OP-06; sections 3, 4, 6
- **Evidence:** V5 P3-PARSE-011 source anchors in Section 2; Step 4 V2 Appendix A P3-PARSE-011; supplied matrix V2 index and available expanded text; independent V2 contract review against V5. No benchmark or exact full-file parity claim.

### 3.2 Affected transition/control coverage

The following exact Phase 2 rows are policy evidence, not a newly inferred protocol state machine. Per-row semantic mapping follows the table; detailed attempt/reset guards remain Phase 2 sections 7-10.

#### TCP transition policy

| Current state | Observation | Conditions | Next state | Meaning |
|---|---|---|---|---|
| `PreHandshake` | Complete ClientHello | Parseable and accepted by the existing TCP path | `InProgress` | ClientHello becomes authoritative. |
| `InProgress` | ChangeCipherSpec | TLS 1.3 is not offered | `ChangeCipherSpec` | Enter the existing TCP-specific intermediate state. |
| `InProgress` | ApplicationData | TLS 1.3 is offered and the early-data guard passes | `Finished` | Accept the existing TLS 1.3 completion heuristic. |
| `ChangeCipherSpec` | Handshake record | Accepted by the existing TCP path | `Finished` | Accept the existing pre-TLS-1.3 completion heuristic. |
| `PreHandshake` or `InProgress` | Accepted terminating TLS alert | Existing TCP alert conditions pass | `TerminatedByAlert` | The attempt ended after terminating-alert evidence. |
| `ChangeCipherSpec` | Explicit unsupported continuation result | Existing TCP path cannot continue reliably | `Unsupported` | Processing cannot continue reliably. |
| `TerminatedByAlert` | Existing terminal payload result | Existing TCP terminal verdict contract requests stop | `Error` | Apply the existing TCP-specific terminal transition. |

#### QUIC transition policy

| Current state | Observation | Conditions | Next state | Meaning |
|---|---|---|---|---|
| `PreHandshake` | Partial ClientHello | Associated with the active attempt | `PreHandshake` | ClientHello is incomplete. |
| `PreHandshake` | Complete ClientHello | Parseable, not a retransmission, and belongs to the active attempt | `InProgress` | It becomes authoritative. |
| `PreHandshake` | Handshake evidence before ClientHello completion | Associated and unambiguous | `PreHandshake` | Retain bounded, non-authoritative progress evidence. |
| `PreHandshake` | First complete ClientHello with retained Handshake evidence | Both belong to the same attempt | `Finished` | The attempt passes through `InProgress` and completes in the same semantic cycle. |
| `InProgress` | Additional complete ClientHello | Parseable, not a retransmission, and belongs to the active attempt | `InProgress` | It replaces the authoritative ClientHello. |
| `PreHandshake` or `InProgress` | Associated 0-RTT | Association is unambiguous | Unchanged | 0-RTT does not advance state. |
| `InProgress` | Associated QUIC v1 Handshake Long Header packet | Association rules in Section 9 hold | `Finished` | Product-level completion evidence was accepted. |
| `PreHandshake` or `InProgress` | Retry-compatible restart | Conditions in Section 8 hold | Unchanged | Initial and CRYPTO processing restart within the same attempt. |
| `InProgress` | Associated non-v1, non-reserved Long Header packet | Belongs to the active attempt | `Unsupported` | The QUIC v1-only release cannot evaluate completion. |
| `InProgress` | Loss of state required to evaluate completion | Required association or completion state is unavailable | `Unsupported` | Processing cannot continue reliably. |
| Any non-terminal state | Explicit unsupported result | Current processing cannot continue reliably | `Unsupported` | Stop current semantic processing without treating it as `Error`. |
| `PreHandshake` or `InProgress` | QUIC terminating-alert evidence | Conditions in Section 9 hold | `TerminatedByAlert` | The attempt ended after an explicitly observed TLS alert carried by QUIC. |
| Any non-terminal state | Existing terminal verdict | Existing verdict contract requests terminal stop | `Error` | Active processing ends. |
| `PreHandshake` or `InProgress` | ClientHello retransmission | Same accepted CRYPTO offsets and bytes | Unchanged | No new ClientHello observation. |
| Any state | Ambiguous input | Attempt association is inconclusive | Unchanged | No merge, reset, replacement, or inferred violation. |
| Any terminal state | Input not accepted as a new-attempt candidate | Attempt-level classification in Section 10 does not succeed | Unchanged | The input does not reach ClientHello parsing for the terminal attempt. |
| Any terminal state | Accepted new-attempt candidate | Attempt-level conditions in Section 10 hold | `PreHandshake` | Reset the previous attempt-local state, then parse input for the new attempt. |
| `PreHandshake` or `InProgress` | Complete new attempt | Active-state conditions in Section 10 hold | `PreHandshake`, then process its first ClientHello | Replace the previous active attempt and reset attempt-local state. |
| Any terminal state | Late input from a retained retired attempt | Retired identity matches | Unchanged | The retired attempt is not reopened. |

---

Transition mapping: authoritative CH -> P3-OBS-001/STATE-003/007/009 and OP-07; TCP CCS -> P3-OBS-002/STATE-006 (TCP-specific); TCP handshake-after-CCS/ApplicationData completion -> P3-OBS-003/004 (heuristics, analogous to QUIC associated Handshake header); terminating alert -> P3-OBS-005/010/REC-004..006/014 (different acquisition/classifier); unsupported -> P3-OBS-006/OUT-003/CONT-011; terminal verdict -> P3-FB-002..007; TCP RST/SYN -> P3-OBS-007/008 (not QUIC flags); new-attempt reset -> P3-STATE-010/IN-010 plus Phase 2 10.1/10.2; deferred evidence -> OP-08/L11; retransmission/Retry/retired/ambiguity -> OP-03/05/07; 0-RTT -> current-scope neutral, not TCP earlyDataCount. D01 is the explicit additional product-limit disposition in OP-06 and S6-28A. No Phase 2 file is edited.

## 4. Semantic processing contract

The complete operation/progress/outcome/cardinality/state-access content is consolidated below. Internal original section labels are prefixed C for evidence navigation; they do not refer to separate mandatory components. Step 5 handoff instructions are replaced by the completed Section 5 register. D01 overrides stale generic required-fingerprint-capacity wording. All remaining limits are centralized in Section 8.

#### Units and invariants

#### 2.1 Distinct units

- **Input span:** bounded bytes at a declared stage: TCP payload, UDP datagram, one QUIC packet, Initial plaintext, CRYPTO range, or one TLS handshake message. A length at one layer does not bound another layer automatically.
- **Acquisition unit:** work that establishes boundaries, protection status, range coverage or message availability. It can produce no policy-visible observation.
- **Logical observation:** one newly produced semantic fact eligible for policy/consumer evaluation. CH facts, completion evidence and terminating-alert evidence are different observation kinds.
- **Feedback unit:** one current logical evidence unit with its applicable projections and outcome metadata. It is not an entire datagram or all retained viewer data.
- **Available authoritative data:** previously accepted CH values for the active attempt. Availability is independent of whether the current operation produced anything new.

For TCP, a current-record analysis unit can expose both record facts and a CH projection; those are facets of the current unit, not two independently committed CHs. U05 preserves the existing TCP retained-Complete callback behavior in this task. It is not a new logical CH or a requirement to fabricate QUIC CH observations; no compatibility choice remains open for this preserved behavior.

For QUIC, one datagram can contain several packets; one packet can contain several CRYPTO ranges; a CH can span ranges/packets/datagrams; one contiguous CRYPTO extent can contain more than one logical TLS message. None of these equalities is assumed.

#### 2.2 Common invariants

1. Every result is associated with the session, active attempt/candidate context where applicable, producing operation and input provenance. No numeric identifier format is required.
2. Every consumed span is bounded; offset plus length and declared message lengths are validated without overflow or out-of-bounds access.
3. For QUIC, no plaintext/frame/CH claim is made from Initial ciphertext before required Initial protection processing succeeds. TCP plaintext acquisition follows its own existing validity predicates; it does not require QUIC Initial protection.
4. No ambiguous or retired input is merged into active reconstruction or used to reset active state.
5. New observation identity is not derived solely from a retained Complete flag, pointer identity, fingerprint equality or packet count.
6. Parsing success, result availability, successful delivery, authority acceptance and state transition are separate facts.
7. Acquisition/parsing may update their own supporting context, but do not write semantic TLSData. Policy disposition owns semantic updates.
8. A subsequent policy-sensitive unit cannot become authoritative or be dispatched using state from before unresolved feedback on the current unit.
9. Parser/observer Stop is not a network-drop instruction. No mitigation or violation producer is introduced.
10. No field is exposed as current evidence merely because a shared viewer still contains an earlier value.

Evidence: P3-IN-001..008; P3-OUT-001..010; P3-CONT-005..008; P3-VIEW-011..015; P3-FB-001..009.

### C / 3. Semantic operations

Each operation states inputs, validity, progress, observations, outcomes, cardinality and semantic-state access. These are semantic stages, not mandatory separately callable operations.

#### OP-01: Establish the current input boundary

**V3 resolved contract:** V3 participation rule: use the same nonzero live-feature event-mask/extension registration rule for TCP and QUIC (U02). No extra transport-only participation gate; feature-specific policy/port predicates and allocation failure remain explicit.

**Inputs:** transport identity, direction, bounded input bytes and available length, DP session identity, source-frame provenance and packet timestamp. TCP also supplies TCP sequence for continuation. QUIC supplies UDP datagram extent; packet boundaries are established within that extent, not assumed from the entire datagram length.

**Validity:** metadata and borrowed bytes remain valid for the operation. No parser reads outside the supplied span. Session participation is an entry precondition supplied by integration, not proved by this contract. Zero bytes mean no byte-driven observation, not handshake completion. An unavailable/invalid span is an input-contract failure, not evidence of malformed client syntax.

**Progress:** begin work within the current input or finish an empty/nonparticipating input. A datagram truncated by its supplied boundary cannot borrow bytes from the next datagram to complete a QUIC packet.

**Observations:** none from binding alone. Session/frame identity is provenance, not a new TLS observation.

**Outcomes:** valid binding, nonparticipating/out-of-scope input, or invalid supplied context. Do not label a caller lifetime error as a wire violation.

**Cardinality:** zero semantic observations. Several later acquisition units may exist in the input.

**TLSData access:** none. Participation and routing are distinct from binding. Supporting packet metadata is read-only.

Evidence: P3-IN-001..004, P3-IN-008, P3-IN-011.

#### OP-02: Acquire the next TCP processing unit

**Inputs:** remaining bounded TCP payload, TCP sequence and session-scoped continuation state, plus current hsState and tlsVersionMask where the existing parser predicates use them.

**Validity:** preserve current supported TCP reconstruction boundaries. V5 records extension-only CH continuation, opaque-record tracking and alert alignment/reassembly; this contract does not silently extend TCP support. Alert level/description are valid only when readableAlert is established. Stored offered versions are not negotiated-version evidence.

**Progress:** the existing public results retain their actual producer meaning: RecordParsed enables current-unit analysis; NeedMoreData ends the current legacy record loop; EndOfPacket exhausts current input; Unsupported reports an explicit inability; Stop ends processing without necessarily producing an observer Stop or Error state. Nested returns must be interpreted at the public boundary, including the cipher soft-stop path recorded in V5.

**Observations:** zero or one current-record evidence unit. A CH facet is separately qualified by message validity/freshness; a retained CH viewer is not proof of a newly parsed CH in this record.

**Outcomes:** record/handshake/CH syntax failures, unsupported continuation, resource/collection/digest limitation, or ordinary progress. Legacy error names do not redefine those categories.

**Cardinality:** at most one current-record evidence unit per step; the packet can yield several steps if feedback/state permits. Record facts and CH projection can coexist within the current unit. No packet-wide precommit is implied.

**TLSData access:** read-only hsState for eligibility/opaque/alert interpretation and read-only tlsVersionMask for the current TCP opaque-ApplicationData predicate. No other direct Processor TLSData reads are established by V5. TCP alert-terminal classification reads offered-version mask at policy evaluation, not as proof of negotiation.

Evidence: P3-IN-005..007; P3-OUT-001..006; P3-CONT-001..004; P3-REC-001..006; P3-ERR-019; Processor::processRecord/processNewRecord/processAlert; matrix V2 P3-IN-007.

#### OP-03: Classify QUIC packet evidence and attempt eligibility

**Inputs:** bounded visible packet header, version/type/connection-ID/token/length fields when available, DP session transport tuple, current hsState, retained active/retired Initial-context identities, retained client SCID when nonzero, duplicate/overlap evidence already available and candidate CRYPTO origin when recovered. Fields not yet acquired are explicitly unavailable.

**Validity:** only validated visible or trusted decoded fields are used. Changed DCID alone neither selects a new retained Initial context nor proves a new attempt. Zero-length SCID neither proves nor rejects association. The fixed bit alone is not mandatory transition evidence under Phase 2. Header association is not cryptographic client authentication.

**Progress:** continue acquisition in the same input, suppress retired/ambiguous/late input, or pause for a policy disposition before processing under a newly accepted attempt. Missing evidence can defer classification; it cannot be filled from an assumption.

**Observations:** association/eligibility disposition, not CH completion. An associated v1 Handshake header can be a distinct completion-evidence candidate for OP-08. An out-of-scope packet is not automatically a state transition.

**Outcomes:** associated active input, candidate input, retired/late input, ambiguous input, malformed bounded header, out-of-scope input, or required-evaluation unsupported candidate. Phase 2 determines which conditions change state.

**Cardinality:** zero or one classification for the current packet/context candidate. Candidate acceptance is a control boundary; it does not require a new public callback type.

**TLSData access:** read-only hsState. Attempt/Initial-context/retired identity are supporting protocol state, not fields inferred to exist in TLSData. No read of prior tlsVersionMask is needed for QUIC packet classification.

**Required Phase 2 ordering:**

- Active PreHandshake/InProgress: a complete parseable CH can provide identity needed to establish a complete new attempt. Candidate acquisition remains isolated until classification; it must not overwrite the active authoritative CH prematurely. Only one semantic TLSData attempt is active.
- Terminal attempt: available attempt-level evidence is classified before CH parsing. Acceptance of a new-attempt candidate retires prior identity when available, resets attempt-local state and enters PreHandshake before parsing/reassembly. It does not wait for CH completeness.
- Retry-compatible restart is the same attempt and retains original/replacement Initial contexts; it is not a complete-new-attempt reset. Same-context duplicate delivery is classified before Retry-compatible restart.

These are data/ordering preconditions, not a key-search algorithm or chosen commit mechanism.

Evidence: Phase 2 sections 7-10 and 12; P3-IN-003/006/010; P3-STATE-010; P3-CONT-007; P3-OBS-008; P3-FB-009.

#### OP-04: Recover one QUIC Initial payload and frame facts

**Inputs:** one bounded v1 Initial packet, selected Initial context and its key-derivation basis, required packet-number reconstruction context, and packet provenance. Context selection must be supported by retained/candidate evidence; a packet DCID is not blindly treated as the correct key basis for every packet.

**Validity:** header protection is removed before interpreting protected packet-number fields; Initial AEAD processing succeeds before payload is used as frame evidence. All frame fields and lengths fit within the recovered plaintext. Successful Initial processing does not authenticate the client. An unknown frame whose extent cannot be established cannot be skipped by guessing its size.

**Progress:** continue through validated frame boundaries, finish this packet, or stop this packet on a concrete acquisition failure. Processing of another packet requires a validated next packet boundary and applicable policy/feedback permission; a failed packet is not automatically an attempt-wide failure.

**Observations:** frame facts such as CRYPTO range or parsed transport close. Padding/ACK or other non-state-producing facts need not create a public observation. CH is not produced merely because a CRYPTO frame was parsed.

**Outcomes:** successful recovery, malformed packet/frame, protection/context failure, unsupported acquisition or resource limitation. Protection failure alone does not prove malformed CH, Error state, new attempt or client violation.

**Cardinality:** one Initial payload can expose zero, one or several frame facts. Only the next feedback-relevant semantic unit is eligible for policy delivery; bulk parsing does not authorize bulk semantic acceptance.

**TLSData access:** none for cryptographic/frame parsing. The operation reads/writes its acquisition context as needed, not semantic TLSData. hsState-based permission is supplied by the eligibility boundary, not required inside AEAD or varint parsing.

Evidence: matrix V2 P3-IN-001/002, P3-CONT-003/006, P3-REC-005/006; Phase 2 sections 2, 8 and 9.1.

#### OP-05: Incorporate CRYPTO bytes and establish message availability

**Inputs:** validated Initial CRYPTO offset, length and bytes; their active/candidate attempt and retained Initial context; previously accepted ranges; current message boundary knowledge and already accepted logical CH identity where needed for duplicate classification.

**Validity:** range arithmetic is bounded and overflow-safe. Bytes from retired/ambiguous or incompatible candidate contexts do not merge into active reconstruction. Identical overlap is duplicate evidence. Conflicting overlap is not merged and does not alone prove a new attempt or cause a state transition. Different encryption levels are not merged; only Initial content is acquired here.

**Progress:** continue in the same input when further usable frames/packets/ranges remain; otherwise require more input for missing coverage. A CRYPTO hole alone is not TCP continuation-gap Unsupported. Current input exhaustion and pending reconstruction can coexist.

**Observations:** range acceptance/duplicate/conflict metadata and availability of the next complete TLS message. No CH-completed observation is produced until CH evaluation succeeds and logical eligibility is established.

**Outcomes:** accepted new bytes, duplicate bytes, conflicting overlap, pending coverage, message-length invalidity, or resource limitation. A complete message whose internal declared fields exceed its own bounds is malformed; missing network coverage is not that condition.

**Cardinality:** one range update can unlock zero, one or several complete messages. Those messages are evaluated as separate logical units, not delivered as one aggregate authoritative update. No batch API is selected.

**TLSData access:** none for range coverage and byte reconstruction. Read supporting attempt/range/accepted-message metadata only. A new-attempt disposition from OP-03/07 must be applied before candidate bytes are treated as active authoritative data.

Evidence: Phase 2 sections 7.2, 7.3 and 8; P3-CONT-003/005..009; P3-PARSE-002..004/007..009.

#### OP-06: Evaluate one complete ClientHello and compute its shared values

**V3 resolved contract:** V3 fingerprint compatibility: preserve the existing evaluator/output, including verified last-suite omission, for both transports (U04). No common-fix choice is pending in this task.

**Inputs:** one exact, complete and logically contiguous TLS ClientHello handshake-message extent including its handshake header, with known provenance/association and declared length. The evaluator must access its bytes in message order under a valid backing-data guarantee; this does not require a particular physical allocation, copy or storage owner. The complete CH bytes may be reconstructed; no TLS record header is part of the shared CH input. Incremental internal parsing is allowed, but it cannot declare a complete result from incomplete coverage.

**Validity:** message type and length, body/list/extension bounds and applicable message-local checks pass. Consumed lengths are checked at their own layer. A complete CH extent is not required to equal a QUIC packet or CRYPTO-frame extent. Full handshake-flow legality is not checked here. No prior session/attempt viewer values are reused as if they belonged to this CH.

**Progress:** complete this CH evaluation or end it with a precise failure. If bytes are genuinely missing, return to acquisition/pending-input semantics; do not call incomplete coverage a malformed complete CH. Later messages remain separate work.

**Observations:** zero or one current CH evaluation result, with separate syntax-completion and per-value validity. A successfully evaluated normal CH candidate has its required values available. A syntax-complete result with unavailable fingerprint or failed result delivery remains distinguishable; it is not silently promoted to a successful normal CH observation. Neither result proves authority acceptance, consumer delivery or state transition.

**Outcomes:** complete parseable CH with valid fingerprint; CH syntax/validation failure; or fingerprint-builder/locator/result capacity failure. Fingerprint failure must not yield a default or stale fingerprint labeled valid. A CH can be syntactically complete while the fingerprint result is unavailable; expose those facts separately rather than claim a successful normal CH result.

**D01 product-capacity disposition:** For an accepted associated attempt in PreHandshake/InProgress, supported required fingerprint-building capacity exhaustion that makes reliable evaluation impossible produces explicit Unsupported. Do not deliver a successful normal CH or replace authority/count/timestamp with missing/default/stale fingerprint. Keep prior authority separately available. This is a product-limit disposition, not malformed syntax, violation, Error or automatic drop. An isolated unaccepted candidate cannot apply this disposition to unrelated active A. A terminal candidate already accepted under Phase 2 10.2 has its own new context. Zero-length digest rejection and legacy record-collection capacity are not automatically D01. Independently applicable observer verdicts remain separate.

**Cardinality:** at most one current CH evaluation result per message evaluation. Cipher/extension/compression counts count CH content, not packets/ranges; narrowing must not silently change their semantic value. A syntactically parseable CH is not redefined as malformed merely because digest or delivery capacity failed.

**TLSData access:** none. CH syntax, offered versions, early_data, SNI, counts, normalization and fingerprint calculation depend on the current CH bytes and shared calculation rules, not hsState, prior offered versions, count, prior fingerprint or timestamp.

**Fingerprint guarantee:** identical complete CH input produces identical fingerprint over TCP and QUIC under the same agreed algorithm. No QUIC-only normalization, transport identity, packetization or key-context data is added. The explicit U04 decision preserves the legacy cipher-loop/output in both transports for this task; no algorithm-selection unknown remains. This neither fixes only QUIC nor claims the defect is correct. The equality requirement is already resolved.

Evidence: P3-VIEW-001..009/011; P3-FP-001..005; P3-PARSE-001..011; P3-ERR-007/009/012..014; USER-FP-01.

#### OP-07: Determine logical CH eligibility and authority disposition

**Inputs:** the current CH evaluation result, including syntax-completion and per-value validity, its session/attempt/context and message range, accepted range/message identity, current hsState, prior authoritative CH identity when needed, Retry-compatible evidence and retired/ambiguous classification. Fingerprint equality alone is not a duplicate or new-attempt proof. Syntax/attempt classification may use sufficient CH identity evidence independently of digest success; normal CH delivery/update must not consume an unavailable fingerprint. Legacy record-collection/delivery failure does not itself select a new QUIC transition. Required fingerprint product-capacity exhaustion follows D01; syntax/association evidence and normal successful CH acceptance remain distinct.

**Validity:** apply Phase 2 rules without adding a full-flow legality gate. Same bytes at already accepted offsets, repacketization, identical overlap and Retry-compatible byte-equivalent resend do not create a new logical CH. A complete CH at new offsets can be a new logical CH even with identical content. Retry-compatible first completion in either retained context is one logical CH, not one per context.

**Progress:** no new observation for duplicates/late/ambiguous candidates; otherwise pause for feedback on the eligible CH unit before a later policy-sensitive unit is accepted. A required attempt reset is a preceding control disposition, not an accidental consequence of parsing.

**Observations:** zero or one newly eligible logical CH observation. Existing authoritative CH data remains separately available. New authority replaces CH-derived values in active PreHandshake/InProgress according to Phase 2, subject to the existing feedback boundary that can suppress normal updates. A terminal attempt cannot accept another CH of itself.

**Outcomes:** eligible new logical CH, duplicate, late/retired, ambiguous, accepted new-attempt disposition or unsuccessful candidate evaluation. These are not automatically parser failures, violations or network actions.

**Cardinality:** one eligible logical CH is counted once when its normal authority update is accepted. It is not counted once per CRYPTO frame, packet, context or listener. Identical-content CHs at new offsets are not collapsed by digest equality.

**TLSData access:** read-only hsState for active/terminal eligibility. Prior CH identity and accepted range/context evidence are supporting attempt inputs; this contract does not require reading TLSData.chFingerprint to establish retransmission or new-attempt identity. A digest alone is not sufficient identity evidence. clientHelloCount is read only by the policy increment/update, not as a parser legality gate. No CH syntax dependency on these fields is introduced. The policy disposition, not acquisition/parsing, writes authoritative values and count.

**Timestamp decision for this proposed contract:** use the supplied packet/frame timestamp of the operation that first makes this eligible complete CH result available, matching the existing completion-packet meaning rather than the first fragment time or callback wall clock. Duplicates do not refresh it. A Retry-compatible completion suppressed as the same logical CH does not refresh it. The accepted value and provenance travel with the observation; feedback pause does not change it.

Evidence: Phase 2 sections 3, 6-10 and 12; P3-STATE-003/007/009/010; P3-OBS-001; P3-CB-002; P3-FB-008/009. Timestamp selection is a Step 4 proposal, not a newly discovered product requirement.

#### OP-08: Qualify completion and terminal-alert evidence

**Inputs:** one current acquired evidence unit, its validity/provenance/association, current hsState and relevant retained evidence. TCP supplies current record facts, readable alert fields and offered-version context for the existing classifier. QUIC supplies an associated v1 client Handshake header or a successfully parsed associated Initial close, not decrypted Handshake payload.

**Validity:** QUIC Handshake association follows Phase 2 section 9: same DP tuple, consistent nonzero retained client SCID, no retired classification and no ambiguity; matching DCID is not required. Evidence before first authoritative CH is bounded and non-authoritative, and is applied only for the same attempt after first CH authority. Zero SCID is not sufficient positive proof by itself.

QUIC terminating-alert evidence requires Initial transport CONNECTION_CLOSE type 0x1c, CRYPTO_ERROR 0x100..0x1ff and active-attempt association under a retained Initial context, excluding retired/ambiguous input. The derived TLS alert description is error code minus 0x100; no alert level is invented. user_canceled 0x15a qualifies under Phase 2. Other close codes do not establish TerminatedByAlert. The transition applies in PreHandshake or InProgress only when this associated readable evidence is established; PreHandshake does not become TerminatedByAlert merely because local CH reconstruction failed.

**Progress:** pause for feedback when an eligible current evidence unit can affect policy; otherwise continue/suppress/retain as applicable. A terminal result ends further active-attempt evaluation, but future terminal candidate classification remains possible.

**Observations:** zero or one qualified completion/close evidence unit. Deferred Handshake evidence can be a supporting facet of the first CH unit, enabling the Phase 2 transition to Finished without inventing a second freshly observed Handshake packet. A product Finished state is not TLS Finished-message evidence.

**Outcomes:** eligible evidence, deferred evidence, irrelevant/out-of-scope evidence, ambiguous/retired suppression, malformed acquired close, or unsupported reliable evaluation. 0-RTT is state-neutral; short-header traffic does not replace the accepted completion signal. An undecrypted Handshake close is not a readable close, even though its associated header can be completion evidence.

**Cardinality:** one evidence unit at a time; a CH plus previously deferred completion fact can cause one combined policy disposition. Repeated headers cannot repeatedly transition an already terminal attempt. A packet containing CH and close can contain more than one distinct unit; no aggregate precommit is allowed.

**TLSData access:** read-only hsState for applicable transition eligibility. TCP policy reads clientHelloCount, tlsVersionMask and earlyDataOffered for existing CCS/ApplicationData predicates; readable-alert classification reads tlsVersionMask. QUIC completion/close qualification needs no prior tlsVersionMask, earlyDataOffered, earlyDataCount or preCcsSeen. Deferred evidence/association are supporting attempt state, not fabricated TLSData fields.

Evidence: Phase 2 sections 5.2, 6, 9/9.1, 11-13; P3-OBS-002..010; P3-REC-004..006/014.

#### OP-09: Resolve feedback for the current evidence unit

**V3 resolved contract:** V3 consumer boundary: callbacks receive immutable result data but may return verdicts and perform existing packet/session side effects. Full-string shared backing stays stable through the Fingerprint consumer nested insert call; detector pointer use stays within live current-unit scope. Discarded Fingerprint Action is not an observer Stop.

**Inputs:** current valid observation/projections or failure report, pre-normal-update semantic context, eligible listener feedback, current hsState and existing verdict metadata where the established policy uses it. No future-unit result is substituted for the current unit.

**Validity:** feedback is tied to the unit that produced it. The consumer-visible prior TLSData context and current CH/record/evidence projection remain distinct. Input backing data remains valid throughout the permitted synchronous delivery. Reentrant processing that overwrites current views cannot be assumed safe; if allowed by a later design, it must preserve this validity guarantee.

**Progress:** pause while feedback/disposition is pending; after resolution either continue with updated permission/context or stop further processing. Parser Stop and observer Stop remain independent. No operation may spin with unchanged progress, unchanged pending input and no feedback/input event.

**Observations:** delivery and normal authority/state acceptance are separate from earlier production. Feedback itself does not manufacture a new CH or wire observation.

**Outcomes:** Continue/Stop plus independent existing violation metadata and normal-update permission. A parser failure or resource limitation is not itself an observer verdict. Existing TCP mask handling remains evidence, not new feature policy.

**Cardinality:** one feedback unit at a time. Multiple eligible listeners/projections can participate in that unit under the existing boundary. Stop within a listener list stops that list immediately. A Stop without accumulated mask can allow current-unit work under existing semantics while preventing the next unit. Nonzero mask suppresses normal current-unit update; combined Stop follows existing Error handling. This states the boundary, not a full callback order or transaction implementation.

**TLSData access:** feedback/state-policy evaluation reads current semantic context as needed. Normal state/authority update is a write-side policy action, not a parser write. Existing accumulated terminalViolationMask is read for its update and TCP reuse gating, not for QUIC CH calculation or attempt identity. No automatic drop or mitigation action is defined.

Evidence: P3-FB-001..007; P3-STATE-008; P3-CB-001..003; current TLSFlowConductor::analyzeProcessResult/notify. U05 explicitly preserves duplicate TCP callback eligibility in this task; it is not silently changed or imported as QUIC freshness policy.

#### OP-10: Access retained data and process lifecycle disposition

**Inputs:** explicit session/attempt association, current result validity or retained authoritative data validity, and lifecycle/reset/resource-loss reason when applicable. No new wire bytes are needed to read an already available result.

**Validity:** distinguish callback-local borrowed projections, processing-owned backing views, independent value copies and retained authoritative values. Existing ClientHelloData borrows callback-local fingerprint/SNI referents: copying the projection does not own them, and no retention after synchronous delivery is guaranteed. Processing-owned views required for current-unit evaluation must remain valid until their last permitted current-unit use, including post-callback policy evaluation where needed; this does not extend callback-local referents to that boundary. An independent owned value copy can remain valid as historical data after feedback or later processing, but is not a current view or proof of fresh completion. Borrowed full-string/digest views have no automatic validity after reset, backing reuse/rebinding, another session's processing or attempt replacement. Retained authoritative values can remain available for the same attempt until replacement/reset/release, without guaranteeing raw CH/full-string retention. Section 5 below assigns holder responsibilities, borrowing and invalidation per result/view family.

**Progress:** reads produce no new processing progress; lifecycle disposition can finish the current context or report loss of reliable future evaluation. Session aging is not handshake completion. Marking reclaimable is not guaranteed immediate release.

**Observations:** zero new CH/completion observations from a getter. A session-end/lifecycle event is distinct from wire termination. Resource loss is reported as such; it is not synthesized as terminating alert or Incomplete Handshake violation.

**Outcomes:** available valid value, unavailable/invalidated view, replaced/reset context, or resource-loss disposition. InProgress loss of required future association/completion state follows Phase 2 Unsupported policy; failure of one packet alone does not imply this loss.

**Cardinality:** zero new CH events for retained reads; at most one current lifecycle disposition per lifecycle event. Reclamation/cleanup can affect multiple resources without producing one semantic event per freed object.

**TLSData access:** read-only retained fields actually requested by policy/consumer. A borrowed const TLSData reference is not an immutable pre-update snapshot: policy update may change its values, and reset/release/reuse invalidates its old semantic association. Independent value snapshots, if made while valid, remain historical values only. Reset/release is a policy/lifecycle write action outside parsing. External retention compatibility is not claimed verified. No storage strategy or new getter API is selected.

Evidence: P3-STATE-010; P3-CONT-011; P3-VIEW-013..015; P3-CB-005/006; P3-OBS-009; Phase 2 sections 12-14.

**Existing TCP control/lifecycle facets retained as evidence:** RST without legal ACK invokes its reset callback; the no-mask path clears TLSData even if dispatch Stop, while a nonzero mask follows existing feedback handling. SYN-without-ACK preserves an existing active extension with no recorded violation; otherwise it can release/rebuild it. Session aging invokes onSessionEnd before extension release and ignores that notification result. These are TCP/lifecycle paths, not QUIC new-attempt or close aliases. No new reset API or routing order is selected. Evidence: P3-IN-009, P3-OBS-007/008, P3-CB-004/005.

**Supporting view facets:** getAllRecords is a borrowed bounded collection, not a guaranteed immutable whole-input batch; getLastRecord borrows mutable current record storage. isSegmented is current TCP ProcessorState != SingleSegment and can become false at completion, not durable CH fragmentation history. getContinuationMode is step metadata, not proven live-buffer state at every instant. No QUIC historical-fragmentation field is required without an actual consumer need; if one is justified, its contract must name its measured unit and validity rather than reuse these TCP getters blindly. Evidence: P3-REC-007/008, P3-VIEW-010, P3-CONT-010.


### C / 4. Progress contract

Progress is described independently from CH validity and outcome. These names are prose states, not proposed enum/API values.

- **More work in the same input:** a validated cursor/boundary identifies unprocessed work and no unresolved feedback blocks it. Missing CH coverage can coexist with usable later frames/packets in this input.
- **More input required:** a pending message needs unavailable bytes/evidence after usable current work is exhausted. Retained pending state and its association must remain valid, or loss is reported separately.
- **Current unit/input exhausted:** the selected unit has no more processable bytes/work. This is not handshake completion or end of CRYPTO stream.
- **Paused for feedback/control disposition:** the current evidence or accepted candidate reset requires policy resolution before later policy-sensitive work. It can occur even when input bytes remain.
- **Processing ended for this input/context:** a concrete stop, terminal permission, nonparticipation or irrecoverable current operation ends traversal. It does not necessarily mean the session/attempt is closed.

Every return boundary must state what unit was consumed, whether a valid remainder exists, whether pending acquisition remains, whether feedback is required and which result values are valid. No numeric cursor layout is chosen. A no-op duplicate can consume its input and produce zero observations without pretending to complete CH again.

TCP legacy NeedMoreData/Stop/EndOfPacket keep their current public behavior; QUIC is not forced to abandon later coalesced packets merely because one CRYPTO message has a hole. No malformed packet boundary is guessed to continue. Any later work depends on valid boundaries and applicable feedback/state permission.

Evidence: P3-OUT-001..006; P3-CONT-005/006/008; P3-FB-001/006/007; matrix V2 handoff.

### C / 5. Observations, availability and freshness

For each current result distinguish:

1. current parser recognition: None, Partial or Complete at its declared parsing scope;
2. current evaluated values and their individual validity;
3. newly eligible logical observation, if any;
4. successful delivery, if it occurs;
5. normal policy acceptance/authority update, if permitted;
6. previously retained authoritative data for the active attempt.

None means no CH recognized in the current parsing context. Partial means recognized but incomplete; it does not prove continuation allocation succeeded. Complete is parser/data completion, not a fresh-event token, successful delivery or authority update. Prior authoritative CH may remain available while a new operation is None/Partial or fails.

A new QUIC CH observation requires complete parseable CH, valid association and non-retransmission classification. Identical bytes at accepted offsets do not refresh CH values/count/timestamp. Identical CH contents at new offsets can be a new logical CH. A different later CH in the active attempt replaces prior values under Phase 2; it is not merged field-by-field with an older CH. An accepted new attempt invalidates prior attempt-local authoritative values before accepting its own first CH.

Current CH-only values are absent/not applicable on non-CH observations; zero/false defaults must not be mistaken for freshly observed version/early-data facts. SNI availability is separate from a default empty projection. CH-derived version means offered/selected-for-viewer interpretation, not negotiated TLS version. Full-string is digest input representation, not automatically raw CH or an owned string.

A failure after CH syntax completion or before result delivery cannot expose stale fingerprint as a successful current fingerprint. Previously accepted data can still be returned only with its previous provenance/validity. No cross-session or retired-attempt contamination is allowed by the proposed contract.

Evidence: P3-OUT-007..010; P3-REC-012/013; P3-VIEW-001..015; P3-CB-002; P3-FB-008; Phase 2 section 7.

#### 5.1 Phase 1 risks: explicit review and non-endorsement

**R-RESET (Phase 1 SD1 / VCF; P3-CONT-007, P3-VIEW-003/013/014):** ordinary beginPacket reaches resetStepState, but a resumed branch returns before that reset. The non-retransmission/non-gap resume clears current record/collection only; session-scoped continuation does not restore/reset every shared canonical CH fact. In particular supported-version mask/parsed flag and shared-parser fallback need scrutiny; early-data/SNI and error/status fields cannot be presumed fresh merely from buffer lookup. The later inventory qualifies Phase 1's broad carry-over claim as static risk, not proven production corruption. The proposed contract forbids cross-session/attempt exposure, but this document does not repair code or certify that the existing viewers meet that guarantee.

The consolidated lifetime/risk review examines ordinary beginPacket, exact resume, full retransmission, overlap, gap/discard, continuation success/failure, interleaved sessions and attempt replacement. For each path it must identify which fields are cleared, restored, recomputed or unavailable. Blind per-record clearing is not an approved fix: it could destroy legitimate pending reconstruction. No generation counter, new interface or per-session viewer object is mandated.

**R-REPEAT (Phase 1 SD8; P3-FB-008, P3-CB-002, P3-OUT-009, P3-VIEW-011):** current onClientHello eligibility tests retained Complete during RecordParsed analysis, not fresh completion or necessarily a CH-shaped current record. A later non-CH record can therefore trigger another CH notification if state/feedback permits reaching it; its record-context CH fields are still zero/false. This is conditional reachable static behavior, not 'every later record always' and not an endorsed compatibility obligation. Complete can also remain visible after capacity failure without normal CH delivery. None of these conditions proves a fresh logical CH.

The consolidated lifetime/risk review distinguishes available evaluated CH, newly completed CH, newly eligible logical CH, callback invocation and committed authority. It must cover CH then non-CH in one input, Complete then result-capacity Stop, feedback suppression, retransmission and same-content CH at new offsets. U05 preserves current TCP notification behavior in this task without declaring it correct or requiring QUIC to reproduce it. Any separately proposed TCP correction needs explicit scope and compatibility review; no correction choice is pending in the present task.

**R-BORROW (P3-CB-002, P3-REC-011, P3-VIEW-005/015, P3-STATE-010):** ClientHelloData reference fields, current record references, digest pointers and const TLSData references have different holders and invalidation events. Copying a wrapper or retaining a viewer pointer is not ownership. Callback-local referents can expire before later feedback/policy work, whereas owned historical copies can survive without becoming current evidence. The consolidated lifetime contract does not give all of them one blanket lifetime.

Evidence: phase1-copilot-report.md sections 1.5 and suspected-defect rows SD1/SD8 (PRIOR-REPORT); V5/matrix row anchors above; static spot-checks of Processor::beginPacket/resetStepState/resumeStepFromSegmentBuffer/processSegmentedClientHelloContinuation and TLSFlowConductor::analyzeProcessResult/onClientHello. No runtime corruption, code fix or external retention behavior is claimed.

### C / 6. Outcomes and disposition

Outcome metadata identifies producing stage, protocol origin, relevant input unit and whether future reliable evaluation remains possible. Categories are not aliases for current error enum names.

| Category | Meaning | What it does not imply |
|---|---|---|
| Successful acquisition/evaluation | Valid work/result at the stated stage | New observation, authority update or delivery |
| Pending bytes/evidence | Current required coverage/evidence is unavailable | Malformed CH, Unsupported or violation |
| Parsing/validation failure | Available bounded data fails a concrete packet/frame/message-local check | Automatic Error state, observer Stop or drop |
| Protection/context failure | Initial recovery fails under available context or required context is unavailable | Client authentication failure, malformed CH or new attempt |
| Unsupported reliable evaluation | Current-scope evaluation cannot continue under Phase 2 disposition | Client violation or equivalence to parsing failure |
| Resource limitation/loss | Required acquisition/result/digest capacity or context is unavailable/lost | Wire termination, completed handshake or Incomplete Handshake violation |
| Duplicate/late/ambiguous suppression | Input is not eligible to alter the active semantic attempt | Automatic parser error, merge or reset |
| Observer verdict | Existing Continue/Stop and independent verdict metadata | Parser result or automatic network action |

D01 required fingerprint product-capacity exhaustion has explicit Unsupported disposition. Other resource limitation can lead to an Unsupported policy disposition when required reliable evaluation is lost; retain both the resource cause and resulting disposition. A conflicting continuation mode is not automatically memory exhaustion. Digest/result-capacity failure is not CH syntax failure. UnsupportedRecordType/UnsupportedTlsVersion are legacy parsing reasons, not proof of the public Unsupported outcome.

For QUIC, an associated non-v1, non-reserved Long Header while InProgress or loss of required association/completion state follows Phase 2 section 13. Out-of-scope input, reserved versions, ambiguity and a recoverable failure of one packet do not all receive that transition. Exact reason-to-policy mapping must retain these guards.

Declared-only MalformedClientHello/ExtensionsLengthExceedsData and enum sentinel are not required runtime outcomes. This contract specifies actual malformed-CH semantics without inventing producers for those names.

Evidence: P3-ERR-001..019; P3-OUT-003/005/010; P3-CONT-011; Phase 2 section 13.

### C / 7. Cardinality and feedback unit

**Decision:** acquisition may expose several raw facts, but policy-visible processing is incremental at the current semantic evidence unit. A unit has zero or one primary new logical observation and can have several valid projections/supporting facts. Here a logical observation is a newly eligible policy fact, not every callback invocation: one TCP record unit can invoke record and CH callbacks, and the retained-Complete CH callback can repeat without new CH completion. The proposed freshness rule is not a claim that current TCP notification behavior has been fixed. The enclosing packet/datagram can yield zero, one or many such units. This is not a batch interface or a selected loop owner.

- One TCP step: zero or one current-record unit, with applicable record/CH/error facets. Legacy duplicate CH callback behavior is preserved by U05 for TCP only; it does not produce a new logical CH or a QUIC requirement.
- One QUIC datagram: zero or many packets/units; no fixed observation count follows.
- One Initial packet: zero or many frame facts; a range can complete no CH or unlock multiple message units.
- One CH evaluation: zero or one evaluated CH candidate; eligibility and delivery are separate.
- One accepted logical CH: one authority/count update, not one per listener or packet. Retransmission produces zero new CH observations.
- One associated Handshake header: one current evidence candidate; deferred evidence is applied once when eligible and does not manufacture a new wire event.
- One getter: zero new logical observations.

Before a later unit is dispatched or accepted, feedback and required current-unit state/attempt disposition must be resolved. Read-only boundary scanning or isolated candidate reconstruction can be performed internally only if it cannot precommit later semantic effects, contaminate active data, invalidate current views or bypass Stop. The contract does not require parsing the whole datagram before feedback.

If CH and close evidence occur in the same packet, their distinct facts are not collapsed into one synthetic TLS record. If a CH arrives after earlier deferred Handshake evidence, the first CH unit can carry that retained supporting fact and produce the Phase 2 Finished disposition. Full ordering of independent callbacks and commit implementation remain later work.

No session-wide or datagram-wide ban on another QUIC CH is inferred from TCP's retained-Complete guard. Message-local validation remains required; full-flow legality is not introduced here.

Evidence: P3-REC-007/008/011; P3-CB-001/002; P3-FB-001..009; Phase 2 sections 3, 6-10.

### C / 8. Minimal protocol context

Expose a fact only if the producing operation, state policy or established consumer needs it. Internal acquisition metadata is not automatically a public observation field.

| Boundary | Required context | Not inferred/exposed merely for symmetry |
|---|---|---|
| Input binding | Transport, direction, bounded span, session/frame identity, timestamp | A TLS record wrapper for UDP |
| TCP acquisition | Sequence/continuation and actual state predicates; valid current record fields | QUIC packet metadata |
| QUIC association/acquisition | v1/type/boundaries, retained/candidate Initial context, relevant IDs/token evidence, packet-number context only after valid recovery, CRYPTO offsets/coverage | Negotiated version, server events, unprotected packet number assumptions |
| CH evaluation | Exact current CH bytes/length and per-value validity | Prior TLSData, transport IDs or sequence in fingerprint |
| CH eligibility | Attempt association, logical message/range identity, duplicate/restart evidence, current state | Fingerprint equality as retransmission proof |
| Completion evidence | Evidence kind, provenance, association, current state, deferred evidence when applicable | Decoded TLS Finished from header-only evidence |
| QUIC terminal-alert evidence | Valid associated Initial close, transport close type, error code/range and derived description when applicable | Alert level or encrypted Handshake close payload |
| Feedback/current view | Current unit identity, valid projections, pre-normal-update state context, actual verdict | Future-unit data or automatic drop policy |
| Retained access/lifecycle | Explicit validity, attempt/session association and reason | Guaranteed raw CH retention or automatic completion on aging |

Current record type/length and alert level remain TCP-only. Shared CH values remain shared even when acquisition differs. No general protocol-capability framework or future-protocol event is introduced.

Evidence: matrix V2 P3-IN/REC/VIEW/OBS rows and Step 4 handoff.

### C / 9. TLSData read/write decision

**Decision:** processing reads semantic state only where the operation requires it; acquisition and CH evaluation do not write TLSData. Existing TCP const TLSData& is retained as a supported read-only input where needed. No interface is created solely to conceal fields, and no full TLSData parameter is made mandatory for every operation.

| Operation/boundary | Justified TLSData reads | Purpose | Writes by this processing operation |
|---|---|---|---|
| OP-01 binding | None | Metadata only | None |
| OP-02 TCP acquisition | hsState; tlsVersionMask | Eligibility, plaintext-alert/opaque interpretation; TCP TLS13 ApplicationData predicate | None |
| OP-03 QUIC classification | hsState | Active/terminal route and candidate eligibility | None; required reset is a separate policy disposition |
| OP-04 Initial recovery | None | Protection/frame parsing uses acquisition context | None |
| OP-05 CRYPTO reconstruction | None | Coverage/duplicate/conflict uses supporting context | None |
| OP-06 CH evaluation/fingerprint | None | Current CH content only | None |
| OP-07 CH authority disposition | hsState; clientHelloCount only for normal increment; prior CH identity is supporting context, not a mandatory chFingerprint read | Phase 2 eligibility and accepted update, not parser legality | Acquisition/evaluation: none; policy disposition writes accepted values/count/timestamp |
| OP-08 TCP state evidence | hsState; clientHelloCount; tlsVersionMask; earlyDataOffered where the exact predicate uses them | Existing CCS/ApplicationData and readable-alert classifier | Evidence qualification: none; policy disposition writes state/counters |
| OP-08 QUIC state evidence | hsState | Phase 2 completion/close applicability | Evidence qualification: none; policy disposition writes state |
| OP-09 feedback/policy | hsState and terminalViolationMask for applicable verdict disposition; clientHelloCount for accepted CH increment; TCP predicate fields are listed under OP-08 | Resolve permission and current-unit disposition | Policy action can write semantic state; parser does not |
| OP-10 retained read/lifecycle | Only the explicitly requested valid retained fields | Consumer access and lifecycle disposition | Read access: none; reset/release policy can write/invalidate |

The prior QUIC tlsVersionMask, earlyDataOffered, earlyDataCount and preCcsSeen are not inputs to Initial decryption, CRYPTO reconstruction, CH parsing or header-based completion. Current CH offered versions/early_data are outputs and may later be retained, not mandatory reads of previous TLSData.

Supporting attempt identity, retained Initial contexts, packet-number reconstruction and CRYPTO ranges may be mutable acquisition state. They are not silently added to TLSData or made separate components here. Read-only TLSData access does not mean the entire processing system is stateless.

Policy writes preserve Phase 2: accepted new-attempt reset, authoritative CH replacement/count/timestamp, applicable state transition and existing feedback metadata. Full write ownership implementation and commit timing are reserved for workplan Phase 4. Logical reset-before-parse requirements are already fixed by Phase 2 and cannot be postponed by that deferral.

Evidence: P3-IN-005..007; P3-STATE-001..010; P3-FB-001..007; matrix V2 P3-IN-005/007 and P3-STATE-010.


## 5. Cardinality, freshness, ownership and lifetime

Cardinality is defined by logical units, not packet count, range count, retained Complete or listener fan-out. A datagram/range can expose several units; each accepted logical CH updates authority/count once. Getter access produces zero new CH. Multiple eligible listeners may receive the same unit, subject to within-list Stop. This is not a batch API.

The complete L01..L14 register and isolation/risk obligations follow. Original Step 5 section labels are prefixed L for navigation. Concrete future holders are not selected. D01 governs required fingerprint product-limit failure; legacy result-collection failure remains S6-28B. Historical copies are not fresh evidence or authority of a replacement attempt.

#### Core definitions and invariants

- **Physical lifetime:** the backing bytes/object still exist. **Semantic validity:** the value belongs to the stated producing unit/session/attempt and passed its required checks. Physical existence alone never proves current validity.
- **Owned value:** holder retains independent bytes/scalars. **Borrowed view:** validity depends on another holder/backing. Copying a pointer, span, reference-bearing wrapper or viewer interface does not acquire ownership.
- **Current evaluated result:** values produced for the current logical message/operation, with per-field validity. **Retained authority:** previously accepted values for the active attempt. They may coexist while a new CH is Partial or fails.
- **Provenance obligation:** every access is bound to the correct session lifecycle, attempt, acquisition context, logical message/evidence unit and producing operation as applicable. This is a semantic association requirement, not a mandated identifier tuple or public fields.
- **Stable use:** backing cannot be overwritten, released or rebound while a valid consumer borrow is being used. Producer last use and consumer callback lifetime can differ; a producer may retain backing for same-unit feedback work without granting consumers post-callback retention.
- **Current invalidation:** changing input/message/session binding makes prior scratch unavailable as current even when its memory remains. Owned historical copies may survive but cannot silently become new evidence.
- **No blanket reset:** new-operation current views are isolated/refreshed; valid pending acquisition and retained same-attempt authority are not erased merely because another packet or callback ends.
- **No stale fallback:** absent/failed current fields are absent/failed, never filled from an unrelated prior CH. Valid prior authoritative fields may be read only as explicitly retained data with their provenance.

Sources: Step 4 V2 sections 2, 5, 5.1, 5.2, 7, 9; Phase 2 sections 7-10, 12-14; inventory P3-VIEW-013..015, P3-CB-002, P3-STATE-010. Concrete source anchors follow the register.


#### 2.1 Last-use and invalidation decisions

- **Callback-local referents:** valid through the permitted synchronous callback/notification use while the wrapper and referents are live. ClientHelloData local fingerprint/SNI referents end when onClientHello returns; later outer feedback cannot retain that wrapper as its backing. No post-callback consumer retention is granted.
- **Processing backing:** if a current result is required after callback for same-unit policy/feedback, the responsible processing holder retains its valid backing until that last use. It may retain a different owned value rather than callback-local referents. No class, copy strategy or buffer layout is mandated.
- **Independent owned copies:** a copy made from valid data can survive feedback, reset or later processing as a historical value with original provenance. Historical survival does not make it current, newly completed or authoritative for another attempt.
- **Live state reference:** const TLSData& is a borrow of a live object, not a snapshot. Normal update can change its values; reset invalidates the prior semantic association without necessarily destroying the object. Release/reuse can additionally end physical lifetime. Do not read it after its guarantee expires.
- **Conflicting input:** invalidate/reject the conflicting incoming candidate/range, not previously accepted active bytes solely because of the conflict. No automatic state transition, authority reset or new-attempt inference follows. An independent explicit resource/lifecycle loss has its own outcome and invalidation.
- **Ordinary input/feedback end:** expires current scratch association and ended borrows, but does not erase valid same-attempt pending acquisition, authoritative copies or deferred evidence merely because a callback/packet ended. Same-unit backing needed after feedback is not overwritten early.
- **Attempt replacement:** old context becomes unavailable as current at the accepted Phase 2 reset boundary. Preserve any accepted candidate evidence through its required last use; old-context cleanup must not destroy the candidate needed to establish new authority.

These are normative semantic guarantees, not proof that existing TCP storage, reset or callbacks already enforce them. CODE-VERIFIED labels below are inherited from the approved inventory/matrix/Step 4; the historical Step 5 review did not independently inspect every source branch. This assembly verifies source digests and spot-checks specific branches, not all runtime paths. The policy premises used here are carried forward from Step 4 V2 and the accepted conversation decisions.

### L / 3. Result/view register

Each family states all applicable concrete facets. A guarantee is normative unless marked current evidence. Ownership holders are semantic responsibilities; actual future placement is reserved for later design. QUIC-only families have this contract/Phase 2 evidence, not invented current QUIC code.

#### L01: Input payload, copied metadata, Session and DPFrame

Operations: OP-01/02/03. Inventory: P3-IN-001, P3-IN-002, P3-IN-003, P3-IN-004, P3-IN-008

- **Current evidence / authority boundary:** Processor::captureStepPacketMetadata copies scalar parameters but retains payload pointer; callbacks borrow Session/DPFrame. Their upstream ownership and post-callback retention are not established by this module.
- **Holder and owned/borrowed nature:** Upstream must supply a holder/lifetime guarantee for packet backing; its actual concrete owner is not established here. Processing borrows bounded bytes from that backing. Copied sequence/length/session/timestamp metadata is owned by the receiving value holder. Session/frame references are borrowed from upstream; no transfer of their ownership is inferred.
- **Valid from:** Input is bound to a known current session/direction/transport and a bounded span whose upstream backing remains live.
- **Invalidated by:** Packet backing release/reuse, input unbinding or scope end; session release/reuse invalidates session binding. Copied scalars remain historical values, not current input.
- **During callback:** Producer must keep referenced packet/session/frame backing live through the callback that consumes it. No promise of upstream validity beyond this scope.
- **After feedback:** Feedback does not transfer ownership. Input may remain usable for same-unit work only while its original binding and backing are retained.
- **Next operation/session/attempt:** No borrowed packet pointer may silently carry over to the next packet. Cross-packet continuation must retain required bytes/metadata independently of expired packet backing.
- **Available versus fresh:** Input presence is not CH completion; timestamp is acquisition metadata until policy accepts an observation.
- **Isolation requirement:** Bind packet and copied metadata to the correct session and attempt/candidate role; session index alone is not proof of the same lifecycle.

#### L02: QUIC Initial plaintext and parsed frame slices

Operations: OP-04/05. Inventory: QUIC-only contract family; Step 4 operations and Phase 2, no current TCP counterpart required.

- **Current evidence / authority boundary:** No QUIC implementation is claimed. Step 4 OP-04/05 defines protected Initial acquisition and frame/range facts; TCP Record viewers are not reused as fabricated QUIC Records.
- **Holder and owned/borrowed nature:** Acquisition backing holder owns recovered plaintext. Frame/range views borrow that backing; copied scalar offsets/types are owned values. This is a responsibility, not a selected class or buffer layout.
- **Valid from:** Correct v1 Initial context, required protection processing succeeds, plaintext extent and frame lengths are bounded, and each exposed field has been validated.
- **Invalidated by:** Backing overwrite/release, operation replacement or rejected association. Protection/parse failure leaves unavailable fields unavailable; residual bytes do not become valid views.
- **During callback:** Exposed frame slices must stay stable and bounded throughout callbacks that consume them.
- **After feedback:** If current-unit processing needs them after feedback, the producer retains backing through that last use. No blanket consumer right to retain slices after callback.
- **Next operation/session/attempt:** Next acquisition may reuse scratch only after prior borrows end. CRYPTO bytes needed later must already have durable retention or equivalent guaranteed backing.
- **Available versus fresh:** Freshly decrypted bytes are acquisition output, not a newly completed CH or confirmed handshake.
- **Isolation requirement:** Plaintext and frame slices carry the producing packet/Initial context and attempt association; a failed candidate cannot expose active-attempt plaintext as its own.

#### L03: CRYPTO coverage, pending/reconstructed message and candidate identity

Operations: OP-03/05/07. Inventory: QUIC-only contract family; Step 4 operations and Phase 2, no current TCP counterpart required.

- **Current evidence / authority boundary:** Phase 2 sections 7-10 and Step 4 OP-03/05/07 define association, duplicates, overlap, candidate isolation and reset. Physical storage and traversal are not selected.
- **Holder and owned/borrowed nature:** Acquisition-state holder retains bytes/coverage/context required across packets. A complete-message view borrows that retained backing or is an owned copy. Candidate reconstruction is isolated from active authoritative data.
- **Valid from:** Ranges accepted into the correctly associated active or isolated candidate acquisition context, with bounded offsets and consistent overlap. Acceptance of bytes into candidate reconstruction is not acceptance of that candidate as the active semantic attempt or authoritative CH. Complete-message access requires complete validated logical extent, not a contiguous allocation assumption.
- **Invalidated by:** Accepted attempt replacement, explicit incompatible-context discard, actual backing eviction/release/overwrite or loss of required association invalidates the affected ranges/views. Conflicting overlap is not merged: reject/quarantine the conflicting incoming range as applicable, but do not erase previously accepted nonconflicting ranges or authoritative CH solely because overlap conflicts. Rejected/ambiguous candidate disposition affects that candidate, not valid active backing. Ordinary packet end does not invalidate valid pending acquisition.
- **During callback:** Complete-message bytes used for current CH evaluation remain stable through relevant callbacks; pending bytes are not exposed as a completed message.
- **After feedback:** Continue/Stop alone does not discard valid pending acquisition. Explicit lifecycle/unsupported disposition may invalidate it. Rejected candidate must not mutate active data.
- **Next operation/session/attempt:** Same-attempt ranges can survive packets. Terminal-attempt candidate acceptance resets old attempt-local context and enters PreHandshake before new CH parsing/reassembly. For an active attempt, isolated candidate acquisition/evaluation may precede complete-new-attempt acceptance; acceptance invalidates old active context before the candidate becomes authoritative, not retroactively before the evaluation needed to classify it. Preserve the accepted candidate result/backing through its required use, or reacquire/re-evaluate it under valid provenance; old-context cleanup must not destroy the only valid candidate evidence. Retry-compatible restart remains the same attempt: retain original/replacement Initial contexts and keep their stream backing distinct unless Phase 2 byte-equivalence/association permits the stated reuse.
- **Available versus fresh:** Byte completeness, parsed completeness, eligible observation and authority are separate. Same accepted ranges are duplicates; equal contents at new logical offsets can be new CH.
- **Isolation requirement:** Associate ranges with session lifecycle, attempt and accepted Initial-stream context. Retired input cannot supply current views; ambiguous evidence cannot merge/reset contexts.

#### L04: CH recognition and evaluation status

Operations: OP-05/06/07. Inventory: P3-OUT-007, P3-OUT-008, P3-OUT-009, P3-VIEW-011

- **Current evidence / authority boundary:** Canonical None/Partial/Complete persists across steps. Complete is set before record-capacity checks and can coexist with Stop; it is not a fresh-event token.
- **Holder and owned/borrowed nature:** Processing result holder owns status values. Current parser/viewer state is mutable backing; copied status is an owned historical scalar.
- **Valid from:** Status is interpreted only for its declared current operation/message scope. Complete means that scope completed, not that every evaluated field or delivery succeeded.
- **Invalidated by:** Operation/context replacement logically invalidates current-status interpretation. Reset, reuse or failed association invalidates old current view even if its scalar value remains Complete.
- **During callback:** Callback sees status and field validity of its own evidence unit; no status is borrowed from an unrelated previous step.
- **After feedback:** Feedback can suppress delivery/authority without undoing completed syntax. Status remains historical; freshness eligibility is not recreated by Continue.
- **Next operation/session/attempt:** New operation starts with its own explicit availability. Resuming acquisition must restore only its own state or recompute it, never inherit another session status.
- **Available versus fresh:** None/Partial/Complete are parsing states. Newly completed, newly eligible, delivered and accepted are separately distinguished.
- **Isolation requirement:** Current operation/message provenance must match; retained Complete from another unit cannot trigger a new CH observation.

#### L05: Fingerprint value and digest/full-string backing

**V3 resolved contract:** V3 concrete consumer: C04 accesses full-string through the shared viewer during onClientHello; digest and fingerprint must match the same CH and backing remains stable until insert returns. No assumption of downstream copy/retention.

Operations: OP-06/10. Inventory: P3-VIEW-001, P3-VIEW-005, P3-VIEW-015, P3-FP-001, P3-FP-002, P3-FP-003, P3-FP-004, P3-FP-005

- **Current evidence / authority boundary:** getFingerprint copies m_fp; getFullString borrows active digest bytes. Digest storage/rebinding/reset and continuation copy have different invalidation boundaries. U04 explicitly preserves the current cipher-loop/output for both transports, and USER-FP-01 preserves identical-CH fingerprint equality including existing selected extension-content rules. These decisions do not claim the legacy algorithm is defect-free.
- **Holder and owned/borrowed nature:** Evaluation backing holder owns digest input and computed fingerprint. getFingerprint output is an owned value copy; full-string pointer/length borrows digest backing. Copying pointer/length is not copying bytes.
- **Valid from:** Current fingerprint requires successful current digest computation and field validity; full-string extent must refer to current initialized accumulated bytes and its stated representation. Complete syntax alone is insufficient.
- **Invalidated by:** Digest init/overwrite, pointer rebind, backing release/eviction or operation replacement invalidates borrowed full-string. Failed computation invalidates current fingerprint availability, not an independently retained prior authoritative fingerprint.
- **During callback:** Current valid fingerprint and any explicitly exposed digest span remain stable during callback. Callback-local projection references have the shorter L12 lifetime.
- **After feedback:** Owned fingerprint copy survives as a historical value. Borrowed digest survives only until guaranteed producer last use, not by virtue of feedback. Suppressed acceptance leaves prior authority separately available.
- **Next operation/session/attempt:** Later digest work may overwrite backing; do not use old full-string pointer. Historical copies remain historical; active retained fingerprint changes only on accepted replacement/reset.
- **Available versus fresh:** Fingerprint equality is not duplicate-message proof. Full-string availability is not a completion event; its contents are digest input, not automatically raw CH.
- **Isolation requirement:** Failed or resumed evaluation cannot return a stale fingerprint as current. Bind each value and digest span to the evaluated message/attempt. USER-FP-01 requires identical fingerprint for identical complete CH over TCP/QUIC; a historical copy or borrowed digest cannot acquire new provenance by matching its value. No QUIC-only repair of a legacy algorithm defect is authorized.

#### L06: SNI value and availability

Operations: OP-06/10. Inventory: P3-VIEW-002, P3-ERR-013

- **Current evidence / authority boundary:** processSni validates and copies into Sni storage; getSni bool distinguishes availability. Current Conductor ignores bool and constructs default Sni when unavailable.
- **Holder and owned/borrowed nature:** Evaluation holder owns copied Sni bytes; getSni destination owns a value copy. ClientHelloData borrows a callback-local Sni referent, not that long-lived evaluation storage.
- **Valid from:** Name is available only when current-message parsing/validation succeeded and availability says so. Default empty name does not prove an observed empty SNI.
- **Invalidated by:** Current operation replacement/reset or failure invalidates current availability. Owned historical copy retains bytes but cannot be relabeled current.
- **During callback:** Expose name and availability consistently for the current CH; projection borrows last only through callback.
- **After feedback:** Owned copies can survive. Feedback does not convert unavailable SNI into valid SNI or accept current name automatically.
- **Next operation/session/attempt:** No stale parsed flag/name may carry into another message/session. Retained prior CH SNI, if retained by the contract consumer, is explicitly prior provenance, not a newly evaluated field.
- **Available versus fresh:** SNI is a field of an evaluated CH, not a separate CH completion event.
- **Isolation requirement:** Clear/restore/recompute both bytes and availability for the producing message; do not merge old name with new CH fields.

#### L07: CH offered versions, viewer version, early_data and counts

Operations: OP-06/07/10. Inventory: P3-VIEW-003, P3-VIEW-004, P3-VIEW-006, P3-VIEW-007, P3-VIEW-008, P3-VIEW-009, P3-REC-012, P3-REC-013

- **Current evidence / authority boundary:** Version mask OR/parsed-flag/shared-parser fallback on resume are static freshness risks. Scalar getters copy values; ClientHelloData count fields narrow uint32_t getters to uint16_t. Non-CH context CH fields are zero/false defaults, not new facts.
- **Holder and owned/borrowed nature:** Evaluation holder owns current scalar fields with individual availability; copies are owned values. Authoritative retained values belong to active semantic attempt, not mutable scratch.
- **Valid from:** Each field is valid only after its producing current-message parse/evaluation succeeds. Offered mask/viewer-selected version is not negotiated version. Counts describe their declared parser scope, not necessarily digest feeding.
- **Invalidated by:** Current-message replacement/reset, failed field parse or context mismatch invalidates affected current field. Prior accepted fields remain separately retained until accepted replacement/reset. A shared parser/viewer can also contain non-CH wire-version data; physical presence of a scalar is not validity as a current CH version. Per-field validity refers to its producing CH, not merely a successful getter.
- **During callback:** All exposed CH fields belong to one evaluated message; absent/not-applicable fields are distinguished from legitimate zero/false.
- **After feedback:** Copied scalars remain historical. Feedback decides authority separately; rejection must not combine current partial fields with old authoritative ones.
- **Next operation/session/attempt:** All mask bits, parsed flags, version fallback and early-data/count values must be restored/recomputed for the resumed message or explicitly absent. Same-message partial accumulation can remain valid; no OR accumulation from another message/session is permitted. Counts and narrowed callback representations retain their declared validity; truncation cannot be silently presented as the complete current count.
- **Available versus fresh:** Field availability does not prove newly eligible CH. Count/timestamp updates follow accepted logical observations, not getter calls or repeated notifications.
- **Isolation requirement:** Publish a coherent current-message field set with per-field validity; replace authoritative CH-derived fields as Phase 2 requires, never field-by-field merge with older CH.

#### L08: Segmentation, continuation mode/gap and acquisition coverage

Operations: OP-02/05/10. Inventory: P3-VIEW-010, P3-CONT-001, P3-CONT-002, P3-CONT-003, P3-CONT-004, P3-CONT-005, P3-CONT-006, P3-CONT-007, P3-CONT-008, P3-CONT-009, P3-CONT-010, P3-CONT-011

- **Current evidence / authority boundary:** isSegmented is current ProcessorState comparison, not durable history; continuation mode/gap are packet-local views. Saved buffers do not restore every CH viewer field.
- **Holder and owned/borrowed nature:** Acquisition holder owns saved continuation metadata and bytes. Current mode/gap/segmentation getters copy scalars; range/coverage views borrow retained acquisition backing.
- **Valid from:** Interpret metadata at its declared packet/operation/stream scope, with valid session/attempt binding. Saved continuation availability is distinct from current view readiness.
- **Invalidated by:** Packet reset replaces packet-local flags; continuation completion/discard/eviction/restart/reset changes saved state. Copied old flags remain historical, not current coverage.
- **During callback:** Current metadata must describe the unit being evaluated; it cannot be assumed to describe the entire CH history.
- **After feedback:** Ordinary feedback preserves valid pending acquisition; explicit discard/lifecycle disposition invalidates affected continuation. Stop is not automatic memory release.
- **Next operation/session/attempt:** Rebind/restore the correct continuation without importing canonical scratch from the last session. Packet-local mode is refreshed independently of retained bytes.
- **Available versus fresh:** NeedMoreData or segmentation is not a fresh CH. Completion of acquisition alone is not callback delivery.
- **Isolation requirement:** Key continuation by the correct session lifecycle/attempt/stream identity; numeric session slot reuse must not resurrect old buffers.

#### L09: Current Record, record collection and TLSRecordContext

Operations: OP-02/09/10. Inventory: P3-REC-001, P3-REC-002, P3-REC-003, P3-REC-004, P3-REC-005, P3-REC-006, P3-REC-007, P3-REC-008, P3-REC-009, P3-REC-010, P3-REC-011, P3-REC-012, P3-REC-013, P3-REC-014

- **Current evidence / authority boundary:** m_record and m_records are mutable Processor-owned storage; getLastRecord/getAllRecords return references. TLSRecordContext borrows Record and owns scalar fields. Capacity failure can leave Complete without current record delivery.
- **Holder and owned/borrowed nature:** Processor owns current TCP record/collection. Consumer borrows references. Context wrapper is callback/analysis-local; scalar record/context copies own values, not packet backing.
- **Valid from:** Current record/context is valid only for successfully produced current unit and field-specific predicates; alert fields require readableAlert, CH fields require current CH applicability. Collection has explicit accumulated/reset scope, not immutable packet-batch semantics.
- **Invalidated by:** Next record write, collection mutation/clear, beginPacket/reset/resume or backing destruction invalidates current-view interpretation; context wrapper scope end invalidates its borrow. Copying wrapper does not own Record.
- **During callback:** Current record/context remains stable throughout consuming callbacks and current-unit analysis. No parsing next record while a live current borrow is being consumed.
- **After feedback:** Producer can use current backing for same-unit work while stable; consumer cannot assume context survives callback. Owned Record copies remain historical, with validity discriminators.
- **Next operation/session/attempt:** Next record/packet may overwrite/reset; re-read only a newly bound result. QUIC uses semantic evidence, never artificial Record storage to satisfy this rule.
- **Available versus fresh:** RecordParsed is not CH completion. A later non-CH record cannot inherit fresh CH fields because canonical status remains Complete.
- **Isolation requirement:** Current result must be bound to producing unit/session/attempt; collection entries retain their own origin if exposed, not just last-viewer association.

#### L10: Errors, reasons and progress outcomes

Operations: OP-02/04/06/09/10. Inventory: P3-OUT-001, P3-OUT-002, P3-OUT-003, P3-OUT-004, P3-OUT-005, P3-OUT-006, P3-OUT-010, P3-VIEW-012, P3-ERR-001, P3-ERR-002, P3-ERR-003, P3-ERR-004, P3-ERR-005, P3-ERR-006, P3-ERR-007, P3-ERR-008, P3-ERR-009, P3-ERR-010, P3-ERR-011, P3-ERR-012, P3-ERR-013, P3-ERR-014, P3-ERR-015, P3-ERR-016, P3-ERR-017, P3-ERR-018, P3-ERR-019

- **Current evidence / authority boundary:** Public step result and getLastError are separate channels. Canonical error can persist on resumed path; local soft Stop can have no dedicated error. Unsupported enum names are not public Unsupported.
- **Holder and owned/borrowed nature:** Current result holder owns outcome/reason scalars and their origin. Getter copies are owned historical values; optional error payload spans, if any, borrow declared backing.
- **Valid from:** Outcome/reason is associated with the operation that produced it; None means no reported error, not proof of every success. Resource/unsupported/parser/feedback outcomes remain distinct.
- **Invalidated by:** Next operation invalidates current designation. Canonical retained last error must not be exposed as new error; declaration-only enum/sentinel has no runtime event.
- **During callback:** Callback receives current reason and origin, with any borrowed details stable through callback. No stale canonical error is attached to an unrelated unit.
- **After feedback:** Feedback outcome can be copied; parser outcome is not rewritten into observer verdict. Historical reason remains readable without becoming fresh evidence. If actual resource loss prevents reliable future association/completion evaluation while InProgress, retain the resource cause and the applicable Phase 2 Unsupported disposition separately. Failure of one packet, a CRYPTO hole, rejected overlap or missing current field is not automatically that attempt-wide loss; no synthetic completion, close or Incomplete Handshake violation follows.
- **Next operation/session/attempt:** Next operation starts with explicit current outcome/availability, even if internal storage is reused or resume bypasses reset.
- **Available versus fresh:** An error, Stop or Complete status is not a new CH notification. Progress is operation-local; retained reason is not replayed.
- **Isolation requirement:** Bind outcome/reason to session/attempt and operation; failure cannot expose prior-session evaluated fields as successful current output.

#### L11: Qualified completion, close and deferred Handshake evidence

Operations: OP-08. Inventory: QUIC-only contract family; Step 4 operations and Phase 2, no current TCP counterpart required.

- **Current evidence / authority boundary:** Phase 2 sections 5.2, 9 and 9.1 govern associated client Handshake completion and supported close evidence; no server Finished, handshake payload decryption or new protocol policy is assumed.
- **Holder and owned/borrowed nature:** Current evidence holder owns copied qualified metadata; frame slices borrow acquisition backing. Deferred supporting evidence is retained by attempt-supporting holder, not a viewer scratch flag.
- **Valid from:** Only evidence meeting Phase 2 association/state/applicability guards is eligible. Header observation, deferred evidence, close classification and state disposition remain separate. An associated client v1 Handshake Long Header is product-level completion evidence, not decoded TLS Finished; before first authoritative CH it is retained as bounded non-authoritative supporting evidence for the same attempt. Associated readable Initial transport CONNECTION_CLOSE type 0x1c with CRYPTO_ERROR 0x100..0x1ff, including user_canceled 0x15a, can qualify in PreHandshake/InProgress under Step 4 OP-08. Other close reasons, encrypted Handshake close contents and retired/ambiguous evidence are not terminating-alert evidence.
- **Invalidated by:** New-attempt reset, rejection/retirement, loss of supporting association or lifecycle release invalidates affected applicability. Terminal state does not reopen from late evidence.
- **During callback:** Qualified evidence and supporting provenance remain stable for its callback/analysis scope; unreadable/unacquired close content is absent, not fabricated.
- **After feedback:** Feedback does not prove endpoint confirmation. Retained same-attempt supporting evidence remains applicable only under the Phase 2 guards until applied/dispositioned; applying deferred evidence does not create a second newly acquired Handshake event or repeat a terminal transition. Required association identity may remain retained even after the progress effect is applied. Copied current evidence is historical; retained deferred evidence is a separately identified supporting fact, not merely a replayed current copy.
- **Next operation/session/attempt:** Deferred evidence can support later CH under Phase 2. Revalidate association before use; ordinary packet end is not invalidation. Retry-compatible restart follows same-attempt policy, not automatic new-attempt reset.
- **Available versus fresh:** Newly acquired evidence is distinguished from retained supporting evidence. Deferred Handshake use is not a new CH; 0-RTT produces no CH completion/state change.
- **Isolation requirement:** Deferred evidence cannot move to a new attempt. Retired or ambiguous evidence cannot be attached to current CH simply because session matches.

#### L12: ClientHelloData and other callback projections

**V3 resolved contract:** V3 concrete consumers: C02 detector setters pass pointers followed by synchronous detect; C05 temporary Params is consumed during collect. No proof of later dereference or deep-copy. The borrow grants no use beyond last guaranteed scope; Phase 4 must preserve this boundary without assuming const means lifetime.

Operations: OP-09. Inventory: P3-CB-001, P3-CB-002, P3-CB-003, P3-CB-004, P3-CB-005, P3-CB-006

- **Current evidence / authority boundary:** Conductor onClientHello creates local Sni/Fingerprint and ClientHelloData holding references to them. TLSRecordContext borrows Processor Record; other callbacks borrow live TLSData and upstream session/frame.
- **Holder and owned/borrowed nature:** Callback producer owns wrapper/local referents; callback consumer borrows. ClientHelloData value copy still borrows its referenced Sni/Fingerprint. Error/verdict/scalar copies own their values.
- **Valid from:** Projection exists only after the producing current observation is qualified for delivery; its fields and availability match that observation, not a later canonical viewer read.
- **Invalidated by:** Wrapper/local referent scope exit ends borrowing. A nested call that overwrites referenced processing backing is prohibited by the proposed stability requirement unless equivalent isolation is guaranteed.
- **During callback:** All supplied referents are live/stable for synchronous callback duration. This is the intended contract; external retention/reentrancy behavior is not proven or declared safe by current code.
- **After feedback:** CH-local referents expire when onClientHello returns, before subsequent outer feedback/policy work. A consumer needing later data must hold an independent owned value/byte copy; retaining wrapper/viewer pointer is insufficient.
- **Next operation/session/attempt:** No callback projection is implicitly reusable on next operation. Independently copied data survives as a historical observation with provenance, not a current viewer.
- **Available versus fresh:** Callback invocation is delivery, not proof of new completion. One eligible logical CH is not multiplied by packet/range/context/getter count. Multiple eligible listeners can each receive their applicable delivery for the same logical unit; within-list Stop still limits delivery. The intended freshness boundary is not a claim that legacy repeated TCP callbacks have been fixed, nor a new ban on legitimate listener fan-out. No automatic retry/replay is introduced.
- **Isolation requirement:** Capture/bind current projection before dispatch; do not let observer getter access accidentally select another session/attempt. No global scratch mutation while current borrowed projection is in use.

#### L13: Live TLSData and retained authoritative CH/count/timestamp/verdict fields

Operations: OP-07/08/09/10. Inventory: P3-STATE-001, P3-STATE-002, P3-STATE-003, P3-STATE-004, P3-STATE-005, P3-STATE-006, P3-STATE-007, P3-STATE-008, P3-STATE-009, P3-STATE-010

- **Current evidence / authority boundary:** TLSSessionExtension owns TLSData and exposes references. Reset changes values; release/reuse can end the old binding and backing lifetime. Marking reclaimable alone neither guarantees release nor invalidates a live view; actual reclamation/reuse is a separate event. Callback const reference is live state, not an immutable historical snapshot.
- **Holder and owned/borrowed nature:** Active semantic context owns authoritative fields. Existing extension is current code holder; future concrete owner remains undecided. Borrowed const reference is live; independent copied fields own historical values.
- **Valid from:** Retained CH is available only after policy acceptance for that attempt, with explicit per-field availability. State/count/metadata can exist before any accepted CH; zero defaults are not a CH.
- **Invalidated by:** Terminal-attempt new-candidate acceptance resets old authority before new CH parsing; active complete-new-attempt acceptance resets old authority after isolated candidate evaluation and before candidate authority acceptance. Accepted later CH replaces the applicable authoritative values. Reset logically invalidates the old semantic association even if the object remains; actual release/reuse can also end physical borrowing. Marking reclaimable alone is not invalidation. Terminal state may retain prior authority for allowed reads, not another CH of that terminal attempt.
- **During callback:** Callback gets live state at the defined pre-normal-update boundary of OP-09; current-unit feedback is distinct from the current CH projection. Const access does not freeze the object across later policy updates. No nested processing may change its promised current-use association while the callback borrow is live unless equivalent isolation is guaranteed. This is a proposed guarantee, not evidence that external reentrancy is safe.
- **After feedback:** Feedback may change state/verdict/authority; re-access only with current binding. Old owned snapshot remains historical; a live reference may physically exist but no longer represent its earlier observation.
- **Next operation/session/attempt:** Same-attempt authority remains available while another CH is Partial/fails. Accepted replacement is coherent; rejected candidate does not change it. New attempt makes old values unavailable as current even if memory remains.
- **Available versus fresh:** Available authoritative CH is not newly completed. Count/timestamp change only on accepted non-retransmission logical CH, never on getter/repeated notification. Preserve Step 4 OP-07 timestamp provenance: the supplied packet/frame timestamp of the operation that first makes the eligible complete result available, not callback wall clock or first-fragment time. Feedback pause does not replace it; suppressed/retransmitted/Retry-equivalent delivery does not refresh authority timestamp. This is the inherited proposed timestamp decision, not a newly inferred product requirement.
- **Isolation requirement:** Only one active semantic attempt. Historical copies retain old origin; reused session/extension cannot inherit old availability or supporting acquisition.

#### L14: Observer verdict and lifecycle disposition

Operations: OP-09/10. Inventory: P3-FB-001, P3-FB-002, P3-FB-003, P3-FB-004, P3-FB-005, P3-FB-006, P3-FB-007, P3-FB-008, P3-FB-009, P3-OBS-009

- **Current evidence / authority boundary:** TLSFeatureVerdict owns Dispatch/mask values; Stop differs from parser Stop and state transition. markReclaimable bool is ignored by current caller; marking is not immediate release.
- **Holder and owned/borrowed nature:** Callback returns owned verdict value; current feedback holder owns accumulation for the current unit. Lifecycle authority retains semantic disposition, not callback-local references.
- **Valid from:** Verdict is valid for the unit/observer that returned it; lifecycle disposition requires its own valid context. No borrowed callback data is implicitly retained with verdict.
- **Invalidated by:** Next evidence unit starts its own feedback scope. Reset/release/replacement ends current context applicability; historical copied verdict remains historical.
- **During callback:** Observer may inspect only valid supplied data; verdict production must not overwrite backing still needed by current callbacks.
- **After feedback:** Feedback resolves permission for current unit and subsequent processing. Stop/suppression does not create fresh CH, release backing automatically or invalidate valid pending acquisition without explicit disposition.
- **Next operation/session/attempt:** Resolve current-unit feedback before a later unit is dispatched or accepted; do not carry an unresolved verdict as that later unit's own feedback. Applicable accepted state and accumulated verdict metadata can persist for the same attempt under this contract/Phase 2, and must be observed before next processing. Stop prevents later processing in the current traversal; it is not automatically a future-packet replay request or memory-release event. New-attempt reset prevents old attempt-local metadata from being relabeled as new-attempt feedback.
- **Available versus fresh:** Feedback is not parsing, delivery or authority; a suppressed fresh observation is not automatically replayed by retained Complete.
- **Isolation requirement:** Associate feedback with its evidence unit and attempt; lifecycle invalidation is logical even if memory is retained. No pool/commit mechanism is selected.

### L / 4. Fresh ClientHello and authority sequence

The contract distinguishes six facts, not one Complete flag:

1. CH syntax/extent is complete for the current logical message.
2. Evaluated fields are available individually; fingerprint failure does not expose stale success.
3. The logical CH is newly eligible: correct association, active-attempt applicability and not a retransmission under Phase 2.
4. Delivery occurs for that eligible observation, if feedback permits it.
5. Policy accepts the observation and updates authority/count/timestamp if permitted.
6. Accepted data remains available later without producing another completion event.

These are logical distinctions, not a selected commit/dispatch algorithm. A Complete result that fails result-capacity or evaluation checks is not automatically delivered/accepted. Retained Complete cannot trigger intended fresh notification on a later non-CH unit. Fingerprint equality is not message identity: same bytes at already accepted ranges are retransmission; identical CH contents at new logical offsets can be a new CH. HRR-compatible later CH remains governed by Phase 2; no session-wide multiple-CH ban is introduced.

Suppression/Stop does not automatically schedule retry delivery. This contract defines no observer replay mechanism. U05 explicitly selects no TCP notification change in this task. Any separately proposed correction requires a new compatibility review; no code change is claimed here.

Phase 2 policy is preserved with two reset boundaries: terminal-attempt new-candidate acceptance resets old attempt-local state and enters PreHandshake before new CH parsing/reassembly; active complete-new-attempt acceptance may require isolated complete candidate CH evaluation first, then resets old active state before accepting the candidate as authoritative. A single semantic attempt remains active; candidate acquisition is not a second active TLSData context. Retry-compatible restart is the same attempt, not automatic authority reset. Retained retired identity suppresses late input when available; ambiguity does not merge/reset; 0-RTT is state-neutral and is not CH completion. Supporting evidence retained for one attempt cannot be applied to its replacement.

### L / 5. Cross-session and cross-attempt isolation obligations

F01. Bind current input/result before access. A getter interface address or numeric session slot is not provenance.

F02. For every operation, current result fields are either produced/restored for that exact context or explicitly unavailable. This includes mask bits, parsed flags, SNI, early-data, counts, status, digest and error origin.

F03. Resume binds correct acquisition backing and restores/recomputes its dependent state coherently. A buffer keyed by session does not prove all shared viewer fields belong to that session.

F04. Do not mutate backing during its guaranteed borrow. Reentrant processing that overwrites shared scratch must be prevented or isolated; no lock, snapshot class or reentrancy implementation is prescribed.

F05. Terminal candidate acceptance invalidates old attempt-local authority/acquisition before new CH parsing. Active complete-new-attempt acceptance follows isolated candidate evaluation, then invalidates old active context before candidate authority acceptance. Candidate evidence needed for that acceptance must remain valid through last use; rejected or ambiguous candidates cannot change active authority. Retired data may survive only as explicitly historical/suppression evidence, not as current views.

F06. Publish per-field availability, not zero/false-as-success. Retained prior authority is separate from current evaluation; replacement never combines fields from different logical CHs.

F07. Scope freshness and feedback to the evidence unit. Repeated reads/callbacks cannot create a new eligible observation. Unresolved feedback cannot be silently carried to a different unit.

F08. Before next processing overwrites/releases backing, all promised borrows have ended or equivalent independent backing is retained. Owned historical copies survive only as historical values.

These guarantees prevent stale exposure in the proposed contract. They are not evidence that the existing implementation already enforces them. Implementation proof remains later work. U05 already supplies the no-TCP-change compatibility disposition for this task; a separately proposed behavior change would require its own review.


#### 5.1 Feedback, state and next-processing boundary

OP-09 governs feedback. Current-unit verdict values and accepted persistent state are different lifetimes. Within a listener list Stop ends that list. Stop with no accumulated violation mask can allow permitted current-unit work/normal update, but prevents a later unit in the current traversal. A nonzero accumulated mask suppresses normal current-unit update; combined Stop follows the existing Error disposition. These are inherited semantic boundaries, not a new callback order or mitigation policy.

Before the next policy-sensitive processing unit, resolve current feedback and use the resulting current hsState/attempt association. A copied old verdict is not that next unit's verdict, but applicable accepted state/mask can persist for the same attempt. Parser Stop, observer Stop, unsupported disposition and resource loss remain separate. Feedback alone does not release pending reconstruction or grant retention of callback-local referents. A resource/terminal/reset disposition can logically invalidate affected context, with physical cleanup deferred until promised borrows/last uses end. No automatic replay or drop is introduced.

Acquisition can expose several raw facts/messages in one input; each later policy-sensitive unit remains subject to current feedback. Read-only scanning or isolated candidate work cannot precommit later authority, overwrite a live borrow, or relabel old views as current. This is a validity constraint, not a selected runtime traversal algorithm.

### L / 6. Explicit Phase 1 risk review

#### R-RESET: mutable viewer reset/resume

PRIOR-REPORT: Phase 1 SD1 and viewer-continuation findings. CODE-VERIFIED anchors: Processor::beginPacket, resetStepState, resumeStepFromSegmentBuffer, processSegmentedClientHelloContinuation, feedSuppVersionsFP; inventory P3-VIEW-003/014 and P3-CONT-007.

The ordinary successful non-resumed beginPacket path reaches resetStepState; a resumed path returns before that reset. Early error paths are not proof that canonical state was refreshed. Saved continuation rebinds backing and restores selected fields, not every CH field. Supported-version feeding ORs mask bits, the parsed flag affects fallback, and continuation fallback reads the shared CH parser version. Thus session-keyed storage alone does not establish complete viewer freshness. Static interleaving risk is recorded; no measured production corruption is claimed.

Required contract response: F01-F04/F06 and L03-L08. Restore/recompute current dependent fields or mark unavailable; keep valid pending acquisition and prior authority separately. Do not blindly reset every session buffer, and do not preserve stale scratch as a compatibility obligation. No fix is claimed.

#### R-REPEAT: repeated ClientHello notification

PRIOR-REPORT: Phase 1 SD8. CODE-VERIFIED anchors: TLSFlowConductor::analyzeProcessResult/onClientHello and Processor::processRecord/resetStepState; inventory P3-FB-008, P3-CB-002, P3-OUT-009, P3-VIEW-011.

Current eligibility tests retained Complete during RecordParsed analysis, not fresh completion or necessarily a CH-shaped current record. A later non-CH record can therefore notify again if state/feedback permits reaching it. Its CH context fields are still zero/false. Complete can also remain after record-capacity failure without normal CH delivery. None proves a fresh logical CH.

Required contract response: Section 4, F06/F07 and L04/L09/L12-L14. Availability, fresh eligibility, delivery and acceptance are distinct. Intended fresh notification is tied to the eligible logical observation, not canonical Complete. This does not retroactively fix TCP or approve duplicate callbacks as mandatory compatibility; U05 explicitly preserves current TCP behavior in this task. A separately scoped proven-bug correction would reopen only its affected compatibility review; no change disposition is open for the preserved task.

#### R-BORROW: different last-use boundaries

ClientHelloData borrows local fingerprint/Sni created inside onClientHello. Their lifetime ends when that function returns, before later outer feedback work. TLSRecordContext borrows mutable Processor Record. Full-string borrows digest backing. const TLSData& borrows live extension state. Their wrappers/copies do not all have the same lifetime. Owned field/byte copies are distinct from reference-bearing wrapper copies.

Required response: L05/L09/L12/L13 and F04/F08. No external retention/reentrancy is assumed safe without evidence.


## 6. Required feedback boundaries

FB-A: resolve input association and accepted reset before candidate data becomes active. Terminal candidate resets before CH parsing; active candidate can need isolated complete evaluation before reset/authority acceptance. Preserve candidate backing through last use.

FB-B: pause after each current feedback-relevant evidence unit before later dispatch/authority acceptance. Validated read-only scanning cannot precommit later semantics, overwrite live views or bypass Stop.

FB-C: within-list Stop ends that listener list. Zero accumulated mask can permit current-unit work/normal update, but prevents later units in the current traversal. Nonzero accumulated mask suppresses normal update; combined Stop follows existing Error disposition. Earlier mask is not erased by a later zero local mask.

FB-D: parser Stop, observer Stop, resource cause, Unsupported and network action are distinct. D01 is product Unsupported, not automatic observer violation/drop. Independent applicable verdicts remain separate.

FB-E: next processing uses resolved hsState/attempt association, not stale InProgress permission. Finished input must pass terminal candidate classification before new CH parsing. Stop does not automatically request replay or free pending acquisition.

FB-F: callback-local borrows end at their last guaranteed use; producer backing needed after callback remains valid independently. const TLSData is live state, not immutable snapshot. Owned copies retain historical provenance only.

Evidence: OP-03/07/09/10, L03/L12/L13/L14, P3-FB-001..009, S6-05/12/29..35. Full ordering of independent CH/close evidence remains Phase 4 decision U06; both branches are representable.

## 7. Focused scenario review

36 complete cases are carried below: 31 PASS and five PASS WITH LIMIT for static representability. These are not runtime tests or all-gates PASS. Original OP/L/P2/M locators refer to the consolidated operation/lifetime/policy/candidate-matrix sections above. Limited cases remain limited, not silently upgraded.



#### S6-01: Complete QUIC CH

**Verdict:** PASS.

**Initial context:** A: PreHandshake, no authority, valid retained Initial context.

**Ordered inputs:** One associated Initial supplies all bytes of CH H1 at offset 0; message-local validation and fingerprint succeed.

**Semantic walkthrough:** OP-03 establishes association; OP-04 recovers Initial; OP-05 establishes complete message; OP-06 evaluates H1; OP-07 identifies a new logical CH; OP-09 resolves its feedback.

**Progress and boundary:** Same input work -> pause for feedback -> current input exhausted.

**New versus available observations:** One new logical CH; current versions/SNI/early_data/counts/fingerprint belong to H1. No TLS record is synthesized.

**Feedback:** Continue, zero mask; normal update allowed.

**Resulting state and authority:** A becomes InProgress; authority H1, count 1, completion-input timestamp t1.

**Ownership, validity and invalidation:** L02/L03 backing survives required evaluation; L12 local callback referents expire on return; L13 authoritative value copies survive for A.

**False-event/information-loss guard:** Complete is not used as an independent event token; no count per frame/listener.

**Authority and contract locators:** P2 §§3,6,7.1; C OP-03..07/09; L L03-L07/L12-L14; M P3-OBS-001/P3-CB-002

#### S6-02: Partial CH then completion

**Verdict:** PASS.

**Initial context:** A: PreHandshake, no authority.

**Ordered inputs:** Input I1 supplies CH prefix/body portion; I2 supplies missing extension bytes under same associated context.

**Semantic walkthrough:** I1: accept valid ranges, identify Partial, no complete evaluation. I2: restore correct coverage and message-local facts, evaluate complete H1 and resolve feedback.

**Progress and boundary:** I1: need more input after usable work exhausted. I2: pause for feedback then finish.

**New versus available observations:** I1: zero new complete CH; Partial/coverage available. I2: one eligible CH.

**Feedback:** No CH feedback on incomplete H1; I2 Continue/zero mask.

**Resulting state and authority:** I1 stays PreHandshake. I2 becomes InProgress, count 1; timestamp is I2 completion provenance, not first fragment time.

**Ownership, validity and invalidation:** Pending ranges survive I1 end; I1 borrowed packet bytes cannot be reused after expiry unless retained under valid backing.

**False-event/information-loss guard:** Missing bytes are not malformed complete syntax; no stale prior fingerprint fills Partial.

**Authority and contract locators:** P2 §6; C OP-05/06/07, §§4-5; L L01/L03/L04/L07/L13

#### S6-03: Partial new CH with prior authority

**Verdict:** PASS.

**Initial context:** A: InProgress, authority H1/count 1.

**Ordered inputs:** A new logical message H2 at new offsets is incomplete.

**Semantic walkthrough:** OP-05 accepts its ranges; OP-06 cannot declare complete H2; retain H1 separately from H2 Partial.

**Progress and boundary:** Need more input; usable later input units can still be considered under feedback permission.

**New versus available observations:** No new complete CH. H1 remains available with H1 provenance; H2 Partial is current.

**Feedback:** No invented H2-complete notification.

**Resulting state and authority:** A remains InProgress; H1/count/timestamp unchanged.

**Ownership, validity and invalidation:** L04/L07 current fields cannot be borrowed from H1 as H2 facts; L13 H1 owned authority persists.

**False-event/information-loss guard:** Available H1 is not H2 completion; no field-by-field merge.

**Authority and contract locators:** P2 §§3,6,7.1; C §§4-5 OP-05/06; L L03/L04/L07/L13

#### S6-04: TCP CH record followed by non-CH record

**Verdict:** PASS WITH LIMIT (K01).

**Initial context:** TCP active context; input contains R1 complete CH then R2 non-CH record, both reached under existing state/feedback.

**Ordered inputs:** One TCP payload, two validated record boundaries.

**Semantic walkthrough:** OP-02 produces R1 unit; feedback and normal update resolve; then R2 unit is acquired using resulting state. Current record CH fields for R2 are absent/default, while prior CH remains available.

**Progress and boundary:** RecordParsed -> feedback -> next record -> feedback/exhaustion.

**New versus available observations:** One newly completed logical CH from R1. Current TCP can invoke CH callback again on R2 if retained Complete and control flow permit; represent that invocation separately from a new CH.

**Feedback:** Per current record unit; no packet-wide commit.

**Resulting state and authority:** Exact TCP record-policy updates remain existing behavior; no second logical CH is inferred solely from the repeated callback.

**Ownership, validity and invalidation:** R1 mutable record borrow expires at overwrite; copied CH value can persist historically. Callback-local CH referents expire each invocation.

**False-event/information-loss guard:** SD8 is representable, not fixed or required compatibility. No claim that actual TCP count/notification is corrected.

**Authority and contract locators:** P2 §5.1; C OP-02/09, §§5.1,7; L L09/L12/L13, §6 R-REPEAT; M P3-FB-008

#### S6-05: Coalesced Initial CH then Handshake packet

**Verdict:** PASS.

**Initial context:** A: PreHandshake, valid association evidence.

**Ordered inputs:** Datagram contains P1 Initial completing H1 and P2 associated v1 Handshake Long Header.

**Semantic walkthrough:** Process P1 CH unit, resolve feedback and authority update, then qualify P2 under current InProgress state. P2 payload is not decrypted.

**Progress and boundary:** Same datagram -> CH feedback pause -> next packet -> completion feedback.

**New versus available observations:** One new CH plus one newly acquired product completion evidence unit.

**Feedback:** Both Continue/zero mask in this case.

**Resulting state and authority:** PreHandshake -> InProgress -> Finished.

**Ownership, validity and invalidation:** Current P1/P2 views have distinct provenance; required attempt association survives until P2 evaluation.

**False-event/information-loss guard:** One datagram is not one observation; Handshake header is not decoded TLS Finished.

**Authority and contract locators:** P2 §§5.2,6,9; C OP-03/04/07/08/09, §7; L L02/L11/L14

#### S6-06: CH and readable close in one Initial

**Verdict:** PASS WITH LIMIT (K02).

**Initial context:** A: PreHandshake; active Initial context retained.

**Ordered inputs:** One Initial contains CRYPTO completing H1 and an associated transport close with CRYPTO_ERROR.

**Semantic walkthrough:** Acquisition exposes two distinct facts. Walk the valid CH-first semantic path: evaluate/resolve H1, then qualify readable close with updated state. No full callback/traversal algorithm is selected.

**Progress and boundary:** CH unit feedback then close unit feedback; input finishes.

**New versus available observations:** CH and close are distinct units, not one fabricated TLS record.

**Feedback:** Continue/zero mask for both units.

**Resulting state and authority:** H1 can become authoritative then A becomes TerminatedByAlert. A close-first permitted path can terminate before later CH eligibility; the contract must not precommit both irrespective of feedback.

**Ownership, validity and invalidation:** Close scalar copy and CRYPTO/CH backing have distinct holders; no early overwrite of live unit.

**False-event/information-loss guard:** Representability does not select precedence among independent facts; full dispatch order remains later work.

**Authority and contract locators:** P2 §§6,9.1,12; C OP-04/07/08/09, §7; L L02/L11/L14

#### S6-07: Several ranges fill one CH in current input

**Verdict:** PASS.

**Initial context:** A: PreHandshake.

**Ordered inputs:** One or several Initial packets supply [0,a), [a,b), [b,end) for H1; all associated and validated.

**Semantic walkthrough:** Accept each range; only final sufficient coverage unlocks OP-06. A range update is not a CH event.

**Progress and boundary:** Continue same input until message available; then pause for CH feedback.

**New versus available observations:** Zero CH for first two ranges; one CH after final coverage.

**Feedback:** Continue/zero mask at eligible CH boundary.

**Resulting state and authority:** InProgress, count 1.

**Ownership, validity and invalidation:** Each plaintext slice is retained/copied only under valid backing; pending reconstruction spans range operations.

**False-event/information-loss guard:** No event per range or packet; same fingerprint as the same complete CH over TCP.

**Authority and contract locators:** P2 §§6,7; C OP-04/05/06/07; L L02/L03/L05

#### S6-08: Gap followed by later coalesced packet that fills it

**Verdict:** PASS.

**Initial context:** A: PreHandshake; [0,a) accepted.

**Ordered inputs:** P1 supplies [b,end), leaving [a,b); P2 in same datagram supplies [a,b).

**Semantic walkthrough:** P1 records a hole without malformed-CH classification; validated P2 boundary permits processing P2, which completes coverage.

**Progress and boundary:** More work in same input despite pending coverage; NeedMoreInput only if hole remains after usable work.

**New versus available observations:** One CH after P2, none from gap itself.

**Feedback:** Continue/zero mask on CH.

**Resulting state and authority:** InProgress; no Unsupported from hole.

**Ownership, validity and invalidation:** Accepted [0,a) and [b,end) survive P1; no discard on ordinary packet end.

**False-event/information-loss guard:** TCP sequence-gap disposition is not imported into CRYPTO holes.

**Authority and contract locators:** P2 §§6,7.2; C OP-05, §4; L L03/L08; M P3-CONT-005

#### S6-09: Repacketized retransmission of accepted CH

**Verdict:** PASS.

**Initial context:** A: InProgress, H1 already accepted.

**Ordered inputs:** New packets deliver identical bytes at already accepted offsets; packet numbers/packetization may differ.

**Semantic walkthrough:** OP-05 identifies duplicate bytes; OP-07 identifies same logical CH; consume current input without fresh CH eligibility.

**Progress and boundary:** Continue/exhaust input; no CH feedback pause required solely for duplicate.

**New versus available observations:** Zero new CH; H1 remains available.

**Feedback:** No replayed CH feedback; any independent current evidence is separately evaluated.

**Resulting state and authority:** State/count/timestamp/authority unchanged.

**Ownership, validity and invalidation:** Duplicate input scratch can expire; accepted H1 and association retain original provenance.

**False-event/information-loss guard:** Equal offsets/bytes, not packet number or digest alone, justify duplicate.

**Authority and contract locators:** P2 §7.2; C OP-05/07; L L03/L04/L13

#### S6-10: Identical overlap with new suffix

**Verdict:** PASS.

**Initial context:** A: PreHandshake; H1 prefix accepted.

**Ordered inputs:** Incoming range overlaps accepted prefix identically and extends into missing suffix.

**Semantic walkthrough:** Ignore duplicate overlap as new bytes; accept new suffix under same association; evaluate H1 if coverage becomes sufficient.

**Progress and boundary:** Continue acquisition then CH feedback if complete.

**New versus available observations:** One CH only when first completed; overlap itself zero CH.

**Feedback:** Continue/zero mask if complete.

**Resulting state and authority:** InProgress/count 1 if complete, otherwise PreHandshake/pending.

**Ownership, validity and invalidation:** Accepted overlap retained; newly supplied suffix has its own valid backing.

**False-event/information-loss guard:** Identical overlap is not a second CH or an unsupported sequence mismatch.

**Authority and contract locators:** P2 §7.2; C OP-05/07; L L03/L08

#### S6-11: Conflicting overlap

**Verdict:** PASS.

**Initial context:** A has accepted nonconflicting ranges and possibly H1 authority.

**Ordered inputs:** Incoming associated range conflicts with accepted bytes at overlapping offsets.

**Semantic walkthrough:** Report conflict and reject/nonmerge conflicting incoming evidence; preserve previously accepted ranges/authority. Do not select a new attempt from conflict alone.

**Progress and boundary:** Current conflicting range ends; other validated units can proceed if permitted.

**New versus available observations:** No new CH or inferred violation from conflict alone.

**Feedback:** No automatic observer verdict; existing explicit outcome remains available.

**Resulting state and authority:** Active state and prior authority unchanged absent an independent disposition.

**Ownership, validity and invalidation:** Incoming invalid view is not current accepted backing; accepted active ranges remain valid.

**False-event/information-loss guard:** No merge/reset/discard of active bytes solely from overlap conflict.

**Authority and contract locators:** P2 §7.3; C OP-05, §6; L L03, §2.1/F05

#### S6-12: One range unlocks two logical CH messages

**Verdict:** PASS.

**Initial context:** A: PreHandshake, same associated Initial stream.

**Ordered inputs:** Contiguous range contains complete H1 followed by complete H2 at new offsets.

**Semantic walkthrough:** OP-05 exposes two message extents; evaluate/resolve H1 first; then H2 under resulting state and feedback permission.

**Progress and boundary:** Feedback between policy-sensitive units even if both extents are already available.

**New versus available observations:** Two eligible logical CHs if both normal units are permitted; no full-flow legality rejection merely from CH position.

**Feedback:** Continue/zero mask for both.

**Resulting state and authority:** First H1 -> InProgress/count 1; H2 replaces authority/count 2, remains InProgress.

**Ownership, validity and invalidation:** Do not overwrite H1 live projection while its feedback is pending; H2 has its own extent/provenance.

**False-event/information-loss guard:** No TCP retained-Complete guard imposed as QUIC datagram-wide ban.

**Authority and contract locators:** P2 §§3,6,7.1; C OP-05/06/07/09, §7; L L03/L12/L14

#### S6-13: Additional CH with identical content at new offsets

**Verdict:** PASS.

**Initial context:** A: InProgress, H1 authoritative.

**Ordered inputs:** Complete H2 at new offsets, byte-identical to H1, same active context.

**Semantic walkthrough:** Message identity is new despite equal fingerprint/content; OP-07 accepts non-retransmission H2.

**Progress and boundary:** Pause for H2 feedback then continue.

**New versus available observations:** One new logical CH with same fingerprint value but new message provenance.

**Feedback:** Continue/zero mask.

**Resulting state and authority:** Authority H2, count increases once, timestamp H2 completion input; InProgress.

**Ownership, validity and invalidation:** Historical H1 copies remain historical; current borrowed fields refer to H2.

**False-event/information-loss guard:** Fingerprint equality does not prove retransmission.

**Authority and contract locators:** P2 §§3,6,7; C OP-07; L L05/L13

#### S6-14: Retry-compatible restart before first CH completion

**Verdict:** PASS.

**Initial context:** A: PreHandshake, partial H1 in original context C0.

**Ordered inputs:** Candidate C1 distinct from every retained context; token evidence, CRYPTO offset 0, byte-equivalent overlap, consistent available identity/nonzero SCID; no contradiction.

**Semantic walkthrough:** First exclude same-context retransmission; accept Retry-compatible C1 as same attempt; retain C0/C1. First parseable completion in either context becomes H1 authority.

**Progress and boundary:** Acquisition continues or needs more input; feedback only on first eligible complete CH.

**New versus available observations:** At most one logical H1 across byte-equivalent C0/C1 completions.

**Feedback:** Continue/zero mask on first completion.

**Resulting state and authority:** Same attempt; PreHandshake until completion then InProgress/count 1.

**Ownership, validity and invalidation:** Separate valid stream backing; retained context identity survives replacement of most recent context.

**False-event/information-loss guard:** Token/DCID change alone is not observed Retry or new attempt.

**Authority and contract locators:** P2 §8; C OP-03/05/07; L L03/L13

#### S6-15: Retry-compatible resend after authoritative CH

**Verdict:** PASS.

**Initial context:** A: InProgress, H1 authoritative in C0.

**Ordered inputs:** C1 satisfies all Retry-compatible conditions and reconstructs byte-equivalent H1.

**Semantic walkthrough:** Associate C1 with same attempt; classify equivalent completion as same logical H1, not fresh.

**Progress and boundary:** Consume acquisition; no new CH delivery solely from resend.

**New versus available observations:** Zero new CH; authority H1 available.

**Feedback:** No repeated normal CH update.

**Resulting state and authority:** InProgress; count/timestamp unchanged; C0/C1 retained.

**Ownership, validity and invalidation:** New context backing is not a second semantic TLSData attempt.

**False-event/information-loss guard:** No reset or timestamp refresh from Retry-equivalent completion.

**Authority and contract locators:** P2 §8; C OP-03/07; L L03/L13

#### S6-16: Complete new attempt while active

**Verdict:** PASS.

**Initial context:** A: InProgress/H1; active retained contexts known.

**Ordered inputs:** Isolated complete, successfully evaluated H2 candidate at offset 0 under C2 distinct from every A context; excludes retransmission/Retry; inconsistent available CH identity; no retained retired match.

**Semantic walkthrough:** Acquire/evaluate H2 in isolation without overwriting A; apply P2 10.1 acceptance; retire/reset A context, enter PreHandshake, then process first CH of B with valid H2 candidate evidence.

**Progress and boundary:** Candidate acquisition -> control disposition -> CH feedback.

**New versus available observations:** One first logical CH for B, not an additional CH of A.

**Feedback:** Continue/zero mask for normal B CH.

**Resulting state and authority:** B becomes InProgress/count 1; A values no longer current.

**Ownership, validity and invalidation:** Preserve H2 backing through acceptance; old cleanup cannot destroy candidate. Historical A copies retain A provenance only.

**False-event/information-loss guard:** No second active semantic TLSData; no reset before evidence needed to classify active candidate.

**Authority and contract locators:** P2 §§10.1,12; C OP-03/07; L L03/L13/F05

#### S6-17: Terminal new-attempt candidate with partial CH

**Verdict:** PASS.

**Initial context:** A: Finished (same rule for Error/Unsupported/TerminatedByAlert).

**Ordered inputs:** Distinct available candidate Initial context; CRYPTO origin 0; no late duplicate/retired match; Retry excluded. CH not yet complete.

**Semantic walkthrough:** Classify before CH parsing per P2 10.2; accept candidate B; reset old attempt-local state and enter PreHandshake; then parse/reassemble B Partial.

**Progress and boundary:** Control disposition before parsing -> need more input.

**New versus available observations:** No complete B CH yet; Partial belongs to B, not A.

**Feedback:** No CH-complete callback; candidate acceptance is a control boundary.

**Resulting state and authority:** B PreHandshake, no authority/count 0; A retained only as historical/retired identity where available.

**Ownership, validity and invalidation:** A live semantic association invalidated at accepted reset; B pending ranges survive current input end.

**False-event/information-loss guard:** Do not wait for complete B CH to reset terminal A; no A fingerprint as B.

**Authority and contract locators:** P2 §10.2; C OP-03; L L03/L13/F05

#### S6-18: Changed DCID or terminal retained-context input

**Verdict:** PASS.

**Initial context:** A terminal; original C0 and Retry replacement C1 retained.

**Ordered inputs:** Variant 1: packet DCID changes without justified distinct candidate context. Variant 2: packet matches old retained C0, not most recent C1.

**Semantic walkthrough:** Neither variant satisfies P2 new-candidate acceptance; matching any retained terminal context is late A input. Do not enter CH parsing for terminal A.

**Progress and boundary:** Suppress current candidate/late input, finish or continue independent validated work.

**New versus available observations:** Zero new CH/new-attempt observation.

**Feedback:** No fabricated reset feedback.

**Resulting state and authority:** A remains terminal.

**Ownership, validity and invalidation:** Retained identity is suppression evidence, not fresh authority.

**False-event/information-loss guard:** Most recent context is not the entire attempt identity; DCID change alone proves nothing.

**Authority and contract locators:** P2 §§8,10.2,12; C OP-03/10; L L03/L11/L13

#### S6-19: Associated Handshake before CH

**Verdict:** PASS.

**Initial context:** A: PreHandshake, incomplete/no authoritative CH.

**Ordered inputs:** Associated unambiguous v1 Handshake header, then complete parseable H1 of same attempt.

**Semantic walkthrough:** Retain bounded non-authoritative progress evidence; stay PreHandshake. On first CH normal authority acceptance, apply same-attempt deferred fact.

**Progress and boundary:** First input can finish with retained evidence; CH unit later pauses for feedback.

**New versus available observations:** One new CH; deferred header is supporting earlier evidence, not a second newly observed packet.

**Feedback:** Continue/zero mask on applicable units.

**Resulting state and authority:** PreHandshake -> InProgress -> Finished in first-CH semantic cycle.

**Ownership, validity and invalidation:** Deferred identity/evidence survives input end and remains A-specific; effect is not reapplied as fresh.

**False-event/information-loss guard:** No server Finished/HANDSHAKE_DONE claim; no completion before authoritative CH.

**Authority and contract locators:** P2 §§6,9; C OP-08; L L11/L13

#### S6-20: Associated Handshake after CH, then repeated header

**Verdict:** PASS.

**Initial context:** A: InProgress/H1.

**Ordered inputs:** Associated v1 Handshake header, then another header of same terminal attempt.

**Semantic walkthrough:** First qualifies product completion; after feedback A is Finished. Later input is terminal/late, not another active completion.

**Progress and boundary:** First unit feedback; later unit suppressed under current terminal state.

**New versus available observations:** One applicable completion effect; repeated header is not another state transition.

**Feedback:** Continue/zero mask first unit.

**Resulting state and authority:** Finished; H1 retained as allowed authority.

**Ownership, validity and invalidation:** Association identity may remain needed after effect; markReclaimable is not guaranteed release.

**False-event/information-loss guard:** ACK-only/encrypted-close Handshake payload is unreadable but header can qualify; not TLS Finished proof.

**Authority and contract locators:** P2 §§5.2,6,9,12; C OP-08/10; L L11/L13

#### S6-21: Unassociated Handshake and state-neutral 0-RTT

**Verdict:** PASS.

**Initial context:** A: PreHandshake or InProgress.

**Ordered inputs:** Handshake header with ambiguous/retired association; separately an associated 0-RTT packet.

**Semantic walkthrough:** Do not retain ambiguous header as A completion. 0-RTT is state-neutral and payload not parsed with Initial keys.

**Progress and boundary:** Consume/suppress eligible current input without CH/completion feedback invention.

**New versus available observations:** No new CH/completion; 0-RTT presence is not CH early_data acceptance.

**Feedback:** No automatic Stop/violation.

**Resulting state and authority:** State unchanged.

**Ownership, validity and invalidation:** No ambiguous supporting evidence contaminates A; current packet scratch expires normally.

**False-event/information-loss guard:** No short-header/0-RTT substitute for accepted Handshake signal.

**Authority and contract locators:** P2 §§2,6,9,11; C OP-03/08; L L11/F01/F05

#### S6-22: Readable Initial terminating-alert evidence

**Verdict:** PASS.

**Initial context:** A: PreHandshake or InProgress with retained active Initial context.

**Ordered inputs:** Successfully processed associated Initial transport close 0x1c with code 0x15a; no retired/ambiguous classification.

**Semantic walkthrough:** OP-04 validates readable close; OP-08 maps CRYPTO_ERROR range to alert description and qualifies terminal evidence.

**Progress and boundary:** Pause for current evidence feedback; then terminal permission.

**New versus available observations:** One terminating-alert evidence unit, no invented alert level.

**Feedback:** Continue/zero mask; normal terminal update allowed.

**Resulting state and authority:** TerminatedByAlert even for user_canceled; not inferred from local CH failure.

**Ownership, validity and invalidation:** Current close copy has A provenance; later A CH cannot reopen terminal attempt.

**False-event/information-loss guard:** Readable close plus valid association is required, not any CONNECTION_CLOSE.

**Authority and contract locators:** P2 §§6,9.1; C OP-04/08; L L02/L11

#### S6-23: Non-CRYPTO close and encrypted Handshake close

**Verdict:** PASS.

**Initial context:** A: InProgress.

**Ordered inputs:** Variant 1: readable Initial transport close outside CRYPTO_ERROR range. Variant 2: associated Handshake header whose encrypted payload happens to contain close, but contents unavailable.

**Semantic walkthrough:** Variant 1 does not qualify TerminatedByAlert. Variant 2 qualifies only header-based completion; do not decode payload.

**Progress and boundary:** Current evidence handled under its own scope; no fabricated close parser result.

**New versus available observations:** Variant 1 no terminating-alert observation; variant 2 product completion evidence only.

**Feedback:** Continue/zero mask where applicable.

**Resulting state and authority:** Variant 1 no TerminatedByAlert from this fact; no additional general-close state is invented. Variant 2 Finished.

**Ownership, validity and invalidation:** No ciphertext-derived alert view; current readable code is separate from absent alert description.

**False-event/information-loss guard:** Transport closure in reality is not an invented product transition; no alert-level synthesis.

**Authority and contract locators:** P2 §§5.2,9.1; C OP-08; L L11

#### S6-24: Ambiguous and retired close/CH input

**Verdict:** PASS.

**Initial context:** A active; B retired identity retained.

**Ordered inputs:** Candidate association ambiguous, or close/CH matches retained B context.

**Semantic walkthrough:** Classify before merging/qualifying; suppress as ambiguous/retired. A acquisition and authority are untouched.

**Progress and boundary:** End/suppress affected input; no reset/feedback fabricated.

**New versus available observations:** Zero new A CH/close/completion.

**Feedback:** No inferred violation or Error.

**Resulting state and authority:** A unchanged; B not reopened.

**Ownership, validity and invalidation:** B copies may be historical/suppression evidence, never A current view.

**False-event/information-loss guard:** No merge/reset/authority replacement from ambiguity or late input.

**Authority and contract locators:** P2 §§6,7.3,9.1,12; C OP-03/05/08; L L03/L11/F05

#### S6-25: Malformed complete CH versus incomplete coverage

**Verdict:** PASS.

**Initial context:** A: PreHandshake or InProgress with prior H1.

**Ordered inputs:** Variant 1: missing declared CH bytes. Variant 2: full bounded CH extent available but an extension length exceeds its message boundary.

**Semantic walkthrough:** Variant 1 waits for coverage; variant 2 reports message-local parsing failure with origin and invalid current fields. Prior authority stays separately available.

**Progress and boundary:** Pending input for variant 1; failed current evaluation ends variant 2; valid later units depend on permission.

**New versus available observations:** No new successful CH; Partial versus malformed reason remains distinguishable.

**Feedback:** Parsing-error feedback if applicable is separate from failure; no automatic observer Stop.

**Resulting state and authority:** No automatic Error/TerminatedByAlert/Unsupported merely from malformed CH.

**Ownership, validity and invalidation:** Invalid current field cannot be filled from H1; historical H1 survives with original association.

**False-event/information-loss guard:** Incomplete acquisition is not malformed complete syntax; no legacy unused error producer invented.

**Authority and contract locators:** P2 §§3,13; C OP-05/06, §6; L L04/L07/L10

#### S6-26: Initial recovery failure, then later valid packet

**Verdict:** PASS.

**Initial context:** A: InProgress, required association state retained.

**Ordered inputs:** P1 Initial recovery fails under available context; later bounded P2 can still be reliably processed.

**Semantic walkthrough:** Report protection/context failure for P1 without CH facts. Continue only at independently validated P2 boundary and permitted state.

**Progress and boundary:** P1 operation ends; later validated work possible.

**New versus available observations:** No CH from P1; any P2 observation has P2 provenance.

**Feedback:** Failure is not an observer verdict; normal feedback on P2 if produced.

**Resulting state and authority:** No transition solely from P1 when reliable later evaluation remains possible.

**Ownership, validity and invalidation:** P1 invalid plaintext never exposed; valid retained A association is not erased.

**False-event/information-loss guard:** No client-authentication claim, malformed-CH claim or new attempt from AEAD failure.

**Authority and contract locators:** P2 §§2,13; C OP-04, §6; L L02/L10

#### S6-27: Unsupported version or required state loss

**Verdict:** PASS.

**Initial context:** A: InProgress.

**Ordered inputs:** Variant 1: associated non-v1, non-reserved Long Header. Variant 2: actual loss of state required for future association/completion.

**Semantic walkthrough:** Retain precise origin/cause; apply P2 explicit Unsupported disposition. Resource cause and policy result are separate.

**Progress and boundary:** End reliable current semantic processing for A; future terminal candidate handling remains possible.

**New versus available observations:** Unsupported disposition, not CH/close/completion.

**Feedback:** No automatic client violation or drop; existing verdict channel remains separate.

**Resulting state and authority:** Unsupported, not Error.

**Ownership, validity and invalidation:** Affected required backing/association invalidated; owned historical values can remain historical.

**False-event/information-loss guard:** Reserved/out-of-scope/unassociated input and recoverable one-packet failure do not all map to this transition.

**Authority and contract locators:** P2 §§6,13; C OP-03/10, §6; L L08/L10/L13

#### S6-28A: Required fingerprint construction exceeds supported product capacity

**Verdict:** PASS under D01 (the supplied F01 closure decision).
**Initial context:** An associated accepted attempt A is PreHandshake or InProgress; earlier H1 authority, if any, is separately identified.
**Ordered inputs:** H2 reaches a supported fingerprint-building capacity limit and reliable required evaluation cannot continue. Syntax is valid to the extent actually checked; capacity failure does not certify unchecked fields.
**Semantic walkthrough:** OP-06 reports precise product-capacity cause and per-field validity. Apply D01 explicit Unsupported before a later policy-sensitive unit. No normal successful H2 CH observation, default/stale fingerprint, partial-authority merge or automatic replay is produced.
**Progress and boundary:** End reliable semantic processing of A; terminal new-candidate classification remains available under Phase 2.
**New versus available observations:** Unsupported/product-limit disposition, not a successfully evaluated new CH. Prior H1 can remain separately available with original provenance.
**Feedback:** Parser outcome and observer verdict remain separate. Capacity failure alone is not a violation, Error, network drop or Incomplete Handshake report. An independently applicable terminal verdict still follows the existing verdict contract; this case assumes no such independent verdict.
**Resulting state and authority:** Unsupported. H2 does not advance normal count/timestamp or replace authoritative fingerprint. H1, if present, is retained authority of the now terminal A, not H2 data.
**Ownership, validity and invalidation:** Failed construction views are invalid as successful current output; outcome backing survives its last use. Logical Unsupported does not destroy every historical owned copy.
**False-event/information-loss guard:** Distinguish true capacity cause from zero-length digest append rejection and from later legacy record collection failure. No blanket mapping of every false calculate return to Unsupported.
**Authority and contract locators:** D01; supplied closure addendum S6-28A; Phase 2 sections 6/13; C OP-06/07/09/10; L L04/L05/L10/L13/L14.


#### S6-28B: Legacy record-collection capacity after completed fingerprint

**Verdict:** PASS WITH LIMIT (K01); current-code representability only.
**Initial context:** TCP processing with a bounded record collection and sufficiently many preceding successful records, if reachable under validation, input extent, state and feedback.
**Ordered inputs:** Current CH completes fingerprint and sets Complete, then the maxRecords check fails before normal record insertion/delivery.
**Semantic walkthrough:** Preserve successful fingerprint computation, Complete status, collection-capacity cause and public Stop as distinct facts. Stop dispatch does not deliver normal CH. Do not describe this later failure as missing fingerprint.
**Progress and boundary:** StopProcessing translates to public Stop and ends the record loop. Lack of a getAllRecords reader does not remove this control-flow effect.
**New versus available observations:** Completed evaluated data can be available; successful normal delivery/authority is not guaranteed. No fabricated fresh event or invalid fingerprint is inferred.
**Feedback:** Actual error feedback remains independent of parser Stop; do not apply D01 merely because the collection is full.
**Resulting state and authority:** Existing TCP feedback/state guards determine disposition; no TCP repair or new QUIC capacity policy is chosen. A record collection is not a required QUIC observation batch.
**Ownership, validity and invalidation:** Canonical Complete/fingerprint may physically remain; record/context delivery validity and borrowed lifetimes remain scoped. No copied vector or 128-record QUIC limit is required.
**False-event/information-loss guard:** Ordinary first CH after reset and CH completion after resumed-path clearing do not establish a full collection. A conditional branch is not a demonstrated production packet; no runtime reachability test is claimed.
**Authority and contract locators:** Static Processor::processNewRecord/processSegmentedClientHelloContinuation/processRecord; maxRecords; getLastRecord/getAllRecords; supplied closure addendum S6-28B; C OP-02/06/09; L L04/L09/L10/L12; inventory P3-ERR-017/P3-OUT-009.


#### S6-29: STOP without violations

**Verdict:** PASS.

**Initial context:** A active; current unit U1 valid; U2 remains in same input.

**Ordered inputs:** Listener returns Stop with accumulated mask zero.

**Semantic walkthrough:** Stop ends that listener list. Permitted current-unit groups/work may finish; if mask stays zero, normal update can occur. Resolve state before considering any later unit.

**Progress and boundary:** Current-unit feedback resolved; end current traversal before U2 dispatch/acceptance.

**New versus available observations:** U1 is not replayed; U2 has no new accepted observation in this traversal.

**Feedback:** Stop, zero mask; no automatic Error/drop.

**Resulting state and authority:** Normal U1 state/authority update may be accepted under existing boundary; next packet uses resulting state.

**Ownership, validity and invalidation:** Local callback referents expire; processing backing lasts through U1 last use; valid pending acquisition not automatically freed.

**False-event/information-loss guard:** Stop is not resource loss, automatic replay or network action.

**Authority and contract locators:** P2 existing verdict contract; C OP-09, §7; L L12-L14, §5.1

#### S6-30: Continue with violations and later Stop with zero local mask

**Verdict:** PASS.

**Initial context:** A active; U1 produces feedback.

**Ordered inputs:** Variant 1: Continue with nonzero mask. Variant 2: earlier group adds mask, later group returns Stop with zero local mask.

**Semantic walkthrough:** Accumulate current-unit mask. Variant 1 suppresses normal update. Variant 2 uses accumulated, not just last local mask, and follows Error/terminal Stop.

**Progress and boundary:** Variant 1 can continue traversal only under resolved state; variant 2 ends immediately at terminal disposition.

**New versus available observations:** No fabricated normal authority update for suppressed U1.

**Feedback:** Variant 1 Continue/nonzero mask; variant 2 combined Stop/nonzero accumulated mask.

**Resulting state and authority:** Variant 1 existing violation metadata retained, normal U1 state update skipped; variant 2 Error under existing contract.

**Ownership, validity and invalidation:** Current feedback is tied to U1; accepted persistent mask/state is not U2 current verdict.

**False-event/information-loss guard:** Do not discard earlier mask or claim last zero-mask Stop is harmless.

**Authority and contract locators:** P2 §6 existing terminal verdict; C OP-09; L L14, §5.1

#### S6-31: STOP with violations

**Verdict:** PASS.

**Initial context:** A nonterminal; current unit U1 valid.

**Ordered inputs:** Current applicable verdict Stop with nonzero mask.

**Semantic walkthrough:** Resolve terminal verdict; suppress normal U1 update and prevent later-unit dispatch/acceptance.

**Progress and boundary:** End current traversal.

**New versus available observations:** No new CH authority from suppressed normal update; current delivered evidence can remain historical.

**Feedback:** Stop/nonzero mask; parser success is independent.

**Resulting state and authority:** Error according to existing verdict contract; no automatic drop implied.

**Ownership, validity and invalidation:** Keep required borrows through last use before cleanup; actual release differs from logical terminal disposition.

**False-event/information-loss guard:** No parser-error relabeling or lifetime destruction merely from Stop.

**Authority and contract locators:** P2 §6; C OP-09; L L12-L14, §5.1

#### S6-32: State feedback before next packet/unit

**Verdict:** PASS.

**Initial context:** A: InProgress/H1; U1 associated Handshake evidence; U2 Initial CH of same attempt remains.

**Ordered inputs:** U1 normal completion feedback accepted, then U2 arrives/remainder is considered.

**Semantic walkthrough:** Resolve U1 -> Finished; U2 eligibility reads updated terminal state, not old InProgress. Unless P2 10.2 accepts a new candidate, U2 does not reach CH parsing for A.

**Progress and boundary:** Completion pause -> updated permission -> terminal suppression/classification.

**New versus available observations:** No additional active-A CH observation after Finished.

**Feedback:** Continue/zero mask on U1; U2 cannot reuse stale permission.

**Resulting state and authority:** Finished unless valid new-attempt candidate resets to PreHandshake.

**Ownership, validity and invalidation:** U1 supporting evidence cannot become U2 fresh event; terminal identity retained where required.

**False-event/information-loss guard:** No precommit U2 based on old state even if bytes were already scanned.

**Authority and contract locators:** P2 §§6,10.2,12; C OP-03/08/09; L L11/L14/F04

#### S6-33: TCP A partial, B processed, A resumes

**Verdict:** PASS WITH LIMIT (K01).

**Initial context:** A has valid pending CH extension continuation; B is another session.

**Ordered inputs:** A prefix/partial extensions, then B packet changes shared scratch, then exact A continuation bytes.

**Semantic walkthrough:** Bind A pending backing; after B, A resume must restore/recompute A mask/flags/SNI/counts/error/parser-dependent facts or mark unavailable. Numeric session slot/viewer pointer is insufficient provenance.

**Progress and boundary:** A waits; B resolves its own feedback; A resumes and completes or reports precise failure.

**New versus available observations:** Only A completion can produce A fresh CH; B retained scratch cannot fill A fields.

**Feedback:** Independent per-unit/session feedback; no B verdict applied as A current verdict.

**Resulting state and authority:** A state/count update only on accepted A logical result; B state remains separate.

**Ownership, validity and invalidation:** A saved bytes survive B processing; A live borrows cannot be overwritten by B while promised valid.

**False-event/information-loss guard:** SD1 risk is represented and forbidden by proposed guarantee, not declared repaired in existing code.

**Authority and contract locators:** P2 TCP baseline; C OP-02/10, §5.1; L L04-L10/F01-F04, §6 R-RESET

#### S6-34: TCP continuation exact, retransmission, overlap and gap

**Verdict:** PASS WITH LIMIT (K01).

**Initial context:** A has pending TCP continuation with expected sequence and mode.

**Ordered inputs:** Variants: exact expected bytes; full retransmission; identical overlap plus new bytes; sequence gap.

**Semantic walkthrough:** Use actual TCP sequence/mode producer rules. Exact/usable overlap advances acquisition; retransmission is not fresh CH; gap can discard affected continuation and produce explicit Unsupported under TCP contract. Do not import this disposition into QUIC.

**Progress and boundary:** Exact/overlap may continue/complete; retransmission consumes no fresh completion; gap ends reliable affected continuation.

**New versus available observations:** At most one newly completed CH from actual completion; current mode/gap/error metadata are distinct from historical segmentation.

**Feedback:** Feedback only for actual eligible current result/outcome.

**Resulting state and authority:** Apply existing TCP disposition; no automatic QUIC-style hole waiting forced onto TCP.

**Ownership, validity and invalidation:** Session-scoped continuation restored coherently; discard invalidates affected saved backing, not another session authority.

**False-event/information-loss guard:** isSegmented/current mode do not prove historical CH fragmentation; resumed Complete does not prove fresh completion.

**Authority and contract locators:** P2 TCP unchanged; C OP-02/10; L L03/L08/L09, §6; M P3-CONT-005/006/010

#### S6-35: Owned copy versus borrowed callback view after reset

**Verdict:** PASS.

**Initial context:** A current CH callback is running; valid local fingerprint/SNI projection.

**Ordered inputs:** Consumer copies value bytes, separately retains only wrapper reference; callback ends, then attempt resets or next session processing occurs.

**Semantic walkthrough:** Owned copy remains readable with A historical provenance. Reference-bearing wrapper loses its local referents at callback return; later outer feedback uses its own valid backing, not that wrapper. Live const TLSData reference changes semantics at update/reset.

**Progress and boundary:** No new processing event from later access; invalid borrow must not be dereferenced.

**New versus available observations:** Zero new CH/completion from either copy or retained pointer.

**Feedback:** Reset/feedback does not turn historical copy into current B authority.

**Resulting state and authority:** Current B association cannot be inferred from equal fingerprint or surviving memory.

**Ownership, validity and invalidation:** Physical lifetime, semantic validity and ownership are independent; marking reclaimable alone is not destruction.

**False-event/information-loss guard:** No wrapper copy ownership or const-reference snapshot assumption.

**Authority and contract locators:** C OP-09/10; L §2.1 L05/L06/L12/L13/F04/F06


## 8. Decisions, unresolved items and deferred work

### Binding decisions

D-STATE: preserve accepted Phase 2 state/attempt policy; no new legality gate or fabricated completion evidence.

D-FP: USER-FP-01 identical complete CH -> identical fingerprint independent of transport/packetization. This is resolved. No QUIC-only algorithm repair.

D-CAPACITY / D01: required fingerprint product-capacity exhaustion that prevents reliable evaluation -> explicit Unsupported for the accepted associated attempt. No successful CH with missing/stale fingerprint, count/timestamp update, partial merge, Error/violation/drop or automatic replay. Prior authority remains separately available. Candidate failure is isolated until acceptance; terminal candidate accepted before parsing owns its later failure. Legacy collection failure and zero-length append rejection are not automatically D01.

D-TIME: supplied completion-operation packet/frame timestamp travels with eligible CH; not first fragment or callback wall clock. This is a proposed contract choice, not a newly discovered product requirement.

D-UNIT: incremental current evidence units; no datagram-wide precommit. D-VALIDITY: available/evaluated/fresh/delivered/accepted are distinct; live references, callback-local borrows and owned copies have different last uses.

### V3 closure: consumer boundaries and preservation

Authority: explicit current user decisions for U02/U04/U05 and immutable-result contract; supplied current-message consumer code for C01-C06; verified TLSFlowConductor/ITlsListener/Processor branches. Selected exact code excerpts and limits are preserved in phase3-user-consumer-evidence-v1.md. This is not a complete downstream implementation audit.

**Participation:** buildExtension is feature-driven, not inherently TCP-only. At least one live subscriber with nonzero mask requests extension allocation. Successful extension acquisition permits inbound/aging registration. Feature status, profile and port checks belong to each feature; the supplied HTTPS predicate is not automatically a universal QUIC port rule. Nonzero mask does not promise successful allocation or all six events. No new callback/event bit is added for symmetry.

**Immutable data versus feedback/side effects:** CH/context/TLSData are const result views. Features cannot mutate them through the supported API, but return verdicts and may change mutable packet/session state through their existing feature actions. Parser-result immutability does not imply no mitigation or no side effects. TLSFlow remains responsible for semantic state/normal commits. No mitigation redesign is included.

**Full-string last use:** the supplied Fingerprint manager reads the shared viewer within onClientHello and passes its bytes to insert. Fingerprint value and digest span must belong to the same current CH, and digest backing must remain live/stable through the entire nested call. A subsequent unit/session/attempt cannot rebind it before that last use. The insert implementation is not supplied; no deep-copy or asynchronous safety is asserted. The defined borrow grants no post-callback use. If a later storage implementation retains data for later access, it must own a copy or equivalent independent backing before the borrow expires; this is an implementation obligation, not a new speculative API.

**Detector pointer setters:** Manager stores &clientHelloData or &context.record through detector setters then calls handleEvent/detect synchronously. Pointer storage alone neither proves a use-after-return nor extends referent lifetime. Valid use is limited to the producing callback/current-unit scope; no stale pointer may be dereferenced on later events or after reset. The contract does not require a new detector class or cleanup API. Phase 4 preserves backing through synchronous evaluation and does not authorize nested processing that invalidates it. Downstream compliance is not claimed from absent detector bodies.

**Visibility:** getParams constructs a projection and collect consumes it synchronously. The Params definition is not supplied, so no deep-copy ownership is inferred. The temporary and its referents are valid through the collect call; registry/recorder later storage must retain owned values/independent backing if needed. The current packet/CH version/SNI/fingerprint fields remain the same semantic facts, not negotiated version or fabricated QUIC record fields.

**Fingerprint Action:** supplied TlsFlowListener discards PacketFlowManager's returned Action and returns default TLSFeatureVerdict. Packet/session side effects may already have occurred. Do not manufacture observer Stop from the discarded Action or remove side effects in a TCP refactor. Preserve the actual boundary. Enforcement explicitly converts violations to verdict mask and Stop/Continue; this is the existing feedback path.

**Preservation:** processCiphers feeds N-1 of N valid suites because the decrement is the loop condition. The final suite omission is a verified legacy defect. Preserve current algorithm/output for both transports as explicitly selected; no default fix, QUIC-only repair or new compatibility choice remains. Existing TCP callbacks/reset/resume/collection behavior stays unchanged in this task. Real bugs are documented and may be fixed only in a separately scoped correction; suspected defects are not required QUIC behavior.

#### Consumer-specific scenario review

| Check | Evidence / expected contract | Verdict |
|---|---|---|
| C-S01: mask zero/nonzero and allocation failure | C01 plus buildExtension: zero/expired listeners do not request participation; nonzero requests it; allocation failure prevents registration | PASS |
| C-S02: fingerprint + full-string within CH callback | C03/C04: same-current-CH backing stable until nested insert returns; no later-unit scratch overwrite; post-callback retention not granted | PASS |
| C-S03: detector pointer setter then detect | C02: referent valid through synchronous evaluation; no proof of later dereference, no right to use expired pointer; later implementation must obey last-use | PASS |
| C-S04: visibility temporary and fingerprint Action | C05/C06: temporary valid through collect; no inferred deep-copy; returned Action is not listener verdict; existing side effects preserved | PASS |

These are static contract reasoning checks, not runtime conformance tests. They close the semantic consumer constraints without inventing downstream storage behavior or blocking Phase 4 on implementation details that the contract already resolves.

### Closed items, bounded limitations and deferred work

| ID | Item / source | Impact | Disposition and closure evidence | Gate / Phase 4 task restriction |
|---|---|---|---|---|
| E01 | Exact full-file matrix V2 archival parity not certified; supplied text/index available | Full expanded-text/byte parity remains unproven; independent semantic review is now recorded | Retain the supplied-text provenance limit. Do not assert an exact V2 hash or verbatim recovery. Section 3 has independent semantic review and is not silently substituted for the original matrix | Archival limitation only; no exact matrix recovery claim. Independent semantic matrix review is complete; archival parity is not claimed |
| U02 | CLOSED: same participation rule as TCP | User decision; onNewSession/buildExtension/registerSessionObservers verified | Nonzero live-listener mask requests participation; extension availability/allocation still required; feature predicates remain feature-owned | No contract-changing integration unknown; Phase 4 uses existing registration path |
| U03 | CLOSED at semantic boundary: immutable results and current-unit consumer access | User decision, listener API, supplied consumer call sites C02-C05 | Full-string backing stable through nested insert call; detector pointer use confined to live current-unit borrow; copies/retention need owned backing if later use is implemented | No architecture selected; no post-callback safety or deep-copy implementation claim. Phase 4 must enforce defined last use |
| U04 | CLOSED: preserve existing fingerprint algorithm/output | Explicit user preservation decision; last-suite omission statically verified | Same existing evaluator behavior in TCP/QUIC; no QUIC-only fix; common correction is separate task | No pending algorithm selection for Phase 4 |
| U05 | CLOSED for this task: preserve TCP behavior | Explicit user no-TCP-change decision | Keep existing notification/reset/collection control flow; proven bug requires separate explicit correction scope, not automatic fix | Intended QUIC freshness is not certification of legacy TCP; no current refactor permission |
| U06 | K02 independent CH/close full ordering | Two valid branches can yield different allowed authority history | Phase 4 chooses full dispatch order consistent with FB boundaries; branch outcomes already explicit | Genuine Phase 4 work, not contract unknown |
| U07 | P3-ERR-008/011 declared-only errors and sentinel | Naming/reachability only if later code depends on names | Use actual syntax failure/cause, do not invent producers; map vocabulary only if required | Nonblocking semantic contract; no new event |
| U08 | Numeric QUIC resource bounds, backing/pool placement, commit mechanism | Implementation performance/storage | Phase 4/later design under validity/D01; no invented limits | Deferred design, not an excuse to leave D01 unknown |
| U09 | Full FRD/SRS requirement mapping | Product completeness beyond approved scope unproven | Explicit user deferral retained; focused excerpt only for a requirement that changes contract | No claim of full product requirement coverage |

F01 is closed by D01. U02-U05 are now closed by explicit user decisions and supplied call-site evidence. Consumer storage internals are not invented: the contract defines permitted last use and Phase 4 must implement it. No missing consumer implementation is used to reopen already-defined semantics. Archival matrix parity remains a provenance limitation; independent row review is retained. Full FRD/SRS remains explicitly deferred.

## 9. KISS/YAGNI concept review

| Concept | Demonstrated current need | What is not introduced | Review |
|---|---|---|---|
| Bounded input/provenance | Safe parsing and session/attempt isolation | Public provenance tuple/API or new generation field | PASS |
| Acquisition versus semantic unit | Coalescing/ranges versus CH/close feedback | Batch event/vector or one component per operation | PASS |
| Current versus retained authority | Partial/failure/retransmission must not replay old CH | Snapshot framework or per-session viewer class | PASS |
| Per-field validity | Missing SNI/fingerprint/non-CH facts are not zero-as-success | Symmetric optional fields for unavailable encrypted data | PASS |
| Owned/borrowed last-use | Actual reference-bearing callbacks/digest/live state | Mandatory copy strategy, lock or pool | PASS |
| Attempt/Initial-context association | Phase 2 restart/new/retired/ambiguous rules | New semantic attempt concurrently active | PASS |
| Outcome/progress/feedback distinction | Parser Stop, resource, Unsupported and observer verdict differ | Generic future-protocol capability framework | PASS |
| Completion/close evidence | Current associated Handshake header and readable Initial close | Decoded TLS Finished, alert level or general-close state | PASS |
| D01 product-limit disposition | F01 closure, no missing fingerprint success | Numeric capacities or capacity event for symmetry | PASS |
| OP/L labels | Traceability of required semantics and lifetime | Mandatory methods/types/classes/interfaces | PASS |

No type/event/API is approved merely for symmetry. Future concrete proposals require their own necessity check.

## 10. Exit-gate evidence and Phase 4 handoff

The eight conditions are the Phase 3 workplan exit bullets, in their original order. PASS is static contract review, not runtime or external-consumer validation. Exact matrix archival recovery is not an additional exit condition.

### EG1: PASS

**Condition:** Every affected input/transition classified with evidence or scope justification.

**Evidence and reasoning:** 131 V5 IDs have one inventory row and one reviewed matrix row; each has source anchors, a comparison boundary, classification and OP trace. Section 3.2 covers the seven TCP and nineteen QUIC policy rows. Participation and task compatibility are resolved by U02-U05. The only remaining Unresolved matrix rows are P3-ERR-008/P3-ERR-011 declared-only vocabulary, explicitly nonblocking under U07; no unverified input is relabeled unavailable.

### EG2: PASS

**Condition:** TCP-specific versus current-scope exclusion is justified.

**Evidence and reasoning:** TCP sequence, record type/length/version predicates, CCS, alert level and TCP flags are exact carrier-specific items. Early-data count, opaque tracking and sentinel exclusions are current-scope/producer exclusions. CH handshake type, SNI, versions and fingerprint remain shared content; record-shaped storage does not decide classification.

### EG3: PASS

**Condition:** no remaining unknown changes the intended semantic contract or prevents Phase 4 work.

**Evidence:** U02 same nonzero feature-mask participation is an explicit user decision and matches buildExtension/onNewSession. U03 immutable results, synchronous forwarding, shared full-string getter and detector pointer setters are supplied evidence; last-use/provenance constraints are now explicit in OP-09/L05/L12 and Section 8. No deep-copy, asynchronous retention or reentrancy safety is falsely claimed. U04 existing algorithm/output preservation is explicit. U05 no TCP behavior change in this task is explicit. D01/F01 remains closed and integrated. U06 ordering, U08 concrete backing/limits and U09 deferred requirement mapping are bounded genuine later work. Declaration-only vocabulary remains nonblocking. Archival matrix parity is disclosed and does not negate the independent row review.

**Boundary:** PASS is semantic-contract readiness, not certification of every downstream implementation. Phase 4 must enforce the defined borrow/feedback/preservation constraints. A proposed behavior change or newly discovered contract conflict reopens the affected review; no silent fix is approved.

### EG4: PASS

**Condition:** TCP and QUIC expressible without fabricated Records or lost information.

**Evidence and reasoning:** OP-01..10 and Sections 5/6 retain distinct packet/frame/message/current-unit facts, current versus retained CH, readable versus unavailable close, failure cause and policy disposition. S6-04/05/06/12/22/23/28A/28B cover compound cases. A non-CRYPTO close is still acquired close evidence even though it does not select TerminatedByAlert; no claim that it is nonterminal on the wire.

### EG5: PASS

**Condition:** Cardinality/freshness/lifetime/ownership explicit.

**Evidence and reasoning:** All L01..L14 specify holder/borrow, validity start, invalidation, callback use, after-feedback use, next-context use, availability/freshness and isolation. OP-07 count/timestamp and S6-02/03/09/13/33/35 distinguish logical CH, duplicates, live state, callback-local references and historical owned copies. Intended guarantees are not existing-code compliance claims.

### EG6: PASS

**Condition:** Protocol-specific outcomes not misleadingly equivalent.

**Evidence and reasoning:** Each matrix row now states the comparison boundary. P3-OUT-001, REC-006/007/008/011/014, OBS-003/004/005 and CB-001/002/006 are analogous roles, not equal wire events. CONT-005/009 and ERR-010/FB-009 retain different predicates. ERR-017 is identical only for completion-versus-delivery validity; exact collection failure is not D01 or a QUIC requirement. D01, syntax failure, protection failure, missing coverage, resource loss and verdict remain separate.

### EG7: PASS

**Condition:** Required STOP and state-feedback points supported.

**Evidence and reasoning:** OP-09, FB-A..F and S6-05/12/29..32 retain within-list Stop, current-unit work on zero accumulated mask, suppression on nonzero mask, combined Stop/Error and updated state before the next unit. Terminal candidate reset and isolated active candidate acceptance are distinct. No precommit, replay or automatic network action is introduced.

### EG8: PASS

**Condition:** No type/event/API solely for symmetry or speculative extensibility.

**Evidence and reasoning:** Section 9 ties each semantic concept to an actual need. OP/L labels are review concepts, not methods/classes. No fabricated QUIC Record, collection/batch API, generation field, detector, concealment interface or future-protocol framework is required.

### Phase 4 handoff decision

**READY FOR PHASE 4. EG1-EG8 PASS at the static semantic-contract boundary.** No contract-changing unknown remains under the explicit participation, immutable-result/borrow and preservation decisions. Phase 4 can define initialization, traversal ownership, observer order and state commits while preserving the current TCP behavior and existing fingerprint algorithm.

Handoff constraints: feature-mask participation and allocation guards; same-CH fingerprint equality with legacy output preservation; current-unit Stop/mask/state feedback; candidate reset-before-parse where Phase 2 requires it; no artificial Records; all 14 lifetime families; same-current-CH full-string backing through nested consumer last use; detector pointers/temporary projections do not extend lifetime; existing packet/session side effects and discarded Action boundary are preserved. Concrete classes, copying strategy, pools and full independent-evidence order remain Phase 4 choices, not prerequisites left undefined here.

Archival matrix V2 byte parity remains unverified; the independently reviewed consolidated matrix is the semantic handoff evidence. This is not a claim of exact historical recovery, runtime PASS or legacy defect repair. No requirement to change TCP is created by this handoff. Any separately approved bug correction is reviewed outside this preserved task scope.

## 11. Review procedure and preservation

Historical V1 procedure (retained as historical evidence, not a new execution claim): complete consolidation of 131 inventory IDs and 131 derived matrix rows; all ten OP definitions; all 14 L families; all 36 focused cases; D01 integration; explicit gate-by-gate review and unresolved impact register; source digest/line checks and baseline preservation. No compilation/runtime/production reachability or external consumer test was run.

Mechanical checks verify presence, parity and integrity, not semantic truth. Two identical-hash structural runs are not two independent full semantic PASS runs. Gate verdicts require semantic evidence, not structural PASS alone. Original inputs are unchanged. Historical Step 4/5/6 readiness prose and placeholder hashes are not promoted into current authority.

### Input evidence manifest

- `inventory-V5.md`: SHA-256 `fec227b881dad023964a3729ceb42f7a445043aa1d128e3f452affd1721791dc`.
- `inventory-V5.json`: SHA-256 `04666988af239c3def8c856552776849448993a39540559fad78f001eba07e89`.
- `phase3-step3-semantic-equivalence-matrix-v1.json`: SHA-256 `1d6db2a4297d0d669ba19931e5c5ed5221014f934d83944a9faf1fff2143ebc6`.
- `phase3-step4-semantic-processing-contract-v2.md`: SHA-256 `b1bc4d9d133a2375de712eac3baebd3d82359c2f923ef8b7cf36167c1ad184ce`.
- `phase3-step5-freshness-ownership-lifetime-v2.md`: SHA-256 `e38c42b1f5c2efb0eef643841cea91eaadab54477b0431a78e1dcb7be287c817`.
- `phase3-step6-focused-contract-walkthrough-v2.md`: SHA-256 `9f87ede88673f4abc85a7e368a129d486b5fe08fe3b530871eae4325730f5f2e`.
- `phase2-state-machine.md`: SHA-256 `b2d0ae9254ed6e96c643281bfc09302dd8c8360b9672ff16611055820f02de77`.
- `phase1-copilot-report.md`: SHA-256 `9c25e615a26c28f65ddc3a26ee61c9e18ce90212c9ee0f9ca0b0506c8b840a69`.
- `tls-flow-quic-pre-design-work-plan-v5.md`: SHA-256 `55ea1536ac4913e3a4ce59cc717fc3f5e8752dcdddf51be66bed34b280c024fa`.
- `TLS-Flow-All-Code.txt`: SHA-256 `70f6f63926a6f9426033aec6903951cc804b2225b153d20d3a3b8e13d2808f5b`.

The supplied matrix V2 text/index is now used as described in Section 3.1. The local placeholder cannot certify exact original bytes; no exact full-file V2 hash is asserted. Historical source digests below were reported verified by V1 against its selected code snapshot; line anchors appear in Section 2.

- `common/include/TLSAlert.hpp`: SHA-256 `af9c4fa72b9344ac92ee6ae22db2f86c5fbe4f4e92d8e0f4614bcfc1d55c7058`.
- `common/include/TLSData.hpp`: SHA-256 `eb0142e48a7a04307604479e71c844ce9eada6d29b7861ee1d49cbc94cb96e1e`.
- `context/engine/src/Factory.cpp`: SHA-256 `6af50d621c8df6b47c06f64567aa01f9d86a7d4e955f3f6c2b820c32e2f9065e`.
- `extension/include/TLSSessionExtension.hpp`: SHA-256 `db64040052aad7c4f45085aeca2f76f4efeacf6d2b34a08a3387202a840d3452`.
- `extension/include/TLSSessionExtensionTable.hpp`: SHA-256 `c3f21dd13e1eaaedf677fb47108190e58d1a728d3acc7cff4b5c559386502c97`.
- `packet-flow/include/ClientHelloData.hpp`: SHA-256 `45646a5980a73892ab5903fc3d81342976b287703074fa46255e0e72ec046b3f`.
- `packet-flow/include/ITlsListener.hpp`: SHA-256 `4f05ee3a61666d05448c8bc3577034f89508b9b9a9f553f556b227d0a55150f8`.
- `packet-flow/include/TLSFeatureVerdict.hpp`: SHA-256 `743e9aa78ab2077fc800d1ad2c737d6a9900f3b9424a8ffa8b6a403321a7ea8c`.
- `packet-flow/include/TLSRecordContext.hpp`: SHA-256 `20a20fe84358f74c2209508099a162e44d89c66725de58f63012ea650d2bbddc`.
- `packet-flow/src/TLSFlowConductor.cpp`: SHA-256 `b5e4281bb1cc0e2109ab077a92e792ce90bdbffdbdf144bbc8b18259282f15ff`.
- `packet-flow/src/TLSFlowConductor.hpp`: SHA-256 `5820f7b6362b63b05cc4a00012e8ab7fd7bb632b567fca19a67a24416f3eeebf`.
- `processor/include/Definitions.hpp`: SHA-256 `69ae45a95914e3feb05f384097f11a67a34157e0c841da3a07cb825e640b18bf`.
- `processor/include/IClientHelloDataViewer.hpp`: SHA-256 `65bc73a1bdab3660e87405e66d2fdcbacaa221732ce1889c70f9e0ad695bf152`.
- `processor/include/IParsingErrorViewer.hpp`: SHA-256 `2dd454b1d9b37b35b460579b8b3c4a7724a062aab9d8d845eb186d7f5baf2ac1`.
- `processor/include/IProcessor.hpp`: SHA-256 `1cd56e2f5bbb9920e175c98b44414c679c6e7cba81ac1e275b98a726a0cd3a1f`.
- `processor/include/IRecordsDataViewer.hpp`: SHA-256 `dd028864167a3a8672a0d48915370250fea74e0da915a3c9b2fb0fb86c9a2876`.
- `processor/include/ParsingError.hpp`: SHA-256 `88b2ccc6e7190ba1c7aacff0211ef0de6de539368001cac1a92ef5df5d9bd777`.
- `processor/include/PayloadParams.hpp`: SHA-256 `6ec6c6543637d8eb8ae5bb75d74974fbdefd03463e869d20c4f9d177f586b17f`.
- `processor/include/Record.hpp`: SHA-256 `01648651771d7bf71429ac8d1360a80d7b835bcd176c4818e5d7a099571be17c`.
- `processor/src/ExtensionsSorting.hpp`: SHA-256 `12998c6f4ebffafa442f96224477c2231723fc8e7d0ecaf7c38b6b52007e90c2`.
- `processor/src/ITLSParser.hpp`: SHA-256 `a31d1b6811d2999849795b686d1865da2f375c38c904c9d459638552470ebc9f`.
- `processor/src/Md5Digest.cpp`: SHA-256 `41307878d7530d8a8ee85a94f0804bcb76dda6fb1ba81601062dd0f09b6be3df`.
- `processor/src/Md5Digest.hpp`: SHA-256 `087752e60bdb578f5984a14ebfa9fe07dde40fc2a7d71525f2393f92d8f5e656`.
- `processor/src/ParsingStatus.hpp`: SHA-256 `6e9de4d55c4a0cf0f7c9b3ba609966c73d819d3425309d2c538fb3eae2e3cf9f`.
- `processor/src/Processor.cpp`: SHA-256 `c27450c00cfc946647ca81f966fb8c0bd47e876965ff5b7fd3ed92b3b51b8750`.
- `processor/src/Processor.hpp`: SHA-256 `3ac11ef4c4f7db0994c4350b734b007b14ad76cb04bae41546007dea84b83759`.
- `processor/src/SegmentBuffer.hpp`: SHA-256 `7a84d8f244a9741bc8c3bd965c7d6e3b1b959d35aefa68558a9abed3df86a351`.
- `processor/src/SegmentBufferManager.cpp`: SHA-256 `08112c60073c4b935fbdc77db365c327d07081864cc3ff2e8619e8d5e160c20a`.
- `processor/src/SegmentBufferManager.hpp`: SHA-256 `a5467690662f8a5dd3b095bcbc24d181f5ac3d7ca7b114337d9101638a52319a`.

## 12. V2 review delta and preservation

This review independently reassesses all 131 consolidated row meanings against the full V5 inventory, the supplied V2 index/available expanded text, and the OP/L/feedback contract. It does not certify full expanded-V2 retrieval or exact byte parity. Original inputs remain unchanged.

Corrections: explicit per-row comparison boundary; acquisition/local-operation cost separation; ERR-017 generic Identical versus exact-mechanism analogy; REC-008 no implied QUIC collection; OUT-007 None versus Partial wording; cardinality list formatting; stale matrix-missing gate rationale replaced with substantive workplan evidence. Sections 4..9 retain their semantic policy and scenario contents. No Phase 2 policy, algorithm repair, registration path or concrete design is introduced.

The accompanying gate review contains forward/reverse traceability and preservation checks. Structural checks are not independent semantic PASS runs. No runtime tests were performed.

## V3 publication evidence

- `phase3-user-consumer-evidence-v1.md`: SHA-256 `bdbeeb2fbc92b171aed9ef219b3071240854e7381aa69881e185cebacb1c0efd`. Selected exact current-message excerpts, not complete consumer files.
- `phase3-semantic-processing-contract-review-v2.md`: SHA-256 `809fe0efee355ce5defb668bfbb2b0b2cd0f7b05e6122df4b64b157f78099683`. Immutable prior snapshot.

V3 review changes: U02-U05 closed by user decisions/evidence; four matrix facets updated; concrete consumer last-use added at OP/L boundaries; no current-code mutation; all prior inventory facts and case bodies retained. No archival V2 matrix hash is invented.

## Final consistency closure

V4 retains the complete V3 semantic content and evidence, and removes three stale statements that still described closed U04/U05 as open. V3 remains a preserved prepublication snapshot. Final all-gate review is against V4 bytes, not the earlier snapshot. No source code or accepted Phase 2 policy was changed.

## V5 Phase 3 exit-gate recheck

The review uses the eight exact Phase 3 conditions, not Phase 4 runtime-flow exit conditions. V4 contained stale prose calling closed TCP compatibility unresolved; V5 aligns those statements with the already accepted U05 decision without changing policy, code, registration, algorithm or consumer APIs. All original inventory facts, row IDs, transition rows and focused case bodies are retained. L05's evidence boundary now reflects the accepted U04/USER-FP-01 decisions rather than calling them unapproved.

**Decision: EG1-EG8 PASS at the semantic-contract boundary; Phase 4 may start.** This is not completion of Phase 4, full downstream compliance, runtime validation or exact historical matrix recovery. Source/consumer constraints are explicit; implementation must obey them rather than infer post-callback ownership from const references. Remaining concrete dispatch/commit/storage choices are the intended Phase 4 work.

P3-ERR-008/P3-ERR-011 remain Unresolved only as declared-only legacy names with no demonstrated producer. Actual malformed-CH/length-failure meaning is defined by OP-06 and the outcome contract. No architecture, state or required behavior depends on choosing those enum names. U07 therefore has no contract-blocking unknown. U06 independent-evidence ordering and U08 concrete backing/capacity choices have defined semantic constraints and are not silently assumed implemented.

Review evidence is recorded in phase3-exit-gate-recheck-v5.md. Repeated structural/source checks are not independent full semantic PASS runs. V4 and all other input artifacts are preserved unchanged.
