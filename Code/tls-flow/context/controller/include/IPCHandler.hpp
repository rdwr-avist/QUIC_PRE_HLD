//
// Created by talc on 7/23/25.
//

#pragma once

#include <ipc/IpcMessage.hpp>

namespace TLSFlow
{
    namespace Context
    {
        namespace Controller
        {
            class IPCHandler
            {
            private:
                IpcMessageClass *m_pIpcMsgObj;

            public:
                /**
                 * Constructor
                 */
                IPCHandler(IpcMessageClass *pIpcMsgObj) : m_pIpcMsgObj(pIpcMsgObj){};

                /**
                 * @brief
                 * Sends the feature status
                 *
                 * @param isEnabled
                 */
                void sendStatus(bool isEnabled);
            };
        } // Controller
    } // Context
} // TLSFlow