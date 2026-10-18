//
// Created by talc on 7/22/25.
//

#pragma once

#include "stm/include/Observer.hpp"
#include "stm/include/SessionTable.hpp"
#include "application/tls-flow/processor/include/IProcessor.hpp"
#include "application/tls-flow/processor/include/IClientHelloDataViewer.hpp"
#include "application/tls-flow/processor/include/IRecordsDataViewer.hpp"
#include "application/tls-flow/processor/include/IParsingErrorViewer.hpp"
#include "application/tls-flow/packet-flow/include/ITlsSubject.hpp"
#include "application/tls-flow/packet-flow/include/ITlsListener.hpp"
#include "application/tls-flow/packet-flow/include/TLSRecordContext.hpp"
#include "application/tls-flow/packet-flow/include/TLSFeatureVerdict.hpp"
#include "../include/TLSPriority.hpp"
#include "application/tls-flow/extension/include/TLSSessionExtensionTable.hpp"
#include "application/tls-flow/extension/include/ITLSSessionExtensionInspector.hpp"

#include <map>
#include <memory>

namespace TLSFlow
{
    namespace PacketFlow
    {
        namespace Engine
        {
            class TLSFlowConductor : public Observer,
                                      public ITlsSubject,
                                      public Extension::Engine::ITLSSessionExtensionInspector
            {
                using Event = ITlsSubject::Event;
                using Extension = TLSFlow::Extension::Engine::TLSSessionExtension;
#ifdef UNIT_TEST
                friend class TLSFlowConductorTestAccess;
#endif
            private:
                /// --------------------------
                /// Members
                /// --------------------------
                shared_ptr<NSProcessor::Engine::IProcessor> processor;
                shared_ptr<NSProcessor::Engine::IClientHelloDataViewer> clientHelloDataViewer;
                shared_ptr<NSProcessor::Engine::IRecordsDataViewer> recordsDataViewer;
                std::shared_ptr<NSProcessor::Engine::IParsingErrorViewer> parsingErrorViewer;
                /// One listener slot per priority; over-dispatches all events to all live subscribers.
                std::map<TLSPriority, std::weak_ptr<ITlsListener>> m_subscribers;
                std::unique_ptr<TLSFlow::TlsSessionExtensionTable> m_extensionTable;

                /**
                 * @brief New session handler — decides whether to register for inbound/aging.
                 */
                ObserverResult onNewSession(Session *session, DPFrame *packet);

                /**
                 * @brief Session inbound packet handler.
                 */
                ObserverResult onInbound(Session * session, DPFrame * packet);

                /**
                 * @brief Prepare the extension required by an inbound packet.
                 *
                 * A client SYN without ACK is a session lifecycle boundary. If
                 * the session has no extension, or its extension is Finished,
                 * the current extension is created or replaced and the packet
                 * is consumed without entering normal TLS processing.
                 *
                 * @param session Session associated with the inbound packet.
                 * @param packet Inbound packet to inspect.
                 * @param extension Receives the current extension when normal
                 *                   processing should continue.
                 * @return true when the packet was consumed by lifecycle
                 *         preparation; false when normal processing may continue.
                 */
                bool prepareInboundExtension(Session& session,
                                             DPFrame& packet,
                                             Extension*& extension);

                /**
                 * @brief Session inbound packet handler when a TLS session extension exists.
                 */
                ObserverResult process(Session& session, DPFrame& packet, Extension& extension);

                ObserverResult dispatchStepResult(Session &session, DPFrame &packet, Extension& extension, NSProcessor::Engine::IProcessor::TLSStepResult stepResult);

                /**
                 * @brief Session pre-aging handler.
                 */
                ObserverResult onAging(Session * session);

                /**
                 * @brief Over-dispatches the notifier to all live observers in @p observers.
                 *
                 * Iterates @p observers in priority order; stops on the first Stop verdict
                 * and preserves all violation bits from dispatched observers.
                 * The Notifier callable must accept a shared_ptr<ITlsListener> and return
                 * a TLSFeatureVerdict.
                 */
                template <typename ObserverList, typename Notifier>
                TLSFeatureVerdict notify(const ObserverList& observers, Notifier&& notifier)
                {
                    TLSFeatureVerdict result{};
                    for (const auto& observer : observers) {
                        auto sharedPtr = observer.lock();
                        if (sharedPtr != nullptr) {
                            const auto v = notifier(sharedPtr);
                            if (v.violationMask != TLSFlow::TLSViolationMaskNone) {
                                result.violationMask |= v.violationMask;
                            }
                            if (v.dispatch == TLSFeatureVerdict::Dispatch::Stop) {
                                result.dispatch = TLSFeatureVerdict::Dispatch::Stop;
                                return result;
                            }
                        }
                    }
                    return result;
                }

                ObserverResult analyzeProcessResult(Session &session, DPFrame &packet, Extension& extension);

                TLSFeatureVerdict checkForParsingError(Session &session, DPFrame &packet, Extension& extension);
                TLSFeatureVerdict onClientHello(Session &session, DPFrame &packet, Extension& extension);
                TLSFeatureVerdict onRecord(Session &session, DPFrame &packet, Extension& extension, const TLSRecordContext& context);
                ObserverResult dispatchTerminalPayload(Session& session,
                                                        DPFrame& packet,
                                                        Extension& extension);

                /**
                 * @brief Apply the full set of normal (non-error) updates for the
                 *        current parsed record to the per-session TLSData.
                 *
                 * This commits counters, flags and version hints discovered via the
                 * client-hello viewer. Does not touch terminalViolationMask — use
                 * commitError() for error/violation commits.
                 */
                void commitNormalCurrentRecord(Extension &extension,
                                               const TLSRecordContext& context,
                                               unsigned long packetTimestamp = 0);

                /**
                 * @brief Update the TLSData for a ClientHello record.
                 * @param tls  TLSData to update.
                 */
                void updateTLSDataOnClientHello(TLSFlow::TLSData& tls,
                                                const TLSRecordContext& context,
                                                unsigned long packetTimestamp);
                /**
                 * @brief Update the TLSData for a ChangeCipherSpec record.
                 * @param tls  TLSData to update.
                 */
                void updateTLSDataOnChangeCipherSpec(TLSFlow::TLSData& tls);

                /**
                 * @brief Update the TLSData for an ApplicationData record.
                 * @param tls  TLSData to update.
                 */
                void updateTLSDataOnApplicationData(TLSFlow::TLSData& tls);

                /**
                 * @brief Update the TLSData for an Alert record.
                 * @param tls  TLSData to update.
                 */
                void updateTLSDataOnAlert(TLSFlow::TLSData& tls,
                                          const TLSRecordContext& context);
                /**
                 * @brief Commit an unsupported TLS lifecycle caused by ClientHello
                 * processing limits or a session continuation gap.
                 */
                void commitUnsupported(Extension &extension, bool continuationGap = false);

                /**
                 * @brief Commit a violation without changing the handshake state.
                 */
                void commitViolation(Extension &extension, TLSFlow::TLSViolationMask violationMask);

                /**
                 * @brief Commit an error state.
                 */
                void commitError(Session &session, Extension &extension, TLSFlow::TLSViolationMask violationMask);

                /**
                 * @brief Convert a TLSFeatureVerdict to an ObserverResult without committing side-effects.
                 */
                static ObserverResult toObserverResult(const TLSFeatureVerdict& v);

                /**
                * @brief Build per-session TLS extension by querying subscribers.
                * @param session Session to evaluate.
                * @return Allocated TLSSessionExtension* or nullptr on no-participation/allocation failure.
                */
                Extension* buildExtension(Session& session);

                /*
                 * Registers TLSFlowConductor as a session observer for the STM events required by TLS-Flow.
                 * Expected events:
                 * - Inbound packet processing
                 * - Aging / session end
                 *
                 * Called only after buildExtension() succeeds and an extension was allocated.
                 */
                void registerSessionObservers(Session& session);

                /**
                 * @brief Appends a listener to every event list in @p extension whose
                 * bit is set in @p mask.
                 *
                 * Observers must be added in priority order by the caller (guaranteed by
                 * iterating the sorted m_subscribers map).
                 *
                 * @param extension  The per-session extension to populate.
                 * @param priority   Priority of the listener (informational; ordering is
                 *                   enforced by the caller).
                 * @param listener   Shared pointer to the listener.
                 * @param mask       Bitmask of ITlsSubject::Event ordinals the listener
                 *                   requested.
                 */
                void addListenerToExtension(Extension& extension,
                                            const std::shared_ptr<ITlsListener>&      listener,
                                            ITlsListener::EventMask                   mask);

            public:
                /**
                 * @brief New packet handler.
                 */
                int onEvent(Session* session, DPFrame* packet, int event) noexcept override;

                /**
                 * @brief Processor setter.
                 */
                void setProcessor(shared_ptr<NSProcessor::Engine::IProcessor> processor) { TLSFlowConductor::processor = processor; }

                /**
                 * @brief ClientHello data viewer setter.
                 */
                void setViewer(shared_ptr<NSProcessor::Engine::IClientHelloDataViewer> viewer) { TLSFlowConductor::clientHelloDataViewer = viewer; }

                /**
                 * @brief Records data viewer setter.
                 */
                void setRecordsViewer(shared_ptr<NSProcessor::Engine::IRecordsDataViewer> viewer) { TLSFlowConductor::recordsDataViewer = viewer; }

                /**
                 * @brief Parsing error viewer setter.
                 */
                void setParsingErrorViewer(std::shared_ptr<NSProcessor::Engine::IParsingErrorViewer> viewer) { TLSFlowConductor::parsingErrorViewer = viewer; }
                
                /**
                 * @brief Register a listener at the given priority slot.
                 *
                 * Overwrites any existing listener at the same priority.
                 */
                void subscribe(TLSPriority priority, std::weak_ptr<ITlsListener> listener) override;

                /**
                 * @brief Unregister the listener at the given priority slot.
                 */
                void unsubscribe(TLSPriority priority) override;

                uint32_t getSessionExtensionCount() const override;

                bool fetchSessionExtensions(
                    const TLSFlow::Extension::Engine::TLSSessionExtensionInspectorQuery& query,
                    const TLSFlow::Extension::Engine::TLSSessionExtensionInspectorVisitor& visitor,
                    bool& done,
                    bool& hasNextCursor,
                    uint32_t& nextCursor) override;

                /// --------------------------
                /// Constructors & Destructors
                /// --------------------------
                TLSFlowConductor() = default;
                explicit TLSFlowConductor(std::unique_ptr<TLSFlow::TlsSessionExtensionTable> extensionTable)
                    : m_extensionTable(std::move(extensionTable)) {}
                ~TLSFlowConductor() override = default;
            };
        } // Engine
    } // PacketFlow
} // TLSFlow