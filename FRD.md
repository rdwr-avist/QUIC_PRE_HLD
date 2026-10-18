# Web DDoS Protection: QUIC and DTLS Support — FRD

**Document Type:** Functional Requirements Document (FRD)\
**Feature Name:** Web DDoS Protection — QUIC and DTLS Protocol Support\
**Date:** 17/08/2026\
**Target Release:** DP TBD

---

## 1. Purpose & Context

Web DDoS Protection currently provides behavioral DDoS mitigation for encrypted traffic over TCP (TLS/HTTP 1.x and HTTP/2). Two widely deployed UDP-based encrypted protocols — QUIC (HTTP/3) and DTLS — are completely outside its protection scope today. As adoption of both protocols grows, attackers can exploit this gap to launch handshake-flood and fingerprint-based DDoS attacks that bypass existing detection entirely. This feature extends the existing Web DDoS Protection pipeline — fingerprinting, baseline learning, RTS generation, packet-drop mitigation, and source suspension — to cover QUIC and DTLS traffic, closing the protection gap without introducing a separate protection model.

---

## 2. Background

- Web DDoS Protection uses TLS ClientHello fingerprinting, behavioral baseline learning, and RTS-based mitigation.
- This pipeline was designed for TCP-based TLS and does not process UDP traffic.
- **QUIC** (RFC 9000) runs over UDP (default port 443) and embeds TLS 1.3 ClientHello inside an Initial packet CRYPTO frame. Handshake flood attacks against QUIC are technically equivalent to TCP TLS handshake floods, but currently fully undetected.
- **DTLS** (RFC 6347 / RFC 9147) is TLS over UDP, used for WebRTC, VoIP, and gaming applications. DTLS ClientHello is structurally similar to TLS ClientHello, making the same fingerprinting approach applicable. DTLS operates on various ports (commonly 443, 4433, 5349).
- Both protocols expose the same handshake-exhaustion attack surface as TCP TLS: high-volume ClientHello floods consume server crypto resources.
- The existing fingerprinting and RTS infrastructure is reused; this feature adds UDP-protocol parsers and per-protocol enable/disable controls.

---

## 3. Scope

### 3.1 In Scope

- QUIC Initial packet detection and TLS ClientHello extraction from CRYPTO frame.
- DTLS record detection and ClientHello extraction.
- Application of existing TLS fingerprinting algorithm to ClientHello messages from both protocols.
- Unified baseline learning covering TCP TLS, QUIC, and DTLS traffic within each policy.
- RTS generation and packet-drop mitigation for QUIC and DTLS.
- Per-profile enable/disable controls for QUIC and DTLS support independently.

### 3.2 Out of Scope

- Changes to the existing TCP TLS fingerprinting, detection, or mitigation logic.
- QUIC 0-RTT or 1-RTT packet inspection (handshake phase only).
- DTLS re-handshake or post-handshake traffic inspection.
- Deep packet inspection of HTTP/3 application-layer content.
- Redesign of the RTS infrastructure or baseline storage model.

### 3.3 Related Features / Dependencies

- Web DDoS Behavioral Protection (baseline, RTS, mitigation engine)
- TLS fingerprinting subsystem
- Policy configuration model (Web DDoS profile)


---

## 4. Definitions

| Term           | Definition                                                                                                |
| -------------- | --------------------------------------------------------------------------------------------------------- |
| QUIC           | Quick UDP Internet Connections (RFC 9000); transport protocol over UDP, foundation for HTTP/3 (RFC 9114). |
| DTLS           | Datagram TLS (RFC 6347 / RFC 9147); TLS adapted for UDP transport, used in WebRTC, VoIP, and gaming.      |
| Initial Packet | First QUIC packet sent by a client; contains TLS ClientHello embedded in a CRYPTO frame.                  |
| CRYPTO Frame   | QUIC frame type carrying TLS handshake messages.                                                          |
| ClientHello    | First TLS handshake message; used as the basis for fingerprinting in Web DDoS Protection.                 |
| RTS            | Real-Time Signature; dynamically generated attack fingerprint used for mitigation.                        |
| ALPN           | Application-Layer Protocol Negotiation; TLS extension indicating protocol (e.g., "h3" for HTTP/3).        |
| HW-ACC         | Hardware acceleration subsystem (FPGA-based) on applicable DefensePro platforms.                          |

---

## 5. Concept of Operation

1. DefensePro receives a UDP packet destined for a protected server.
2. The packet is classified as QUIC (QUIC long header present) or DTLS (DTLS record header present), or neither.
3. For QUIC: the Initial packet is parsed, the CRYPTO frame is extracted, and the embedded TLS ClientHello is recovered.
4. For DTLS: the DTLS record is parsed and the ClientHello message is extracted.
5. The existing TLS fingerprinting algorithm is applied to the extracted ClientHello, producing a fingerprint identical in format to TCP TLS fingerprints.
6. The fingerprint is matched against both the rate-based and rate-invariant baselines maintained per protocol (TCP TLS, QUIC, DTLS) within the same Web DDoS policy.
7. Deviation from baseline triggers RTS generation for the offending protocol; the RTS contains the malicious fingerprint.
8. Subsequent QUIC Initial or DTLS ClientHello packets matching an active RTS fingerprint are dropped.

---


## 6. Functional Requirements

### FR-01 — QUIC Initial Packet Detection

Interface: Traffic

Priority: High

DefensePro shall identify QUIC traffic by recognizing UDP packets with a valid QUIC long header (flags, version field, DCID, SCID). Non-QUIC UDP traffic shall not be affected.

Acceptance: QUIC Initial packets are identified and forwarded to the QUIC parser. Non-QUIC UDP packets pass through without modification.

### FR-02 — QUIC ClientHello Extraction

Interface: Traffic

Priority: High

DefensePro shall parse QUIC Initial packets to extract the TLS ClientHello from the embedded CRYPTO frame. Fragmented or multi-fragment CRYPTO frames shall be handled per RFC 9001.

Acceptance: TLS ClientHello is successfully extracted from well-formed QUIC Initial packets and passed to the fingerprinting subsystem. Malformed or non-Initial QUIC packets are discarded without error.

### FR-03 — DTLS Record Detection

Interface: Traffic

Priority: High

DefensePro shall identify DTLS traffic by recognizing UDP packets with a valid DTLS record header (content type 22 — handshake).

Acceptance: DTLS ClientHello-bearing UDP packets are identified and forwarded to the DTLS parser. Non-DTLS UDP packets pass through without modification.

### FR-04 — DTLS ClientHello Extraction

Interface: Traffic

Priority: High

DefensePro shall parse DTLS records to extract the ClientHello handshake message, accounting for DTLS-specific header fields (epoch, sequence number, fragment offset per RFC 6347 / RFC 9147).

Acceptance: TLS ClientHello content is successfully extracted from well-formed DTLS ClientHello records and passed to the fingerprinting subsystem. Fragmented or malformed DTLS records are handled without causing pipeline errors.

### FR-05 — Unified Fingerprinting for QUIC and DTLS

Interface: Traffic

Priority: High

DefensePro shall apply the existing TLS fingerprinting algorithm to ClientHello messages extracted from QUIC and DTLS packets. Resulting fingerprints shall use the same format and be stored in the same fingerprint infrastructure as TCP TLS fingerprints.

Acceptance: A fingerprint generated from a QUIC or DTLS ClientHello is structurally identical to one generated from the same ClientHello over TCP TLS, and is recognized by the RTS and mitigation subsystems without modification.

### FR-06 — Unified Baseline Learning

Interface: Traffic

Priority: High

DefensePro shall incorporate QUIC and DTLS traffic into the existing Web DDoS baseline learning engine, maintaining both **rate-based** and **rate-invariant** baselines per protocol (TCP TLS, QUIC, DTLS) within each policy, using the existing baseline storage model without architectural changes.

Acceptance: After the learning period, the system holds separate rate-based and rate-invariant baselines for TCP TLS, QUIC, and DTLS traffic within the same policy. A sudden spike in QUIC handshake rate is detected as a rate-based deviation even when TCP TLS traffic is within baseline.

### FR-07 — Attack Detection and RTS Generation

Interface: Traffic

Priority: High

DefensePro shall apply the existing deviation-based attack detection logic uniformly to QUIC and DTLS traffic. Upon detecting an attack, RTS entries shall be generated for the offending protocol fingerprints using the existing RTS generation mechanism.

Acceptance: A simulated QUIC handshake flood that deviates from baseline results in an RTS entry containing the malicious QUIC fingerprint within the same detection latency as an equivalent TCP TLS attack.

### FR-08 — QUIC Packet Drop Mitigation

Interface: Traffic

Priority: High

DefensePro shall drop QUIC Initial packets whose extracted fingerprint matches an active RTS entry. Packets not matching any RTS entry shall be forwarded. The same anti-oscillation mechanism used for TCP TLS mitigation shall apply.

Acceptance: QUIC Initial packets matching an active RTS are dropped. QUIC packets not matching any RTS pass through. QUIC 1-RTT or 0-RTT packets are not affected.

### FR-09 — DTLS Packet Drop Mitigation

Interface: Traffic

Priority: High

DefensePro shall drop DTLS packets carrying a ClientHello that matches an active RTS fingerprint. DTLS packets not containing a ClientHello, and non-matching ClientHellos, shall pass through.

Acceptance: DTLS ClientHello packets matching an active RTS are dropped. Non-ClientHello DTLS records from non-suspended sources are not affected.

### FR-10 — Per-Profile Protocol Enable/Disable

Interface: UI, CLI, REST

Priority: Medium

DefensePro shall expose per-Web-DDoS-profile enable/disable controls for QUIC support and DTLS support independently. When a protocol is disabled, its traffic bypasses Web DDoS Protection and the corresponding parsing and learning pipeline is inactive for that policy.

Parameter: `quic-protection-enabled` — enables QUIC parsing, learning, and mitigation for this profile. Unit: — (boolean). Default: Disabled. Valid values: Enabled, Disabled.

Parameter: `dtls-protection-enabled` — enables DTLS parsing, learning, and mitigation for this profile. Unit: — (boolean). Default: Disabled. Valid values: Enabled, Disabled.

Acceptance: Disabling QUIC protection on a profile causes QUIC traffic for that profile to pass through without detection or mitigation, while TCP TLS protection remains active. The same applies independently for DTLS.

---

## 7. Configuration Parameters

| Parameter                 | Meaning                                                               | Unit        | Default  | Valid Range / Allowed Values |
| ------------------------- | --------------------------------------------------------------------- | ----------- | -------- | ---------------------------- |
| `quic-protection-enabled` | Enables QUIC parsing, learning, detection, and mitigation per profile | — (boolean) | Disabled | Enabled, Disabled            |
| `dtls-protection-enabled` | Enables DTLS parsing, learning, detection, and mitigation per profile | — (boolean) | Disabled | Enabled, Disabled            |

---

## 8. Non-Functional Requirements

### NFR-01 — Performance Neutrality

Enabling QUIC and DTLS parsing shall not degrade TCP TLS processing throughput or increase packet latency for non-QUIC/non-DTLS traffic. QUIC and DTLS parsing overhead shall not exceed the overhead introduced by TCP TLS parsing on equivalent traffic volumes.

### NFR-02 — Protocol Standards Compliance

QUIC parsing shall conform to RFC 9000 and RFC 9001 (QUIC v1). DTLS parsing shall conform to RFC 6347 (DTLS 1.2) and RFC 9147 (DTLS 1.3).

### NFR-03 — Platform Coverage

QUIC and DTLS protection shall be supported on all DefensePro platforms that support Web DDoS Behavioral Protection, including HW-ACC-accelerated platforms.

### NFR-04 — Upgrade and Backward Compatibility

Upgrading from a version without QUIC/DTLS support shall not modify existing Web DDoS profile configurations. Both new parameters shall default to Disabled, preserving pre-upgrade behavior.

---

## 9. Open Questions

**Q1** (Owner: R&D) — Which QUIC versions are in scope for the initial release — v1 only (RFC 9000), or also v2 (RFC 9369) and/or draft versions used by major CDNs?

**Q2** (Owner: R&D) — Which DTLS versions are in scope — DTLS 1.2 (RFC 6347) only, or also DTLS 1.3 (RFC 9147)?

**Q3** (Owner: R&D) — What is the capacity impact on the fingerprint table and RTS infrastructure when QUIC and DTLS fingerprints are added alongside TCP TLS? Are any table-size limits affected?

**Q4** (Owner: R&D) — How are DTLS re-handshakes handled — should the parser ignore them, or should they be treated as new ClientHellos for fingerprinting purposes?

