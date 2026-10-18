//
// Created by talc on 7/27/25.
//

#include "UI.hpp"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "../../../context/controller/include/Application.hpp"
#include "../../../diagnostics/controller/include/TLSSessionExtensionInspector.hpp"
#include "common/Definitions.hpp"

using namespace TLSFlow;
using namespace TLSFlow::NSDiagnostics::Controller;
using namespace Context::Controller;
namespace
{
TERMG_ERROR printDiagnosticsStatisticsHelp(TERMG_ID id) {
    termg_printf(id, "Print the aggregated statistics events counters\n\n");
    termg_printf(id, "Command Syntax:\n\n");
    termg_printf(id, "\tsystem internal tls parser events <get|reset>\n");
    return TERMG_error_okey_E;
}

void printOutput(TERMG_ID termId, std::stringstream& output)
{
    std::string line;
    while (std::getline(output, line))
    {
        if (termg_printf(termId, "%s\n", line.c_str()) < 0)
        {
            return;
        }
    }
}

bool parseUnsigned(const char* text, uint32_t& value)
{
    if (text == nullptr || *text == '\0' || *text == '-')
    {
        return false;
    }

    errno = 0;
    char* end = nullptr;
    const unsigned long parsed = std::strtoul(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' || parsed > UINT32_MAX)
    {
        return false;
    }
    value = static_cast<uint32_t>(parsed);
    return true;
}

bool parseSessionIndex(const char* text, int32_t& value)
{
    if (text == nullptr || *text == '\0')
    {
        return false;
    }

    errno = 0;
    char* end = nullptr;
    const long parsed = std::strtol(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' || parsed < INT32_MIN || parsed > INT32_MAX)
    {
        return false;
    }
    value = static_cast<int32_t>(parsed);
    return true;
}

bool parseCore(const char* text, uint32_t& coreId)
{
    if (std::strcmp(text, "all") == 0)
    {
        coreId = 0;
        return true;
    }

    return parseUnsigned(text, coreId) && coreId > 0;
}

void printUsage(TERMG_ID termId)
{
    termg_printf(termId, "Usage:\n");
    termg_printf(termId, "  system internal tls session-extension-table size <all|engine-index>\n");
    termg_printf(termId, "  system internal tls session-extension-table search <all|engine-index> <session-index>\n");
    termg_printf(termId, "  system internal tls session-extension-table print <all|engine-index> <all|session-index> <max-elements-in-engine>\n");
}
}

TERMG_ERROR TLSFlow::NSDiagnostics::Controller::printEventsStatistics(TERMG_ID id, UINT_32 argc, char *argv[]) {
    termg_printf(id, "\nWeb DDoS Aggregated Counter Events:\n\n");
    uint64_t val;

    static constexpr char eventNames[to_underlying(NSDiagnostics::Common::DiagnosticsEvents::MaxEventId)][50] = {
        "Segmented State allocations                   ",
        "Segmented State frees                         ",
        "Segmented State Reused                        ",
        "Segmented State allocation failures           ",
        "Unexpected tcp seq during segmented process   ",
        "Unexpected length during segmented process    ",
        "Unsegmented ClientHello completed             ",
        "Two-segment ClientHello completed             ",
        "Three-or-more-segment ClientHello completed  ",
        "First Segment Prcocess success                ",
        "Extensions Parser Failures                    ",
        "General Parser failures                       ",
        "Unsupported version                           ",
        "Unsupported extensions exceed buffer          ",
        "Unsupported third segment required            ",
        "Unsupported record-level fragmentation        ",
        "Segmented state wrong sequence                ",
        "Unsupported early ClientHello body            ",
        "Continuation gap                              ",
        "ClientHello continuation started              ",
        "ClientHello continuation gap                  ",
        "Opaque record continuation started            ",
        "Opaque record continuation gap                ",
        "Continuation full retransmission              ",
        "Continuation partial overlap                  ",
        "Segment buffer full                           ",
        };

    bool bGet = false, bReset = false;

    if (argc == 1) {
        bGet = true;
    }
    else if (argc == 2) {
        std::string command = argv[1];
        if (command == "get") {
            bGet = true;
        }else if (command == "reset"){

            bReset = true;
        }
        else {
            printDiagnosticsStatisticsHelp(id);
            return TERMG_error_okey_E;
        }
    }


    if(bGet) {
        for(uint8_t event = 0; event < to_underlying(NSDiagnostics::Common::DiagnosticsEvents::MaxEventId); ++event) {
            val = TLSFlow::Context::Controller::Application::getInstance().getDiagnosticsApi()->getDiagnosticsCounter(static_cast<NSDiagnostics::Common::DiagnosticsEvents>(event));
            termg_printf(id, "%s : %llu\n", eventNames[event], val);
        }
        termg_printf(id, "\n");
    }
    else if (bReset) {
        for(uint8_t event = 0; event < to_underlying(NSDiagnostics::Common::DiagnosticsEvents::MaxEventId); ++event) {
            TLSFlow::Context::Controller::Application::getInstance().getDiagnosticsApi()->resetDiagnosticsCounter(static_cast<NSDiagnostics::Common::DiagnosticsEvents>(event));
        }
        termg_printf(id, "Ressetting Event Counters - Done\n");
    }
}

TERMG_ERROR TLSFlow::NSDiagnostics::Controller::printSessionExtensionTableSize(TERMG_ID termId, UINT_32 argc, char* argv[])
{
    if (argc != 2 || std::strcmp(argv[1], "help") == 0)
    {
        printUsage(termId);
        return argc == 2 ? TERMG_error_okey_E : TERMG_error_syntax_E;
    }

    uint32_t coreId = 0;
    if (!parseCore(argv[1], coreId))
    {
        termg_printf(termId, "Error: invalid engine index.\n");
        return TERMG_error_unknown_parameter_E;
    }

    auto manager = Application::getInstance().getSessionExtensionInspector();
    if (manager == nullptr || !manager->requestSize(coreId))
    {
        termg_printf(termId, "Error: failed to fetch TLS session extension table size.\n");
        return TERMG_error_error_E;
    }

    std::stringstream output;
    manager->fillSizeResultStream(&output);
    printOutput(termId, output);
    return TERMG_error_okey_E;
}

TERMG_ERROR TLSFlow::NSDiagnostics::Controller::searchSessionExtensionTable(TERMG_ID termId, UINT_32 argc, char* argv[])
{
    if (argc != 3 || std::strcmp(argv[1], "help") == 0)
    {
        printUsage(termId);
        return argc == 2 ? TERMG_error_okey_E : TERMG_error_syntax_E;
    }

    uint32_t coreId = 0;
    int32_t sessionIndex = 0;
    if (!parseCore(argv[1], coreId) || !parseSessionIndex(argv[2], sessionIndex))
    {
        termg_printf(termId, "Error: invalid engine index or session index.\n");
        return TERMG_error_unknown_parameter_E;
    }

    auto manager = Application::getInstance().getSessionExtensionInspector();
    if (manager == nullptr || !manager->requestEntries(coreId, false, sessionIndex, 1))
    {
        termg_printf(termId, "Error: failed to search TLS session extension table.\n");
        return TERMG_error_error_E;
    }

    std::stringstream output;
    if (manager->hasEntryResults())
    {
        manager->fillEntryResultStream(&output);
    }
    else
    {
        output << "No TLS session extension found for session index " << sessionIndex << ".\n";
    }
    printOutput(termId, output);
    return TERMG_error_okey_E;
}

TERMG_ERROR TLSFlow::NSDiagnostics::Controller::printSessionExtensionTable(TERMG_ID termId, UINT_32 argc, char* argv[])
{
    if (argc != 4 || std::strcmp(argv[1], "help") == 0)
    {
        printUsage(termId);
        return argc == 2 ? TERMG_error_okey_E : TERMG_error_syntax_E;
    }

    uint32_t coreId = 0;
    uint32_t maxEntries = 0;
    bool allSessions = false;
    int32_t sessionIndex = 0;
    if (!parseCore(argv[1], coreId) || !parseUnsigned(argv[3], maxEntries) || maxEntries == 0)
    {
        termg_printf(termId, "Error: invalid engine index or maximum entries.\n");
        return TERMG_error_unknown_parameter_E;
    }

    if (std::strcmp(argv[2], "all") == 0)
    {
        allSessions = true;
    }
    else if (!parseSessionIndex(argv[2], sessionIndex))
    {
        termg_printf(termId, "Error: invalid session index.\n");
        return TERMG_error_unknown_parameter_E;
    }

    auto manager = Application::getInstance().getSessionExtensionInspector();
    if (manager == nullptr || !manager->requestEntries(coreId, allSessions, sessionIndex, maxEntries))
    {
        termg_printf(termId, "Error: failed to print TLS session extension table.\n");
        return TERMG_error_error_E;
    }

    std::stringstream output;
    if (manager->hasEntryResults())
    {
        manager->fillEntryResultStream(&output);
    }
    else
    {
        output << "TLS session extension table is empty.\n";
    }
    printOutput(termId, output);
    return TERMG_error_okey_E;
}
