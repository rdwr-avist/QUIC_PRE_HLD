#pragma once

#include "../../common/include/DiagnosticsCounters.hpp"

namespace TLSFlow
{
    namespace NSDiagnostics
    {
        namespace Controller
        {
            class IDiagnosticsCounters
            {
            public:

            	/**
            	 * External Interfaces
            	 */

            	/**
				 *
				 * @param event
				 * @return the total number of occurences of the given event, aggregated from all engines.
				 */
                virtual uint64_t getDiagnosticsCounter(Common::DiagnosticsEvents event) const =0;

				/**
				 * Reset the aggregated value of a given event.
				 * @param event
				 */
                virtual void     resetDiagnosticsCounter(Common::DiagnosticsEvents event)  =0;

                /**
                 * Default base destructor
                 */
                virtual ~IDiagnosticsCounters() = default;
            };
        }
    }
}
