//
// Created by talc on 7/27/25.
//

#pragma once

#include "ClientHelloData.hpp"
#include <stm/include/SessionTable.hpp>
#include "TLSFeatureVerdict.hpp"
#include "../../common/include/TLSData.hpp"
#include "../../processor/include/ParsingError.hpp"
#include "TLSRecordContext.hpp"

namespace TLSFlow
{
    namespace PacketFlow
    {
        namespace Engine
        {
            class ITlsListener
            {
            public:
                using EventMask = uint8_t;
                using ParsingError = TLSFlow::NSProcessor::Engine::ParsingError;

                /**
                 * @brief Per-session event mask query.
                 *
                 * Called once per new session to determine whether this listener
                 * wants to participate in packet inspection for that session.
                 * Returns a bitmask of ITlsSubject::Event ordinals.
                 * Default: 0 (not interested — over-dispatch is a no-op via default callbacks).
                 *
                 * @note NOT pure virtual — subclasses that don't need per-session gating
                 *       can leave this unoverridden.
                 * @param session The new session being evaluated.
                 */
                virtual EventMask getSessionEventMask(Session& session) noexcept { return 0; }

                /**
                 * @brief Parsed ClientHello event handler.
                 */
                virtual TLSFeatureVerdict onClientHello(Session& session, DPFrame& packet, const ClientHelloData& data, const TLSData& tlsData) noexcept { return {}; }

                /**
                 * @brief Parsing error event handler.
                 */
                virtual TLSFeatureVerdict onParsingError(Session& session, DPFrame& packet, ParsingError parsingError, const TLSData& tlsData) noexcept { return {}; }

                /**
                 * @brief TCP reset event handler.
                 */
                virtual TLSFeatureVerdict onTCPReset(Session& session, DPFrame& packet, const TLSData& tlsData) noexcept { return {}; }

                /**
                 * @brief Session end event handler.
                 */
                virtual TLSFeatureVerdict onSessionEnd(Session& session, const TLSData& tlsData) noexcept { return {}; }

                /**
                 * @brief TLS record event handler (replaces onOtherType; called per record).
                 */
                virtual TLSFeatureVerdict onTLSRecord(Session& session, DPFrame& packet, const TLSRecordContext& context, const TLSData& tlsData) noexcept { return {}; }

                /**
                 * @brief Terminal payload event handler.
                 */
                virtual TLSFeatureVerdict onTerminalPayload(Session& session, DPFrame& packet, const TLSData& tlsData) noexcept { return {}; }

                /**
                 * Destructor
                 */
                virtual ~ITlsListener() = default;
            };
        } // Engine
    } // PacketFlow
} // TLSFlow