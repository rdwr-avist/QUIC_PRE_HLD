//
// Created by talc on 7/22/25.
//

#pragma once

#include "IFactory.hpp"
#include "../../../processor/src/Processor.hpp"
#include "../../../packet-flow/src/TLSFlowConductor.hpp"
#include "../../../diagnostics/engine/include/TLSSessionExtensionInspectorObserver.hpp"

namespace TLSFlow
{
    namespace Context
    {
        namespace Engine
        {
            class Factory : public IFactory
            {
            private:
                /// --------------------------
                /// Members
                /// --------------------------
                /* IPC */
                IpcMessageClass* ipcMessage;

                /* Memory */
                SharedMemoryManager* sharedMemoryManager;
                uint32_t coreId;

                /* Session observer */
                std::shared_ptr<PacketFlow::Engine::TLSFlowConductor> tlsFlowConductor;
                SessionTable* sessionTable;

                /* Diagnostic */
                unique_ptr<NSDiagnostics::Engine::DiagnosticsCounters> diagnosticsCounters;
                unique_ptr<NSDiagnostics::Engine::TLSSessionExtensionInspectorObserver> sessionExtensionInspectorObserver;

                /* Processor */
                shared_ptr<NSProcessor::Engine::Processor> processor;

                /// --------------------------
                /// Helpers
                /// --------------------------

                /**
                 * @brief
                 * Build session observer
                 */
                void buildTLSFlowConductor();

                /**
                 * @brief
                 * Build processor
                 */
                void buildProcessor();

                /**
                 * @brief
                 * Build diagnostics
                 */
                void buildDiagnostics();

                /**
                 * @brief Build the engine-side TLS extension visibility observer.
                 */
                void buildSessionExtensionInspector();

            public:
                /**
                 * Constructor
                 */
                Factory(IpcMessageClass* ipcMessage,
                    SharedMemoryManager* sharedMemoryManager,
                    uint32_t coreId,
                    SessionTable* sessionTable);

                /**
                 * @override IFactory::init
                 *
                 * @throws InitException
                 */
                void init() override;

                /**
                 * @override IFactory::getClientHelloParserSubject
                 *
                 * @return std::shared_ptr<PacketFlow::Engine::TLSFlowConductor>
                 */
                std::shared_ptr<PacketFlow::Engine::ITlsSubject> getClientHelloParserSubject() override;

                /**
                 * @override IFactory::getProcessor
                 *
                 * @return std::shared_ptr<NSProcessor::Engine::IProcessor>
                 */
                std::shared_ptr<NSProcessor::Engine::IProcessor> getProcessor() override;

                /**
                 * @override IFactory::getProcessorViewer
                 *
                 * @return std::shared_ptr<NSProcessor::Engine::IClientHelloDataViewer>
                 */
                std::shared_ptr<NSProcessor::Engine::IClientHelloDataViewer> getProcessorViewer() override;

                /**
                 * @override IFactory::getRecordsDataViewer
                 *
                 * @return std::shared_ptr<NSProcessor::Engine::IRecordsDataViewer>
                 */
                std::shared_ptr<NSProcessor::Engine::IRecordsDataViewer> getRecordsDataViewer() override;

                /**
                 * @override IFactory::getParsingErrorViewer
                 *
                 * @return std::shared_ptr<NSProcessor::Engine::IParsingErrorViewer>
                 */
                std::shared_ptr<NSProcessor::Engine::IParsingErrorViewer> getParsingErrorViewer() override;

            };
        } // Engine
    } // Context
} // TLSFlow

