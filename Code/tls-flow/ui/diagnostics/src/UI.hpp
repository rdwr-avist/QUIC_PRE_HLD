//
// Created by talc on 7/27/25.
//

#pragma once

extern "C"
{
#include <lib/term/exp/term.h>
}
namespace TLSFlow
{
    namespace NSDiagnostics
    {
        namespace Controller
        {
            /**
             * @brief
             * Print diagnostics events counters internal command handler
             */
            TERMG_ERROR printEventsStatistics(TERMG_ID id, UINT_32 argc, char *argv[]);

            TERMG_ERROR printSessionExtensionTableSize(TERMG_ID id, UINT_32 argc, char *argv[]);
            TERMG_ERROR searchSessionExtensionTable(TERMG_ID id, UINT_32 argc, char *argv[]);
            TERMG_ERROR printSessionExtensionTable(TERMG_ID id, UINT_32 argc, char *argv[]);
        }
    }
}
