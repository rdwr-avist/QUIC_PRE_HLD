//
// Created by talc on 7/22/25.
//

#include "../include/Factory.hpp"
#include "../../../diagnostics/common/include/DiagnosticsCounters.hpp"
#include "../../../diagnostics/engine/src/DiagnosticsCounters.hpp"
#include "../../../processor/src/Md5Digest.hpp"
#include "../../../processor/src/TlsParser.hpp"
#include "../../common/include/InitException.hpp"
#include "sh-data-struct/counters/counter-table/common/Status.hpp"
#include "../../../extension/include/TLSSessionExtensionTable.hpp"
#include "../../../common/include/Definitions.hpp"
#include "../../../diagnostics/engine/include/TLSSessionExtensionInspectorObserver.hpp"

using namespace TLSFlow;
using namespace TLSFlow::Context;
using namespace TLSFlow::Context::Engine;

Factory::Factory(IpcMessageClass* ipcMessage,
                 SharedMemoryManager* sharedMemoryManager,
                 uint32_t coreId,
                 SessionTable* sessionTable) :
    ipcMessage(ipcMessage),
    sharedMemoryManager(sharedMemoryManager),
    coreId(coreId),
    sessionTable(sessionTable) {}

/// ----------------------------------------------------------------------------------
///     Interface implementations - IFactory
/// ----------------------------------------------------------------------------------

void Factory::init()
{
    /* Build modules */
    buildTLSFlowConductor();
    buildSessionExtensionInspector();
    buildDiagnostics();
    buildProcessor();

    /* Inject dependencies */
    tlsFlowConductor->setProcessor(processor);
    tlsFlowConductor->setViewer(processor);
    tlsFlowConductor->setRecordsViewer(processor);
    tlsFlowConductor->setParsingErrorViewer(processor);
}

std::shared_ptr<PacketFlow::Engine::ITlsSubject> Factory::getClientHelloParserSubject() { return tlsFlowConductor; }
shared_ptr<NSProcessor::Engine::IProcessor> Factory::getProcessor() { return processor; }
shared_ptr<NSProcessor::Engine::IClientHelloDataViewer> Factory::getProcessorViewer() { return processor; }
shared_ptr<NSProcessor::Engine::IRecordsDataViewer> Factory::getRecordsDataViewer() { return processor; }
shared_ptr<NSProcessor::Engine::IParsingErrorViewer> Factory::getParsingErrorViewer() { return processor; }

/// ----------------------------------------------------------------------------------
///     Helpers
/// ----------------------------------------------------------------------------------

void Factory::buildTLSFlowConductor()
{
    /* Init session observer */
    auto extensionTable = std::make_unique<TLSFlow::TlsSessionExtensionTable>();
    tlsFlowConductor = std::make_shared<PacketFlow::Engine::TLSFlowConductor>(std::move(extensionTable));
    if (tlsFlowConductor == nullptr)
        throw InitException("Engine Init", "Packet-Flow", "Failed to create session observer object");

    /* Register to session global observer */
    if (sessionTable->registerGlobalObserver(tlsFlowConductor.get(), STM_EVENT_NEW_SESSION, NewSessionPriorityTLS) != RET_STM_SUCCESS)
        throw InitException("Engine Init", "Packet-Flow", "Failed to register connection observer to session table");
}

void Factory::buildDiagnostics(){

    /* Build diagnostics counters  */
    auto diagnosticsCountersTable = make_unique<NSDiagnostics::Engine::DiagnosticsCountersTable>();
    if(diagnosticsCountersTable->init(sharedMemoryManager, coreId, MemoryChunksID::MEMORY_INSTANCE_SHARED_TLS_FLOW_DIAGNOSTICS_FIRST_ID, MemoryChunksID::MEMORY_INSTANCE_SHARED_TLS_FLOW_DIAGNOSTICS_LAST_ID - 1, to_underlying(NSDiagnostics::Common::DiagnosticsEvents::MaxEventId)) != NSCounterTable::Status::Ok)
    {
        throw InitException("Engine Init", "Diagnostics", "Failed to init diagnostic counters table");
    }
	diagnosticsCounters = make_unique<NSDiagnostics::Engine::DiagnosticsCounters>();
    diagnosticsCounters->setDiagnosticsCounter(std::move(diagnosticsCountersTable));
}

void Factory::buildSessionExtensionInspector()
{
    sessionExtensionInspectorObserver =
        make_unique<NSDiagnostics::Engine::TLSSessionExtensionInspectorObserver>(
            ipcMessage, coreId, tlsFlowConductor);
    if (sessionExtensionInspectorObserver->init() != OK)
    {
        throw InitException("Engine Init", "Diagnostics", "Failed to init TLS extension visibility observer");
    }
}

void Factory::buildProcessor()
{
    // Build Digest
    auto digester = make_unique<NSProcessor::Engine::Md5Digest>();

    // Build parsers
    auto trecord  = make_unique<NSProcessor::Engine::TLSRecordParser>();
    auto thsake   = make_unique<NSProcessor::Engine::TlsHandshakeParser>();
    auto hello    = make_unique<NSProcessor::Engine::ClientHelloParser>();
    auto ext      = make_unique<NSProcessor::Engine::ExtensionsParser>();
    auto suppVer  = make_unique<NSProcessor::Engine::SupportedVersionsExtensionParser>();
    auto suppGrou = make_unique<NSProcessor::Engine::SupportedGroupsExtensionParser>();
    auto ciphers  = make_unique<NSProcessor::Engine::CipherSuitesParser>();
    auto sni      = make_unique<NSProcessor::Engine::SniParser>();
    auto alert    = make_unique<NSProcessor::Engine::TlsAlertParser>();

    // Create and init parsers container
    auto parsers =  make_unique<NSProcessor::Engine::IParsersContainer>();
	parsers->init(move(trecord), move(thsake), move(hello), move(ext), move(suppGrou), move(suppVer), move(ciphers), move(sni), move(alert), move(digester));

    /* Init processor */
    processor = make_shared<NSProcessor::Engine::Processor>();
    processor->init(move(parsers), sessionTable, std::move(diagnosticsCounters));
}