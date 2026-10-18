
#include "DiagnosticsCounters.hpp"
#include "../../../../vDP/common/Definitions.hpp"

using namespace TLSFlow::NSDiagnostics::Controller;
using namespace TLSFlow::NSDiagnostics::Common;

/**
 *
 * @param event
 * @return the total number of occurences of the given event, aggregated from all engines.
 */
uint64_t DiagnosticsCounters::getDiagnosticsCounter(DiagnosticsEvents event) const  {
    return m_diagnosticsCounters->updateAggregateCounter(to_underlying(event), true)->fetchCounter();
}

/**
 * Reset the aggregated value of a given enent.
 * @param event
 */
void DiagnosticsCounters::resetDiagnosticsCounter(DiagnosticsEvents event)   {
	m_diagnosticsCounters->updateAggregateCounter(to_underlying(event), true)->reset();
}


/**
 * Set the counter manager object of the diagnostics counter
 * @param diagnosticsCounters
 */
void DiagnosticsCounters::setDiagnosticsCounter(std::unique_ptr<DiagnosticsCountersTable>  diagnosticsCounters)  {
   m_diagnosticsCounters = std::move(diagnosticsCounters);
}

