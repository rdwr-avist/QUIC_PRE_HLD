#pragma once

#include <cstdint>
#include <functional>

#include "../../common/include/TLSData.hpp"

namespace TLSFlow
{
    namespace Extension
    {
        namespace Engine
        {
            struct TLSSessionExtensionInspectorQuery
            {
                bool allSessions{false};
                int32_t sessionIndex{0};
                uint32_t maxEntries{0};
                bool hasCursor{false};
                uint32_t cursor{0};
            };

            struct TLSSessionExtensionInspectorSnapshot
            {
                int32_t sessionIndex{0};
                HsState hsState{HsState::PreHandshake};
                uint8_t tlsVersionMask{0};
                uint8_t clientHelloCount{0};
                uint8_t earlyDataCount{0};
                bool earlyDataOffered{false};
                uint8_t preCcsSeen{0};
                Fingerprint chFingerprint{};
                TLSViolationMask terminalViolationMask{TLSViolationMaskNone};
                uint64_t clientHelloTimeStamp{0};
            };

            using TLSSessionExtensionInspectorVisitor =
                std::function<bool(const TLSSessionExtensionInspectorSnapshot&)>;

            class ITLSSessionExtensionInspector
            {
            public:
                virtual ~ITLSSessionExtensionInspector() = default;

                virtual uint32_t getSessionExtensionCount() const = 0;

                virtual bool fetchSessionExtensions(
                    const TLSSessionExtensionInspectorQuery& query,
                    const TLSSessionExtensionInspectorVisitor& visitor,
                    bool& done,
                    bool& hasNextCursor,
                    uint32_t& nextCursor) = 0;
            };
        }
    }
}
