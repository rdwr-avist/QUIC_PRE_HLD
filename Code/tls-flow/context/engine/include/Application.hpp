//
// Created by talc on 7/22/25.
//

#pragma once

#include "IFactory.hpp"
#include "../../../processor/include/IRecordsDataViewer.hpp"
#include "../../../processor/include/IParsingErrorViewer.hpp"
#include <memory>

namespace TLSFlow
{
    namespace Context
    {
        namespace Engine
        {
            class Application
            {
            private:
                /// --------------------------
                /// Singleton hidden methods
                /// --------------------------
                Application() = default;
                ~Application() = default;

                /// --------------------------
                /// Members
                /// --------------------------
                std::unique_ptr<IFactory> m_pFactory;
                static Application* instance;

            public:
                /// --------------------------
                /// Singleton deleted methods
                /// --------------------------
                Application(const Application& other) = delete;
                Application(Application&& other) = delete;
                Application& operator=(const Application& other) = delete;
                Application& operator=(Application&& other) = delete;

                /**
                 * @brief
                 * Initialize TLSFlow

                 * @param factory

                 * @return true if initialization was successful, false otherwise.
                 */
                bool init(std::unique_ptr<IFactory> factory);

                /**
                 * @brief
                 * Get singleton instance
                 *
                 * @return The only instance of context
                 */
                static Application& getInstance();
                static void destroyInstance();

                /// --------------------------
                /// Getters & Setters
                /// --------------------------
                std::shared_ptr<NSProcessor::Engine::IProcessor> getProcessor();
                std::shared_ptr<NSProcessor::Engine::IClientHelloDataViewer> getClientHelloDataViewer();
                std::shared_ptr<NSProcessor::Engine::IRecordsDataViewer> getRecordsDataViewer();
                std::shared_ptr<NSProcessor::Engine::IParsingErrorViewer> getParsingErrorViewer();
                std::shared_ptr<PacketFlow::Engine::ITlsSubject> getClientHelloParserSubject();

            };
        } // Engine
    } // Context
} // TLSFlow


