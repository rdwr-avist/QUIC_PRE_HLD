//
// Created by talc on 7/27/25.
//

#pragma once

#include "misc/TlsVersion.hpp"
#include "../../common/include/Fingerprint.hpp"
#include "../../common/include/Sni.hpp"

namespace TLSFlow
{
    namespace PacketFlow
    {
        namespace Engine
        {
            class ClientHelloData
            {
            private:
                /// --------------------------
                /// Members
                /// --------------------------
                const Fingerprint& fingerprint;
                const Sni& sni;
                Misc::TLSVersion tlsVersion;
                uint16_t numOfCiphers;
                uint16_t numOfExtensions;
                uint16_t numOfCompressionMethods;

            public:
                /// --------------------------
                /// Constructor & Destructor
                /// --------------------------
                ClientHelloData(const Fingerprint& fingerprint, const Sni& sni, Misc::TLSVersion tlsVersion, uint16_t numOfCiphers, uint16_t numOfExtensions, uint16_t numOfCompressionMethods)
                    : fingerprint{fingerprint}, sni{sni}, tlsVersion{tlsVersion}, numOfCiphers{numOfCiphers}, numOfExtensions{numOfExtensions}, numOfCompressionMethods{numOfCompressionMethods} {}
                ~ClientHelloData() = default;

                /// --------------------------
                /// Getters
                /// --------------------------
                const Fingerprint& getFingerprint() const { return fingerprint; }
                const Sni& getSni() const { return sni; }
                Misc::TLSVersion getTlsVersion() const { return tlsVersion; }
                uint16_t getNumOfCiphers() const { return numOfCiphers; }
                uint16_t getNumOfExtensions() const { return numOfExtensions; }
                uint16_t getNumOfCompressionMethods() const { return numOfCompressionMethods; }
            };
        } // Engine
    } // PacketFlow
} // TLSFlow
