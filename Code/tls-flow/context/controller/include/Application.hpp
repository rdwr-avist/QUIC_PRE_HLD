//
// Created by talc on 7/23/25.
//

#pragma once

#include "IFactory.hpp"

namespace TLSFlow
{
    namespace Context
    {
        namespace Controller
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
                /// Init phase enumeration
                /// --------------------------
                enum InitPhase
                {
                    Controller_E,
                    Legacy_E
                };

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
                 *
                 * @param phase
                 * @param factory
                 *
                 * @return true if initialization was successful, false otherwise.
                 */
                bool init(InitPhase phase, std::unique_ptr<IFactory> factory = nullptr);

                /**
                 * @brief
                 * Get singleton instance
                 *
                 * @return The only instance of application
                 */
                static Application& getInstance();
                static void destroyInstance();

                /// --------------------------
                /// Getters & Setters
                /// --------------------------
                std::shared_ptr<NSDiagnostics::Controller::IDiagnosticsCounters> getDiagnosticsApi();
                std::shared_ptr<NSDiagnostics::Controller::TLSSessionExtensionInspector>
                getSessionExtensionInspector();

            };
        } // Controller
    } // Context
} // TLSFlow