//
// Created by sherik on 6/1/22.
//

#pragma once

#include <stdint.h>
#include "application/tls-flow/common/include/Fingerprint.hpp"
#include "application/tls-flow/common/include/Sni.hpp"
#include "PayloadParams.hpp"
#include "../../common/include/TLSData.hpp"

namespace TLSFlow
{
    namespace NSProcessor
	{
        namespace Engine {
		    class IProcessor {

            public:
                enum class ContinuationMode {
                    None,
                    OpaqueRecordTracking,
                    ClientHelloExtensionReassembly,
                    AlertRecordReassembly
                };

                /// --------------------------
                /// TLS Step Result
                /// --------------------------
                enum class TLSStepResult {
                    RecordParsed,
                    NeedMoreData,
                    Unsupported,
                    EndOfPacket,
                    Stop
                };
                /**
                 * @brief
                 * Initialize packet-local TLS processing state for the new step-oriented API.
                 *
                 * @param payloadParams
                 */
                virtual void beginPacket(const PayloadParams &payloadParams) = 0;

                /**
                 * @brief
                 * Process at most one TLS record from the currently associated packet.
                 *
                 * @return TLSStepResult for the current step
                 */
                virtual TLSStepResult processRecord(const TLSData &data) = 0;

                virtual bool isContinuationGap() const { return false; }

                virtual ContinuationMode getContinuationMode() const { return ContinuationMode::None; }

                /**
                 * @brief
                 * Destructor
                 */
                virtual ~IProcessor() = default;
            };
        }
    }
}

