//
// Created by talc on 7/23/25.
//

#pragma once

#include <memory>
#include "../../../diagnostics/controller/include/IDiagnosticsCounters.hpp"
#include "../../../diagnostics/controller/include/TLSSessionExtensionInspector.hpp"

namespace TLSFlow
{
    namespace Context
    {
        namespace Controller
        {
            class IFactory
            {
            public:
                /**
                 * @brief
                 * Init controller code
                 *
                 * @throws InitException
                 */
                virtual void initController() =0;

                /**
                 * @brief
                 * Init legacy code
                 *
                 * @throws InitException
                 */
                virtual void initLegacy() =0;

                /**
                 * @brief
                 * Get Diagnostic module
                 */
                virtual std::shared_ptr<NSDiagnostics::Controller::IDiagnosticsCounters> getDiagnosticsCounters() const =0;

                virtual std::shared_ptr<NSDiagnostics::Controller::TLSSessionExtensionInspector>
                getSessionExtensionInspector() const = 0;

                /**
                 * Destructor
                 */
                virtual ~IFactory() = default;
            };
        } // Controller
    } // Context
} // TLSFlow
