#include "../include/TLSSessionExtensionInspectorObserver.hpp"

#include <algorithm>

TLSSessionExtensionInspectorObserver::~TLSSessionExtensionInspectorObserver() noexcept = default;

TLSSessionExtensionInspectorObserver::TLSSessionExtensionInspectorObserver(
    TLSInspectorIpcMessageClass* ipcMessage,
    uint32_t coreId,
    const std::shared_ptr<TLSFlow::Extension::Engine::ITLSSessionExtensionInspector>& visibility)
    : m_ipcMessage(ipcMessage), m_coreId(coreId), m_visibility(visibility)
{
}

STATUS TLSSessionExtensionInspectorObserver::init()
{
    if (m_ipcMessage == nullptr || m_visibility == nullptr)
    {
        return ERROR;
    }

    if (m_ipcMessage->registerObserver(
            IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_SIZE_REQUEST, this) != OK)
    {
        return ERROR;
    }

    if (m_ipcMessage->registerObserver(
            IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_FETCH_REQUEST, this) != OK)
    {
        return ERROR;
    }

    return OK;
}

void TLSSessionExtensionInspectorObserver::handleMsg(IpcMsgData& ipcMsg)
{
    switch (ipcMsg.m_msgType)
    {
        case IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_SIZE_REQUEST:
            handleSizeRequest(ipcMsg);
            break;
        case IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_FETCH_REQUEST:
            handleFetchRequest(ipcMsg);
            break;
        default:
            break;
    }
}

void TLSSessionExtensionInspectorObserver::handleSizeRequest(const IpcMsgData& ipcMsg)
{
    const auto& request = ipcMsg.m_data.m_tlsSessionExtensionTableSizeRequest;
    if (request.m_coreId != 0 && request.m_coreId != m_coreId)
    {
        return;
    }

    IpcMsgData response{};
    response.m_destMask = 0;
    response.m_msgType = IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_SIZE_RESPONSE;
    response.m_data.m_tlsSessionExtensionTableSizeResponse.m_coreId = m_coreId;
    response.m_data.m_tlsSessionExtensionTableSizeResponse.m_size =
        m_visibility->getSessionExtensionCount();
    response.m_data.m_tlsSessionExtensionTableSizeResponse.m_requestId = request.m_requestId;
    m_ipcMessage->sendMsg(response);
}

void TLSSessionExtensionInspectorObserver::handleFetchRequest(const IpcMsgData& ipcMsg)
{
    const auto& request = ipcMsg.m_data.m_tlsSessionExtensionTableFetchRequest;
    if (request.m_coreId != 0 && request.m_coreId != m_coreId)
    {
        return;
    }

    IpcMsgData response{};
    response.m_destMask = 0;
    response.m_msgType = IPC_MSG_TYPE_TLS_SESSION_EXTENSION_TABLE_FETCH_RESPONSE;
    response.m_data.m_tlsSessionExtensionTableFetchResponse.m_coreId = m_coreId;
    response.m_data.m_tlsSessionExtensionTableFetchResponse.m_requestId = request.m_requestId;
    response.m_data.m_tlsSessionExtensionTableFetchResponse.m_activeSize =
        m_visibility->getSessionExtensionCount();
    response.m_data.m_tlsSessionExtensionTableFetchResponse.m_isDone = true;

    if (request.m_maxEntries == 0)
    {
        m_ipcMessage->sendMsg(response);
        return;
    }

    TLSFlow::Extension::Engine::TLSSessionExtensionInspectorQuery query{};
    query.allSessions = request.m_allSessions;
    query.sessionIndex = request.m_sessionIndex;
    query.maxEntries = std::min<uint32_t>(
        request.m_maxEntries, MAX_TLS_SESSION_EXTENSION_RESULT_PER_ENGINE);
    query.hasCursor = request.m_hasCursor;
    query.cursor = request.m_cursor;

    bool done = true;
    bool hasNextCursor = false;
    uint32_t nextCursor = 0;
    const bool fetched = m_visibility->fetchSessionExtensions(
        query,
        [&response](const TLSFlow::Extension::Engine::TLSSessionExtensionInspectorSnapshot& snapshot) {
            auto& result = response.m_data.m_tlsSessionExtensionTableFetchResponse;
            if (result.m_numOfValidEntries >= MAX_TLS_SESSION_EXTENSION_RESULT_PER_ENGINE)
            {
                return false;
            }

            auto& record = result.m_records[result.m_numOfValidEntries++];
            record.m_sessionIndex = snapshot.sessionIndex;
            record.m_hsState = static_cast<uint8_t>(snapshot.hsState);
            record.m_tlsVersionMask = snapshot.tlsVersionMask;
            record.m_clientHelloCount = snapshot.clientHelloCount;
            record.m_earlyDataCount = snapshot.earlyDataCount;
            record.m_earlyDataOffered = snapshot.earlyDataOffered;
            record.m_preCcsSeen = snapshot.preCcsSeen;
            record.m_fingerprintHigh = snapshot.chFingerprint.value().high;
            record.m_fingerprintLow = snapshot.chFingerprint.value().low;
            record.m_terminalViolation = static_cast<uint8_t>(
                TLSFlow::highestTLSViolation(snapshot.terminalViolationMask));
            record.m_clientHelloTimeStamp = snapshot.clientHelloTimeStamp;
            return result.m_numOfValidEntries < MAX_TLS_SESSION_EXTENSION_RESULT_PER_ENGINE;
        },
        done,
        hasNextCursor,
        nextCursor);

    if (fetched)
    {
        response.m_data.m_tlsSessionExtensionTableFetchResponse.m_isDone =
            done || response.m_data.m_tlsSessionExtensionTableFetchResponse.m_numOfValidEntries >=
                        request.m_maxEntries;
        response.m_data.m_tlsSessionExtensionTableFetchResponse.m_hasNextCursor =
            !response.m_data.m_tlsSessionExtensionTableFetchResponse.m_isDone && hasNextCursor;
        response.m_data.m_tlsSessionExtensionTableFetchResponse.m_nextCursor = nextCursor;
    }

    m_ipcMessage->sendMsg(response);
}
