#pragma once

#include <cstdint>
#include <memory>

#include "../../../../../exp/vdpCapi.h"
#include "../../../../../ipc/IpcMessage.hpp"
#include "application/tls-flow/extension/include/ITLSSessionExtensionInspector.hpp"

using TLSInspectorIpcMessageClass =
    ::IpcCommonMessageClass<::IncomingQueue<::IpcMsgData, ::IPC_QUEUE_MAX_ITEMS>,
                            ::OutgoingQueue<::IpcMsgData, ::IPC_QUEUE_MAX_ITEMS>>;

class TLSSessionExtensionInspectorObserver : public IpcMessageObserverBase
{
public:
    ~TLSSessionExtensionInspectorObserver() noexcept;

    TLSSessionExtensionInspectorObserver(
        TLSInspectorIpcMessageClass* ipcMessage,
        uint32_t coreId,
        const std::shared_ptr<TLSFlow::Extension::Engine::ITLSSessionExtensionInspector>& visibility);

    STATUS init();
    void handleMsg(IpcMsgData& ipcMsg);

private:
    void handleSizeRequest(const IpcMsgData& ipcMsg);
    void handleFetchRequest(const IpcMsgData& ipcMsg);

    TLSInspectorIpcMessageClass* m_ipcMessage;
    uint32_t m_coreId;
    std::shared_ptr<TLSFlow::Extension::Engine::ITLSSessionExtensionInspector> m_visibility;
};

namespace TLSFlow
{
    namespace NSDiagnostics
    {
        namespace Engine
        {
            using ::TLSSessionExtensionInspectorObserver;
        }
    }
}
