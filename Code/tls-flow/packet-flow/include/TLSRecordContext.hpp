#pragma once

#include "../../processor/include/Record.hpp"

namespace TLSFlow
{
    namespace PacketFlow
    {
        namespace Engine
        {
            /**
             * @brief Current TLS record data available before session state is committed.
             *
             * The record reference is non-owning and is valid for the duration of
             * the current record dispatch. The ClientHello-derived fields are
             * populated for ClientHello records and are zero/false otherwise.
             */
            struct TLSRecordContext
            {
                const NSProcessor::Engine::Record& record;
                uint8_t currentClientHelloSupportedVersionsMask;
                bool earlyDataOffered;
                bool alertTerminating{false};
            };
        } // Engine
    } // PacketFlow
} // TLSFlow