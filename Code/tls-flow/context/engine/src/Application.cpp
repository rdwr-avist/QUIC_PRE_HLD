//
// Created by talc on 7/22/25.
//

#include "../include/Application.hpp"
#include "../../common/include/InitException.hpp"
#include <diag/diag_log_inject.h>

using namespace TLSFlow;
using namespace TLSFlow::Context;
using namespace TLSFlow::Context::Engine;

Application* Application::instance = nullptr;

bool Application::init(std::unique_ptr<IFactory> factory)
{
    /* Validate */
    if(factory == nullptr)
    {
        DIAG_LOGGER_INJECT(TLS_FLOW_INIT);
        return false;
    }

    /* Save factory */
    m_pFactory = move(factory);

    /* Init */
    try
    {
        m_pFactory->init();
    }
    catch(const InitException& e)
    {
        DIAG_LOGGER_INJECT(TLS_FLOW_COMPONENTS_INIT, e.getPhase().c_str(), e.getModule().c_str(), e.getMessage().c_str());
        return false;
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

std::shared_ptr<NSProcessor::Engine::IProcessor> Application::getProcessor() { return m_pFactory->getProcessor(); }

shared_ptr<NSProcessor::Engine::IClientHelloDataViewer> Application::getClientHelloDataViewer()
{
    return m_pFactory->getProcessorViewer();
}

shared_ptr<NSProcessor::Engine::IRecordsDataViewer> Application::getRecordsDataViewer()
{
    return m_pFactory->getRecordsDataViewer();
}

shared_ptr<NSProcessor::Engine::IParsingErrorViewer> Application::getParsingErrorViewer()
{
    return m_pFactory->getParsingErrorViewer();
}

std::shared_ptr<PacketFlow::Engine::ITlsSubject> Application::getClientHelloParserSubject()
{
    return m_pFactory->getClientHelloParserSubject();
}