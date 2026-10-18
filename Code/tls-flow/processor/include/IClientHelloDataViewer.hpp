//
// Created by erezr on 10/10/24.
//

#ifndef VIRUALDP_IClientHelloDataViewer_HPP
#define VIRUALDP_IClientHelloDataViewer_HPP

#include <stdint.h>
#include "application/tls-flow/common/include/Fingerprint.hpp"
#include "application/tls-flow/common/include/Sni.hpp"
#include "FullStringData.hpp"

namespace TLSFlow
{
    namespace NSProcessor
    {
        namespace Engine
        {
            class IClientHelloDataViewer
            {
            public:
                enum class ClientHelloParsingStatus : uint8_t
                {
                    None,
                    Partial,
                    Complete
                };
                /**
				 *
				 * @brief sets the reference on the attack fingerprint that was processed in TLSFP attack flow and matched an attack
				 * @return true on success, false in case not parsed or not existed in the parsed hello
				 * @param Fingerprint - input/output reference to write on the sni
				 */
                virtual void getFingerprint(Fingerprint&) const = 0;

                /**
				 *
				 * @brief sets the reference on the attack SNI that was processed in TLSFP attack flow and matched an attack
				 * @return true on success, false in case not parsed or not existed in the parsed hello
				 * @param Sni - input/output reference to write on the sni
				 */
                virtual bool getSni(Sni&) = 0;

                /**
                 * @brief Get the advertised TLS version bitmask for the last processed ClientHello.
                 *
                 * The mask is populated from the SupportedVersions extension when present.
                 * If that extension is absent, the mask contains only the selected `lastVersion` bit.
                 */
                virtual uint8_t getSupportedVersionBitMask() const = 0;

                /**
                 * @brief Check whether the current ClientHello offers early data.
                 */
                virtual bool isEarlyDataOffered() const = 0;

                /**
                 * @brief will get a ptr to the array of the fingerprint full string
                 */
                virtual FullStringData getFullString() const = 0;

                /**
                 * @brief
                 * Get the TLS version from the last processed Client Hello.
                 * This function should be called only after a successful call to process().
                 *
                 * @return TLS version as uint16_t
                 */
                virtual uint16_t getTlsVersion() const = 0;

                /**
                 * @brief
                 * Get the number of ciphers from the last processed Client Hello.
                 * This function should be called only after a successful call to process().
                 *
                 * @return Number of ciphers as uint32_t
                 */
                virtual uint32_t getCipherCount() const = 0;


                /**
                 * @brief
                 * Get the number of extensions from the last processed Client Hello.
                 * This function should be called only after a successful call to process().
                 *
                 * @return Number of extensions as uint32_t
                 */
                virtual uint32_t getExtensionCount() const = 0;

                /**
                 * @brief
                 * Get the number of compressions from the last processed Client Hello.
                 * This function should be called only after a successful call to process().
                 *
                 * @return Number of compressions as uint32_t
                 */
                virtual uint32_t getCompressionCount() const = 0;

                virtual bool isSegmented() const = 0;

                /**
                 * @brief
                 * Check if a ClientHello was found during parsing.
                 * This should be called after process() to determine if ClientHello data is available.
                 *
                 * @return true if ClientHello was found, false otherwise
                 */
                virtual ClientHelloParsingStatus getClientHelloParsingStatus() const = 0;

                /**
                 * @brief
                 * Destructor
                 */
                virtual ~IClientHelloDataViewer() = default;

                
            };
        }
    }
}

#endif//VIRUALDP_IClientHelloDataViewer_HPP
