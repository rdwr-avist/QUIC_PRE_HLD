#pragma once

#include "../../common/include/TLSViolation.hpp"

namespace TLSFlow
{
    namespace PacketFlow
    {
        namespace Engine
        {
            struct TLSFeatureVerdict
            {
                enum class Dispatch : uint8_t
                {
                    Continue,
                    Stop
                };

                Dispatch dispatch;
                TLSViolationMask violationMask;

                TLSFeatureVerdict()
                    : dispatch(Dispatch::Continue), violationMask(TLSViolationMaskNone) {}

                TLSFeatureVerdict(Dispatch d)
                    : dispatch(d), violationMask(TLSViolationMaskNone) {}

                TLSFeatureVerdict(Dispatch d, TLSViolationMask m)
                    : dispatch(d), violationMask(m) {}

                TLSFeatureVerdict(Dispatch d, TLSViolation v)
                    : dispatch(d), violationMask(toTLSViolationMask(v)) {}
            };
        } // Engine
    } // PacketFlow
} // TLSFlow
