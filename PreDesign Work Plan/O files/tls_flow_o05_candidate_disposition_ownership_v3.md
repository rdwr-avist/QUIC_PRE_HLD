# O05: Candidate Disposition, Ownership and Continuation Handoff V3

**Status: design analysis delivered with explicit evidence gaps. Logical handoff specified; final physical attempt/lifecycle API and full requirements-alignment acceptance are NOT closed. No migration, implementation or runtime PASS.**

O04 V2 applies the focused viewer/selection/lifetime corrections first. The user's current instruction then authorizes this O05 design work while O04 G01 remains open. O05 answers who decides candidate disposition, what information must cross that boundary, which data survives it and when backing can be released. It does not reopen O02 architecture or create one class per responsibility.

## 1. Authority and honest scope

### 1.1 Available sources

- TLS-Flow-All-Code.txt: current supplied TCP implementation; physical aggregate line numbers used below.
- O01 V4: shared CH progress, accepted-byte frontier, one-time digest contribution and full-last-use constraints.
- O02 V2: orchestration comparison; B/B1 are proposals, B2 remains viable. No architecture is newly accepted here.
- O03 V7 and Consolidated V6: selected working cross-record projection, D01-D04 distinctions, CP01-CP09, terminal reset-before-CH versus active isolated evaluation, explicit disposition need at CP07.
- O04 V2: existing viewer interfaces and available readers; external consumer coverage remains G01.
- Accepted constraints explicitly recorded in the conversation: QUIC v1/client-to-server/Initial-only visibility, one active TLSData, attempt continuity/replacement/suppression rules, D-TIME, verdict authority and startup-only allocation.

Hashes of available files are in the delivery manifest. Source files are not overwritten. Official HLD V10 remains unchanged; later HLD filenames and interfaces V5 are not silently promoted to authority.

### 1.2 Missing primary evidence

Quic_FRD.md.txt, Quic_SRS.md.txt and TLS-Consumers code.txt are referenced in prior material but are not present locally. Named-file fetch returned no usable content; opening their surfaced reference IDs also failed. Therefore this delivery cannot claim a fresh complete comparison against FRD/SRS, HLD V10 or all consumers. The available consolidated contracts and recorded decisions are used explicitly as inherited authority, not as replacement copies of those files. No requirement IDs, line numbers, digests or source text are fabricated for unavailable documents.

This is a design task, not an implementation task. No source-code changes, builds, packet tests, allocation audit or performance measurements were performed. CODE FACT, INHERITED CONTRACT, DESIGN PROPOSAL, PLANNED VALIDATION and EVIDENCE GAP are distinguished below.

## 2. Current code: what exists and what does not follow from it

| Evidence | CODE FACT | O05 implication |
| --- | --- | --- |
| TLSData, L421 onward | Semantic handshake state, version mask, CH/early-data counters, FP, terminal mask and timestamp | Semantic fields do not encode accepted/rejected/deferred acquisition ownership. Do not infer admission from equality of these fields |
| Conductor process/analyze/commit paths, L3514-L3625 and L3788-L3900 | Conductor dispatches callbacks and commits semantic updates using actual verdict reduction | Keep semantic authority here; shared CH parsing must not reset active TLSData or manufacture violations |
| Processor begin/resume/segmented completion, L5585-L5857 | Packet traversal and session-associated segment access coexist with Processor-wide parser/viewer state | A/B/A restoration must cover all fields/backing, not just sequence/length or digest |
| Viewer getters, L6930-L7020 | Canonical members exposed directly; FullString points into digest storage; record list is borrowed | Selecting a viewer context is not transferring ownership or committing the candidate |
| SegmentBuffer, L7625 onward | TCP continuation state and record/extension bookkeeping exist | Reuse logical TCP responsibilities; current layout is not proof of arbitrary-byte CH or QUIC CRYPTO support |
| SegmentBufferManager allocate, L7797-L7844 | Session-keyed FIFO map; full map chooses oldest, removes its aging observer and pops it before insertion; callback receives entry | Existing lifecycle path is relevant. It has no demonstrated QUIC candidate/published-view protection or semantic victim disposition |
| SegmentBufferManager deallocate/access, L7846-L7880 | Erase/unregister and synchronous entry access | A callback-scoped reference is not a durable owner. Entry lookup must be revalidated after operations that can mutate/reclaim storage |
| SegmentBufferManager aging, L7882-L7893 | STM_EVENT_AGING erases by session index | Use existing aging integration, but do not assume it already handles QUIC attempt retirement, retired suppression or outstanding borrows |

The map/container internals and complete allocation paths are not proved allocation-free by these call sites. A method named allocate can mean slot acquisition rather than heap allocation, but that interpretation must be established from actual storage implementation. Current TCP FIFO behavior is a source fact, not automatic permission to apply it to new QUIC state.

## 3. Ownership split without architecture expansion

**INHERITED CONTRACT:** one active TLSData semantic context per DP Session. A candidate can have isolated acquisition/CH evaluation state; it is not a second active semantic session.

**DESIGN PROPOSAL:** preserve the existing division: session extension owns committed semantics, Conductor decides semantic disposition, protocol processing maintains association/acquisition, existing session-associated storage/lifecycle infrastructure supplies retained backing, and shared CH interpretation mutates only the selected CH work state. Exact QUIC storage integration is an evidence gap, not a new manager or approved public API.

| Data / responsibility | Logical authority / storage requirement | Permitted consumers | End of lifetime |
| --- | --- | --- | --- |
| Active TLSData | Existing session extension; Conductor is semantic writer | Processor reads current committed facts; callbacks under existing contract | Existing semantic lifecycle/replacement, not end of packet |
| Active attempt association | Protocol-specific retained association for this session | Classification, duplicate/retired suppression and Conductor disposition | Accepted attempt replacement or existing lifecycle disposition |
| Candidate acquisition | Isolated prepared backing associated with candidate/session | Protocol acquisition and shared CH evaluator | Rejection after last use, accepted transfer, or explicit lifecycle/resource disposition |
| CH checkpoint and accepted frontier | Retained with the selected logical CH | Shared CH interpretation only for eligible contiguous message bytes | Completion/failure retirement after last use; no refeeding already accepted bytes |
| CH facts, SNI, counts, FP and digest | Same selected CH context/backing; explicit absence flags | Existing viewers and synchronous consumers | After every final reader, or longer if required by existing retained-Complete semantics |
| TCP record projection/list and error | Existing Processor view with explicit current-unit association | Existing Conductor and actual readers | Existing scope, with D04 exceptional visibility preserved |
| QUIC datagram/decryption scratch | Engine prepared transient storage | Current admitted input processing | After its final read; no retained pointer from continuation |
| CRYPTO ranges and CH bytes | Prepared retained acquisition backing with logical offsets | QUIC reassembly; shared parser only sees eligible contiguous CH region | Retired/rejected/completed acquisition after all borrowers finish |
| Candidate decision | Conductor's actual disposition, bound to the evaluated candidate | Processing path that transfers/discards/retains acquisition | Applied once, or explicitly still pending; never inferred from TLSData alone |
| Retired-attempt suppression | Existing accepted bounded attempt/lifecycle responsibility | Association/suppression path | Accepted retirement lifecycle; not arbitrarily cleared by scratch cleanup |
| Completing-datagram arrival timestamp | Captured scalar attached to the completed QUIC CH | Commit and visibility | Commit copies value; deferred decision must not replace it with callback time |

Logical ownership does not require a class, interface, registry, heap object or new viewer. No CurrentProducerBindingAPI, SharedPolicyAPI, SupportBinding, CHInput/CHWorkspace or retireActualVictimBeforeMutation API is approved by this document.

## 4. Minimal disposition information

Consolidated V6 CP07 already requires more than current TLSData for candidate admission. O05 specifies the minimum logical information, not a C++ signature or mandatory enum:

1. Which session and logical candidate/message was evaluated, including its association with retained backing. Reused session/slot identity alone must not let a stale result target a new lifecycle.
2. Whether the decision is accepted, rejected or still deferred. These are ownership dispositions, not TLS-Enforcement verdicts.
3. Which attempt/context is authoritative afterward: same active attempt, same-attempt restart/CH2, newly admitted replacement, or no admitted candidate. This is not a new state-machine taxonomy.
4. Which acquisition/checkpoint backing is retained, transferred or retired; which views remain valid through last use.
5. Whether traversal of the current input may continue after reduction/commit. Admission does not override Stop; Stop does not imply rejection.
6. For successful QUIC CH commit, the completing-datagram arrival timestamp and finalized CH facts associated with that decision.

**DESIGN PROPOSAL, KISS:** prefer synchronous direct control flow in the existing Conductor/Processor collaboration. Keep the candidate's association and the actual disposition explicit through the current processing turn. Do not add a generic asynchronous acknowledgment bus or persistent decision registry. If a decision genuinely survives input end, retain the necessary association and candidate data in prepared session-associated storage; a stack local or packet pointer cannot carry it.

No concrete API is selected until physical ownership/reader gaps are resolved. A generation counter is not automatically required: use the existing lifecycle identity if it already prevents stale slot/session reuse. If it does not, a concrete identity fix must be designed and reviewed, not assumed away.

## 5. Handoff and mutation ordering

### 5.1 Ordinary active-attempt input

Validate association and restore its selected CH state. Acquire only eligible protocol bytes, respecting O01 accounting. If more input is genuinely needed, retain continuation and finish transient cleanup. On a publishable unit, finish all callbacks, reduction, permitted commit and final reads before another pull. Refresh committed TLSData and any affected association afterward. No candidate handshake is needed for ordinary continuation merely for symmetry.

### 5.2 Candidate against a live active attempt

**INHERITED CONTRACT:** isolated evaluation precedes admission. Do not reset or overwrite active semantics/backing merely because another Initial or CH-like input arrived.

**DESIGN PROPOSAL ordering:**

1. Establish eligible association and safe isolated storage before candidate interpretation. If no permitted prepared storage is available, do not evict the active attempt implicitly to make the candidate fit.
2. Parse/evaluate using candidate state. Keep active semantic facts and required backing intact. Shared parsing does not call listeners or decide session reuse.
3. Yield the candidate's actual eligible facts/findings to the existing Conductor policy. Synchronous viewer selection may expose candidate facts for inspection without admitting it.
4. Apply actual verdict reduction and applicable admission policy. A nonblocking applicability finding is not structural failure; structural completion alone is not acceptance.
5. Before replacement, ensure all storage/association changes needed to preserve the admitted result are feasible without runtime growth. Do not publish a new active identity and then discover its bytes were discarded. If the operation cannot complete safely, retain old authority and use the existing applicable resource/disposition route; no invented violation.
6. Apply the explicit disposition. Accepted replacement installs one authoritative semantic context and correct association, preserves required retired suppression, and transfers/retains backing coherently. Rejection discards only the candidate after its last use. Deferral retains isolated candidate state and leaves active authority unchanged.
7. Release old/candidate backing only after its own final use. Admission, commit, release and next-input permission are distinct decisions. Re-read authority before any permitted next pull.

This ordering is a logical safety requirement, not a demand for a transactional framework or a second active TLSData. Exact listener-versus-admission ordering within the inherited policy is not redefined by this document; actual policy sources must be checked before fixing a public API.

### 5.3 Terminal-context candidate

Consolidated V6 explicitly distinguishes terminal reset-before-CH from live-active isolated-evaluate-before-admit. Preserve that inherited distinction. Do not copy the live-active recipe into a generic blind reset, and do not postpone an accepted terminal reset policy merely to unify code. Reset must not invalidate still-borrowed old backing; terminal semantic reset is not permission to reuse bytes before last use. The exact terminal eligibility conditions remain the existing accepted policy, not a new list invented here.

### 5.4 Accepted / rejected / deferred outcomes

| Outcome | Active semantic authority | Candidate backing | Current view / traversal |
| --- | --- | --- | --- |
| Accepted same-attempt restart or CH2 | Same attempt under its accepted continuity policy; no automatic session reuse | Correct new CH acquisition selected; prior CH progress not merged blindly | Retain current facts through last use; resume only if allowed |
| Accepted complete new attempt | Replace active context as session reuse in same DP Session | Transfer/retain admitted result; retire old acquisition safely | Explicit new authority; old borrows finish before reuse |
| Rejected | Active context unchanged except separately authorized actual verdict effects | Release after candidate readers finish | Invalidate/restore candidate selection before later processing |
| Deferred | Active context unchanged | Retain bounded isolated data and pending association | No implicit acceptance; no unbounded candidate queue or packet borrow |
| Ambiguous association | No merge/reset/replacement | Preserve only what accepted ambiguity policy permits | Do not resolve ambiguity by choosing newest/oldest candidate |

Stop/Continue and violation mask are orthogonal to this table. A zero-mask Stop can coexist with permitted normal commit; later same-unit events follow the existing Conductor reduction. No next pull follows Stop. A nonzero actual verdict can affect semantic state according to the accepted contract, not because a candidate was rejected.

## 6. Complete A/B/A restoration checklist

O04 C04-02 is applied here as an explicit field obligation. On selecting A after B, restore or derive from A-owned retained state:

- Session/attempt/message association and protocol acquisition cursor/ranges.
- CH status, parser phase, declared extents, accepted frontier and already-contributed digest progress.
- Cipher/extension/compression counts, extension sorting/locator state and all referenced backing.
- SNI presence and value; absence must not return B's SNI.
- Legacy version and supported-version mask, including no-supported_versions fallback; do not consult B's last parser state.
- Early-data presence/value; FP and digest/FullString bytes and valid length.
- Current projection/list scope, error/exceptional outcome and successful-publication eligibility. Retained Complete is not freshness.
- Completing-datagram timestamp for QUIC, independent of decision/commit time.

This checklist does not require copying all fields on every packet. Fields may remain in their owning retained context and be selected directly. Derived fields may be recomputed only if safe, bounded and consistent with one-time FP contribution. No repeated parsing/hash merely to compensate for missing ownership.

For TCP, preserve existing supported segmentation callbacks and O03 C-O03-01 for the new cross-record path. For QUIC, use logical CRYPTO offsets; do not interpret datagram order as contiguous CH bytes. No TCP record headers enter shared CH input.

## 7. Lifecycle and existing reclaim integration

**Full last-use rule:** result/backing and every borrowed view remain valid through all applicable callbacks, actual-verdict reduction, permitted commit and every final internal read. Release, invalidation or reuse is allowed only after all those uses end, and only if no retained continuation or retained-view obligation still requires the backing.

**CODE FACT:** SegmentBufferManager's full-map path removes the oldest entry's aging observer and pops the entry; aging erases by session index. It does not visibly consult candidate disposition, published-view last use or QUIC retired suppression.

**DESIGN PROPOSAL:** reuse the existing lifecycle/reclaim integration rather than creating a parallel timer/reclaimer. Any adaptation must make the actual victim's logical acquisition unavailable before its storage is overwritten, without violating active readers. This is a correctness requirement, not approval of a named victim-retirement API. The shown TCP manager cannot simply be declared sufficient for QUIC.

- **Input end:** clear only transient packet/decryption cursors after final use; retain pending CH/candidate/association data.
- **CH completion:** release acquisition bytes only when no parser, locator, callback, commit or retained-view consumer still needs them. A copied FP alone does not prove FullString/locators safe.
- **Rejection:** retire candidate acquisition only; do not reset active semantic state as a cleanup side effect.
- **Accepted replacement:** preserve old suppression identity as required; invalidate old continuation association before slot reuse; refresh references after replacement.
- **Resource eviction:** invalidate the evicted acquisition coherently; do not generate IncompleteHandshake violation merely because storage was reclaimed. Preserve committed semantic facts unless an accepted lifecycle rule says otherwise.
- **Aging/session destruction:** existing session lifecycle removes retained associations/storage; any pending decision becomes unusable. A late result must not bind to a new session occupying the same index.
- **Published-view protection:** while last use is outstanding, the corresponding backing cannot be a reclaim victim. If the existing mechanism cannot enforce this, refuse/defer that storage mutation through the applicable bounded failure path; do not invent an unbounded pin queue or a heap fallback.

Single-threaded Engine execution does not prove safety against synchronous callback re-entry or helper allocation that reclaims the current slot. Verify actual call paths. No locks or background workers are added here.

## 8. QUIC alignment matrix

The following are inherited constraints from available consolidated material and recorded accepted decisions. They are not freshly verified FRD/SRS clauses. O05 concerns their ownership consequences, not a new QUIC protocol implementation.

| ID | Inherited constraint | O05 consequence |
| --- | --- | --- |
| Q01 | QUIC v1 only; configured QUIC Application Port group admission before crypto/CH work | Do not create durable candidate CH state for unrelated UDP merely because a port matched; port alone is not protocol proof |
| Q02 | Client-to-server visibility, Initial keys only | No ownership transition assumes visible server Retry/HRR/Finished/HANDSHAKE_DONE/transport parameters |
| Q03 | Protected fields need header-protection removal or trusted decoded classification | Do not use raw protected packet number/length/key-phase bits to select or retire an attempt |
| Q04 | Initial decryption, CRYPTO reassembly and shared CH/TLSFP | Retain eligible CRYPTO ranges only after successful Initial protection removal/decryption and integrity verification; this is not proof of peer identity; feed shared parser only contiguous logical CH bytes, without TCP framing or replayed digest contribution |
| Q05 | Retry-compatible restart and HRR-compatible CH2 remain same attempt | Reset only applicable acquisition/message state; do not assert observed Retry/HRR or create automatic session reuse |
| Q06 | Complete new attempt replaces active as reuse in same DP Session; one active TLSData | Candidate isolation is not a second active semantic context; replacement requires explicit disposition |
| Q07 | Retired suppression; ambiguity without merge/reset | Old traffic cannot resurrect retired acquisition; ambiguous bytes cannot contaminate active or candidate state |
| Q08 | 0-RTT does not change state; completion policy selected in O08 V3 | After successful completion/publication and permitted commit of the active attempt's CH acquired from eligible Initial CRYPTO data, the attempt can be InProgress. Only a subsequently observed, structurally qualifying, associated client Handshake packet can then move InProgress -> Finished, including ACK-only. A Handshake observed before complete authoritative CH does not establish InProgress or Finished and is not latched for later completion. No Handshake payload decryption or TLS Finished detection is required. Finished is the accepted product heuristic, not cryptographic handshake verification. Candidate acceptance, CH completion and storage release alone do not produce Finished. |
| Q09 | D-TIME: timestamp of arrival of datagram completing full CH | Capture scalar on completion, preserve across deferred decision; no first-byte or callback-time substitution |
| Q10 | Processor/TLS-Flow do not define violations; TLS-Enforcement owns them | Structural failure stops affected parsing; nonblocking no-TLS1.3 finding still permits full parse/FP; Error requires actual appropriate verdict. Do not change TCP policy automatically |
| Q11 | QUIC mitigation Initial with CONNECTION_CLOSE/CONNECTION_REFUSED; counters/visibility remain required | Candidate cleanup is not a mitigation verdict. Required mitigation/reporting facts must be consumed/copied before backing reuse; exact readers are not verified here |
| Q12 | Startup allocation only; prepared finite runtime storage, no growth/heap fallback | Active/candidate/retired/published-view joint peak belongs to O06; crypto and failure paths belong to O07; no numeric capacity invented |

Q11 does not prescribe a new mitigation component or expand O05 into mitigation design. The primary FRD/SRS audit remains an evidence gap; this matrix prevents known inherited requirements from being lost during ownership work.

## 9. Static walkthroughs and planned validation

These are design checks, not executed tests. Each implementation test must record source revision, input bytes/segmentation, initial authority, listener script, storage identity and actual trace.

| ID | Scenario | Required invariant / design result |
| --- | --- | --- |
| W01 | A partial, B complete, A resumes | All section 6 fields and backing belong to A; no parser-version/SNI fallback from B |
| W02 | Active A, candidate B accepted with TLSData fields coincidentally equal | Explicit disposition still transfers correct association/backing; equality is not admission |
| W03 | Same starting state, B rejected | A unchanged by candidate cleanup; B's views invalidated after last use |
| W04 | B deferred past datagram end | B survives in prepared retained storage, no packet/scratch pointer; timestamp preserved |
| W05 | Same-attempt restart or CH2 | Correct new message checkpoint, same attempt, no automatic reuse; no proof of observed server signal |
| W06 | Complete new attempt, late old Initial | One active context, retired suppression prevents resurrection |
| W07 | Ambiguous candidate association | No merge/reset or guessed authority |
| W08 | Stop without bits at the TCP record event; later eligible CH event effects | Existing verdict reduction and permitted commit retained; no next pull; all backing survives final reads |
| W09 | FullString nested read, then commit | Digest backing not released on first callback return |
| W10 | Allocation/reclaim would select currently borrowed slot | No overwrite; applicable bounded failure/defer route; no heap fallback |
| W11 | Candidate rejection/resource eviction/aging | Correct affected acquisition retired, no fabricated IncompleteHandshake violation; session reuse cannot consume stale decision |
| W12 | Terminal candidate versus live active candidate | Preserve distinct reset/admission ordering; no generic blind reset |
| W13 | Cross-record TCP CH, then separate record | One selected completion projection, barrier before next record; existing retained-Complete callbacks preserved |
| W14 | QUIC no-TLS1.3 applicability finding | Full parse/FP still eligible if structurally valid; no parser-created Error |
| W15 | Candidate accepted after delayed policy decision | Commit uses completing-datagram arrival timestamp, not decision time |
| W16 | Protected classification unavailable / 0-RTT / completion unknown | No speculative attempt retirement or Finished transition |

## 10. Review result, unresolved evidence and handoff

Two static reasoning passes were made: first code/contract mapping and ownership/disposition paths; second cross-check of selected projection, Stop versus admission, deferred lifetime, terminal/live distinction, A/B/A, reclaim, D-TIME and QUIC exclusions. Mechanical integrity checks are separate and do not simulate execution.

### 10.1 First closure gate

**Full O05 alignment closure: BLOCKED at E01, missing primary requirements evidence.** Quic_FRD.md.txt and Quic_SRS.md.txt could not be read in this turn. The logical design above is useful and aligned with available contracts, but a complete fresh requirements-alignment PASS would be unsupported. Stop the ordered full closure review here. Later items are inventoried gaps, not passed gates:

- **E02:** O04 G01 external consumers, including borrow retention and recordLength assumptions, remains open before integration.
- **E03:** concrete existing QUIC storage/attempt/lifecycle code and approved policy mapping are not demonstrated in the supplied TCP aggregate. The physical storage/decision binding and exact terminal/admission conditions cannot be declared implemented or finally approved.
- **E04:** O06 must settle maximum CH, finite candidate/active/retired and published-view joint peaks, widths and exhaustion behavior without invented numeric values.
- **E05:** O07 must verify actual provider, allocation, callbacks and failure paths; O08 V3 selects the accepted completion heuristic; it is not inferred from ownership completion. Provider/runtime validation remains separate.

### 10.2 What is delivered now

O04 document corrections are applied. O05 specifies the logical authority, minimum disposition information, acceptance/rejection/deferral ownership consequences, full field restoration, full-last-use interval and existing-lifecycle integration requirements. It deliberately does not freeze a public API or pretend missing source is present. User authorization to proceed to O05 is not interpreted as accepting every proposed storage detail or closing O04 G01.

No architecture restart, new viewer, parallel reclaimer, numeric capacity or automatic violation/drop was introduced. O01-O03, Consolidated V6 and current production code are unchanged. Do not progress automatically to O06 as a passed stage; the design can supply its named capacity questions without claiming O05 full closure.

### V2 pre-delivery review correction

The final check made the full-last-use sentence explicit, corrected W08 to place the zero-mask Stop at the record event before the eligible CH event (CH is the last normal TCP stage), and distinguished Initial integrity verification from peer authentication. V1 is preserved. The initial mechanical phrase-presence check failed before packaging; it was not a semantic or runtime PASS. The corrected V2 was checked again from the source/authority boundary through the scoped ownership cases. Full closure still stops at E01; no later gate is claimed passed.

## V3 focused R01 coordination

Current completion policy is O08 V3. Only R01 synchronization is changed; physical ownership, numeric capacities and implementation validation are not newly closed. Earlier review records describe their original snapshots.
