# Phase 2: Shared TLS State Machine and QUIC Behavior

## 1. Purpose and Scope

This document defines the proposed shared TLS state machine after adding QUIC support. It covers only:

- state meanings;
- supported transitions and their triggers;
- protocol-specific evidence used by TCP-TLS and QUIC;
- ClientHello, retransmission, Retry-compatible restart, additional ClientHello, new-attempt, ambiguity, late-input, and resource-loss behavior;
- the current-release observation boundary.

It does not define parser architecture, packet traversal, key-search order, storage layout, classes, interfaces, APIs, reporting, mitigation, or TLS-Enforcement detector behavior.

The current release supports **QUIC v1 only** and observes **client-to-server traffic only**.

---

## 2. Observation Boundary

### 2.1 Available QUIC evidence

The current release can use:

- QUIC v1 invariant fields and visible Long Header fields, including Version, Long Packet Type, connection IDs, Initial Token, and Length;
- successfully unprotected Initial packets;
- CRYPTO offsets and bytes from Initial packets;
- complete ClientHello handshake messages reconstructed from the Initial CRYPTO stream;
- visible client Handshake Long Header packets as product-level progress evidence;
- successfully parsed QUIC v1 `CONNECTION_CLOSE` frames carried in Initial packets.

Successful Initial protection processing does not authenticate the client.

### 2.2 Unavailable and non-authoritative evidence

The current release cannot rely on:

- server Retry;
- server HelloRetryRequest;
- negotiated TLS version selected by the server;
- server Finished;
- decrypted QUIC Handshake payload;
- 0-RTT or 1-RTT payload;
- `HANDSHAKE_DONE`;
- server transport parameters.

The system must not interpret header-protected Reserved bits, Packet Number Length, or Packet Number without removing header protection. Key Phase is a Short Header field and is not used by this state machine.

The fixed QUIC bit value alone is not state-transition evidence and cannot reject an otherwise classifiable Initial, 0-RTT, or Handshake packet.

A reserved-version value does not by itself prove that the active attempt changed version and does not by itself trigger `Unsupported`. Association of such a packet uses only version-independent invariant fields, and the state machine does not infer version-specific packet-type meaning from it.

A changed packet DCID alone does not create or select a retained Initial context and does not prove a new attempt.

---

## 3. Responsibility Boundary

The Processor validates and parses the current packet, frame, TLS message, or reassembled ClientHello according to packet-local and message-local rules.

The state machine does not enforce the legality of the complete TLS handshake-message sequence. A second, third, or otherwise flow-unexpected ClientHello is not rejected by the state machine merely because of its position in the flow.

While the active attempt is in `PreHandshake` or `InProgress`, every complete, parseable ClientHello that is proven not to be a retransmission and is associated with that attempt becomes the authoritative ClientHello. It replaces the previously retained ClientHello-derived information. Flow-level legality is evaluated outside this state-machine contract.

---

## 4. State Meanings

| State | Meaning |
|---|---|
| `PreHandshake` | No complete, parseable, non-retransmitted ClientHello is authoritative for the active attempt. |
| `InProgress` | At least one ClientHello is authoritative, but product-level completion evidence has not been accepted. |
| `ChangeCipherSpec` | TCP-specific intermediate state used by the existing TCP heuristic. It has no QUIC equivalent. |
| `Finished` | Sufficient protocol-specific evidence was accepted to end active state tracking. It does not mean that TLS Finished was observed. |
| `Error` | The existing terminal verdict contract ended processing for the attempt. |
| `Unsupported` | Processing cannot continue reliably under the current-release contract. |
| `TerminatedByAlert` | The attempt ended after accepted protocol-specific terminating-alert evidence. |

Terminal states are:

- `Finished`;
- `Error`;
- `Unsupported`;
- `TerminatedByAlert`.

---

## 5. Shared Semantics and Protocol-Specific Evidence

The shared model is:

```text
PreHandshake
    -- first authoritative ClientHello -->
InProgress
    -- protocol-specific completion evidence -->
Finished
```

The meanings are shared. The evidence is protocol-specific.

### 5.1 TCP-TLS completion evidence

The existing TCP behavior remains unchanged:

- when the offered-version mask does not contain TLS 1.3, a Handshake record after `ChangeCipherSpec` provides completion evidence;
- when the offered-version mask contains TLS 1.3, ApplicationData after ClientHello, subject to the existing early-data guard, provides completion evidence.

The existing TCP transitions relevant to the shared state model are:

| Current state | Observation | Conditions | Next state | Meaning |
|---|---|---|---|---|
| `PreHandshake` | Complete ClientHello | Parseable and accepted by the existing TCP path | `InProgress` | ClientHello becomes authoritative. |
| `InProgress` | ChangeCipherSpec | TLS 1.3 is not offered | `ChangeCipherSpec` | Enter the existing TCP-specific intermediate state. |
| `InProgress` | ApplicationData | TLS 1.3 is offered and the early-data guard passes | `Finished` | Accept the existing TLS 1.3 completion heuristic. |
| `ChangeCipherSpec` | Handshake record | Accepted by the existing TCP path | `Finished` | Accept the existing pre-TLS-1.3 completion heuristic. |
| `PreHandshake` or `InProgress` | Accepted terminating TLS alert | Existing TCP alert conditions pass | `TerminatedByAlert` | The attempt ended after terminating-alert evidence. |
| `ChangeCipherSpec` | Explicit unsupported continuation result | Existing TCP path cannot continue reliably | `Unsupported` | Processing cannot continue reliably. |
| `TerminatedByAlert` | Existing terminal payload result | Existing TCP terminal verdict contract requests stop | `Error` | Apply the existing TCP-specific terminal transition. |

Transport reset, replacement, and resource-reclamation mechanics are unchanged and are outside this state-machine summary.

### 5.2 QUIC completion evidence

For QUIC v1, an associated client Handshake Long Header packet provides product-level completion evidence.

This means only that sufficient visible progress was observed for the feature. It does not prove that TLS Finished was sent or received. The packet can be ACK-only or can carry encrypted content, including an encrypted close. Because Handshake payload is not decrypted, the visible associated Handshake packet remains completion evidence and does not provide terminating-alert evidence.

---

## 6. QUIC Transition Matrix

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

## 7. ClientHello and Retransmission Rules

### 7.1 Authoritative ClientHello

For QUIC, `parseable ClientHello` means a complete TLS ClientHello handshake message reconstructed from the Initial CRYPTO stream and successfully parsed without applying TLS record-layer checks. This term does not decide full-handshake-flow legality.

While the active attempt is in `PreHandshake` or `InProgress`, every complete, parseable ClientHello that is proven not to be a retransmission and is associated with that attempt becomes authoritative.

It replaces the current ClientHello-derived values, including:

- offered-version mask;
- `early_data` indication;
- ClientHello fingerprint;
- ClientHello timestamp.

A logical ClientHello count increases for every such ClientHello and resets for a complete new attempt. The count records observations; it does not make the state machine enforce full-flow legality.

After the attempt reaches a terminal state, input is not processed as another ClientHello of that attempt. Only an accepted new-attempt candidate resets the previous attempt-local state. Its ClientHello can become authoritative only after reset and parsing in `PreHandshake`.

### 7.2 Retransmission

A ClientHello is a retransmission when already accepted CRYPTO bytes are delivered again at the same offsets.

The following do not create a new ClientHello observation:

- the same bytes at the same CRYPTO offsets;
- re-packetization of the same bytes at the same offsets;
- identical overlap;
- a Retry-compatible byte-equivalent resend.

A complete ClientHello at new CRYPTO offsets is a new logical ClientHello, even if its parsed contents are identical to an earlier ClientHello.

### 7.3 Conflicting overlap

Conflicting bytes at an already accepted CRYPTO range are not merged into the active reassembly state. Conflicting overlap alone does not prove a new attempt and causes no state transition.

---

## 8. Retry-Compatible Restart

Server Retry is not visible. The state machine therefore recognizes only a Retry-compatible restart and does not claim that Retry was observed.

Before Retry-compatible restart is considered, same-context delivery at already accepted CRYPTO offsets and bytes is classified as retransmission under Section 7.2.

A restart is Retry-compatible when:

- the candidate is processed under an Initial context distinct from every Initial context already retained for the active attempt;
- token evidence is present, but token presence alone is not treated as proof of Retry;
- the candidate CRYPTO stream begins at offset `0`;
- bytes overlapping the active partial or complete ClientHello are identical;
- available attempt identity does not contradict the active attempt;
- non-zero client SCID evidence, when available, remains consistent.

A changed packet DCID alone does not create or select a new retained Initial context and does not prove a new attempt.

Retry-compatible restart can occur:

- in `PreHandshake`, while the first ClientHello is incomplete;
- in `InProgress`, after an authoritative ClientHello already exists.

It remains the same attempt. The attempt identity retains both the original Initial context and every accepted Retry-compatible replacement context.

Input matching any Initial context already retained for the active attempt is processed as input of that same attempt. It is not a new Retry-compatible restart merely because that context differs from the most recently used context.

If no authoritative ClientHello exists, the first complete, parseable ClientHello completed in either the pre-restart or post-restart context becomes authoritative. If a byte-equivalent ClientHello is later completed in another retained context of the same attempt, it is the same logical ClientHello and creates no additional observation.

If an authoritative ClientHello already exists, Retry-compatible byte-equivalent delivery creates no additional ClientHello observation. A different later complete ClientHello follows the active-attempt authority rules in Section 7.

---

## 9. Handshake Association and Deferred Evidence

A client Handshake Long Header packet is associated with the active attempt when:

- it belongs to the same DP Session transport tuple;
- its client SCID matches the retained client SCID when that SCID is non-zero;
- it is not classified as belonging to a retained retired attempt;
- the available evidence is not ambiguous.

A matching packet DCID is not required. A zero-length client SCID neither proves nor rejects association. The fixed bit alone is not required as state-transition validity evidence.

Associated Handshake evidence observed before ClientHello completion remains non-authoritative. It is applied only after the first ClientHello of the same attempt becomes authoritative.

### 9.1 QUIC terminating-alert evidence

QUIC terminating-alert evidence is an associated and successfully parsed QUIC v1 `CONNECTION_CLOSE` frame of type `0x1c` with an error code in the `CRYPTO_ERROR` range `0x0100` through `0x01ff`.

A QUIC `CONNECTION_CLOSE` frame is associated with the active attempt when it is recovered from a successfully unprotected Initial packet processed under an Initial context retained for that attempt, and the packet is neither classified as retired input nor left ambiguous.

A close recovered under an Initial context retained for another or retired attempt does not terminate the active attempt.

The range explicitly includes `user_canceled` (`0x015a`). For QUIC, an associated `user_canceled` `CRYPTO_ERROR` is accepted as terminating-alert evidence because the observed `CONNECTION_CLOSE` terminates the QUIC connection.

A transport error code outside that range, `APPLICATION_ERROR`, or an unclassified close does not satisfy the `TerminatedByAlert` transition. The transition is not inferred in `PreHandshake` merely because local ClientHello reconstruction failed.

---

## 10. Complete New Attempt

New-attempt handling depends on whether the current attempt is active or terminal. The two rules remain intentionally separate because different evidence is available before ClientHello processing: an active attempt can use already parsed ClientHello identity, while a terminal attempt must classify a candidate before any new ClientHello parsing.

### 10.1 Active attempt: `PreHandshake` or `InProgress`

A complete ClientHello candidate can replace the active attempt when:

- its CRYPTO stream begins at offset `0`;
- it is processed under a candidate Initial context distinct from every Initial context retained for the active attempt;
- retransmission and Retry-compatible restart are excluded;
- available ClientHello identity is inconsistent with the active attempt when that identity is known;
- it does not match a retained retired identity that is available.

SCID is supporting evidence only when non-zero. A zero-length SCID neither proves nor rejects a new attempt. Missing retired identity is not a rejection condition.

### 10.2 Terminal attempt: classify before ClientHello parsing

When the current attempt is `Finished`, `Error`, `Unsupported`, or `TerminatedByAlert`, new-attempt candidate classification occurs before ClientHello parsing and uses only attempt-level evidence available at that point:

- a candidate Initial context distinct from every Initial context retained for the terminal attempt is available;
- the candidate begins a CRYPTO stream at offset `0`;
- packet-level duplicate or retransmission evidence does not identify it as late input from the completed attempt;
- Retry-compatible restart does not apply;
- it does not match a retained retired attempt identity that is available.

The identity of an attempt includes every Initial context accepted for that attempt, including the original Initial context and every Retry-compatible replacement context. Input matching any retained context of the terminal attempt is late input for that attempt and cannot establish a new attempt merely because it differs from the most recent context.

A changed packet DCID alone does not create or select a retained Initial context and does not prove a new attempt. SCID is supporting evidence only when non-zero. Missing retired identity is not a rejection condition.

If these conditions do not accept a new-attempt candidate, the input does not reach ClientHello parsing. The current terminal state remains unchanged, and the authoritative ClientHello and logical ClientHello count remain unchanged.

If a new-attempt candidate is accepted:

1. retire the completed attempt identity when available;
2. reset all attempt-local semantic and supporting state;
3. enter `PreHandshake`;
4. parse and reassemble input for the new attempt;
5. allow a complete, parseable ClientHello proven not to be a retransmission to become authoritative for the new attempt.

Only one semantic attempt context is active at a time.

---

## 11. 0-RTT

0-RTT does not change semantic state.

It does not:

- establish ClientHello completion;
- create another ClientHello observation;
- satisfy the `Finished` heuristic;
- reset the attempt.

The presence of a 0-RTT packet is not validated against the currently authoritative `early_data` indication.

---

## 12. Terminal and Late Input

A terminal attempt is not reopened by late input.

For any terminal state, new-attempt candidate classification occurs before ClientHello processing and uses only attempt-level evidence. If input is not accepted as a new-attempt candidate, the current terminal state remains unchanged and the input is not parsed or counted as another ClientHello of that terminal attempt. The authoritative ClientHello and logical ClientHello count remain unchanged. Only after candidate acceptance and reset to `PreHandshake` can ClientHello processing begin for the new attempt.

For QUIC:

- a later close retransmission does not cause `TerminatedByAlert -> Error`;
- an Initial packet after `Finished` is either late input or a candidate new attempt;
- it is not progress of the same completed attempt;
- known retired input is ignored semantically;
- ambiguous late input causes no transition.

Retired-attempt suppression is bounded. If retained identity is unavailable, the state machine does not infer that the packet is retired, merge it into the active attempt, or reset the active attempt. Input can become a new-attempt candidate only if the remaining attempt-level conditions are independently satisfied.

---

## 13. Unsupported Outcomes

`Unsupported` means that the current-release implementation cannot continue reliable state evaluation.

For QUIC, this includes:

- an associated non-v1, non-reserved Long Header packet while the attempt is `InProgress`;
- loss of state required to associate or evaluate future completion evidence while the attempt is `InProgress`;
- another explicit protocol-processing result that makes reliable continuation impossible.

`Unsupported` is not `Error` and does not imply a client violation.

A failure to process one packet causes no transition when later processing can still continue.

---

## 14. Reuse Decisions

### 14.1 Existing state-relevant data

Only data that affects the state-machine result is classified here:

- `hsState` remains the shared state representation, with protocol-specific transition evidence;
- while the attempt is `PreHandshake` or `InProgress`, ClientHello-derived values represent the latest authoritative non-retransmitted ClientHello associated with that attempt;
- the logical ClientHello count increases for each such ClientHello and resets for a new attempt; after any terminal state, neither the count nor ClientHello-derived values change unless a new-attempt candidate is accepted, state resets to `PreHandshake`, and a new authoritative ClientHello is then parsed;
- `preCcsSeen` and `earlyDataCount` remain TCP-only heuristics and are not reused for QUIC;
- existing terminal verdict state remains governed by the existing verdict contract;
- `Finished` can become reclaimable without changing its semantic meaning; unavailable retired identity follows the late-input rules in Section 12;
- accepting a new-attempt candidate from any terminal state resets all attempt-local semantic and supporting state before processing its first ClientHello;
- QUIC reassembly, candidate, deferred-evidence, and retired-attempt data are bounded protocol-specific support data, not additional semantic states;
- attempt identity includes the original Initial context and every accepted Retry-compatible replacement context until the attempt identity is retired or reclaimed.

### 14.2 Five levels of reuse

The following reuse levels are independent:

1. **State semantics:** shared where the same state meaning applies.
2. **Transition policy:** shared only where the trigger and result are genuinely common.
3. **State representation:** the existing enum can remain shared.
4. **Mutable storage:** shared semantics do not require shared mutable storage.
5. **Implementation:** no new generic state-machine object is required by this phase.

Protocol-specific evidence does not require artificial shared states. In particular, QUIC does not receive a `ChangeCipherSpec` equivalent.

The shared alternatives were considered and rejected where they add no distinct semantic result:

- QUIC does not use `ChangeCipherSpec` because it has no equivalent observation or later transition dependency;
- 0-RTT does not receive a progress state because it proves neither ClientHello completion nor product-level completion;
- Retry-compatible restart does not reset semantic state because it continues the same attempt;
- additional ClientHello messages do not require another state because the same attempt remains active;
- ambiguous evidence cannot justify reset, replacement, `Error`, or another semantic context.

---

## 15. Exit Gates

| Exit gate | Result | Evidence |
|---|---|---|
| Every supported transition has an explicit trigger and meaning. | PASS | Sections 4-6, 9.1, and 12-13 define state meanings, the TCP and QUIC transition summaries, QUIC terminating-alert association and evidence including `user_canceled`, terminal behavior, and unsupported outcomes. |
| Existing enum, fields, and transition rules are evaluated as abstractions rather than accepted merely because they exist. | PASS | Sections 4-5, 7, 12-14.1 classify state meaning, shared and TCP-only state-relevant data, ClientHello authority, terminal/reclaim behavior, new-attempt reset, and protocol-specific support data. |
| The five reuse levels are evaluated separately. | PASS | Section 14.2 evaluates semantics, transition policy, representation, mutable storage, and implementation independently. |
| Current-release transitions are separated from architectural capability. | PASS | Section 2 distinguishes visible Long Header fields from header-protected fields and bounds unavailable observations; Sections 5-6 use only current-release evidence. |
| States not used by QUIC are identified without artificial equivalents. | PASS | Sections 4 and 14 keep `ChangeCipherSpec` TCP-specific. |
| Protocol-specific or no-transition policy is accepted only after evaluating the shared alternative. | PASS | Section 14.2 explicitly records the rejected shared-state alternatives for QUIC `ChangeCipherSpec`, 0-RTT, Retry-compatible restart, additional ClientHello, and ambiguity. |
| The investigation remains bounded to current behavior and requirements. | PASS | Section 1 excludes implementation design and other feature behavior; Section 2 limits the observation boundary. |
| Retransmission, additional ClientHello, and attempt reuse are sufficiently defined. | PASS | Sections 6-10 and 12 define retransmission precedence, require a Retry-compatible candidate to differ from every Initial context retained for the active attempt, retain all accepted Initial contexts as attempt identity, preserve active-state authority, and classify terminal-state candidates before ClientHello processing. |
| No unresolved state question prevents the result contract. | PASS | Sections 6-13 define active and terminal behavior, explicit QUIC TLS-alert evidence, reserved-version neutrality, resource loss, and candidate classification before reset and ClientHello processing. |
| No extra state or lifecycle mechanism was introduced for hypothetical future features. | PASS | The existing state set is retained, bounded attempt metadata serves current behavior, and no future-version state is added. |

---

## 16. Final Decision

**Phase 2: PASS.**

The approved QUIC progression is:

```text
PreHandshake
    -- first authoritative ClientHello -->
InProgress
    -- associated client QUIC v1 Handshake Long Header packet -->
Finished
```

The approved model also includes:

- no state transition for 0-RTT;
- same-attempt Retry-compatible restart with retransmission precedence and all accepted Initial contexts retained as attempt identity;
- while the attempt is `PreHandshake` or `InProgress`, the latest associated ClientHello proven not to be a retransmission is authoritative;
- after any terminal state, only accepted new-attempt candidate classification can reset the state and permit a later parsed ClientHello to replace the authoritative ClientHello;
- complete new attempt resets and replaces the active attempt;
- ambiguity does not merge, reset, or change state;
- required completion-state loss in `InProgress` produces `Unsupported`;
- only an associated QUIC v1 `CONNECTION_CLOSE` carrying a `CRYPTO_ERROR` code satisfies QUIC terminating-alert evidence, and the accepted range explicitly includes `user_canceled` (`0x015a`);
- an associated Handshake packet remains completion evidence even when its undecrypted payload carries an encrypted close;
- reserved-version values do not by themselves cause `Unsupported`;
- terminal QUIC attempts are not reopened by late close retransmissions;
- no artificial QUIC `ChangeCipherSpec` state.
