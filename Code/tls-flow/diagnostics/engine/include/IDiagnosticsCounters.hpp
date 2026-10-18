#pragma once

#include "../../common/include/DiagnosticsCounters.hpp"

namespace TLSFlow
{
    namespace NSDiagnostics
    {
        namespace Engine
        {
            class IDiagnosticsCounters
            {
            public:
            	/**
            	* External Interfaces
            	*/

            	/**
            	 * Increment occurance counter for a given event on the engine side
            	 * @param event : event id to increment
            	 */
            	virtual void incEvent(NSDiagnostics::Common::DiagnosticsEvents event) = 0;


            	virtual ~IDiagnosticsCounters() = default;
            };
        } // Engine
    }
} // TLSFingerprint


