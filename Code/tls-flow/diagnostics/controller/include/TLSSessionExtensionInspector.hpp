#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

#include "../../../../../exp/vdpCapi.h"
#include "../../../../../ipc/IpcMessage.hpp"
#include "sh-data-struct/table-printer/TablePrinter.hpp"

namespace TLSFlow
{
    namespace NSDiagnostics
    {
        namespace Controller
        {
            class TLSSessionExtensionInspector : public IpcMessageObserverBase
            {
            public:
                TLSSessionExtensionInspector(IpcMessageClass* ipcMessage, uint32_t numOfCores);
                ~TLSSessionExtensionInspector() override = default;

                STATUS init();
                void handleMsg(IpcMsgData& ipcMsg) override;

                bool requestSize(uint32_t coreId);
                bool requestEntries(uint32_t coreId,
                                    bool allSessions,
                                    int32_t sessionIndex,
                                    uint32_t maxEntries);

                void fillSizeResultStream(std::ostream* output, bool clearRows = true);
                void fillEntryResultStream(std::ostream* output, bool clearRows = true);
                bool hasEntryResults() const;

            private:
                enum class RequestKind
                {
                    None,
                    Size,
                    Entries
                };

                struct EnginePageState
                {
                    uint32_t remaining{0};
                    bool hasCursor{false};
                    uint32_t cursor{0};
                    bool complete{false};
                };

                struct EntryRow
                {
                    uint32_t engineId;
                    int32_t sessionIndex;
                    std::vector<std::string> cells;
                };

                bool sendSizeRequest(uint32_t coreId);
                bool sendFetchRequest(uint32_t coreId,
                                      bool hasCursor,
                                      uint32_t cursor,
                                      uint32_t maxEntries);
                bool waitForResponses();
                bool isSelectedCore(uint32_t coreId) const;
                void completeEngine(uint32_t coreId);
                void signalIfComplete();
                void resetRequestState();

                void handleSizeResponse(const IpcMsgData& ipcMsg);
                void handleFetchResponse(const IpcMsgData& ipcMsg);

                IpcMessageClass* m_ipcMessage;
                uint32_t m_numOfCores;
                OSITCG_sem_id_TYP m_responseSem;
                uint32_t m_pendingResponses{0};
                uint32_t m_requestId{0};
                bool m_requestActive{false};
                RequestKind m_requestKind{RequestKind::None};
                uint32_t m_requestedCore{0};
                bool m_allSessions{false};
                int32_t m_sessionIndex{0};
                std::vector<EnginePageState> m_pageState;
                std::vector<EntryRow> m_entryRows;

                TablePrinter m_sizeTablePrinter;
                TablePrinter m_entryTablePrinter;
            };
        }
    }
}
