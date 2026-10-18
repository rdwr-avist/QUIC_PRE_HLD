#pragma once

#include "../include/IDiagnosticsCounters.hpp"
#include "../../../../../sh-data-struct/counters/DPECountersManager.hpp"
#include "../../../../../sh-data-struct/counters/DPCounters.hpp"
#include "memory"

namespace TLSFlow
{
    namespace NSDiagnostics
    {
        namespace Engine
        {
            /**
            * Public aliases
            */
            using DiagnosticsCountersTable = DPECountersManager<DPUint64Counter>;


            class DiagnosticsCounters : public IDiagnosticsCounters
            {
                public:
            	/**
				 * External Interfaces
				 */

	            /**
		         * @implements IDiagnosticsCounters::incEvent
	          	 * Increment occurance counter for a given event on the engine side
				 * @param event : event id to increment
                */
                void incEvent(Common::DiagnosticsEvents e) override;

            	/**
				* Set the counter manager object of the diagnostics counters
				* @param diagnosticsCounters
				*/
                void setDiagnosticsCounter(std::unique_ptr<DiagnosticsCountersTable> diagnosticsCounters);

            	~DiagnosticsCounters() override = default;

                private:
                 /**
                 * Members
                 */
                 std::unique_ptr<DiagnosticsCountersTable> m_diagnosticsCounters = nullptr;
            };
        }
    }
}
