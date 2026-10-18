#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

#include "../../common/include/Definitions.hpp"
#include "TLSSessionExtension.hpp"
#include <sh-data-struct/ESTL/FixedUnorderedMap.hpp>
#include <sh-data-struct/FIFOHashSet/FIFOSimpleSet.hpp>

namespace TLSFlow
{
    namespace Extension
    {
        namespace Engine
        {
            /**
             * @brief Per-session TLS extension storage table.
             *
             * Thread-safety: none. All calls must be serialised by the caller.
             */
            template <std::size_t MainCapacity, std::size_t ReclaimableCapacity>
            class TLSSessionExtensionTable
            {
                static_assert(MainCapacity >= 2,
                              "Main capacity must be at least 2");
                static_assert(ReclaimableCapacity > 0,
                              "Reclaimable capacity must be greater than zero");

                static constexpr std::size_t PrimaryCapacity = MainCapacity / 2;
                static constexpr std::size_t OverflowCapacity = MainCapacity - PrimaryCapacity;

                using Storage = ESTL::CTUnorderedMap<int,
                                                     TLSSessionExtension,
                                                     PrimaryCapacity,
                                                     OverflowCapacity,
                                                     std::hash<int>,
                                                     false>;
                using ReclaimableSet = FIFOSimpleSet<int, ReclaimableCapacity>;

            public:
                using FetchCallback = std::function<bool(int, const TLSSessionExtension&)>;

                TLSSessionExtensionTable() = default;
                virtual ~TLSSessionExtensionTable() = default;

                TLSSessionExtensionTable(const TLSSessionExtensionTable&)            = delete;
                TLSSessionExtensionTable& operator=(const TLSSessionExtensionTable&) = delete;

                /**
                 * @brief Allocates and returns an extension for @p sessionId.
                 *
                 * If the table is full, the oldest reclaimable extension is
                 * removed before the new extension is inserted.
                 *
                 * @return A pointer to the extension, or nullptr if no storage
                 *         is available.
                 */
                virtual TLSSessionExtension* alloc(int sessionId)
                {
                    auto* existing = m_storage.find(sessionId);
                    if (existing != nullptr)
                    {
                        return existing;
                    }

                    auto* extension = insertClean(sessionId);
                    if (extension != nullptr)
                    {
                        return extension;
                    }

                    if (m_reclaimable.empty())
                    {
                        return nullptr;
                    }

                    auto oldest = m_reclaimable.begin();
                    if (oldest == m_reclaimable.end())
                    {
                        return nullptr;
                    }

                    const int victimSessionId = *oldest;
                    auto* victim = m_storage.find(victimSessionId);
                    if (victim == nullptr)
                    {
                        m_reclaimable.erase(victimSessionId);
                        return nullptr;
                    }

                    m_reclaimable.erase(victimSessionId);
                    victim->resetValue();
                    if (!m_storage.erase(victimSessionId))
                    {
                        return nullptr;
                    }

                    return insertClean(sessionId);
                }

                /**
                 * @brief Returns the extension for @p sessionId, or nullptr.
                 *
                 * Does not create an entry. Safe to call before alloc() or
                 * after release().
                 */
                TLSSessionExtension* get(int sessionId)
                {
                    return m_storage.find(sessionId);
                }

                /**
                 * @brief Releases the extension for @p sessionId.
                 *
                 * Removes the entry from the table so that a future alloc()
                 * for the same ID returns a fresh, default-constructed
                 * extension.
                 *
                 * No-op if @p sessionId is not in the table.
                 */
                void release(int sessionId)
                {
                    m_reclaimable.erase(sessionId);

                    auto* extension = m_storage.find(sessionId);
                    if (extension != nullptr)
                    {
                        extension->resetValue();
                        m_storage.erase(sessionId);
                    }
                }

                virtual bool markReclaimable(int sessionId)
                {
                    if (m_storage.find(sessionId) == nullptr)
                    {
                        return false;
                    }

                    if (m_reclaimable.contains(sessionId))
                    {
                        return true;
                    }

                    return m_reclaimable.insert(sessionId);
                }

                uint32_t size() const
                {
                    return static_cast<uint32_t>(m_storage.size());
                }

                bool fetch(bool allSessions,
                           int sessionId,
                           uint32_t maxEntries,
                           bool hasCursor,
                           uint32_t cursor,
                           const FetchCallback& callback,
                           bool& done,
                           bool& hasNextCursor,
                           uint32_t& nextCursor)
                {
                    done = true;
                    hasNextCursor = false;
                    nextCursor = 0;

                    if (maxEntries == 0 || !callback)
                    {
                        return false;
                    }

                    if (!allSessions)
                    {
                        auto* extension = m_storage.find(sessionId);
                        if (extension != nullptr)
                        {
                            callback(sessionId, *extension);
                        }
                        return true;
                    }

                    const uint32_t tableCursor = hasCursor ? cursor : UINT32_MAX;
                    auto iterator = m_storage.beginFromCursor(tableCursor);
                    const auto end = m_storage.end();
                    uint32_t fetchedEntries = 0;
                    bool continueFetching = true;

                    while (iterator != end && fetchedEntries < maxEntries && continueFetching)
                    {
                        auto entry = *iterator;
                        continueFetching = callback(entry.first, entry.second);
                        ++iterator;
                        ++fetchedEntries;
                    }

                    if (iterator != end)
                    {
                        hasNextCursor = true;
                        done = false;
                        nextCursor = iterator.cursor();
                    }
                    return true;
                }

            private:
                TLSSessionExtension* insertClean(int sessionId)
                {
                    if (m_storage.size() >= MainCapacity)
                    {
                        return nullptr;
                    }

                    return m_storage.insertDefault(sessionId);
                }

                Storage m_storage;
                ReclaimableSet m_reclaimable;
            };

        } // namespace Engine
    } // namespace Extension
} // namespace TLSFlow