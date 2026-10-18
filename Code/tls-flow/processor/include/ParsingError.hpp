#pragma once

#include <cstdint>

namespace TLSFlow
{
    namespace NSProcessor
    {
        namespace Engine
        {
            enum class ParsingError : uint8_t
            {
                None = 0,
                UnsupportedTlsVersion,
                MalformedTlsRecord,
                UnsupportedRecordType,
                RecordLengthExceedsData,
                HandshakeLengthNotMatchingRecordLength,
                MalformedHandshake,
                MalformedClientHello,           // Generic - legacy/catch-all
                ClientHelloBodyInvalid,         // Specific - version/random/sid/ciphers/compression fields
                MultipleClientHello,
                ExtensionsLengthExceedsData,
                ExtensionParsingFailure,
                MalformedSniExtension,
                CipherParsingFailure,
                SegmentBufferAllocationFailure,
                SegmentMismatch,
                RecordsExceedDataViewerCapacity,

                MaxParsingErrorValue
            };
        } // namespace Engine
    } // namespace NSProcessor
} // namespace TLSFlow
