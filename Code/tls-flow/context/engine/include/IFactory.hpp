//
// Created by talc on 7/22/25.
//

#pragma once

#include "../../../packet-flow/include/ITlsSubject.hpp"
#include "../../../processor/include/IClientHelloDataViewer.hpp"
#include "../../../processor/include/IRecordsDataViewer.hpp"
#include "../../../processor/include/IParsingErrorViewer.hpp"
#include "../../../processor/include/IProcessor.hpp"

namespace TLSFlow
{
    namespace Context
    {
        namespace Engine
        {
            class IFactory
            {
            public:
                /**
                 * @brief
                 * Initialize TLSFlow
                 *
                 * @throws InitException
                 */
                virtual void init() =0;

                /**
                 * @brief
                 * Get session observer
                 *
                 * @return shared_ptr<SessionObserver>
                 */
                virtual std::shared_ptr<PacketFlow::Engine::ITlsSubject> getClientHelloParserSubject() =0;

                /**
                 * @brief
                 * Get IProcessor module
                 */
                virtual std::shared_ptr<NSProcessor::Engine::IProcessor> getProcessor() =0;

                /**
                 * @brief
                 * Get IClientHelloDataViewer module
                 */
                virtual shared_ptr<NSProcessor::Engine::IClientHelloDataViewer> getProcessorViewer() = 0;

                /**
                 * @brief
                 * Get IRecordsDataViewer module
                 */
                virtual shared_ptr<NSProcessor::Engine::IRecordsDataViewer> getRecordsDataViewer() = 0;

                /**
                 * @brief
                 * Get IParsingErrorViewer module
                 */
                virtual shared_ptr<NSProcessor::Engine::IParsingErrorViewer> getParsingErrorViewer() = 0;

                /**
                 * Destructor
                 */
                virtual ~IFactory() = default;
            };
        } // Engine
    } // Context
} // TLSFlow

