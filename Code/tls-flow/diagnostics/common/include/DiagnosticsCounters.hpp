//
// Created by Yohay on 8/9/24
//

#pragma once

#include <cstdint>

namespace TLSFlow
{
    namespace NSDiagnostics
    {
        namespace Common
        {
            enum class DiagnosticsEvents : uint8_t
            {
                SegmentedStateEntriesAlloc,
                SegmentedStateEntriesFree,
                SegmentedStateEntriesReused,
                SegmentedStateEntriesAllocationFailures,
                SegmentedStateNonMatchingSeq,
                SegmentedStateSecondSegmentTooShort, // Probably third segment
                UnsegmentedClientHelloCompleted,
                TwoSegmentClientHelloCompleted,
                ThreeOrMoreSegmentClientHelloCompleted,
                FirstSegmentProcessSuccess,
                ExtensionParserFailures,
            	GeneralParserFailures,
                UnsupportedVersion,
                UnsupportedExtensionsExceedBuffer,
                UnsupportedThirdSegmentRequired,
                UnsupportedRecordLevelFragmentation,
                SegmentedStateWrongSequence,
                UnsupportedEarlyClientHelloBody,
                ContinuationGap,
                ClientHelloContinuationStarted,
                ClientHelloContinuationGap,
                OpaqueRecordContinuationStarted,
                OpaqueRecordContinuationGap,
                ContinuationFullRetransmission,
                ContinuationPartialOverlap,
                SegmentBufferFull,
                // Keep MaxEventId last - used to count number of events
                MaxEventId
            };

        }
    }
}
