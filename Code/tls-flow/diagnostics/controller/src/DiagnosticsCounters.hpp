#pragma once

#include "../include/IDiagnosticsCounters.hpp"
#include "../../../../../sh-data-struct/counters/DPCACountersManager.hpp"
#include "../../../../../sh-data-struct/counters/DPCounters.hpp"
#include "memory"

namespace TLSFlow
{
    namespace NSDiagnostics
    {
        namespace Controller
        {
            /**
            * Public aliases
            */
            using DiagnosticsCountersTable = DPCACountersManager<DPUint64Counter>;

            class DiagnosticsCounters : public IDiagnosticsCounters
            {
                public:
            	/*
            	 * Override functions
                 */

				/*
				 * @param event
				 * @implements IDiagnosticsCounters::getDiagnosticsCounter
				 * @return the total number of occurences of the given event, aggregated from all engines.
				 */
                uint64_t getDiagnosticsCounter(Common::DiagnosticsEvents event) const override;
            	/**
				 * Reset the aggregated value of a given event.
				 * @param event
				 * @implements IDiagnosticsCounters::resetDiagnosticsCounter
				 */
                void     resetDiagnosticsCounter(Common::DiagnosticsEvents event)  override;

				/**
				 * Default destructor
				 */
				~DiagnosticsCounters() override = default;

            	/**
				 * Set the counter manager object of the diagnostics counter
				 * @param diagnosticsCounters
				 */
            	void setDiagnosticsCounter(std::unique_ptr<DiagnosticsCountersTable> diagnosticsCounters);

			private:
                 /**
                 * Members
                 */
                 std::unique_ptr<DiagnosticsCountersTable> m_diagnosticsCounters = nullptr;
            };
        }
    }
}
