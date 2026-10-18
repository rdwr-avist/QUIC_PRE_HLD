//
// Created by talc on 7/23/25.
//

#include "../include/Factory.hpp"
#include "../../common/include/InitException.hpp"
#include <application/shared-data/DPSharedDoublePolicyArray.hpp>
#include <sh-data-struct/counters/counter-table/controller/CounterTable.hpp>
#include "../../../diagnostics/controller/src/DiagnosticsCounters.hpp"
#include "../../../diagnostics/common/include/DiagnosticsCounters.hpp"
#include "common/Definitions.hpp"
#include <shmman/include/SharedMemoryChunkInstanceIDs.hpp>
#include "../../../ui/diagnostics/src/UI.hpp"

extern "C"
{
#include "../../../IDS/exp/ids_cmn.h"
}

using namespace TLSFlow;
using namespace TLSFlow::Context;
using namespace TLSFlow::Context::Controller;

Factory::Factory(IpcMessageClass* pIpcMsgOb, SharedMemoryManager *sharedMemoryManager, RegularMemoryManager *regularMemoryManager, uint32_t numOfCores) :
    ipcMsgOb(pIpcMsgOb), sharedMemoryManager(sharedMemoryManager), regularMemoryManager(regularMemoryManager), numOfCores(numOfCores) {}

/// ----------------------------------------------------------------------------------
///     Interface implementations - IFactory
/// ----------------------------------------------------------------------------------

void Factory::initController()
{
    /* Build IPC Handler */
    ipcHandler = make_shared<IPCHandler>(move(ipcMsgOb));

    buildDiagnostics();
}

void Factory::initLegacy()
{
    if(IDSP_SSL_LICENSE != 0)
    {
        /* Notify TLS Flow feature is enabled */
        ipcHandler->sendStatus(true);
    }

    buildUI();
}

shared_ptr<NSDiagnostics::Controller::IDiagnosticsCounters> Factory::getDiagnosticsCounters() const { return diagnosticsCounters; }

/// ----------------------------------------------------------------------------------
///     Helpers
/// ----------------------------------------------------------------------------------

void Factory::buildDiagnostics()
{
    /* Build diagnostics counters table */
	diagnosticsCounters = make_shared<NSDiagnostics::Controller::DiagnosticsCounters>();
    auto diagnosticsCountersTable = make_unique<NSDiagnostics::Controller::DiagnosticsCountersTable>();
    if(diagnosticsCountersTable->init(0, MemoryChunksID::MEMORY_INSTANCE_SHARED_TLS_FLOW_DIAGNOSTICS_FIRST_ID, MemoryChunksID::MEMORY_INSTANCE_SHARED_TLS_FLOW_DIAGNOSTICS_LAST_ID - 1, MemoryChunksID::MEMORY_INSTANCE_SHARED_TLS_FLOW_DIAGNOSTICS_LAST_ID,
         sharedMemoryManager, regularMemoryManager, to_underlying(NSDiagnostics::Common::DiagnosticsEvents::MaxEventId), numOfCores) != NSCounterTable::Status::Ok)
    {
        throw InitException("Controller Init", "Diagnostics", "Failed to init diagnostic counters table");
    }
	diagnosticsCounters->setDiagnosticsCounter(std::move(diagnosticsCountersTable));

    buildSessionExtensionInspector();
}

void Factory::buildSessionExtensionInspector()
{
    sessionExtensionInspector =
        make_shared<NSDiagnostics::Controller::TLSSessionExtensionInspector>(ipcMsgOb, numOfCores);
    if (sessionExtensionInspector->init() != OK)
    {
        throw InitException("Controller Init", "Diagnostics", "Failed to init TLS extension visibility");
    }
}

void Factory::buildUI()
{
        termgBind(MIB_rsSystemInternalTlsParserEvents_CNS, NSDiagnostics::Controller::printEventsStatistics, 0, nullptr, 0);
        termgBind(MIB_rsSystemInternalTlsSessionExtensionTableSize_CNS,
              NSDiagnostics::Controller::printSessionExtensionTableSize, 0, nullptr, 0);
        termgBind(MIB_rsSystemInternalTlsSessionExtensionTableSearch_CNS,
              NSDiagnostics::Controller::searchSessionExtensionTable, 0, nullptr, 0);
        termgBind(MIB_rsSystemInternalTlsSessionExtensionTablePrint_CNS,
              NSDiagnostics::Controller::printSessionExtensionTable, 0, nullptr, 0);
        termgAddHelp(MIB_rsSystemInternalTlsParser_CNS);
        termgAddHelp(MIB_rsSystemInternalTlsSessionExtensionTable_CNS);

        /* Bind some internal commands to the support file. */
        termSupportG_RegisterCmd(MIB_rsSystemInternalTlsParserEvents_CNS, nullptr);
        termSupportG_RegisterCmd(MIB_rsSystemInternalTlsSessionExtensionTableSize_CNS, nullptr);
        termSupportG_RegisterCmd(MIB_rsSystemInternalTlsSessionExtensionTableSearch_CNS, nullptr);
        termSupportG_RegisterCmd(MIB_rsSystemInternalTlsSessionExtensionTablePrint_CNS, nullptr);
}

shared_ptr<NSDiagnostics::Controller::TLSSessionExtensionInspector>
Factory::getSessionExtensionInspector() const
{
    return sessionExtensionInspector;
}