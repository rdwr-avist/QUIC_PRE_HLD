# O08: Completion Evidence and the Encrypted-Payload Boundary

Version: V3
Status: FOCUSED STATIC DESIGN REVIEW COMPLETE; accepted QUIC completion heuristic restored.
Scope: Static design only. No implementation, runtime, performance, migration, or exhaustive FRD/SRS alignment PASS is claimed.

## 1. Purpose and authority

O08 defines the product evidence for moving an active attempt from InProgress to Finished without decrypting later protected traffic. It does not redesign orchestration, CH publication, ownership, storage, or provider contracts.

**Accepted project decision:** after the active attempt's ClientHello is complete, observing an associated client QUIC v1 Handshake packet transitions InProgress -> Finished. This includes an ACK-only Handshake packet. Finished is a system-level completion heuristic, not proof that TLS Finished was sent, received, or verified. No Handshake payload decryption, CRYPTO detection, or TLS Finished detection is required or allowed for this trigger.

**Accepted inspection constraint:** do not add later traffic secrets or decrypt protected handshake/application payload to discover completion. Existing QUIC Initial deprotection remains in scope. The shorthand 'TLS 1.3 after CH' does not remove supported plaintext CH2 or eligible later Initial handling.

These distinctions remain mandatory:

- Completing CH is not completing the TLS handshake and is not itself the Finished trigger.
- Stopping payload parsing, releasing scratch, reclaiming storage, and retiring an attempt are not completion evidence.
- Exposed-header classification is not payload decryption or packet authentication.
- Product Finished is not RFC endpoint handshake-complete or handshake-confirmed status.

### 1.1 Source basis and review limits

Directly inspected in this V3 review: the attached O08 V2 and the RFC references in section 10. The accepted project decisions available for this review include the QUIC Handshake heuristic above, O03 deferred CH publication, the TCP segmentation extension, one-way client-to-server visibility, and the allocation/lifecycle constraints retained by V2.

V2 reports earlier inspection of TLS-Flow-All-Code.txt, phase2-state-machine.md, consolidated V6 and O07 V4, and identifies O01 V4, O02 V2, O03 V7, O04 V2, O05 V2 and O06 V1 as companion baselines. Those source files, the full FRD/SRS and consumer code are not present in this review's local inputs. Their reported details are retained as inherited evidence, not presented as a fresh code or cross-document audit. No new retrieval failure for those files is claimed.

**Authority correction:** V2 incorrectly leaves the QUIC Finished trigger focused BLOCKED and treats the Handshake heuristic as unaccepted. The accepted project decision available for V3 supersedes that obsolete blocker. This correction does not claim that an RFC mandates the heuristic or that every historical companion document is already synchronized. Earlier files remain unchanged.

### 1.2 V2 findings resolved in V3

| Finding | Correction |
| --- | --- |
| F01: obsolete BLOCKED status contradicts the accepted trigger | Restore the accepted post-CH, associated Handshake trigger throughout status, decisions, scenarios and handoff. |
| F02: ACK-only behavior and the non-cryptographic meaning of Finished are not operationally explicit | State that ACK-only qualifies without inspecting its payload; record false-completion and spoofing limitations. |
| F03: candidate text leaves ordering and producer/owner responsibilities unclear | Define focused design rules in section 6 without inventing an API, component or runtime allocation. |
| F04: inherited code/source inspections could be read as fresh review evidence | Label them inherited; restrict the V3 review conclusion to sources actually available. |

## 2. Encrypted-payload boundary

### D08.1: No later traffic decryption

ACCEPTED CONSTRAINT: do not add TLS record decryption, QUIC Handshake/0-RTT/1-RTT decryption, later-secret derivation, key-log ingestion, or an endpoint TLS stack merely to obtain completion evidence. Do not interpret ciphertext bytes as plaintext handshake headers, alerts, CRYPTO frames, or Finished messages.

This does not ban the already required QUIC v1 Initial header-protection removal and AEAD deprotection used to recover Initial frames and ClientHello bytes. Initial protection is already part of the baseline, not a newly introduced post-CH decryption feature. Initial AEAD success is packet-integrity evidence under public Initial keys, not authenticated peer identity. [R2 sections 5, 5.2, 7]

### D08.2: TLS over TCP

For the observed client-to-server direction:

- TLS 1.2: after the applicable CCS boundary, the following protected record bodies are opaque. Do not parse an encrypted Finished or Alert body. Exposed record framing may still be tracked where required by the existing contract. CCS changes protection state before Finished; CCS alone is not proof of completed handshake. [R1 sections 7.1, 7.4.9]
- TLS 1.3: protected client records remain opaque. The user's phrase 'after CH' is shorthand for the inspection restriction, not a universal wire-protocol encryption boundary. In the ordinary handshake, handshake messages after the final ServerHello are encrypted; a second plaintext CH can follow HRR before that ServerHello. Encrypted early application data can already follow the first CH, and compatibility CCS is not a cryptographic transition. Do not infer the unseen server's negotiated state from the first CH alone. [R3 sections 1.2, 2.1, 2.3, 4.1.2 and 5]
- The client CH advertises supported versions; it does not establish which version the unseen server selected. Preserve existing TCP version-mask behavior without presenting it as negotiated-version proof. [R3 section 4.2.1]

No new TCP state transition or stricter stop-at-first-CH behavior is authorized by this document. If the new constraint exposes an existing ciphertext-as-plaintext path, fix that specific correctness issue in a new reviewed revision rather than silently changing unrelated TCP semantics.

### D08.3: QUIC

Keep the supported boundary at Initial-only deprotection. Do not decrypt Handshake, 0-RTT or 1-RTT packets. A coalesced datagram does not extend Initial keys to other packet types. Each constituent packet retains its own protection level. [R2 sections 4, 5]

CH completion does not globally disable the accepted Initial classifier. Eligible Initial input may still be needed for retransmission suppression, HRR-compatible CH2, Retry-compatible restart, an Initial-level close, or a distinct new-attempt candidate. Apply the existing association, retirement, ambiguity and ownership rules before any authoritative update. No unrestricted decrypt-and-scan loop is introduced.

## 3. Existing TCP behavior: inherited code observations, not new decisions

V2 reports the following code observations. They are retained for TCP-preservation traceability, not independently reverified against code in V3:

| Code location in TLS-Flow-All-Code.txt | Observed behavior | O08 interpretation |
| --- | --- | --- |
| Lines 3799-3806, commitNormalCurrentRecord | A handshake-content record while state is ChangeCipherSpec sets Finished; otherwise a CH can update TLSData. | Existing record/state-based product policy, not verified Finished content. |
| Lines 3819-3837, updateTLSDataOnClientHello | Only a complete CH updates count, timestamp, fingerprint and version mask; PreHandshake becomes InProgress. | CH completion does not set Finished. |
| Lines 3840-3847, updateTLSDataOnChangeCipherSpec | Pre-CH CCS increments preCcsSeen; otherwise non-TLS13 InProgress becomes ChangeCipherSpec. | Preserve the exact existing guards. |
| Lines 3849-3857, updateTLSDataOnApplicationData | Pre-CH application data increments earlyDataCount; TLS13 InProgress with no earlyDataOffered becomes Finished. | Preserve the early-data guard; do not generalize it to QUIC. |
| Lines 6227-6235 | Selected application-data, CCS and post-CCS handshake records are classified opaque. | Opaque tracking is separate from inner-message parsing. |
| Lines 6254-6275 | Handshake-header parsing is enabled in PreHandshake/InProgress; non-CH messages are recorded rather than parsed as CH. | This is not evidence that every possible post-CH record path is safe for ciphertext. |

V2 reports that the HsState enum/helpers at lines 332-370 treat Finished as terminal and silent. Therefore choosing Finished has lifecycle and consumer consequences; it is not a harmless label for 'we stopped parsing'.

TCP migration must preserve the existing complete-record boundary and segmented opaque tracking. A partial record must not acquire a new completion event merely because a header or first payload byte arrived. O03's approved deferred CH publication remains unchanged.

## 4. What can be observed without later decryption?

### 4.1 Header protection: precise field distinction

For QUIC v1 Initial, 0-RTT and Handshake packets, the long-header packet-type bits are outside the protected low four bits of the first byte. They can be structurally classified without removing header protection, after validating the version-specific format and packet bounds. The Packet Number Length bits and packet number remain protected, while the long-header Length field is exposed. For protected short-header packets, Key Phase is also protected. [R2 section 5.4; R4 sections 17.2.2-17.2.4 and 17.3]

**Exceptions:** Retry and Version Negotiation packets do not use this header-protection process and do not have the protected Packet Number / Packet Number Length fields or the Length field of Initial/0-RTT/Handshake packets. Retry has its own integrity mechanism; Version Negotiation has a separate version-independent format. Do not apply a v1 protected-long-header layout or v1 packet-type interpretation to Version Negotiation. These exceptions clarify protocol facts; they do not add server-to-client visibility or new processing scope. [R2 sections 5.4 and 5.8; R4 sections 17.2.1 and 17.2.5]

Consequently, 'all QUIC packet-type bits are encrypted' is too broad. However, the opposite shortcut is also invalid: an exposed Handshake type does not authenticate the packet, prove its payload contains Finished, or establish association with the active attempt.

The project guardrail is applied per field: protected fields require valid deprotection or trusted decoded metadata; exposed fields require a validated version-specific structural parse. Neither source proves cryptographic completion. The accepted product decision is applied separately under section 6.

### 4.2 Handshake packet is not Finished

A Handshake packet can carry handshake-level frames without proving that the complete client Finished was delivered or accepted. QUIC handshake completion at an endpoint involves sending Finished and verifying the peer Finished. An observer of an exposed packet type has not performed that verification. [R2 sections 4.1.1, 4.1.3, 4.1.5]

RFC 9001 section 4.1.4 explicitly allows a client to acknowledge Handshake packets before it is ready to send CRYPTO frames at that level. Therefore even a genuine client Handshake packet may be ACK-only and precede the client's Finished. The accepted product heuristic deliberately includes this case; it does not verify ACK, CRYPTO, or Finished content. [R2 sections 4.1.1, 4.1.4]

### 4.3 Unavailable signals

Do not assume server Retry, HRR, Finished, HANDSHAKE_DONE, server 1-RTT, or server transport parameters are visible. Do not assume Handshake/1-RTT keys exist. Do not invent trusted upstream completion metadata because an API would be convenient.

0-RTT remains non-completion input. Neither its presence nor its absence supplies a new Finished trigger. [R2 section 4.6; accepted project state contract]

## 5. Alternatives and disposition

| Alternative | Disposition |
| --- | --- |
| A. Finished immediately when CH becomes complete | Rejected: CH publication is not the completion trigger. |
| B. Decrypt Handshake/1-RTT and inspect Finished or frames | Rejected: violates the accepted inspection boundary. |
| C. Associated client v1 Handshake packet observed after complete CH | **Accepted product heuristic**, including ACK-only; apply section 6. |
| D. Short-header/1-RTT-looking input as completion | Not selected; do not add a second heuristic. |
| E. Trusted upstream completion signal | Not selected; no concrete producer/trust contract is established. |
| F. Keep InProgress without qualifying evidence | Retained only when C's guards are not met, until an accepted lifecycle disposition. Not a global blocker on C. |

C is selected by the accepted project decision, not derived from the no-decryption constraint alone. The lack of cryptographic proof is a documented limitation of the selected policy, not a reason to reopen its product acceptance.

## 6. Accepted QUIC completion contract

### D08.4: Trigger and observable evidence

The policy owner transitions the active attempt InProgress -> Finished only when all of these conditions hold:

1. The active attempt has a complete authoritative CH and its accepted CH publication processing has completed without an intervening terminal disposition, retirement or replacement.
2. A client-to-server QUIC v1 Handshake constituent packet is subsequently observed and structurally classified using exposed fields within valid packet/datagram bounds.
3. That packet is unambiguously associated with the same active attempt under the retained association contract.
4. The attempt is still InProgress when the owner applies the transition.

**ACK-only qualifies.** No payload frame test is performed. ACK-only is a scenario known to a test fixture or endpoint, not metadata the observer must discover. Do not decrypt the payload to distinguish ACK-only from CRYPTO-bearing packets, require TLS Finished bytes, or infer a successful AEAD check that was not performed. [R2 sections 4.1.1, 4.1.4, 5.4]

### D08.4a: Structural classification, not authentication

The existing QUIC processing path supplies the exposed-header classification and packet bounds to the existing orchestration/policy path. This is a responsibility contract, not approval of a new function signature or interface. A provider/parser does not independently mutate Finished or issue an enforcement verdict.

Validate the supported v1 version, long-header form, v1 fixed-bit policy, Handshake type, exposed CID lengths and CID bytes, complete Length varint, and the constituent packet end within the datagram. Apply the supported v1 structural minimum for protected-packet sampling space without interpreting protected PN-length bits. Do not treat an incomplete header, impossible length, or truncated constituent as qualifying evidence. [R2 section 5.4.2; R4 sections 17.2, 17.2.4]

The Length field bounds the packet-number-plus-protected-payload region. The actual packet-number width, packet number, reserved bits and inner frames are not validated or interpreted without the relevant deprotection. Do not use Initial keys on Handshake packets. Do not scan opaque ciphertext for a plausible next header. [R2 section 5.4; R4 sections 12.2, 17.2.4]

Here, 'structurally qualifying' means only the checks possible from exposed fields. It never means a cryptographically valid or endpoint-accepted packet. Retry, Version Negotiation and unknown versions cannot qualify by reusing v1 Handshake type bits.

### D08.4b: Attempt association

Retain the association constraints reported by V2 from Phase 2 section 9 and the accepted O05/O06 lifecycle contracts:

- Require client direction, the correct DP Session and the same transport tuple.
- Where a nonzero client SCID is retained for association, require the observed client SCID to match it. A mismatch cannot complete the active attempt.
- A zero-length SCID alone supplies neither positive nor negative association proof. Use the retained association rules; if they do not resolve the attempt unambiguously, do not transition. Do not invent tuple-only sufficiency for every zero-SCID case.
- Exclude retired-attempt input and unresolved candidate/active ambiguity. A matching CID alone does not override those exclusions.
- Do not add a mandatory equality with the original Initial DCID. Client DCID can change during establishment; the unseen server CID is not an invented prerequisite. [R4 section 7.2]

These rules constrain the accepted heuristic but do not authenticate it. The exact source-level integration still requires implementation review against the companion contracts; no fresh equivalence PASS is claimed here.

### D08.4c: Ordering and idempotence

The following are explicit V3 design resolutions of V2's ordering gap, not additional historical user approvals:

- Process coalesced constituents in their encountered order. If an Initial completes CH, complete the accepted CH processing/publication before applying a following qualifying Handshake constituent in that same datagram.
- A Handshake observation encountered before CH completion does not finish the attempt. V3 introduces no persistent pre-CH evidence latch, packet queue or replay service. A later qualifying observation after CH completion is required. This is a conservative policy choice, not a claim that network reordering cannot occur.
- Recheck active-attempt identity and state at the transition point. A callback-induced terminal outcome or replacement prevents completion of the old attempt.
- A duplicate Handshake observation after Finished does not repeat the transition, republish CH, update CH counters, or change clientHelloTimeStamp.
- Finished is terminal for that attempt. Do not regress it to InProgress. Separate eligible new-attempt classification and retirement remain governed by O05/O06; terminal behavior for one attempt is not a global ban on examining all future Initial input.

### D08.4d: Meaning and security limitation

Finished retains its existing system lifecycle/consumer meaning. For QUIC, the accepted header heuristic may end subsequent incomplete-handshake tracking for the attempt even if the peer handshake later fails or never completes. It does not establish peer identity, successful TLS Finished verification, handshake confirmation, application readiness, or authenticated transport parameters. [R2 sections 4.1.1-4.1.2, 8.2]

A forged packet satisfying the exposed structural and association checks is indistinguishable to this observer from genuine qualifying input. Such a packet **can trigger the heuristic**. Do not promise that structural validation or SCID matching rejects every forgery. Association and malformed-input checks reduce accidental misassociation; they do not remove this accepted false-completion risk. This is a limitation to document and test, not a new decryption requirement.

## 7. State, publication, resources and enforcement

### D08.5: Publication and resource lifecycle

- A complete authoritative CH follows the existing shared processing/publication contract and can establish InProgress. O03 deferred publication remains unchanged: no callbacks for intermediate contributing records or an incomplete record; publish the accepted projection only at complete CH.
- Do not set Finished from CH completion alone, fingerprint availability, ignored encrypted payload, scratch release, timeout bookkeeping or reclaim. Apply section 6 for the selected QUIC trigger.
- Handshake/0-RTT/1-RTT payload remains opaque. Ignoring protected payload is not itself an error or enforcement verdict. Do not erase or republish CH when the completion heuristic fires.
- clientHelloTimeStamp remains the arrival timestamp of the datagram completing the full CH, not callback time or the later Handshake observation.

Without qualifying evidence, retain InProgress until the accepted lifecycle disposition. This does not retain all temporary storage until aging: return packet scratch after its last consumer and CH workspace under its accepted last-use contract. Do not free borrowed data while callbacks/readers still use it. Retain only bounded identity/semantic state needed by existing contracts.

A pending candidate remains separate from the active attempt. Publication, replacement, retirement suppression and ambiguity remain under O05/O06. Retire a resource victim under the accepted lifecycle contract before reuse. Reclaim is neither Finished nor an Incomplete Handshake violation by itself.

Use existing aging/reclaim mechanisms. No new timeout value, capacity, storage pool, state enum, generic completion component or parallel reclaim service is introduced. Engine allocation remains startup-only, with prepared storage during packet processing and aging. No maximum CH size is invented.

### D08.6: Enforcement boundary

Processor and TLS-Flow do not define violations. TLS-Enforcement owns verdicts and incomplete-handshake policy. The accepted Finished transition supplies system state, not an Allow verdict or authenticated-handshake assertion.

Lack of qualifying completion evidence is not an automatic drop, Error state or invented violation. An InProgress attempt is not automatically an attack. Existing legitimate lifecycle events and resource dispositions remain distinct. Full downstream QUIC consumer behavior still requires source-backed validation; it is not manufactured from TCP behavior.

## 8. Focused static scenarios and implementation test requirements

The outcomes below were reviewed as design scenarios. No packet replay, unit suite, provider instrumentation or runtime allocation test was executed.

| ID | Scenario | Required outcome / anchor |
| --- | --- | --- |
| T01 | CH split across supported TCP segments/records or QUIC Initial packets | Complete supported collection before publication; no Finished from partial CH. Sections 2, 3, 7. |
| T02 | TLS 1.2 CCS followed by protected handshake record | Opaque body; preserve inherited complete-record state policy, not inner Finished parsing. Sections 2, 3. |
| T03 | TLS 1.3 compatibility CCS | No new cryptographic-boundary or Finished inference from CCS. Sections 2, 3. |
| T04 | TLS 1.3 CH followed by encrypted record | No payload decryption; preserve inherited TCP earlyDataOffered guard and complete-record framing. Sections 2, 3. |
| T05 | Supported plaintext CH2 / HRR-compatible sequence | No blanket stop at first CH that silently removes accepted CH2 handling. Sections 1, 2. |
| T06 | Initial completing CH followed by associated Handshake in one datagram | Publish complete CH first; then InProgress -> Finished if all guards still hold. Never decrypt Handshake. Section 6. |
| T07 | Complete CH without later qualifying evidence | Publish CH; remain InProgress until accepted lifecycle disposition. Section 7. |
| T08 | Associated client Handshake after complete CH | InProgress -> Finished once, without requiring CRYPTO or TLS Finished. Section 6. |
| T09 | 0-RTT before or after CH | No completion transition; no payload decryption. Sections 4, 5. |
| T10 | Protected bits resemble useful values | No raw PN, PN-length, reserved-bit or Key Phase interpretation. Sections 4, 6. |
| T11 | Unknown/wrong version, malformed Length/CID, incomplete header or truncated packet | No completion from that constituent; no invented drop/verdict. Section 6. |
| T12 | Retired, mismatched-SCID or ambiguous-attempt input | No active-attempt completion, merge or reset. Section 6. |
| T13 | Handshake observed before CH completion | No transition and no persistent pre-CH latch; require a later qualifying observation. Section 6. |
| T14 | Scratch release, eviction or aging | No synthetic Finished; eviction alone is not an incomplete-handshake violation. Section 7. |
| T15 | Later eligible Initial after CH or after an attempt finishes | Preserve accepted retransmission/restart/CH2/new-attempt classification subject to lifecycle rules; no regression of the finished attempt. Sections 2, 6, 7. |
| T16 | Protected Alert or CONNECTION_CLOSE body | No inner evidence from ciphertext. Initial-level close remains under its accepted contract. Sections 2, 7. |
| T17 | Retry or Version Negotiation | No v1 protected-long-header Length/PN layout or completion inference; no added receive-direction scope. Section 4. |
| T18 | ACK-only Handshake fixture after complete CH | Same Finished transition as T08. Observer neither identifies ACK-only nor decrypts it. Section 6. |
| T19 | Forged header passing all exposed structural/association guards | Heuristic can transition to Finished; record false-completion limitation, never claim authentication. Section 6. |
| T20 | Zero-length SCID | No automatic rejection or acceptance from SCID length alone; unresolved association means no transition. Section 6. |
| T21 | Duplicate qualifying Handshake after Finished | No repeated transition, CH publication, counter update or CH timestamp change. Section 6. |
| T22 | CH processing causes terminal disposition/replacement | Following Handshake cannot finish the old attempt or leak evidence into the replacement. Section 6. |
| T23 | Original Initial DCID differs from later client Handshake DCID | Do not reject solely for that difference; apply retained association rules. Section 6. |
| T24 | Valid earlier constituent followed by malformed/truncated constituent | Do not derive evidence from the malformed constituent or scan past an unbounded packet; preserve existing per-unit commit/error semantics, without adding datagram-wide rollback. Section 6. |

## 9. Handoff and remaining validation

| ID | Disposition | Boundary |
| --- | --- | --- |
| B08.1 | Product-selection blocker closed by the accepted project decision | C is the selected heuristic, including ACK-only; not cryptographic completion. |
| B08.2 | No global first-CH stop is introduced | Initial-only deprotection and accepted CH2/restart/new-attempt scope are preserved. A broader stop would require a separate scope decision. |
| E08.1 | Full FRD/SRS, companion-document and consumer-code audit not performed in V3 | Required before claiming exhaustive alignment or migration readiness. |
| I08.1 | Source-level classifier-to-policy integration remains implementation work | Verify exposed bounds/classification, exact retained association and existing owner integration without inventing an API here. |
| V08.1 | Runtime no-decryption/no-allocation and scenario validation remain outstanding | Required for implementation acceptance, not for selecting the product heuristic. |

O08's completion-policy selection is no longer focused BLOCKED. The next design work may use this selected contract. This does not approve implementation, certify unavailable companion files, or rename Finished. Synchronize any obsolete blocker references in later working copies of companion documents; do not overwrite their preserved baselines.

## 10. Protocol references

These references establish wire-format facts, not project-level product acceptance. No RFC text is reproduced.

- R1: [RFC 5246, TLS 1.2](https://www.rfc-editor.org/rfc/rfc5246.html), sections 7.1 and 7.4.9: CCS and protected Finished.
- R2: [RFC 9001, TLS for QUIC](https://www.rfc-editor.org/rfc/rfc9001.html), sections 4.1.1-4.1.5, 4.6, 5, 5.2, 5.4, 5.4.2, 7 and 8.2: completion, encryption levels, Initial protection and header protection.
- R3: [RFC 8446, TLS 1.3 baseline specification](https://www.rfc-editor.org/rfc/rfc8446.html), sections 2.1, 4.1.2, 4.2.1 and 5: CH2, offered versions and compatibility CCS. Used for the project's existing TLS 1.3 wire semantics, not a claim of latest standards status.
- R4: [RFC 9000, QUIC transport](https://www.rfc-editor.org/rfc/rfc9000.html), sections 7.2, 12.2, 17.2-17.2.5 and 17.3: Version Negotiation, Initial/0-RTT/Handshake, Retry and short-header field distinctions.

## 11. Change summary

V3 corrects V2's obsolete authority interpretation and restores the already accepted Handshake-based Finished heuristic, explicitly including ACK-only. Sections 5-9 now agree on the trigger, fallback, ownership, ordering, association, idempotence, spoofing limitation and handoff. Section 3 retains TCP observations as inherited evidence rather than claiming fresh source inspection.

The no-later-decryption boundary, Initial-only exception, exposed/protected field distinction, Retry/Version Negotiation exceptions, deferred CH publication, TCP-preservation scope and lifecycle/enforcement separation remain intact. V3 adds focused ordering resolutions without new persistent evidence state. V2 and all earlier inputs remain unchanged.

## 12. V3 pre-delivery double-check

Review boundary: the attached V2, accepted decisions available in this conversation context, and the protocol references above. This is not a fresh review of unavailable code or primary requirements.

### Pass 1: authority and protocol semantics

- Accepted decision restored consistently: complete active CH followed by associated client Handshake can produce Finished; ACK-only is included. Sections 1, 5, 6, 8, 9.
- RFC endpoint completion is kept distinct from product Finished. RFC 9001 sections 4.1.1 and 4.1.4 support the explicit limitation that a Handshake ACK may precede client CRYPTO/Finished.
- Header classification uses exposed type/version/CID/Length fields, not protected PN-length/PN/reserved bits or payload frames. RFC 9001 section 5.4 and RFC 9000 section 17.2.4 support the distinction.
- Retry and Version Negotiation exceptions remain explicit. No unknown-version v1 mapping or new server visibility is introduced.
- TLS 1.2 CCS and TLS 1.3 CH/HRR/early-data distinctions remain separate from TCP product heuristics. No later traffic decryption is authorized.

### Pass 2: adversarial and cross-section consistency

- ACK-only requires no frame classification; spoofed qualifying input is not falsely promised rejection.
- Pre-CH observations do not complete an attempt or persist into a replacement. Coalesced post-CH Handshake is handled after CH processing; duplicates are idempotent.
- Zero-SCID ambiguity, mismatched SCID, retired input, changed DCID, malformed lengths and truncation have explicit outcomes without cryptographic validation claims.
- No completion is synthesized by cleanup, no CH timestamp is rewritten, no new consumer callback or verdict is invented, and no runtime allocation/capacity is added.
- No current decision still labels C unaccepted or the product trigger blocked. Remaining integration and validation work is separated from product acceptance.
- TCP observations and companion-contract claims are explicitly inherited, not falsely reported as freshly inspected evidence.

Outcome: focused static consistency review complete for the available inputs. No unresolved contradiction was identified within that boundary. No build, packet test, provider test, performance measurement, allocation audit or exhaustive FRD/SRS/consumer alignment was executed.
