//
// Created by talc on 7/23/25.
//

#include "../include/IPCHandler.hpp"

using namespace TLSFlow;
using namespace TLSFlow::Context;
using namespace TLSFlow::Context::Controller;

void IPCHandler::sendStatus(bool isEnabled) {
    IpcMsgData msg;

    msg.m_msgType = IPC_MSG_TYPE_TLS_SSL_LICENSE_UPDATE;
    msg.m_destMask = IPC_SHARED_MEMORY_BROADCAST_INDEX;
    msg.m_data.m_asnProtectionStatus.status = isEnabled;

    /* Send message */
    if (m_pIpcMsgObj != nullptr) {
        m_pIpcMsgObj->sendMsg(msg);
    }
}