# SRS Template

## Document Metadata

- Document status: Draft 
- Feature name: Web DDoS TLSFP support over QUIC
- Feature slug: Web DDoS over QUIC
- Release version:
- Document version:
- Date (YYYY-MM-DD): 22/09/2026
- Author: Simcha Stern
- Reviewers:
- Related FRD path:

## Change and Revision History

- Version:
    - Date:
    - Author:
    - Summary of changes:

## 1. Introduction
- TLSFP Web DDoS is supported for HTTPS over TCP.
- Other protocols as QUIC and DTLS support TLS too and can be protected by TLSFP Web DDos as well. 
- we will add support in this release for TLSFP WEb DDoS for the QUIC protocol which is used for HTTP/3.
- for the first stage, we will use the same Web DDoS baseline for both TCP and QUIC TLSFP detection and mitigation.
- in the future we will consider splitting the baseline by protocols.


### 1.1 Key Goals and Motivations

- UDP packets with a destination port in the **QUIC Application port** group will be parsed for QUIC.
- DP will look for the initial QUIC packet with the client hello CRYPTO frame send from the client.
- the TLSFP is calculated for QUIC similar to the way it is parsedf  in case of TCP.
- 

### 1.2 General Concept of Operation

- High-level behavior:
- In-scope interfaces: CLI | UI | REST | SNMP | Syslog
- Out-of-scope items:

### 1.3 Related Documents

- FRD:
- HLTP:
- OnePager:
- Design notes:

## 2. Interface Contracts

### 2.1 CLI Command Contracts

- there is a new flag to control if we support QUIC along with HTTPS for TLSFP.
- the new flag is part of the webddos profile.
- the new flag will influence all TLSFP detection and mitigation methods, as manualt filters, behavioral, reputation etc.
- dp web-ddos-protection profiles add/del/get/set \<profile name\>  \<-switch value\>   
    -quic : quic protection enabled

### 2.2 Internal, Debug, and Visibility Commands

- add the Total policy hits and TLSFP hits per HTTPS and QUIC to the system internal comamnds
- Total policy hits per protocol comamnd
    - "system internal security web-ddos-protection statistics policy **hits** \<all| policy-name\>" 
    - display the following coloumns, policy name,  HTTPS hits/sec, QUIC hits/sec, Total hits/sec.
- Policy TLSFP hits per protocol command
    - "system internal security web-ddos-protection statistics fingerprints **hits** \<all | policy-name\> \<switch\>"
        - -f Writes the output to a file on the device.
        - -c \<number-of-entries\> Displays on the terminal the first x elements (maximum 200).
    - display the following coloumns, policy name, Fingerprint, HTTPS hits/sec, QUIC hits/sec, Total hits/sec

### 2.3 UI Page Contracts

- add the new QUIC flag `quic-protection-enabled` to UI according to FRD.

### 2.4 REST API Contracts

- no change

### 2.5 SNMP and Syslog Contracts


## 3. Data and Persistence

### 3.1 Data Model and CDB

- new flag on profile should be kept to CDB

### 3.2 Save and Reboot Behavior

- new flag on profile should be persist over reboot

### 3.3 Import and Export

- new flag is part of profile and shold be import/export

## 4. System Requirements - Compliance Matrix

### 4.1 User Interface Requirements

- none

### 4.2 Configuration Requirements

- the default for QUIC is disble

### 4.3 Run-time Functional Requirements

- DP will add support for QUIC parsing in order to retrive the Handshake Client Hello message
- DP will add support to create a QUIC Initial packet with a CONNECTION_CLOSE frame in case of TLSFP mitigation. 
- detetcion and mistigation for TLSFP will be based on TLSFP collected from both TCP and QUIC sessions.
- no change to reporting.

#### 4.3.1 QUIC TLS Client Hello parser

- QUIC **packets** are carried over UDP datagrams.
    - one UDP datagram can hold several QUIC packets.
- QUIC packets contain one or more QUIC **frames**.
- a QUIC connection starts with a TLS handshake.
- TLS handshake messages are carried directly over QUIC frames and not over TLS records as in TCP.
- TLS handshake messages are carried over QUIC **CRYPTO** frames.
    - a TLS handhskae data can be split over a few CRYPTO frames.
- The TLS **Client Hello** message is carried over the QUIC Initial packet.
    -  the Client Hello message can be carried over a few QUIC initial packets.
- TLS over QUIC uses TLS version 1.3 and newer.
    - lower TLS version are consider a violation.
- the parser will be looking for the Client Hello HS message over QUIC initial packets and CRYPTO frames.
- parse the QUIC initial packet for the Client Hello message which is a CRYPTO frame.
    - parse UDP packets with the detination port included in the **QUIC application port** group
    - check for the QUIC **Inital** packet header as **0xC0**
    - check for the **CRYPTO** frame type **0x06**
    - check for handshake type as **0x01** for **Client Hello** TLS message

#### 4.3.2 TLSFP calculation

- TLSFP will be calculated for QUIC the same way it is calculated for TCP.
- the TLSFP generated from QUIC packet will be add to the statistics table in the same way it is added for a TCP packet.
- the TOP TLSFP will include both TLSFP generated by QUIC and TCP. 
- do not calculate TLSFP for UDP retransmitted packets.

#### 4.3.3 TLSFP protection mitigation

- in case a TLSFP is part of an attack, either manual, behavioral or ondemand, and a block action is required, the QUIC connection will be closed.
- DP should send an **Initial** QUIC packet containing a CONNECTION_CLOSE frame with error code as CONNECTION_REFUSED.
- the packet DCID is set to the SCID value from the original client hello packet 
- the packet SCID should be set by DP. (as the client hello is the first packet received). 

#### 4.3.4 TLSFP statistics

- add statistics for Policy Total hits per protocol, HTTPS and QUIC.  
- add statistics for Policy TLSFP hits per protocol, HTTPS and QUIC.
- the new statistics should be displayed on new statistics system internal commands.
- the new statitics are informative only and have no affect on detection process on this release.

#### 4.3.5 Manual filters and reputation feed

- treat TLSFP manual filters on QUIC traffic in case QUIC is enabled on the profile attached to policy.
- treat TLSFP reputation feed on QUIC traffic in case QUIC is enabled on the profile attached to the policy 
- compare the QUIC TLSFP to the Manual filters or reputation feed for mitigation.

#### 4.3.6 Behavioral detection & mitigation 

- we will use one behavioral baseline for HTTPS and QUIC
- the detection and mitigation will be based on the Total hit count per policy and TLSFP hit count for both the protocols, as one.
- in case of mitigation of a TLSFP, both HTTPS and QUIC connections will be closed as result of TLSFP match.
- the above is true for ramdomize detection too.
- we should check if the TLSFP baseline capacity should be incresaed, as both protocols may use different TLSFP values.

#### 4.4.7 on demand 

- both TLSFP from HTTPS and QUIC will be learned for on demand detection.
- we will have to check if the on demand algoritim works when both protocols are counted together. TBD.

#### 4.4.8 TLS enforement 

- TLS enforcemnt will not be supported for QUIC for this version.
    - the existing configuration settings do not suit QUIC.
    - different protocols have different condition restrictions.
- note that QUIC uses TLS 1.3 (and up)
    - older TLS versions are considered a viloation.

#### 4.4.9 TLS visibility 

- add the Client Hello to TLS visibality 

### 4.4 API Requirements

- API-01: Realizes FRD ID:
- API-02: Realizes FRD ID:

### 4.5 Reporting Requirements

-  no change to reporting

### 4.6 Error Handling Requirements

- ERR-01: Realizes FRD ID:
- ERR-02: Realizes FRD ID:

### 4.7 Debugging Requirements

- DBG-01: Realizes FRD ID:
- DBG-02: Realizes FRD ID:

### 4.8 Performance Requirements

- PERF-01: Realizes FRD ID:
- PERF-02: Realizes FRD ID:

### 4.9 Capacity Requirements

- CAP-01: Realizes FRD ID:
- CAP-02: Realizes FRD ID:

## 5. Values, Boundaries, and Limits

### 5.1 Defaults

- DEF-01:
    - Realizes FRD ID:
    - Parameter:
    - Factory default (exact):

### 5.2 Numeric Ranges

- RNG-01:
    - Realizes FRD ID:
    - Parameter:
    - Minimum:
    - Maximum:
    - Step:
    - Unit:

### 5.3 Invalid Value Handling

- INV-01:
    - Realizes FRD ID:
    - Invalid value cases:
    - Expected error message or code (exact):

### 5.4 Scale Limits

- LIM-01:
    - Realizes FRD ID:
    - Max entries:
    - Max character length:
    - Max policies or objects:

## 6. Pre-Conditions and Post-Conditions

- OP-01:
    - Realizes FRD ID:
    - Operation name:
    - Preconditions:
    - Postconditions:
    - Rollback behavior:

## 7. Operational Coverage for HLTP Areas

- HLTP-Display-Values: Applicable | Not applicable. Evidence sections:
- HLTP-Default-Values: Applicable | Not applicable. Evidence sections:
- HLTP-Edge-Values: Applicable | Not applicable. Evidence sections:
- HLTP-Max-Characters-Entries: Applicable | Not applicable. Evidence sections:
- HLTP-Invalid-Values: Applicable | Not applicable. Evidence sections:
- HLTP-Enable-Disable: Applicable | Not applicable. Evidence sections:
- HLTP-Save-Reboot: Applicable | Not applicable. Evidence sections:
- HLTP-Config-Import-Export: Applicable | Not applicable. Evidence sections:
- HLTP-CDB: Applicable | Not applicable. Evidence sections:
- HLTP-Tech-Support: Applicable | Not applicable. Evidence sections:
- HLTP-CLI-Help-Tooltips: Applicable | Not applicable. Evidence sections:
- HLTP-Upgrade: Applicable | Not applicable. Evidence sections:
- HLTP-Functional-Under-Traffic: Applicable | Not applicable. Evidence sections:
- HLTP-Debug-Commands: Applicable | Not applicable. Evidence sections:
- HLTP-CLI-Traps: Applicable | Not applicable. Evidence sections:
- HLTP-Vision-CC-Dashboard-Monitoring: Applicable | Not applicable. Evidence sections:
- HLTP-Syslog-Server: Applicable | Not applicable. Evidence sections:
- HLTP-Config-Changes-During-Traffic: Applicable | Not applicable. Evidence sections:
- HLTP-Reboot-Scenarios: Applicable | Not applicable. Evidence sections:
- HLTP-Negative-Tests: Applicable | Not applicable. Evidence sections:
- HLTP-Max-Config: Applicable | Not applicable. Evidence sections:
- HLTP-Cross-Functionality: Applicable | Not applicable. Evidence sections:
- HLTP-Backward-Compatibility: Applicable | Not applicable. Evidence sections:
- HLTP-Stress-Long-Runs: Applicable | Not applicable. Evidence sections:
- HLTP-REST-API-Tests: Applicable | Not applicable. Evidence sections:
- HLTP-Performance: Applicable | Not applicable. Evidence sections:

## 8. High-Level Design

### 8.1 Architecture Summary

- Components:
- Data flow:
- Control flow:

### 8.2 Key Interactions

- Interaction 1:
- Interaction 2:

## 9. Security

### 9.1 Authentication and Authorization

- SEC-AUTH-01:
- SEC-AUTH-02:

### 9.2 Input Validation and Sanitization

- SEC-INP-01:
- SEC-INP-02:

### 9.3 Data Protection

- SEC-DAT-01:
- SEC-DAT-02:
- SEC-DAT-03:

### 9.4 Attack Surface and Hardening

- SEC-HARDEN-01:
- SEC-HARDEN-02:
- SEC-HARDEN-03:

### 9.5 Audit and Logging

- SEC-LOG-01:
- SEC-LOG-02:

### 9.6 Security Testing Requirements

- SEC-TEST-01:
- SEC-TEST-02:

## 10. Open Issues

- ISSUE-01:
- ISSUE-02:

## 11. Risks

- RISK-01:
    - Description:
    - Impact:
    - Likelihood:
    - Mitigation:
- RISK-02:
    - Description:
    - Impact:
    - Likelihood:
    - Mitigation:

## 12. Definition of Ready Checklist

- [ ] Document version and revision history completed.
- [ ] Every requirement references at least one FRD ID.
- [ ] All CLI contracts include exact syntax and exact success and error outputs.
- [ ] Internal or debug verification commands are fully specified with exact outputs and access level.
- [ ] All UI contracts include navigation, stable locators, validations, and post-action state.
- [ ] All REST contracts include request or response schema and status codes.
- [ ] Defaults, ranges, steps, units, and limits are exact.
- [ ] Invalid-value handling includes exact error message or code.
- [ ] Volatile fields are flagged.
- [ ] Pre-conditions and post-conditions are defined per operation.
- [ ] SNMP and Syslog behavior is specified or explicitly marked not applicable.
- [ ] No unresolved placeholders remain in release-ready documents.

## Appendix (Optional)

### A. CLI Output Examples

```text
<paste exact success output>
```

```text
<paste exact error output>
```

### B. REST Examples

```json
{
    "example": "request"
}
```

```json
{
    "example": "response"
}
```

### C. UI Locator Reference

- Page:
- Locator:
- Control type:
