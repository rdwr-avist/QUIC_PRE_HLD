//
// Created by maxg on 8/7/24.
//

#pragma once
#include <cstdint>

namespace TLSFlow
{
    namespace NSProcessor
    {
        namespace Engine
        {
            /// --------------------------
            /// Constants
            /// --------------------------
            constexpr static uint64_t SegmentBufferSize = 3000;
            constexpr static uint64_t MaxNumberOfLocators = 100;
            constexpr static uint64_t MaxNumberOfSegmentBuffers = 1000;

            /// --------------------------
            /// TLS Sizes
            /// --------------------------
            constexpr static uint8_t TLSRecordHeaderSize = 5;
            constexpr static uint8_t TLSHandshakeHeaderSize = 4;
            constexpr static uint8_t TLSChangeCipherSpecSize = 1;
            constexpr static uint8_t TLSExtensionsLengthFieldSize = 2;
            constexpr static uint16_t MaxTlsRecordPayloadSize = 16384; // 2^14 bytes per RFC 5246 Section 6.2.1
        }
    }
}