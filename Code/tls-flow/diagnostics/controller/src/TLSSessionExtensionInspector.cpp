#include "../include/TLSSessionExtensionInspector.hpp"

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <sstream>

#include "../../../common/include/TLSData.hpp"
#include "../../../../../exp/vdpCapi.h"

using namespace TLSFlow;
using namespace TLSFlow::NSDiagnostics::Controller;

namespace
{
constexpr uint32_t ResponseTimeoutMs = 10000;

bool compareEngineIds(const std::string& first, const std::string& second)
{
    return std::atoi(first.c_str()) < std::atoi(second.c_str());
}

std::string hsStateToString(uint8_t state)
{
    switch (static_cast<TLSFlow::HsState>(state))
    {
        case TLSFlow::HsState::PreHandshake: return "PreHandshake";
        case TLSFlow::HsState::InProgress: return "InProgress";
        case TLSFlow::HsState::ChangeCipherSpec: return "ChangeCipherSpec";
        case TLSFlow::HsState::Finished: return "Finished";
        case TLSFlow::HsState::Error: return "Error";
        case TLSFlow::HsState::TerminatedByAlert: return "TerminatedByAlert";
        case TLSFlow::HsState::Unsupported: return "Unsupported";
        default: return "Unknown";
    }
}

std::string tlsVersionMaskToString(uint8_t mask)
{
    static const char* names[] = {"SSL3.0", "TLS1.0", "TLS1.1", "TLS1.2", "TLS1.3"};
    std::ostringstream output;
    bool first = true;
    for (uint8_t bit = 0; bit < 5; ++bit)
    {
        if ((mask & static_cast<uint8_t>(1u << bit)) != 0)
        {
            if (!first)
            {
                output << ',';
            }
            output << names[bit];
            first = false;
        }
    }
    return first ? "None" : output.str();
}

std::string violationToString(uint8_t violation)
{
    switch (static_cast<TLSFlow::TLSViolation>(violation))
    {
        case TLSFlow::TLSViolation::None: return "None";
        case TLSFlow::TLSViolation::TLSEnforcementMalformedTLSHandshake: return "MalformedHandshake";
        case TLSFlow::TLSViolation::TLSEnforcementWrongVersion: return "WrongVersion";
        case TLSFlow::TLSViolation::TLSEnforcementMaxCiphers: return "MaxCiphers";
        case TLSFlow::TLSViolation::TLSEnforcementMaxCompression: return "MaxCompression";
        case TLSFlow::TLSViolation::TLSEnforcementMaxExtensions: return "MaxExtensions";
        case TLSFlow::TLSViolation::TLSEnforcementPrematureClosure: return "PrematureClosure";
        case TLSFlow::TLSViolation::TLSEnforcementIncompleteHandshake: return "IncompleteHandshake";
        default: return "Unknown";
    }
}

std::string fingerprintToString(const IpcMsgTlsSessionExtensionTableRecord& record)
{
    std::ostringstream output;
    output << std::uppercase << std::hex << std::setfill('0')
           << std::setw(16) << record.m_fingerprintHigh
           << std::setw(16) << record.m_fingerprintLow;
    return output.str();
}

}

TLSSessionExtensionInspector::TLSSessionExtensionInspector(
    IpcMessageClass* ipcMessage,
    uint32_t numOfCores)
    : m_ipcMessage(ipcMessage), m_numOfCores(numOfCores), m_pageState(numOfCores + 1)
{
}

STATUS TLSSessionExtensionInspector::init()
{
    if (m_ipcMessage == nullptr)
    {
        return ERROR;
    }

    char semaphoreName[] = "tls_session_extension_inspector";
    OSITCG_sem_create(semaphoreName, 1, &m_responseSem);

    const auto sizeStatus = m_ipcMessage->registerObserver(
        IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_SIZE_RESPONSE, this);
    const auto fetchStatus = m_ipcMessage->registerObserver(
        IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_FETCH_RESPONSE, this);
    if (sizeStatus != OK || fetchStatus != OK)
    {
        return ERROR;
    }

    const std::vector<uint8_t> sizeWidths{8, 12};
    const std::vector<std::string> sizeNames{"Engine", "Size"};
    m_sizeTablePrinter.init(&sizeWidths, &sizeNames);

    const std::vector<uint8_t> entryWidths{8, 12, 18, 18, 20};
    const std::vector<std::string> entryNames{"Engine", "Session", "State", "TLS Versions", "Violation"};
    m_entryTablePrinter.init(&entryWidths, &entryNames);
    return OK;
}

void TLSSessionExtensionInspector::handleMsg(IpcMsgData& ipcMsg)
{
    switch (ipcMsg.m_msgType)
    {
        case IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_SIZE_RESPONSE:
            handleSizeResponse(ipcMsg);
            break;
        case IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_FETCH_RESPONSE:
            handleFetchResponse(ipcMsg);
            break;
        default:
            break;
    }
}

bool TLSSessionExtensionInspector::requestSize(uint32_t coreId)
{
    if (coreId > m_numOfCores)
    {
        return false;
    }

    resetRequestState();
    m_requestKind = RequestKind::Size;
    m_requestedCore = coreId;
    m_pendingResponses = coreId == 0 ? m_numOfCores : 1;
    m_requestActive = true;
    ++m_requestId;
    OSITCG_sem_p(m_responseSem);

    if (!sendSizeRequest(coreId))
    {
        m_requestActive = false;
        m_pendingResponses = 0;
        OSITCG_sem_v(m_responseSem);
        return false;
    }
    return waitForResponses();
}

bool TLSSessionExtensionInspector::requestEntries(uint32_t coreId,
                                                   bool allSessions,
                                                   int32_t sessionIndex,
                                                   uint32_t maxEntries)
{
    if (coreId > m_numOfCores || maxEntries == 0)
    {
        return false;
    }

    resetRequestState();
    m_requestKind = RequestKind::Entries;
    m_requestedCore = coreId;
    m_allSessions = allSessions;
    m_sessionIndex = sessionIndex;
    m_pendingResponses = coreId == 0 ? m_numOfCores : 1;
    m_requestActive = true;
    ++m_requestId;
    for (uint32_t engine = 1; engine <= m_numOfCores; ++engine)
    {
        m_pageState[engine].remaining = maxEntries;
    }
    OSITCG_sem_p(m_responseSem);

    if (!sendFetchRequest(coreId, false, 0, maxEntries))
    {
        m_requestActive = false;
        m_pendingResponses = 0;
        OSITCG_sem_v(m_responseSem);
        return false;
    }
    return waitForResponses();
}

bool TLSSessionExtensionInspector::sendSizeRequest(uint32_t coreId)
{
    IpcMsgData message{};
    message.m_destMask = IPC_SHARED_MEMORY_BROADCAST_INDEX;
    message.m_msgType = IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_SIZE_REQUEST;
    message.m_data.m_tlsSessionExtensionTableSizeRequest.m_coreId = coreId;
    message.m_data.m_tlsSessionExtensionTableSizeRequest.m_requestId = m_requestId;
    return m_ipcMessage->sendMsg(message) == QUEUE_OK;
}

bool TLSSessionExtensionInspector::sendFetchRequest(uint32_t coreId,
                                                     bool hasCursor,
                                                     uint32_t cursor,
                                                     uint32_t maxEntries)
{
    IpcMsgData message{};
    message.m_destMask = IPC_SHARED_MEMORY_BROADCAST_INDEX;
    message.m_msgType = IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_FETCH_REQUEST;
    message.m_data.m_tlsSessionExtensionTableFetchRequest.m_coreId = coreId;
    message.m_data.m_tlsSessionExtensionTableFetchRequest.m_allSessions = m_allSessions;
    message.m_data.m_tlsSessionExtensionTableFetchRequest.m_sessionIndex = m_sessionIndex;
    message.m_data.m_tlsSessionExtensionTableFetchRequest.m_maxEntries = maxEntries;
    message.m_data.m_tlsSessionExtensionTableFetchRequest.m_hasCursor = hasCursor;
    message.m_data.m_tlsSessionExtensionTableFetchRequest.m_cursor = cursor;
    message.m_data.m_tlsSessionExtensionTableFetchRequest.m_requestId = m_requestId;
    return m_ipcMessage->sendMsg(message) == QUEUE_OK;
}

bool TLSSessionExtensionInspector::waitForResponses()
{
    const bool completed = OSITCG_sem_p_wait(
        m_responseSem, OSTIMG_ms_to_tick(ResponseTimeoutMs));
    const bool success = completed && m_pendingResponses == 0;
    if (!completed)
    {
        m_pendingResponses = 0;
    }
    m_requestActive = false;
    OSITCG_sem_v(m_responseSem);
    return success;
}

bool TLSSessionExtensionInspector::isSelectedCore(uint32_t coreId) const
{
    return coreId > 0 && coreId <= m_numOfCores &&
           (m_requestedCore == 0 || m_requestedCore == coreId);
}

void TLSSessionExtensionInspector::completeEngine(uint32_t coreId)
{
    if (coreId > m_numOfCores || m_pageState[coreId].complete)
    {
        return;
    }
    m_pageState[coreId].complete = true;
    if (m_pendingResponses > 0)
    {
        --m_pendingResponses;
    }
    signalIfComplete();
}

void TLSSessionExtensionInspector::signalIfComplete()
{
    if (m_pendingResponses == 0)
    {
        OSITCG_sem_v(m_responseSem);
    }
}

void TLSSessionExtensionInspector::resetRequestState()
{
    m_requestActive = false;
    m_requestKind = RequestKind::None;
    m_requestedCore = 0;
    m_pendingResponses = 0;
    for (auto& state : m_pageState)
    {
        state = EnginePageState{};
    }
    m_sizeTablePrinter.clearTableRows();
    m_entryTablePrinter.clearTableRows();
    m_entryRows.clear();
}

void TLSSessionExtensionInspector::handleSizeResponse(const IpcMsgData& ipcMsg)
{
    if (!m_requestActive || m_requestKind != RequestKind::Size)
    {
        return;
    }

    const auto& response = ipcMsg.m_data.m_tlsSessionExtensionTableSizeResponse;
    if (response.m_requestId != m_requestId || !isSelectedCore(response.m_coreId))
    {
        return;
    }

    if (m_pageState[response.m_coreId].complete)
    {
        return;
    }

    m_sizeTablePrinter.addTableRow({std::to_string(response.m_coreId), std::to_string(response.m_size)});
    completeEngine(response.m_coreId);
}

void TLSSessionExtensionInspector::handleFetchResponse(const IpcMsgData& ipcMsg)
{
    if (!m_requestActive || m_requestKind != RequestKind::Entries)
    {
        return;
    }

    const auto& response = ipcMsg.m_data.m_tlsSessionExtensionTableFetchResponse;
    if (response.m_requestId != m_requestId || !isSelectedCore(response.m_coreId))
    {
        return;
    }

    auto& page = m_pageState[response.m_coreId];
    if (page.complete || response.m_numOfValidEntries > page.remaining)
    {
        return;
    }

    for (uint32_t index = 0;
         index < response.m_numOfValidEntries && index < MAX_TLS_SESSION_EXTENSION_RESULT_PER_ENGINE;
         ++index)
    {
        const auto& record = response.m_records[index];
        m_entryRows.push_back({response.m_coreId, record.m_sessionIndex, {
            std::to_string(response.m_coreId),
            std::to_string(record.m_sessionIndex),
            hsStateToString(record.m_hsState),
            tlsVersionMaskToString(record.m_tlsVersionMask),
            violationToString(record.m_terminalViolation)}});
    }

    page.remaining -= response.m_numOfValidEntries;
    if (!m_allSessions || response.m_isDone || page.remaining == 0 ||
        response.m_numOfValidEntries == 0 || !response.m_hasNextCursor)
    {
        completeEngine(response.m_coreId);
        return;
    }

    page.hasCursor = true;
    page.cursor = response.m_nextCursor;
    if (!sendFetchRequest(response.m_coreId, page.hasCursor, page.cursor, page.remaining))
    {
        completeEngine(response.m_coreId);
    }
}

void TLSSessionExtensionInspector::fillSizeResultStream(std::ostream* output, bool clearRows)
{
    TablePrinter::TableSorterByColumn sorter{compareEngineIds, 0};
    sorter.sortTable(m_sizeTablePrinter);
    m_sizeTablePrinter.printTable(output);
    if (clearRows)
    {
        m_sizeTablePrinter.clearTableRows();
    }
}

void TLSSessionExtensionInspector::fillEntryResultStream(std::ostream* output, bool clearRows)
{
    std::sort(m_entryRows.begin(), m_entryRows.end(), [](const EntryRow& first, const EntryRow& second) {
        if (first.engineId != second.engineId)
        {
            return first.engineId < second.engineId;
        }
        return first.sessionIndex < second.sessionIndex;
    });

    m_entryTablePrinter.clearTableRows();
    for (const auto& row : m_entryRows)
    {
        m_entryTablePrinter.addTableRow(row.cells);
    }

    TablePrinter::TableSorterByColumn sorter{compareEngineIds, 0};
    sorter.sortTable(m_entryTablePrinter);
    m_entryTablePrinter.printTable(output);
    if (clearRows)
    {
        m_entryTablePrinter.clearTableRows();
        m_entryRows.clear();
    }
}

bool TLSSessionExtensionInspector::hasEntryResults() const
{
    return !m_entryRows.empty();
}
