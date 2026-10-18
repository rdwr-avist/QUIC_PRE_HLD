//
// Created by YohayG on 07/06/2022.
//

#include "TlsParser.hpp"
#include "../../common/include/Sni.hpp"

using namespace TLSFlow::NSProcessor::Engine;

// RFC 6066 SNI list maximum length, hardened for DoS prevention
static constexpr uint16_t max_sni_list_length = 64;

bool BaseTlsParser::init(const uint8_t *const payload, uint16_t len){

    if (!payload){
        return false;
    }

    if (len < m_minimalLength) {
        return false;
    }
    m_payload       = const_cast<uint8_t*>(payload);
    m_maxOffset     = len - 1;
    m_offset        = 0;

    return true;
}



ParsingStatus TLSRecordParser::parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) {

    m_status = ParsingStatus::Failure;

    if (!init(payload, len)){
        return m_status;
    }

    m_offset        = contentTypeOffset;
    m_contentType   = getDataAtOffsetU8();
    m_offset        = tlsVersionFieldOffset;
    m_recordVersion = getDataAtOffsetU16();
    m_offset        = lengthFieldOffset;
    m_recordLen     = getDataAtOffsetU16();

    setOffset(tlsRecordHeaderLen - 1); // move offset to end of layer
    consumed = getConsumed();
    m_status  = ParsingStatus::Complete;
    return m_status;
}
bool TLSRecordParser::isSupportedVersion()
{
    if (m_status != ParsingStatus::Complete) {
        return false;
    }

    if (m_recordVersion < versionSSL3 || m_recordVersion > versionTLS12) {
        return false; // unsupported version
    }
    return true;
}

bool TLSRecordParser::isSupportedRecordType()
{
    if (m_status != ParsingStatus::Complete) {
        return false;
    }

    // Check if record type is within valid TLS record types range (20-23)
    return (m_contentType >= static_cast<uint8_t>(RecordTypesE::changeCipherSpecContent) &&
            m_contentType <= static_cast<uint8_t>(RecordTypesE::applicationDataContent));
}

bool TLSRecordParser::isHandshake(){

    if (m_status != ParsingStatus::Complete) {
        return false;
    }
    if (m_contentType == static_cast<uint8_t>(RecordTypesE::handshakeContent)) {
        return true;
    }
    return false;
}

bool TLSRecordParser::isApplicationData() const
{
    if (m_status != ParsingStatus::Complete) {
        return false;
    }
    if (m_contentType == static_cast<uint8_t>(RecordTypesE::applicationDataContent)) {
        return true;
    }
    return false;
}

ParsingStatus TlsHandshakeParser::parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) {

    m_status = ParsingStatus::Failure;

    if (!init(payload, len)){
        return m_status;
    }

    setOffset(typeOffset);
    m_handshake.type  = static_cast<HandShakeTypesE>(getDataAtOffsetU8());
    setOffset(lengthOffset);
    m_handshake.length  = getDataAtOffsetU24();
    
    // All handshake messages have the same 4-byte header: type (1) + length (3)
    setOffset(minimalHandshakeLength - 1); // move offset to end of 4-byte header

    consumed = getConsumed();
    m_valid  = ParsingStatus::Complete;
    return m_valid;
}
bool TlsHandshakeParser::isClientHello() const{
    if (m_valid != ParsingStatus::Complete){
        return false;
    }
    return (m_handshake.type == HandShakeTypesE::client_hello);
}

uint32_t TlsHandshakeParser::getHandshakeLength() const{
    if (m_valid != ParsingStatus::Complete){
        return false;
    }
    return m_handshake.length;
}

uint8_t TlsHandshakeParser::getHandshakeType() const
{
    if (m_valid != ParsingStatus::Complete){
        return static_cast<uint8_t>(RecordTypesE::invalid);
    }
    return static_cast<uint8_t>(m_handshake.type);
}


IClientHelloParser::Status ClientHelloParser::parse(const uint8_t *const payload, uint16_t bufferLen, uint32_t expectedLength, uint16_t &consumed) {

    m_status = Status::Failure;

    if (!payload) {
        return m_status;
    }

    if (bufferLen < m_minimalLength) {
		m_status = Status::IncompleteClientHelloPrefix;
        consumed = bufferLen;
        return m_status;
    }

    if (!init(payload, bufferLen)){
        return m_status;
    }

    // Parse version (first field in ClientHello body)
    if (!(setOffset(versionOffset)) ){
        return m_status;
    }
    m_hello.version = getDataAtOffsetU16();

    // Move to Random field
    if (!incOffset(versionLength)) {
        return m_status;
    }
    // Save pointer to random
    m_hello.pRandom = getDataPtrAtOffset();

    // Move to Session ID length identifier
    if (!incOffset(randomLength)) {
        return m_status;
    }
    // Read Session ID length (guaranteed by minimalHelloLength=37)
    m_hello.sidLen  = getDataAtOffsetU8();

    if (!incOffset(sessionIdLenFiledLen)){
        return m_status;
    }
    // Save pointer to Session ID
    m_hello.pSid = getDataPtrAtOffset();

    // Check if Session ID data is within payload (variable length)
    if (!incOffset(m_hello.sidLen)){
        return m_status;
    }
    // Read cipher suites length field (guaranteed by minimalHelloLength=37)
    m_hello.suitesLen = getDataAtOffsetU16() + suitesLengthFieldLen;

    // Make sure that cipher suites length is even (each cipher suite is 2 bytes long)
    if (m_hello.suitesLen % 2) {
      return m_status;
    }

    // Save pointer to Cipher Suites (including ciphers length field)
    m_hello.pCipherSuits = getDataPtrAtOffset();

    // Check if all ciphers are in payload and skip
    if (!incOffset(m_hello.suitesLen)) {
        return m_status;
    }

    // Check if compression methods length filed is within payload
    if (!canReadLength(compressionsLengthFieldLen)){
        return m_status;
    }
    // Read compression methods length field
    m_hello.compressionMethodsLen = getDataAtOffsetU8();

    // Skip compression methods length field
    if (!incOffset(compressionsLengthFieldLen)){
        return m_status;
    }

    // Save pointer to Compression Methods
    m_hello.pCompressionMethods = getDataPtrAtOffset();

    m_hello.extensionLen = 0;
    m_hello.pExtensions  = nullptr;

    // Check if extensions exist by comparing against expectedLength
    // (no extensions means no extension length field either)
    if (getOffset() + m_hello.compressionMethodsLen == expectedLength){
        // No extensions - ClientHello ends after compression methods
        
        // Defense-in-depth: Verify compression methods data actually exists in buffer
        // This prevents trusting a malicious compressionMethodsLen that claims more data than present
        if (!canReadLength(m_hello.compressionMethodsLen)) {
            m_status = Status::Failure;
            return m_status;
        }
        
        consumed = getConsumed() + m_hello.compressionMethodsLen;
        
        // Validate lengths (no extension length field in packet)
        if (!validateLengths(expectedLength, false)) {
            m_status = Status::Failure;
            return m_status;
        }
        
        m_status  = Status::Complete;
        return m_status;
    }

    // Skip all compression methods
    if (!incOffset(m_hello.compressionMethodsLen)){
        m_status = Status::IncompleteClientHelloBody;
        return m_status;
    }

    // Check if extensions length filed is within payload
    if (!canReadLength(extensionLengthFieldLen)){
        m_status = Status::IncompleteClientHelloBody;
        return m_status;
    }
    // Read extensions length field
    m_hello.extensionLen = getDataAtOffsetU16();

    // Validate that sum of all field lengths matches expected ClientHello length
    // Extension length field IS present in packet (even if extensionLen=0)
    if (!validateLengths(expectedLength, true)) {
        m_status = Status::Failure;
        return m_status;
    }

    // Skip extensions length field
    if (!incOffset(extensionLengthFieldLen)){
        return m_status;
    }

    // Save pointer to extensions
    m_hello.pExtensions = getDataPtrAtOffset();

    // Skip all extensions
    if (m_hello.extensionLen == 0) {
        consumed = getConsumed();
        m_status = Status::Complete;
        return m_status;
    }

    if (!incOffset(m_hello.extensionLen - 1)){
        m_status = Status::IncompleteExtensionSection;
        consumeAll();
        consumed = getConsumed();
        return m_status;
    }

    consumed = getConsumed();
    
    m_status  = Status::Complete;
    return m_status;
}

bool ClientHelloParser::validateLengths(uint32_t expectedLength, bool hasExtensionLengthField) const
{
    // Calculate sum of all field lengths
    uint32_t sumFieldsLength = versionLength                     // 2 bytes
                              + randomLength                      // 32 bytes
                              + sessionIdLenFiledLen              // 1 byte
                              + m_hello.sidLen                    // variable
                              + m_hello.suitesLen                 // variable (includes 2-byte length field)
                              + compressionsLengthFieldLen        // 1 byte
                              + m_hello.compressionMethodsLen;    // variable
    
    // Only add extension length field if it exists in the packet
    // (no extensions path = no extension length field per RFC 5246)
    if (hasExtensionLengthField) {
        sumFieldsLength += extensionLengthFieldLen;    // 2 bytes (always present if field exists)
        sumFieldsLength += m_hello.extensionLen;       // variable (may be 0)
    }
                               
    if (expectedLength != sumFieldsLength) {
        return false;
    }                
    
    return true;
}


IExtensionsParser::Status ExtensionsParser::parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) {

    m_status = Status::Failure;

    m_extensionType = 0;
    m_extensionLen = 0;
    m_extensionDataPtr = nullptr;

    if (!init(payload, len)) {
        return m_status;
    }
    m_status = Status::PendingTypeField;

    if (len == 0) {
        return m_status;
    }

    if (!canReadLength(1 )){ //
        return m_status;
    }
    m_status = Status::IncompleteExtensionTypeField;
    if (!canReadLength(m_extensionTypeFieldLen)){
        uint8_t  firstByte = getDataAtOffsetU8();
        m_extensionType = firstByte << 8;
        return m_status;
    }

    // Got the complete extension type field
    m_extensionType = getDataAtOffsetU16();

    m_status = Status::PendingLengthField;
    if (!incOffset(m_extensionTypeFieldLen)){
        return m_status;
    }

    m_status = Status::IncompleteExtensionLengthField;
    if (!canReadLength(m_extensionLengthFieldLen)){
        uint8_t  firstByte = getDataAtOffsetU8();
        m_extensionLen = firstByte << 8;
        return m_status;
    }

    // Got the complete extension Length Field
    m_extensionLen = getDataAtOffsetU16();
    const uint16_t extensionDataBytes =
        static_cast<uint16_t>(len - m_extensionTypeFieldLen - m_extensionLengthFieldLen);
    if(m_extensionLen > extensionDataBytes) {
        m_status = Status::Failure;
        return m_status; // extension length exceeds available data
    }

    m_status = Status::PendingExtensionData;

    if (0 == m_extensionLen){
        incOffset(m_extensionLengthFieldLen - 1);
        consumed = getConsumed();
        m_status = Status::Complete;
        return m_status;
    }

    if (!incOffset(m_extensionLengthFieldLen)){
        return m_status;
    }

    m_status = Status::IncompleteExtensionData;

    m_extensionDataPtr = getDataPtrAtOffset();

    if(isExtensionDataValid() == false) {
        m_status = Status::Failure;
        return m_status; // invalid extension data pointer
    }

    if (!incOffset(m_extensionLen - 1)){
        return m_status;
    }
    consumed = getConsumed();
    m_status = Status::Complete;
    return m_status;
}

bool ExtensionsParser::isExtensionDataValid() const
{
    uint32_t numOfBytesOfDataLengthField = getNumOfBytesOfDataLengthField();

    if (numOfBytesOfDataLengthField == 0) {
        // Extension doesn't have simple length field at byte 0
        // Check if it needs specific validation
        return validateComplexExtension();
    }
    
    // Check if extension has enough bytes for the length field
    if (m_extensionLen < numOfBytesOfDataLengthField) {
        return false;  // Malformed: extension too small for its type
    }

    uint16_t extensionDataLength{0};

    if(numOfBytesOfDataLengthField == 2)
    {
        extensionDataLength = (m_extensionDataPtr[0] << 8) | m_extensionDataPtr[1];
    }
    else if(numOfBytesOfDataLengthField == 1)
    {
        extensionDataLength = m_extensionDataPtr[0];
    }
    
    if(extensionDataLength + numOfBytesOfDataLengthField > m_extensionLen)
    {
        return false; // Malformed: inner length exceeds extension length
    }

    return true;
}

bool ExtensionsParser::validateComplexExtension() const
{
    switch (static_cast<ExtensionTypeE>(m_extensionType)) {
        case ExtensionTypeE::status_request:
            // RFC 6066: Byte 0 = status_type, then responder_id_list with 2-byte length
            if (m_extensionLen < 1) return false;
            {
                uint8_t status_type = m_extensionDataPtr[0];
                // Valid status types: 1 (ocsp), 2 (ocsp_multi)
                if (status_type != 1 && status_type != 2) return false;
                
                // If there's more data, validate responder_id_list length
                if (m_extensionLen >= 3) {
                    uint16_t responder_list_len = (m_extensionDataPtr[1] << 8) | m_extensionDataPtr[2];
                    if (responder_list_len + 3 > m_extensionLen) return false;
                }
            }
            return true;

        case ExtensionTypeE::status_request_v2:
            // Similar to status_request but with version 2 structure
            if (m_extensionLen < 3) return false;
            {
                uint8_t status_type = m_extensionDataPtr[2];
                if (status_type != 1 && status_type != 2) return false;
            }
            return true;

        case ExtensionTypeE::pre_shared_key:
            // RFC 8446: identities (2-byte length) then binders (2-byte length)
            // Too complex to validate fully here without knowing context
            // At minimum, check we have room for identities length
            if (m_extensionLen < 2) return false;
            {
                uint16_t identities_len = (m_extensionDataPtr[0] << 8) | m_extensionDataPtr[1];
                // Identities + binders length field (2 bytes minimum)
                if (identities_len + 4 > m_extensionLen) return false;
            }
            return true;

        case ExtensionTypeE::key_share:
            // RFC 8446: Different structure for ClientHello vs ServerHello
            // ClientHello: client_shares length (2 bytes) + shares
            // ServerHello: single KeyShareEntry (no length prefix)
            // For safety, just check minimum size
            if (m_extensionLen < 2) return false;
            return true;

        // Extensions without inner length fields - no validation needed
        case ExtensionTypeE::session_ticket:
        case ExtensionTypeE::heartbeat:
        case ExtensionTypeE::extended_master_secret:
        case ExtensionTypeE::padding:
        default:
            // No specific validation for these extensions
            return true;
    }
}

uint32_t ExtensionsParser::getNumOfBytesOfDataLengthField() const
{
    switch (static_cast<ExtensionTypeE>(m_extensionType)) {
        /* Extensions with 2-byte length field AT BYTE 0 */
        case ExtensionTypeE::server_name:                               // RFC 6066: ServerNameList length
        case ExtensionTypeE::supported_groups:                          // RFC 8422: NamedGroupList length
        case ExtensionTypeE::signature_algorithms:                      // RFC 5246: supported_signature_algorithms length
        case ExtensionTypeE::application_layer_protocol_negotiation:    // RFC 7301: ProtocolNameList length
        case ExtensionTypeE::signature_algorithms_cert:                 // RFC 8446: supported_signature_algorithms length
        case ExtensionTypeE::certificate_authorities:                   // RFC 8446: authorities length
        case ExtensionTypeE::cookie:                                    // RFC 8446: cookie length
            return 2;

        /* Extensions with 1-byte length field AT BYTE 0 */
        case ExtensionTypeE::ec_point_formats:                          // RFC 4492: ECPointFormatList length
        case ExtensionTypeE::supported_versions:                        // RFC 8446: versions length (ClientHello)
        case ExtensionTypeE::psk_key_exchange_modes:                    // RFC 8446: ke_modes length
        case ExtensionTypeE::compress_certificate:                      // RFC 8879: algorithms length
            return 1;

        /* Extensions with complex structure - validated in validateComplexExtension() */
        case ExtensionTypeE::status_request:        // Has status_type (1 byte) BEFORE length fields
        case ExtensionTypeE::status_request_v2:     // Has status_type (1 byte) BEFORE length fields
        case ExtensionTypeE::pre_shared_key:        // Multiple length fields (identities + binders)
        case ExtensionTypeE::key_share:             // Different structure in ClientHello vs ServerHello
        
        /* Extensions without inner length fields */
        default:
            return 0;
    }
}



ParsingStatus SupportedVersionsExtensionParser::parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) {

    m_status = ParsingStatus::Failure;

    if (!init(payload, len)) {
        return m_status;
    }

    if (!canReadLength(2)){
        return m_status;
    }

    mVersionsLength = getDataAtOffsetU8();

    if (0 == mVersionsLength){
        // TBD should we return error? is this valid?
        incOffset( m_numOfVersionsValueLength - 1);
        m_versionPtr = nullptr;
        return ParsingStatus::Complete;

    }

    if (!incOffset(m_numOfVersionsValueLength)){
        return m_status;
    }

    if (!canReadLength(mVersionsLength)){
        return m_status;
    }

    m_versionPtr = getDataPtrAtOffsetU16();

    if (!incOffset(mVersionsLength - 1)){
        return m_status;
    }
    consumed = getConsumed();
    m_status = ParsingStatus::Complete;
    return m_status;
}

ParsingStatus SupportedGroupsExtensionParser::parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) {

    m_status = ParsingStatus::Failure;

    if (!init(payload, len)) {
        return m_status;
    }

    if (!canReadLength(2)){
        return m_status;
    }
    m_groupsLength = getDataAtOffsetU16();

    if (0 == m_groupsLength){
        // TBD should we return error? is this valid?
        incOffset(m_numOfGourpsValueLength - 1);
        m_groupPtr = nullptr;
        return ParsingStatus::Complete;

    }
    if (!incOffset(m_numOfGourpsValueLength)){
        return m_status;
    }

    if (!canReadLength(m_groupsLength)){
        return m_status;
    }

    m_groupPtr = getDataPtrAtOffsetU16();

    if (!incOffset(m_groupsLength - 1)){
        return m_status;
    }
    consumed = getConsumed();
    m_status = ParsingStatus::Complete;
    return m_status;

}

ParsingStatus CipherSuitesParser::parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) {

    m_status = ParsingStatus::Failure;

    if (!init(payload, len)) {
        return m_status;
    }

    if (!canReadLength(m_ciphersSuiteValueLength)){
        return m_status;
    }

    m_suitesLength = getDataAtOffsetU16();

    if (!incOffset(m_ciphersSuiteValueLength)){
        return m_status;
    }

    if (!canReadLength(m_suitesLength)){
        return m_status;
    }

    m_pSuits = getDataPtrAtOffsetU16();

    if (!incOffset(m_suitesLength - 1)){
        return m_status;
    }
    consumed = getConsumed();
    m_status = ParsingStatus::Complete;
    return m_status;

}

ParsingStatus SniParser::parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) {

	m_status = ParsingStatus::Failure;

    if (!init(payload, len)) {
        return m_status;
    }
    if (!canReadLength(sniListFieldLength)){
        return m_status;
    }
    m_SniListLength = getDataAtOffsetU16();

    // RFC 6066 + DoS hardening: SNI list must not exceed 64 bytes
    if (m_SniListLength > max_sni_list_length) {
        return m_status;
    }

    if (!incOffset(sniListFieldLength)){
        return m_status;
    }

    uint16_t  currSniLen;
    m_numOfSnis = 0;
    while (canReadLength(1)) {
        if (m_numOfSnis < max_snis_supported) {
            m_sniTypes[m_numOfSnis] = getDataAtOffsetU8();
        }
        if (!incOffset(sniTypeFieldLength)){
            return m_status;
        }
        if (!canReadLength(sniLengthFieldLength)){
            return m_status;
        }
        currSniLen = getDataAtOffsetU16();

        // RFC 6066: SNI name must be at least 1 byte and at most 255 bytes.
        // Zero-length names are malformed and should be rejected.
        if (currSniLen == 0 || currSniLen > TLSFlow::max_sni_length) {
            return m_status;
        }
        if (m_numOfSnis < max_snis_supported) {
            m_sniLengths[m_numOfSnis] = currSniLen;
        }
        if (!incOffset(sniLengthFieldLength)){
            return m_status;
        }
        if (m_numOfSnis < max_snis_supported) {
            m_sniPointers[m_numOfSnis] = getDataPtrAtOffset();
        }
        if (!incOffset(currSniLen - 1)){
            return m_status;
        }
        // Ended parsing of SNI record, count it
        m_numOfSnis++;
        // Check if we have parsed the complete list of SNIs
        if (getConsumed() == m_SniListLength + sniListFieldLength) {
            m_status = ParsingStatus::Complete;
            break;
        }
    	if (getConsumed() > m_SniListLength + sniListFieldLength){
    		// Consumed somehow more than the declared size, could be malformed extension on top level where the input len function argument is too long
    		return m_status;
    	}
    	// prepare to next SNI
    	if (!incOffset(1)) {
    		return m_status;
    	}
    }

    return m_status;
}

ParsingStatus TlsAlertParser::parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed)
{
    m_status = ParsingStatus::Failure;
    consumed = 0;

    if (!init(payload, len))
    {
        return m_status;
    }

    m_alertLevel = getDataAtOffsetU8();
    incOffset(1);
    m_alertDescription = getDataAtOffsetU8();
    consumed = getConsumed();
    m_status = ParsingStatus::Complete;
    return m_status;
}

