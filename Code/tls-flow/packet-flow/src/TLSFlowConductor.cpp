//
// Created by talc on 7/22/25.
//

#include "TLSFlowConductor.hpp"
#include <stm/include/SessionTable.hpp>
#include "../../processor/include/ParsingTypes.hpp"
#include "application/tls-flow/common/include/TLSAlert.hpp"
#include "application/tls-flow/common/include/TLSData.hpp"

using namespace TLSFlow;
using namespace TLSFlow::PacketFlow;
using namespace TLSFlow::PacketFlow::Engine;
using namespace TLSFlow::NSProcessor::Engine;

Misc::TLSVersion convertToMiscTlsVersion(uint16_t version) {
    switch (version) {
        case 0x0300:
            return Misc::TLSVersion::SSL_3_0;
        case 0x0301:
            return Misc::TLSVersion::V1_0;
        case 0x0302:
            return Misc::TLSVersion::V1_1;
        case 0x0303:
            return Misc::TLSVersion::V1_2;
        case 0x0304:
            return Misc::TLSVersion::V1_3;
        default:
            return Misc::TLSVersion::Other;
    }
}

int TLSFlowConductor::onEvent(Session * session, DPFrame * dpFrame, int event) noexcept
{
    switch (event) {
        case STM_EVENT_NEW_SESSION:
            return onNewSession(session, dpFrame);
        case STM_EVENT_INBOUND_PACKET_PROCESS:
            return onInbound(session, dpFrame);
        case STM_EVENT_AGING:
            return onAging(session);
        default:
            return RET_OBSERVER_CONTINUE;
    }
}

ObserverResult TLSFlowConductor::onNewSession(Session *session, DPFrame * /*packet*/)
{
    Extension* extension = buildExtension(*session);
    if (extension == nullptr) {
        return RET_OBSERVER_CONTINUE;
    }

    registerSessionObservers(*session);
    return RET_OBSERVER_CONTINUE;
}

ObserverResult TLSFlowConductor::onInbound(Session * session, DPFrame * packet)
{
    
    Extension* extension = nullptr;
    if (prepareInboundExtension(*session, *packet, extension)) {
        return RET_OBSERVER_CONTINUE;
    }

    if (extension == nullptr) {
        return RET_OBSERVER_CONTINUE;
    }

    return process(*session, *packet, *extension);
}

bool TLSFlowConductor::prepareInboundExtension(Session& session,
                                               DPFrame& packet,
                                               Extension*& extension)
{
    extension = m_extensionTable->get(session.getSessionIndex());

    if (packet.getPayloadLayer4Protocol() != PARSE_IP_PROTOCOL_TCP) {
        return false;
    }

    auto* tcpHeader = reinterpret_cast<IPTCPG_tcp_hdr_STC*>(
        packet.getPayloadLayer4Ptr());
    if (!IPTCPG_is_syn_no_ack_MAC(tcpHeader)) {
        return false;
    }

    if (extension == nullptr) {
        buildExtension(session);
        return true;
    }

    const auto& tlsData = extension->getTlsData();
    const bool hasRecordedViolation =
        tlsData.terminalViolationMask != TLSFlow::TLSViolationMaskNone;

    if (isActiveHandshakeState(tlsData.hsState) && !hasRecordedViolation) {
        return false;
    }

    m_extensionTable->release(session.getSessionIndex());
    buildExtension(session);
    extension = nullptr;
    return true;
}

ObserverResult TLSFlowConductor::process(Session& session, DPFrame& packet, Extension& extension)
{
    auto &tlsData = extension.getTlsData();

    if (packet.getPayloadLayer4Protocol() == PARSE_IP_PROTOCOL_TCP) {
        auto pTcpHeader = (IPTCPG_tcp_hdr_STC *)packet.getPayloadLayer4Ptr();
        if (SYN_PROTECT_IS_LEGAL_RST(pTcpHeader) && !SYN_PROTECT_IS_LEGAL_ACK(pTcpHeader)) {
            const auto verdict = notify(extension.getObserverList(Event::TCPReset), [&session, &packet, &tlsData](std::shared_ptr<ITlsListener> listener) {
                return listener->onTCPReset(session, packet, tlsData);
            });
            
            if (verdict.violationMask != TLSFlow::TLSViolationMaskNone) {
                if (verdict.dispatch == TLSFeatureVerdict::Dispatch::Stop) {
                    commitError(session, extension, verdict.violationMask);
                } else {
                    commitViolation(extension, verdict.violationMask);
                }
            } else {
                // Normal reset path: clear transient TLS data.
                tlsData.clear();
            }
            return toObserverResult(verdict);
        }
    }

    if (isSilentTerminalState(tlsData.hsState))
    {
        return RET_OBSERVER_CONTINUE;
    }

    uint16_t len = packet.getFrameTotalLen() - (packet.getPayloadLayer7Ptr() - packet.getLayer2Ptr());
    if (0 == len) {
        return RET_OBSERVER_CONTINUE;
    }


        if (usesTerminalPayloadPath(tlsData.hsState)) {
            return dispatchTerminalPayload(session, packet, extension);
        }

    uint32_t seq = packet.getTcpSeqNum();

    processor->beginPacket({packet.getPayloadLayer7Ptr(), len, static_cast<uint32_t>(session.getSessionIndex()), seq });

    IProcessor::TLSStepResult stepResult;
    do {
        stepResult = processor->processRecord(tlsData);

        const ObserverResult observerResult = dispatchStepResult(session, packet, extension, stepResult);
        if (observerResult == RET_OBSERVER_STOP) {
            return RET_OBSERVER_STOP;
        }
    } while (stepResult == IProcessor::TLSStepResult::RecordParsed &&
             isActiveHandshakeState(extension.getTlsData().hsState));

    return RET_OBSERVER_CONTINUE;
}

ObserverResult TLSFlowConductor::dispatchStepResult(Session &session, DPFrame &packet, Extension& extension, IProcessor::TLSStepResult stepResult)
{
    switch (stepResult) {
        case IProcessor::TLSStepResult::RecordParsed:
            return analyzeProcessResult(session, packet, extension);
        case IProcessor::TLSStepResult::Stop:
        {
            const auto verdict = checkForParsingError(session, packet, extension);
            if (verdict.violationMask != TLSFlow::TLSViolationMaskNone) {
                if (verdict.dispatch == TLSFeatureVerdict::Dispatch::Stop) {
                    commitError(session, extension, verdict.violationMask);
                } else {
                    commitViolation(extension, verdict.violationMask);
                }
            }
            return toObserverResult(verdict);
        }
        case IProcessor::TLSStepResult::NeedMoreData:
            return RET_OBSERVER_CONTINUE;
        case IProcessor::TLSStepResult::Unsupported:
            commitUnsupported(extension, processor->isContinuationGap());
            return RET_OBSERVER_CONTINUE;
        case IProcessor::TLSStepResult::EndOfPacket:
        default:
            return RET_OBSERVER_CONTINUE;
    }
}

ObserverResult TLSFlowConductor::analyzeProcessResult(Session &session, DPFrame &packet, Extension& extension)
{
    bool sawStop = false;
    TLSFlow::TLSViolationMask accumulatedViolationMask = TLSFlow::TLSViolationMaskNone;

    const auto applyVerdict = [&](const TLSFeatureVerdict& verdict) -> ObserverResult {
        if (verdict.violationMask != TLSFlow::TLSViolationMaskNone) {
            accumulatedViolationMask |= verdict.violationMask;
        }

        if (verdict.dispatch == TLSFeatureVerdict::Dispatch::Stop) {
            if (accumulatedViolationMask != TLSFlow::TLSViolationMaskNone) {
                commitError(session, extension, accumulatedViolationMask);
                return RET_OBSERVER_STOP;
            }
            sawStop = true;
        }
        return RET_OBSERVER_CONTINUE;
    };

    // 1) parsing error observers
    const auto parseVerdict = checkForParsingError(session, packet, extension);
    if (applyVerdict(parseVerdict) == RET_OBSERVER_STOP) {
        return RET_OBSERVER_STOP;
    }

    const auto& record = recordsDataViewer->getLastRecord();
    TLSRecordContext recordContext{
        record,
        record.isClientHello() ? clientHelloDataViewer->getSupportedVersionBitMask() : 0u,
        record.isClientHello() ? clientHelloDataViewer->isEarlyDataOffered() : false};
    if (record.isAlert() && record.readableAlert) {
        recordContext.alertTerminating = TLSFlow::isTerminatingAlert(
            extension.getTlsData().tlsVersionMask,
            record.alertLevel,
            record.alertDescription);
    }

    // 2) record observers
    const auto recordVerdict = onRecord(session, packet, extension, recordContext);
    if (applyVerdict(recordVerdict) == RET_OBSERVER_STOP) {
        return RET_OBSERVER_STOP;
    }

    // 3) client hello observers (only if complete)
    if (clientHelloDataViewer->getClientHelloParsingStatus() == IClientHelloDataViewer::ClientHelloParsingStatus::Complete) {
        const auto chVerdict = onClientHello(session, packet, extension);
        if (applyVerdict(chVerdict) == RET_OBSERVER_STOP) {
            return RET_OBSERVER_STOP;
        }
    }

    if (accumulatedViolationMask != TLSFlow::TLSViolationMaskNone) {
        if (sawStop) {
            commitError(session, extension, accumulatedViolationMask);
        } else {
            commitViolation(extension, accumulatedViolationMask);
        }

        return sawStop ? RET_OBSERVER_STOP : RET_OBSERVER_CONTINUE;
    }

    const auto previousHandshakeState = extension.getTlsData().hsState;
    commitNormalCurrentRecord(extension, recordContext, session.getLastPacketTimestamp());
    if (m_extensionTable &&
        previousHandshakeState != HsState::Finished &&
        extension.getTlsData().hsState == HsState::Finished) {
        m_extensionTable->markReclaimable(session.getSessionIndex());
    }

    return sawStop ? RET_OBSERVER_STOP : RET_OBSERVER_CONTINUE;
}

TLSFeatureVerdict TLSFlowConductor::checkForParsingError(Session &session, DPFrame &packet, Extension& extension)
{
    const ParsingError error = parsingErrorViewer->getLastError();
    if (error == ParsingError::None) {
        return TLSFeatureVerdict{};
    }

    const auto& observers = extension.getObserverList(Event::Anomaly);
    if (observers.empty()) {
        return TLSFeatureVerdict{};
    }

    auto& tlsData = extension.getTlsData();
    return notify(observers, [&session, &packet, &error, &tlsData](std::shared_ptr<ITlsListener> listener) {
        return listener->onParsingError(session, packet, error, tlsData);
    });
}

TLSFeatureVerdict TLSFlowConductor::onClientHello(Session &session, DPFrame &packet, Extension& extension)
{
    const auto& chObservers = extension.getObserverList(Event::ClientHello);
    if (chObservers.empty()) {
        return TLSFeatureVerdict{};
    }

    auto& tlsData = extension.getTlsData();
    Sni sni;
    clientHelloDataViewer->getSni(sni);
    Fingerprint fingerprint;
    clientHelloDataViewer->getFingerprint(fingerprint);

    ClientHelloData chData = {fingerprint,
                            sni,
                            convertToMiscTlsVersion(clientHelloDataViewer->getTlsVersion()),
                            clientHelloDataViewer->getCipherCount(),
                            clientHelloDataViewer->getExtensionCount(),
                            clientHelloDataViewer->getCompressionCount()};

    return notify(chObservers, [&session, &packet, &chData, &tlsData](std::shared_ptr<ITlsListener> listener) {
        return listener->onClientHello(session, packet, chData, tlsData);
    });
}

TLSFeatureVerdict TLSFlowConductor::onRecord(Session &session, DPFrame &packet, Extension& extension, const TLSRecordContext& context)
{
    const auto& observers = extension.getObserverList(Event::TLSRecord);
    if (observers.empty()) {
        return TLSFeatureVerdict{};
    }
    auto& tlsData = extension.getTlsData();

    return notify(observers, [&session, &packet, &context, &tlsData](std::shared_ptr<ITlsListener> listener) {
        return listener->onTLSRecord(session, packet, context, tlsData);
    });
}

ObserverResult TLSFlowConductor::dispatchTerminalPayload(Session& session,
                                                         DPFrame& packet,
                                                         Extension& extension)
{
    const auto& observers = extension.getObserverList(Event::TerminalPayload);
    if (observers.empty()) {
        return RET_OBSERVER_CONTINUE;
    }

    auto& tlsData = extension.getTlsData();

    const auto verdict = notify(observers, [&session, &packet, &tlsData](std::shared_ptr<ITlsListener> listener) {
        return listener->onTerminalPayload(session, packet, tlsData);
    });

    if (verdict.violationMask != TLSFlow::TLSViolationMaskNone) {
        if (verdict.dispatch == TLSFeatureVerdict::Dispatch::Stop) {
            commitError(session, extension, verdict.violationMask);
        } else {
            commitViolation(extension, verdict.violationMask);
        }
    }

    return toObserverResult(verdict);
}

ObserverResult TLSFlowConductor::onAging(Session * session)
{
    Extension* extension = m_extensionTable->get(session->getSessionIndex());
    if (extension == nullptr) {
        return RET_OBSERVER_CONTINUE;
    }

    auto& tlsData = extension->getTlsData();
    notify(extension->getObserverList(Event::SessionEnd), [&session, &tlsData](std::shared_ptr<ITlsListener> listener) {
        return listener->onSessionEnd(*session, tlsData);
    });
    m_extensionTable->release(session->getSessionIndex());
    return RET_OBSERVER_CONTINUE;
}

void TLSFlowConductor::subscribe(TLSPriority priority, std::weak_ptr<ITlsListener> listener)
{
    m_subscribers[priority] = listener;
}

void TLSFlowConductor::unsubscribe(TLSPriority priority)
{
    m_subscribers.erase(priority);
}

uint32_t TLSFlowConductor::getSessionExtensionCount() const
{
    return m_extensionTable == nullptr ? 0 : m_extensionTable->size();
}

bool TLSFlowConductor::fetchSessionExtensions(
    const TLSFlow::Extension::Engine::TLSSessionExtensionInspectorQuery& query,
    const TLSFlow::Extension::Engine::TLSSessionExtensionInspectorVisitor& visitor,
    bool& done,
    bool& hasNextCursor,
    uint32_t& nextCursor)
{
    if (m_extensionTable == nullptr || !visitor)
    {
        done = true;
        hasNextCursor = false;
        nextCursor = 0;
        return false;
    }

    return m_extensionTable->fetch(
        query.allSessions,
        query.sessionIndex,
        query.maxEntries,
        query.hasCursor,
        query.cursor,
        [&visitor](int sessionIndex, const Extension& extension) {
            const auto& tlsData = extension.getTlsData();
            TLSFlow::Extension::Engine::TLSSessionExtensionInspectorSnapshot snapshot{};
            snapshot.sessionIndex = sessionIndex;
            snapshot.hsState = tlsData.hsState;
            snapshot.tlsVersionMask = tlsData.tlsVersionMask;
            snapshot.clientHelloCount = tlsData.clientHelloCount;
            snapshot.earlyDataCount = tlsData.earlyDataCount;
            snapshot.earlyDataOffered = tlsData.earlyDataOffered;
            snapshot.preCcsSeen = tlsData.preCcsSeen;
            snapshot.chFingerprint = tlsData.chFingerprint;
            snapshot.terminalViolationMask = tlsData.terminalViolationMask;
            snapshot.clientHelloTimeStamp = tlsData.clientHelloTimeStamp;
            return visitor(snapshot);
        },
        done,
        hasNextCursor,
        nextCursor);
}

TLSFlowConductor::Extension* TLSFlowConductor::buildExtension(Session& session)
{
    Extension* ext = nullptr;

    for (auto& entry : m_subscribers) {
        auto sharedPtr = entry.second.lock();
        if (!sharedPtr) {
            continue;
        }

        ITlsListener::EventMask mask = sharedPtr->getSessionEventMask(session);
        if (mask == 0) {
            continue;
        }

        if (ext == nullptr) {
            if (!m_extensionTable) {
                return nullptr;
            }
            ext = m_extensionTable->alloc(session.getSessionIndex());
            if (ext == nullptr) {
                return nullptr;
            }
        }

        addListenerToExtension(*ext, sharedPtr, mask);
    }

    return ext;
}

void TLSFlowConductor::registerSessionObservers(Session& session)
{
    session.registerObserver(this, STM_EVENT_INBOUND_PACKET_PROCESS, InboundPriorityTLS);
    session.registerObserver(this, STM_EVENT_AGING, AgingPriorityTLS);
}

void TLSFlowConductor::addListenerToExtension(Extension& extension,
                                              const std::shared_ptr<ITlsListener>& listener,
                                              ITlsListener::EventMask mask)
{
    for (uint8_t i = 0; i < static_cast<uint8_t>(Event::MaxTypes); ++i) {
        if (mask & static_cast<ITlsListener::EventMask>(1u << i)) {
            extension.addObserver(static_cast<Event>(i), listener);
        }
    }
}

void TLSFlowConductor::commitNormalCurrentRecord(Extension &extension,
                                                 const TLSRecordContext& context,
                                                 unsigned long packetTimestamp)
{
    auto& tls = extension.getTlsData();
    if (usesTerminalPayloadPath(tls.hsState)) {
        return;
    }

    switch(context.record.recordType) {
        case static_cast<uint8_t>(RecordTypesE::handshakeContent):
            if (tls.hsState == HsState::ChangeCipherSpec) {
                tls.hsState = HsState::Finished;
            } else if(context.record.isClientHello()) {
                updateTLSDataOnClientHello(tls, context, packetTimestamp);
            }
            break;
        case static_cast<uint8_t>(RecordTypesE::changeCipherSpecContent):
            updateTLSDataOnChangeCipherSpec(tls);
            break;
        case static_cast<uint8_t>(RecordTypesE::applicationDataContent):
            updateTLSDataOnApplicationData(tls);
            break;
        case static_cast<uint8_t>(RecordTypesE::alertContent):
            updateTLSDataOnAlert(tls, context);
            break;
            
        default:
            break;
    }
}

void TLSFlowConductor::updateTLSDataOnClientHello(TLSFlow::TLSData& tls,
                                                  const TLSRecordContext& context,
                                                  unsigned long packetTimestamp)
{
    if (clientHelloDataViewer->getClientHelloParsingStatus() !=
        NSProcessor::Engine::IClientHelloDataViewer::ClientHelloParsingStatus::Complete) {
        return;
    }

    ++tls.clientHelloCount;
    tls.clientHelloTimeStamp = packetTimestamp;
    clientHelloDataViewer->getFingerprint(tls.chFingerprint);
    tls.tlsVersionMask = clientHelloDataViewer->getSupportedVersionBitMask();
    tls.earlyDataOffered = context.earlyDataOffered;

    if (tls.hsState == HsState::PreHandshake) {
        tls.hsState = HsState::InProgress;
    }
}

void TLSFlowConductor::updateTLSDataOnChangeCipherSpec(TLSFlow::TLSData& tls)
{
    if(tls.clientHelloCount == 0) {
        ++tls.preCcsSeen;
    } else if (!TLSFlow::isTLS13(tls.tlsVersionMask) && tls.hsState == HsState::InProgress) {
        tls.hsState = HsState::ChangeCipherSpec;
    }
}

void TLSFlowConductor::updateTLSDataOnApplicationData(TLSFlow::TLSData& tls)
{
    if (tls.clientHelloCount == 0) {
        ++tls.earlyDataCount;
    } else if (TLSFlow::isTLS13(tls.tlsVersionMask) &&
               tls.hsState == HsState::InProgress &&
               !tls.earlyDataOffered) {
        tls.hsState = HsState::Finished;
    }
}

void TLSFlowConductor::updateTLSDataOnAlert(TLSFlow::TLSData& tls,
                                           const TLSRecordContext& context)
{
    if (!isActiveHandshakeState(tls.hsState) ||
        !context.record.isAlert() ||
        !context.record.readableAlert ||
        !context.alertTerminating) {
        return;
    }

    tls.hsState = HsState::TerminatedByAlert;
}

void TLSFlowConductor::commitUnsupported(Extension &extension, bool continuationGap)
{
    auto& tls = extension.getTlsData();
    if (continuationGap ||
        tls.hsState == HsState::PreHandshake ||
        (tls.hsState == HsState::InProgress && clientHelloDataViewer &&
         clientHelloDataViewer->getClientHelloParsingStatus() ==
             IClientHelloDataViewer::ClientHelloParsingStatus::Partial))
    {
        tls.hsState = HsState::Unsupported;
    }
}

void TLSFlowConductor::commitViolation(Extension &extension, TLSFlow::TLSViolationMask violationMask)
{
    extension.getTlsData().terminalViolationMask |= violationMask;
}

void TLSFlowConductor::commitError(Session &session, Extension &extension, TLSFlow::TLSViolationMask violationMask)
{
    auto& dst = extension.getTlsData();
    dst.hsState = HsState::Error;
    commitViolation(extension, violationMask);
}

ObserverResult TLSFlowConductor::toObserverResult(const TLSFeatureVerdict& v)
{
    return (v.dispatch == TLSFeatureVerdict::Dispatch::Stop) ? RET_OBSERVER_STOP : RET_OBSERVER_CONTINUE;
}

