
#pragma once

#include <cstdint>

namespace TLSFlow
{
    namespace PacketFlow
    {
        namespace Engine
        {
            enum class TLSPriority : uint8_t
            {
                Enforcement,
                Visibility,
                Fingerprint,

                Max // Keep this as last
            };
        } // Engine
    } // PacketFlow
} // TLSFlow
