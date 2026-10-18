
#include "DiagnosticsCounters.hpp"
#include "../../../../vDP/common/Definitions.hpp"


using namespace TLSFlow::NSDiagnostics::Engine;
using namespace TLSFlow::NSDiagnostics::Common;

/**
* Increment occurance counter for a given event on the engine side
* @param event : event id to increment
* @implements IDiagnosticsCounters::incEvent
*/
void DiagnosticsCounters::incEvent(DiagnosticsEvents event) {
    m_diagnosticsCounters->incCounter(to_underlying(event), 1);
}

/**
* Set the counter manager object of the diagnostics counters
* @param diagnosticsCounters
*/
void DiagnosticsCounters::setDiagnosticsCounter(std::unique_ptr<DiagnosticsCountersTable> diagnosticsCounters) {
    m_diagnosticsCounters = std::move(diagnosticsCounters);
}

