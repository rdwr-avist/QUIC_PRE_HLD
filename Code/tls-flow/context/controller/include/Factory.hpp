//
// Created by talc on 7/23/25.
//

#pragma once

#include "IFactory.hpp"
#include "IPCHandler.hpp"
#include "../../../diagnostics/controller/src/DiagnosticsCounters.hpp"

namespace TLSFlow
{
    namespace Context
    {
        namespace Controller
        {
            class Factory : public IFactory
            {
            private:
                /// --------------------------
                /// Members
                /// --------------------------
                /* IPC */
                IpcMessageClass* ipcMsgOb;
                std::shared_ptr<IPCHandler> ipcHandler;

                /* Memory */
                SharedMemoryManager* sharedMemoryManager;
                RegularMemoryManager* regularMemoryManager;
                uint32_t numOfCores;

                /* Diagnostic */
                shared_ptr<NSDiagnostics::Controller::DiagnosticsCounters> diagnosticsCounters;
                shared_ptr<NSDiagnostics::Controller::TLSSessionExtensionInspector> sessionExtensionInspector;

                /// --------------------------
                /// Helpers
                /// --------------------------
                /**
                 * @brief
                 * Build diagnostics
                 */
                void buildDiagnostics();

                /**
                 * @brief
                 * Build UI
                 */
                void buildUI();
                void buildSessionExtensionInspector();
            public:
                /**
                 * Constructor
                 */
                Factory(IpcMessageClass* pIpcMsgOb, SharedMemoryManager *sharedMemoryManager, RegularMemoryManager *regularMemoryManager, uint32_t numOfCores);

                /**
                 * @override IFactory::initController
                 *
                 * @throws InitException
                 */
                void initController() override;

                /**
                 * @override IFactory::initLegacy
                 *
                 * @throws InitException
                 */
                void initLegacy() override;

                /**
                 * @override IFactory::getDiagnosticsCounters
                 *
                 * @return std::shared_ptr<NSDiagnostics::Controller::IDiagnosticsCounters>
                 */
                std::shared_ptr<NSDiagnostics::Controller::IDiagnosticsCounters> getDiagnosticsCounters() const override;
                std::shared_ptr<NSDiagnostics::Controller::TLSSessionExtensionInspector>
                getSessionExtensionInspector() const override;
            };
        } // Controller
    } // Context
} // TLSFlow
