# O04: Existing Viewers and Readers Against the Selected CH Projection V2

## V2 applied corrections and authorized progression

These are corrections to the design documents, not patches to production code. V1 and all source snapshots are preserved. The user explicitly requested corrections followed by O05; this authorizes the next design analysis despite the documented reader-evidence gap. It does not make G01 PASS or waive compatibility before integration.

- **C04-01: publication versus selection.** The existing viewer objects remain. A candidate may be selected for synchronous inspection without becoming the active attempt. Current viewer selection, committed TLSData and backing ownership are three distinct facts. After candidate rejection, the selected candidate view must be invalidated or restored to the authoritative context before a later pull. No new public viewer or mandatory binding API is selected.
- **C04-02: complete field coherence.** Status, FP, digest/FullString, SNI presence/value, version/mask, early-data, counts, error and record projection must be selected/restored together for the same session/attempt/message. Missing fields have explicit absence/default semantics, not previous-session values. This is an O05 design obligation, not a claim the current code already restores every field.
- **C04-03: final use.** No backing is released/reused at callback return alone. Applicable callbacks, actual-verdict reduction, permitted commit and every final internal read finish first. Input cleanup does not discard live continuation. A shared_ptr to a viewer is not ownership of borrowed digest bytes or record references.
- **C04-04: exceptional visibility.** Complete status does not authorize successful CH publication after structural/resource failure. Existing TCP D04 error/step/record-list distinctions remain; no blanket list clearing or parser-created violation is introduced.
- **C04-05: projection preserved.** C-O03-01 remains unchanged: no intermediate publication for contributing cross-record CH records; one completion projection, then the feedback barrier. Cross-record recordLength remains final contributing record payload length, not CH size. QUIC does not acquire a synthetic TLSRecord.

G01 cannot be fixed by prose: the separate consumer source remains unavailable after local inspection and retrieval attempts. No external reader compatibility or retention behavior is invented. O05 may resolve the logical ownership/disposition design now; G01 remains a pre-integration evidence obligation. FRD/SRS source retrieval also failed in this turn; QUIC alignment below is limited to available O01-O03/Consolidated V6 and explicitly recorded accepted constraints, not a fresh complete FRD/SRS audit.


**Status: corrected design handoff to O05 under the current explicit user instruction. Full reader-compatibility closure remains BLOCKED at G01. No migration, implementation or runtime PASS.**

The publication conflict was coordinated first in Consolidated V6 and O03 V7; O03's focused progression review was restarted before this O04 analysis. This document checks existing viewer contracts, visible readers and lifetime obligations against that selected projection. It does not choose inheritance/composition, reopen O02, replace viewers, add public APIs or select physical ownership.

## 1. Sources and evidence boundary

Direct inputs: O03 V6, Consolidated V5 and TLS-Flow-All-Code.txt; unchanged O01 V4 and O02 V2 were consulted specifically for cross-record feedback wording. Coordinated working inputs: O03 V7 and Consolidated V6. Hashes are in the delivery manifest. Code anchors below are aggregate physical line numbers.

CODE FACT means a visible declaration, getter or caller. DESIGN REQUIREMENT means the proposed migration must satisfy the selected contract; it is not an implemented behavior. INHERITED EVIDENCE is not independently rerun. PLANNED VALIDATION is NOT RUN.

The separate TLS-Consumers code.txt mentioned in prior material is not present in the current filesystem. Retrieval of its surfaced reference failed. Its contents, readers and digest are not inferred from earlier summaries. This is a concrete evidence gap, not proof that a reader is incompatible. The available aggregate contains Application, Conductor, Processor, interfaces and supporting types; external enforcement/visibility listener implementations cannot be claimed covered by this review. A public interface declaration is not a list of all production callers.

## 2. Projection under review

- Pending cross-record CH: no intermediate TLSRecord/CH callback or normal commit. Eligible record payloads continue acquisition; headers are validated and excluded. Genuine exhaustion yields waiting, not forced waiting at every record boundary.
- Completion: one handshake/client_hello projection; cross-record recordLength is final contributing actual record payload length. Single-record length semantics are unchanged. This is not total CH length, total wire length or a packet-relative locator.
- CH Complete requires successful parsing and FP completion. A structural/resource failure is not successful CH publication. No unrelated record becomes CH input.
- Following separately processed records retain existing callback eligibility, including retained TCP Complete. No fresh CH parse/FP or CH counter increment is inferred solely from repeated eligibility.
- After yield, all applicable callbacks, actual-verdict reduction, permitted commit and every final internal read finish before next-unit processing or backing reuse. Pending continuation survives transient input cleanup.

## 3. Actual reader inventory and compatibility

### 3.1 Interfaces and implementations

CODE FACT: IClientHelloDataViewer at L4266 exposes CH status, fingerprint, SNI, version/mask, early-data, FullString and counts. IParsingErrorViewer begins L4393; IRecordsDataViewer L4489 exposes the record collection, last record, type and uint16 length. Record at L4676 carries record/handshake type, uint16 payload length and alert metadata; isClientHello tests the type pair, not physical presence of a CH header in that record. Processor getters at L6930-L7020 mostly return canonical members directly. There is no automatic status/eligibility guard in those getters.

| Reader | Visible read and use | Assessment against selected projection |
| --- | --- | --- |
| Application viewer getters | Return existing shared viewer handles, e.g. getClientHelloDataViewer at L1445 | Preserve handle/interface contract; handle lifetime alone does not pin current CH backing or session selection |
| Conductor analyzeProcessResult L3514 | Last record; CH mask/early-data if isClientHello; alert metadata for readable alert | Completing projection is structurally expressible. It must carry the correct complete CH metadata; no intermediate unknown-field event is introduced |
| Conductor CH eligibility L3557-L3562 | Complete status triggers onClientHello, not a freshness test | Preserve retained-Complete behavior for later TCP units; do not apply QUIC duplicate suppression to TCP callbacks |
| Conductor normal commit L3576-L3584 | Last record selects CH/CCS/Finished/alert update | One completion projection can select CH commit. Pending acquisition must not reach this branch as a successful record |
| Conductor updateTLSDataOnClientHello L3605-L3625 | Complete guard, FP copy, version mask, early-data and session timestamp | Full CH/FP must be available after callbacks; final-record length is not read in this commit path. Completing packet timestamp is retained for TCP |
| Conductor parsing-error path | getLastError determines anomaly callback; actual returned verdict controls state | Error viewer must describe this exceptional step, not a previous session's error; no fabricated violation or successful CH callback |
| External listener / record-list readers | Implementations unavailable in this review | G01: cannot verify assumptions about recordLength, physical header placement, number of record callbacks, FullString retention or failed-step reads |

**Local finding:** no incompatible use of the final-record-length convention was found in the inspected Conductor paths. That is narrower than saying all Record readers are compatible. In particular, no visible Conductor length arithmetic proves that an external reader does not interpret recordLength as total CH extent.

### 3.2 Getter-by-getter obligations

| Getter/data | CODE FACT | DESIGN REQUIREMENT / remaining proof |
| --- | --- | --- |
| getLastRecord / type / length | L6989-L7009 return canonical record/vector members | All getters for one published unit must agree; no prior-session record or pending fragment may masquerade as completion |
| getAllRecords | Returns const reference to m_records | Preserve the existing collection scope; it is not automatically a per-unit singleton or complete physical wire-record log. Cross-record acquisition adds no intermediate entries solely to simulate callbacks. Preserve prior published entries and D04 failure-side effects unless separately dispositioned |
| CH parsing status | L7013 returns m_clientHelloParsingStatus | Status alone does not establish current-step success or freshness; exceptional outcome can override Complete publication eligibility |
| Fingerprint | L6945 copies m_fp | Read only when eligible; copy must be finalized for this CH. No second hash/finalize on retained Complete |
| SNI | L6950 copies only if m_bSniExtensionParsed, otherwise false | Caller must honor bool. Reset/restore flag and value together; no stale SNI when absent in resumed CH |
| Version/mask | L6930-L6937 return lastVersion/mask | Restore correct context including no-supported_versions fallback; no parser-wide last-session fallback |
| Early-data | L6940 returns flag | False/absent and restored true belong to selected CH, not preceding packet/session |
| Counts | L6966-L6978 expose canonical counts | Must be coherent with selected CH and not doubled by resumed input; do not assume unknown partial counts are complete |
| FullString | L6961 returns digest array pointer and length | Return-by-value wrapper is not ownership of bytes. Nested synchronous reads need the same digest/context and stable storage through full last use |
| Error | L7018 returns m_lastError | Failed-step diagnostics require explicit routing/clearing rules; no assumption every resumed beginPacket resets all fields |

## 4. Lifetime, routing and interleaving review

The current Processor is shared mutable execution machinery; shared_ptr to its viewer interface does not make a returned pointer/reference an immutable snapshot. A caller retaining FullStringData, Record reference or the Records collection across another pull/session selection can observe reused storage. This review does not invent a new permission to retain such borrows asynchronously. Actual consumer retention behavior is part of G01.

Required validity intervals:

1. **Pending acquisition:** CH checkpoint, collected fields and backing persist across packets/records without exposing a new successful unit. O05 specifies actual ownership; Engine scratch is not persistent CH state.
2. **Published completion:** record, CH fields, digest and any locators stay associated with the same CH through nested callbacks, reduction, permitted commit and every final internal read. The next pull must see committed TLSData, not a cached packet-start reference.
3. **Exceptional step:** diagnostic and any legitimately exposed failed-step record-list entries remain coherent for their actual readers. Do not infer success from a leftover Complete status or nonempty list. D04 failure mapping is not erased.
4. **After final use:** existing lifecycle may invalidate transient views, but not retained continuation still needed by the active logical CH. No parallel reclaim, dynamic growth or heap fallback is introduced.

### A/B/A source concern

O03 T20/section 6.1 identify Processor-wide parser/viewer fields and resumed-entry paths that bypass ordinary reset. The segmented completion path L5780-L5857 restores some saved fields and uses the parser version fallback when supported_versions was not parsed. This is not a proof that every CH field is restored, nor a reproduced corruption. The required migration contract is field-complete selection/restoration for A after B: status, FP/digest, SNI presence/value, version/mask, early-data, counts, record projection, diagnostic and all borrowed backing. O05 must assign concrete storage/ownership and O06 its bounds. Do not preserve accidental stale values as compatibility behavior.

## 5. Static scenario review

These are reasoning checks, not executed packet tests or proof of future implementation.

| ID | Scenario | Required observation / review outcome |
| --- | --- | --- |
| S01 | Existing CH completed in one record or existing extension segmentation | Same single-record length and existing callback/commit timing; no architecture change required |
| S02 | Record A ends inside handshake header; B continues | No unknown-handshake placeholder event. One final projection; isClientHello describes completion semantics, not B's first physical byte |
| S03 | CH spans several records, final record payload shorter than whole CH | Conductor does not use length as whole-CH extent; external length/offset assumptions remain G01 |
| S04 | Complete CH then CCS/alert/other separate record | Full completion feedback/commit precedes next record; retained-Complete CH callback eligibility preserved without a second CH commit on non-CH |
| S05 | Zero-mask Stop at completion, later same-unit Continue with bits | Preserve accumulated verdict reduction; no next pull; backing remains valid until all permitted same-unit uses end |
| S06 | Malformed next record or unrelated-record interruption while collecting | Applicable diagnostic/Stop or Unsupported route, no successful completion projection and no fabricated violation |
| S07 | A partial, B complete, A resumed without SNI/supported_versions | Correct A fields required; restoration proof remains O05, not assumed from a few saved scalars |
| S08 | Nested FullString read during callback and commit afterward | Same CH/digest backing survives both; no release on callback return alone |
| S09 | Digest/locator/record capacity failure | D04 site-specific error/step/list semantics; no generic successful CH publication, no retry of partially changed digest |
| S10 | No extensions / explicit empty block | Preserve O03 D03 bounded accounting disposition; viewer availability does not prove legacy parser boundary correctness |
| S11 | Failed begin / end input / another session selected | No stale selected result; pending continuation survives cleanup. Actual routing/lifetime proof is not supplied by interface signatures |
| S12 | QUIC selected path | No fake TLSRecord, no TCP length convention exported as QUIC wire fact; shared CH getters need qualified eligibility and selected backing. No QUIC implementation proof claimed |

## 6. Handoff register and first closure blocker

| ID | Question | Status / required evidence |
| --- | --- | --- |
| G01 | Are all actual external readers compatible with final-record length, deferred callbacks and current-view lifetime? | **BLOCKED: external consumer source unavailable.** Need exact implementations/call sites and retention behavior. No incompatible reader has been demonstrated, but absence cannot be inferred |
| G02 | Are all canonical fields and backing correctly selected/restored across A/B/A? | OPEN O05 dependency; enumerate storage and each field, not a new owner API by default |
| G03 | Do error/record-list readers match D04 at each failure site? | OPEN; external coverage G01 plus concrete failure mapping. No generic clearing policy selected |
| G04 | Are no-extension/empty-block bounds and widths safe? | OPEN O03 D03/O06 boundary work; not solved by projection choice |
| G05 | Is current-unit lifetime and no-allocation runtime implemented? | OPEN O05-O07; source design is not execution evidence |

Ordered O04 closure review stops at G01. Later rows are inventoried dependencies, not passed gates. The current explicit user instruction authorizes O05 design work with G01 carried forward. It does not authorize consumer integration or implementation acceptance. Resolving G01 means inspecting the exact missing readers, not requiring a benchmark or reopening architecture. If a concrete reader requires total CH size or a physical CH header at the last record, revise the projection or that reader's explicitly approved behavior in a new version; do not silently reinterpret recordLength globally.

## 7. Double-check before delivery

First pass: source declarations/getters and visible Conductor reads were compared with O03 D02. Second pass: callback freshness versus eligibility, post-callback commit, FullString borrow, record-list versus dispatched-event distinction, failed-step routing, A/B/A and the narrow coordination exception were checked again. No claim of full consumer coverage is made. Mechanical occurrence inventory below is not a call-graph or semantic proof.

The coordinated O03 design-progression conflict is resolved. O04 review work is delivered with G01 explicit; **O04 full compatibility closure is not passed**. No build, packet execution, fault injection, runtime allocation audit or performance measurement was performed. Official HLD V10, architecture selection and input snapshots remain unchanged.

## 8. Getter occurrence index in supplied code

Occurrences include declarations, definitions and calls, not only readers. Line numbers allow reproduction without inventing a complete external call graph.

- `getAllRecords`: L4505, L6989, L7100.
- `getLastRecord`: L3540, L4507, L6994, L7101.
- `getRecordType`: L4508, L4961, L6017, L6194, L7002, L7102, L8087.
- `getRecordLength`: L4509, L7007, L7103.
- `getClientHelloParsingStatus`: L3559, L3824, L3879, L4375, L7013, L7095.
- `getFingerprint`: L3071, L3616, L3831, L4301, L6945, L7089.
- `getSni`: L3072, L3614, L4309, L6950, L7086.
- `getSupportedVersionBitMask`: L3543, L3832, L4317, L6935, L7087.
- `isEarlyDataOffered`: L3544, L4322, L6940, L7088.
- `getFullString`: L4327, L6961, L7090.
- `getTlsVersion`: L3073, L3620, L4336, L4957, L6006, L6281, L6923, L6930, L7085, L8083.
- `getCipherCount`: L3621, L4345, L6966, L7091.
- `getExtensionCount`: L3622, L4355, L6971, L7092.
- `getCompressionCount`: L3623, L4364, L6976, L7093.
- `getLastError`: L3589, L4409, L7018, L7108.
