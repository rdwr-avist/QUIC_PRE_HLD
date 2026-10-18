#pragma once
#include "ParsingTypes.hpp"

namespace TLSFlow {
    namespace NSProcessor {
        namespace Engine {

            struct Record {
                uint8_t recordType{static_cast<uint8_t>(RecordTypesE::invalid)};      // Raw TLS record type value (allows unknown types)
                uint8_t handshakeType{static_cast<uint8_t>(HandShakeTypesE::hello_request_RESERVED)};   // Raw handshake type value (allows unknown types)
                uint16_t recordLength{0};   // Length of the TLS record payload (not including header)
                uint8_t alertLevel{0};
                uint8_t alertDescription{0};
                bool readableAlert{false};

                bool isHandshake() const {
                    return recordType == static_cast<uint8_t>(RecordTypesE::handshakeContent);
                }

                bool isClientHello() const {
                    return isHandshake() && handshakeType == static_cast<uint8_t>(HandShakeTypesE::client_hello);
                }

                bool isChangeCipherSpec() const {
                    return recordType == static_cast<uint8_t>(RecordTypesE::changeCipherSpecContent);
                }

                bool isAlert() const {
                    return recordType == static_cast<uint8_t>(RecordTypesE::alertContent);
                }

                bool isApplicationData() const {
                    return recordType == static_cast<uint8_t>(RecordTypesE::applicationDataContent);
                }
                
            };

        } // namespace Engine
    }     // namespace NSProcessor
} // namespace TLSFlow