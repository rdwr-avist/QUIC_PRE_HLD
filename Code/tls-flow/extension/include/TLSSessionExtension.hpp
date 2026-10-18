#pragma once

#include <array>
#include <cassert>
#include <memory>

#include "../../common/include/TLSData.hpp"
#include "../../packet-flow/include/ITlsSubject.hpp"
#include "../../packet-flow/include/TLSPriority.hpp"
#include <sh-data-struct/ESTL/FixedVector.hpp>

namespace TLSFlow
{
    namespace Extension
    {
        namespace Engine
        {
            /**
             * @brief Per-session TLS extension state.
             *
             * Owns the TLSData for a session and maintains per-event observer
             * lists, each with capacity equal to the number of priority slots
             * (TLSPriority::Max). Intended to be embedded in a session object
             * and recycled via init() / clear() rather than re-allocated.
             */
            class TLSSessionExtension
            {
            public:
                using Event       = PacketFlow::Engine::ITlsSubject::Event;
                using ListenerVec = ESTL::CTVector<
                    std::weak_ptr<PacketFlow::Engine::ITlsListener>,
                    static_cast<std::size_t>(PacketFlow::Engine::TLSPriority::Max),
                    false>;

                TLSSessionExtension() = default;

                /**
                 * @brief Resets all state to defaults. Equivalent to clear().
                 */
                void init()
                {
                    resetValue();
                }

                /**
                 * @brief Resets TLSData to defaults and empties all observer lists.
                 */
                void resetValue()
                {
                    m_tlsData = TLSData{};
                    for (auto& vec : m_observers)
                    {
                        vec.clear();
                    }
                }

                /**
                 * @brief Appends an observer to the list for the given event.
                 *
                 * Observers must be added in priority order by the caller (e.g. by
                 * iterating a sorted map keyed on TLSPriority during onNewSession).
                 * This class preserves insertion order; it does not re-sort.
                 *
                 * @param event    The event type to observe.
                 * @param observer Weak pointer to the listener.
                 */
                void addObserver(Event                                              event,
                                 std::weak_ptr<PacketFlow::Engine::ITlsListener>   observer)
                {
                    assert(static_cast<std::size_t>(event) < static_cast<std::size_t>(Event::MaxTypes));
                    assert(m_observers[static_cast<std::size_t>(event)].size() <
                           m_observers[static_cast<std::size_t>(event)].capacity());
                    m_observers[static_cast<std::size_t>(event)].push_back(std::move(observer));
                }

                /**
                 * @brief Returns the observer list for the given event.
                 *
                 * @param event  The event type to query.
                 * @return Const reference to the CTVector for that event.
                 */
                const ListenerVec& getObserverList(Event event) const
                {
                    assert(static_cast<std::size_t>(event) < static_cast<std::size_t>(Event::MaxTypes));
                    return m_observers[static_cast<std::size_t>(event)];
                }

                /**
                 * @brief Returns a mutable reference to the session TLS data.
                 */
                TLSData& getTlsData()
                {
                    return m_tlsData;
                }
                /**
                 * @brief Returns a const reference to the session TLS data.
                 */
                const TLSData& getTlsData() const
                {
                    return m_tlsData;
                }

            private:
                TLSData m_tlsData;

                std::array<ListenerVec,
                           static_cast<std::size_t>(Event::MaxTypes)> m_observers;
            };

        } // namespace Engine
    } // namespace Extension
} // namespace TLSFlow
