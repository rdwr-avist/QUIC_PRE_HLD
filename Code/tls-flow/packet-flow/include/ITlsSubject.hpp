//
// Created by avist on 8/14/25.
//
#pragma once
#include "ITlsListener.hpp"

#include "TLSPriority.hpp"
#include <memory>

namespace TLSFlow
{
    namespace PacketFlow
    {
        namespace Engine
        {
            class ITlsSubject
            {
            public:
                enum class Event : uint8_t
                {
                    ClientHello     = 0,
                    Anomaly         = 1,
                    TCPReset        = 2,
                    SessionEnd      = 3,
                    TLSRecord       = 4,
                    TerminalPayload = 5,
                    MaxTypes        = 6  // Keep this as the last element
                };

                /// --------------------------
                /// Interfaces
                /// --------------------------

                /**
                 * @brief Register a listener for all TLS events on this subject.
                 *
                 * @param priority Priority slot — one listener per priority.
                 * @param listener Weak pointer to the listener (ownership stays with caller).
                 */
                virtual void subscribe(TLSPriority priority, std::weak_ptr<ITlsListener> listener) = 0;

                /**
                 * @brief Unregister the listener at the given priority slot.
                 */
                virtual void unsubscribe(TLSPriority priority) = 0;

                virtual ~ITlsSubject() = default;
            };
        } // Engine
    } // PacketFlow
} // TlsFlow