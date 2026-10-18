//
// Created by YohayG on 07/06/2022.
//

#pragma once

#include <stdint.h>
#include "ITLSParser.hpp"
namespace TLSFlow {
    namespace NSProcessor {

        namespace Engine {

            class BaseTlsParser {
            public:
                explicit BaseTlsParser(uint16_t min = 100) : m_minimalLength{min} {};

                virtual ~BaseTlsParser() = default;

                bool init(const uint8_t *const payload, uint16_t len);

            protected:

                inline uint8_t *getDataPtrAtOffset() const { return m_payload + m_offset; }

                inline uint8_t getDataAtOffsetU8() const { return *getDataPtrAtOffset(); }

                inline uint16_t *
                getDataPtrAtOffsetU16() const { return reinterpret_cast<uint16_t *>(getDataPtrAtOffset()); }

                inline uint16_t getDataAtOffsetU16() const { return htons(*getDataPtrAtOffsetU16()); }

                inline uint32_t *
                getDataPtrAtOffsetU32() const { return reinterpret_cast<uint32_t *>(getDataPtrAtOffset()); }

                inline uint32_t getDataAtOffsetU32() const { return htonl(*getDataPtrAtOffsetU32()); }

                inline uint32_t getDataAtOffsetU24() const { return (getDataAtOffsetU32()) >> 8; }

                inline uint16_t getOffset() const { return m_offset; }

                inline uint16_t getConsumed() const { return m_offset + 1; }

                inline void     consumeAll() { m_offset = m_maxOffset; }

                inline uint16_t canReadLength(uint16_t inc) const {
                    return (m_offset + inc == 0) || isOffsetInRange(m_offset + inc - 1);
                }

                inline bool canIncOffset(uint16_t inc) const {
                    uint32_t sum = (uint32_t)m_offset + (uint32_t)inc;
                    if (sum > UINT16_MAX) return false;
                    return isOffsetInRange((uint16_t)sum);
                }

                inline bool isOffsetInRange(uint16_t offset) const { return (offset <= m_maxOffset); }

                inline bool incOffset(uint16_t inc) {
                    if (!canIncOffset(inc)) return false;
                    m_offset += inc;
                    return true;
                }

                inline bool setOffset(uint16_t offset) {
                    if (!isOffsetInRange(offset)) return false;
                    m_offset = offset;
                    return true;
                }

                uint8_t *m_payload = nullptr;
                uint16_t m_maxOffset{};
                uint16_t m_offset{};
                uint16_t m_minimalLength;

            private:

            };

            class TLSRecordParser : public BaseTlsParser, public IRecordParser {
            public:
                TLSRecordParser() : BaseTlsParser{minimalTlsLength} {
                    m_contentType = static_cast<uint8_t>(RecordTypesE::invalid);
                    m_recordVersion = 0;
                    m_recordLen = 0;
                }

                ~TLSRecordParser() = default;

                ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) override;

                bool isHandshake() override;

                bool isApplicationData() const override;

                bool isSupportedVersion() override;

                bool isSupportedRecordType() override;

                using BaseTlsParser::getOffset;

                uint16_t getTlsVersion() const { return m_recordVersion; }

                uint16_t getTlsRecordLength() const { return m_recordLen; }

                uint8_t getRecordType() const override { return m_contentType; }

            private:                

                typedef enum {
                    versionSSL3 = 0x0300,
                    versionTLS12 = 0x0303
                } Version;

                static const uint16_t minimalTlsLength = 5;
                static const uint16_t contentTypeOffset = 0;
                static const uint16_t tlsVersionFieldOffset = 1;
                static const uint16_t lengthFieldOffset = 3;
                static const uint16_t tlsRecordHeaderLen = 5;


                uint8_t m_contentType;
                uint16_t m_recordVersion;
                uint16_t m_recordLen;
                ParsingStatus m_status = ParsingStatus::Failure;

            };

            class TlsHandshakeParser : public BaseTlsParser, public IHandshakeParser {
            public:
                TlsHandshakeParser() : BaseTlsParser(minimalHandshakeLength) {
                    m_handshake.length = 0;
                    m_handshake.type = HandShakeTypesE::hello_request_RESERVED;
                }

                ~TlsHandshakeParser() = default;

                using BaseTlsParser::getOffset;

                ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) override;

                bool isClientHello() const override;

                uint32_t getHandshakeLength() const override;

                uint8_t getHandshakeType() const override;

            private:

                ParsingStatus m_valid = ParsingStatus::Failure;
                ParsingStatus m_status = ParsingStatus::Failure;

                struct handshake {
                    HandShakeTypesE type;
                    uint32_t length;
                } m_handshake{};

                static const uint8_t typeOffset = 0;
                static const uint16_t lengthOffset = 1;
                static const uint16_t minimalHandshakeLength = 4; // Handshake header: type (1 byte) + length (3 bytes)

            private:

            };


            class ClientHelloParser : public BaseTlsParser, public IClientHelloParser {
            public:
                ClientHelloParser() : BaseTlsParser(minimalHelloLength) {
                    m_hello = {0};
                }

                ~ClientHelloParser() = default;

                using BaseTlsParser::getOffset;

                IClientHelloParser::Status parse(const uint8_t *const payload, uint16_t bufferLen, uint32_t expectedLength, uint16_t &consumed) override;

                uint8_t getsidLen() const override { return m_hello.sidLen; }

                const uint8_t *getpRandom() const override { return m_hello.pRandom; }

                const uint8_t *getpSid() const override { return m_hello.pSid; }

                uint16_t getCipherSuitesLen() const override { return m_hello.suitesLen; }

                const uint8_t *getpCipherSuites() const override { return m_hello.pCipherSuits; }

                uint8_t getCompressionLen() const override { return m_hello.compressionMethodsLen; }

                const uint8_t *getpCompressionMethods() const override { return m_hello.pCompressionMethods; }

                uint16_t getExtensionsLen() const override { return m_hello.extensionLen; }

                const uint8_t *getpExtensions() const override { return m_hello.pExtensions; }

                uint16_t getVersion() const override { return m_hello.version; }

            private:

                static const uint16_t minimalHelloLength = 37; // version(2) + random(32) + sidLen(1) + minCipherSuitesLen(2) + compressionLen(1) + minCompression(1) = 37
                static const uint16_t versionOffset = 0;
                static const uint16_t versionLength = 2;
                static const uint16_t randomOffset = 2;
                static const uint16_t randomLength = 32;
                static const uint16_t sessionIdLenFiledLen = 1;
                static const uint16_t suitesLengthFieldLen = 2;
                static const uint16_t compressionsLengthFieldLen = 1;
                static const uint16_t extensionLengthFieldLen = 2;

                IClientHelloParser::Status m_status = Status::Failure;



                struct clientHello {
                    uint16_t version;
                    uint8_t sidLen;
                    uint8_t *pRandom;
                    uint8_t *pSid;
                    uint16_t suitesLen;
                    uint8_t *pCipherSuits;
                    uint8_t compressionMethodsLen;
                    uint8_t *pCompressionMethods;
                    uint16_t extensionLen;
                    uint8_t *pExtensions;
                } m_hello;


            private:

                bool validateLengths(uint32_t expectedLength, bool hasExtensionLengthField) const;
            };


            class ExtensionsParser : public BaseTlsParser, public IExtensionsParser {
            public:
                ExtensionsParser() : BaseTlsParser(0) {
                }

                ~ExtensionsParser() = default;

                using BaseTlsParser::getOffset;

                IExtensionsParser::Status parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) override;

                inline uint16_t getExtensionDataLength() const override { return m_extensionLen; }

                inline uint16_t getSingleExtensionTotalLength() const override {
                    return getExtensionDataLength() + m_extensionTypeFieldLen + m_extensionLengthFieldLen;
                }

                ExtensionTypeE getExtensionType() const override { return static_cast<ExtensionTypeE>(m_extensionType); }

                const uint8_t *getExtensionValuePtr() const override { return m_extensionDataPtr; }


            private:
                uint16_t m_extensionLen;
                uint16_t m_extensionType;
                uint8_t *m_extensionDataPtr;
                static constexpr uint16_t m_extensionTypeFieldLen = 2;
                static constexpr uint16_t m_extensionLengthFieldLen = 2;
                IExtensionsParser::Status m_status = Status::Failure;

                bool isExtensionDataValid() const;
                bool validateComplexExtension() const;
                uint32_t getNumOfBytesOfDataLengthField() const;
            };


            class SupportedVersionsExtensionParser : public BaseTlsParser, public ISupportedVersionsExtensionParser {
            public:
                SupportedVersionsExtensionParser() : BaseTlsParser(0) {
                }

                ~SupportedVersionsExtensionParser() = default;

                using BaseTlsParser::getOffset;

                ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) override;

                inline uint8_t getLength() const override { return mVersionsLength; }

                inline uint16_t getNumOfVersions() const override {
                    return getLength() ? getLength() / m_supportedVersionValueLength : 0;
                }

                uint16_t getVersion(uint16_t index) const override { return htons(m_versionPtr[index]); }

            private:
                uint8_t mVersionsLength;
                uint16_t *m_versionPtr;

                static constexpr uint16_t m_supportedVersionValueLength = 2;
                static constexpr uint16_t m_numOfVersionsValueLength = 1;

                ParsingStatus m_status = ParsingStatus::Failure;

            };


            class SupportedGroupsExtensionParser : public BaseTlsParser, public ISupportedGroupsExtensionParser {
            public:
                SupportedGroupsExtensionParser() : BaseTlsParser(0) {
                }

                ~SupportedGroupsExtensionParser() = default;

                using BaseTlsParser::getOffset;

                ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) override;

                inline uint16_t getGroupsLen() const override { return m_groupsLength; }

                inline uint16_t getNumOfGroups() const override {
                    return getGroupsLen() ? getGroupsLen() / m_supportedGroupValueLength : 0;
                }

                inline uint16_t getGroup(uint16_t index) const override { return htons(m_groupPtr[index]); }

            private:
                uint16_t m_groupsLength;
                uint16_t *m_groupPtr;
                static constexpr uint16_t m_supportedGroupValueLength = 2;
                static constexpr uint16_t m_numOfGourpsValueLength = 2;

                ParsingStatus m_status = ParsingStatus::Failure;

            };

            class CipherSuitesParser : public BaseTlsParser, public ICipherSuitesParser {
            public:
                CipherSuitesParser() : BaseTlsParser(0) {
                }

                ~CipherSuitesParser() = default;

                using BaseTlsParser::getOffset;

                ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) override;

                uint16_t getSuite(uint16_t index) const override { return htons(m_pSuits[index]); }

                uint16_t getNumOfSuites() const override {
                    return m_suitesLength ? m_suitesLength / m_ciphersSuiteValueLength : 0;
                }

                const uint16_t *getpCipherSuites() const override { return m_pSuits; }

                uint16_t getCipherSuitesLen() const override { return m_suitesLength; }


            private:

                uint16_t m_suitesLength;
                uint16_t *m_pSuits;
                static constexpr uint16_t m_ciphersSuiteValueLength = 2;

                ParsingStatus m_status = ParsingStatus::Failure;

            };


        class SniParser : public BaseTlsParser, public ISniParser {
        public:
            SniParser() : BaseTlsParser(0) {
            }

            ~SniParser() = default;

            using BaseTlsParser::getOffset;

            ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) override;
            uint16_t getSniListLen() const override {return m_SniListLength;};
            uint16_t getNumOfSni() const override {return m_numOfSnis;}
            uint16_t getSniLen(uint16_t index) const override { return (index < max_snis_supported) ? m_sniLengths[index] : 0; }
            const uint8_t  *getpSni(uint16_t index) const override { return index < max_snis_supported ? m_sniPointers[index] : nullptr; }
            uint8_t  getSniType(uint16_t index) const override { return index < max_snis_supported ? m_sniTypes[index] : 0; }

        private:
            static constexpr uint16_t sniListFieldLength     = 2;
            static constexpr uint16_t sniTypeFieldLength     = 1;
            static constexpr uint16_t sniLengthFieldLength   = 2;
            static constexpr uint8_t max_snis_supported      = 2;
            uint16_t  m_SniListLength;
            uint16_t  m_sniLengths[max_snis_supported];
            uint8_t   m_sniTypes[max_snis_supported];
            uint8_t  *m_sniPointers[max_snis_supported];
            uint16_t  m_numOfSnis;
        	ParsingStatus m_status = ParsingStatus::Failure;

        };

        class TlsAlertParser : public BaseTlsParser, public IAlertParser {
        public:
            TlsAlertParser() : BaseTlsParser(minimalAlertLength) {}

            ~TlsAlertParser() = default;

            ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) override;

            uint8_t getAlertLevel() const override { return m_alertLevel; }

            uint8_t getAlertDescription() const override { return m_alertDescription; }

        private:
            static constexpr uint16_t minimalAlertLength = 2;
            uint8_t m_alertLevel{0};
            uint8_t m_alertDescription{0};
            ParsingStatus m_status{ParsingStatus::Failure};
        };


        }
    }
}



