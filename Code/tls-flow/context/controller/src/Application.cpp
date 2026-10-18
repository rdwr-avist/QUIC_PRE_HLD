//
// Created by talc on 7/23/25.
//

#include "../include/Application.hpp"
#include "../../common/include/InitException.hpp"
#include <diag/diag_log_inject.h>

using namespace TLSFlow;
using namespace TLSFlow::Context;
using namespace TLSFlow::Context::Controller;

Application* Application::instance = nullptr;

bool Application::init(InitPhase phase, std::unique_ptr<IFactory> factory)
{
    switch(phase)
    {
        case Controller_E:
        {
            /* Validate */
            if(factory == nullptr)
            {
                DIAG_LOGGER_INJECT(TLS_FLOW_INIT);
                return false;
            }

            /* Save factory */
            m_pFactory = std::move(factory);

            /* Init */
            try
            {
                m_pFactory->initController();
            }
            catch (const InitException& e)
            {
                DIAG_LOGGER_INJECT(TLS_FLOW_COMPONENTS_INIT, e.getPhase().c_str(), e.getModule().c_str(), e.getMessage().c_str());
                return false;
            }
            break;
        }
        case Legacy_E:
        {
            if(m_pFactory == nullptr)
            {
                DIAG_LOGGER_INJECT(TLS_FLOW_INIT);
                return false;
            }

            try
            {
                m_pFactory->initLegacy();
            }
            catch (const InitException& e)
            {
                DIAG_LOGGER_INJECT(TLS_FLOW_COMPONENTS_INIT, e.getPhase().c_str(), e.getModule().c_str(), e.getMessage().c_str());
                return false;
            }
            break;
        }
        default:
            break;
    }
    return true;
}

Application &Application::getInstance() {
    if (instance == nullptr) {
        instance = new Application();
    }
    return *instance;
}

void Application::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

std::shared_ptr<NSDiagnostics::Controller::IDiagnosticsCounters> Application::getDiagnosticsApi()
{
    return m_pFactory->getDiagnosticsCounters();
}

std::shared_ptr<NSDiagnostics::Controller::TLSSessionExtensionInspector>
Application::getSessionExtensionInspector()
{
    return m_pFactory->getSessionExtensionInspector();
}