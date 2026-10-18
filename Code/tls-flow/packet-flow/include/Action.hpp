//
// Created by talc on 7/28/25.
//

#pragma once

#include <cstdint>

namespace TLSFlow
{
    namespace PacketFlow
    {
        namespace Engine
        {
            enum class Action : uint8_t
            {
                Stop,
                Continue
            };
        } // Engine
    } // PacketFlow
} // TLSFlow
