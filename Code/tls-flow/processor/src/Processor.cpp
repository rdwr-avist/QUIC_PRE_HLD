//
// Created by YohayG on 14/06/2022.
//

#include "Processor.hpp"
#include "../include/PayloadParams.hpp"
#include "application/tls-flow/common/include/TLSData.hpp"

using namespace TLSFlow::NSProcessor::Engine;
using namespace TLSFlow::NSDiagnostics::Common;
constexpr uint8_t Processor::openParentheses;
constexpr uint8_t Processor::closingParentheses;
constexpr uint8_t Processor::openSqParentheses;
constexpr uint8_t Processor::closingSqParentheses;

namespace
{
bool serialLess(uint32_t left, uint32_t right)
{
    return static_cast<int32_t>(left - right) < 0;
}

bool serialLessOrEqual(uint32_t left, uint32_t right)
{
    return left == right || serialLess(left, right);
}

bool serialGreater(uint32_t left, uint32_t right)
{
    return serialLess(right, left);
}

uint8_t tlsVersionMaskFromWireVersion(uint16_t version)
{
    using TLSFlow::TLSVersionBit;

    switch (version)
    {
        case 0x0300:
            return static_cast<uint8_t>(1u << static_cast<uint8_t>(TLSVersionBit::SSL30));
        case 0x0301:
            return static_cast<uint8_t>(1u << static_cast<uint8_t>(TLSVersionBit::TLS10));
        case 0x0302:
            return static_cast<uint8_t>(1u << static_cast<uint8_t>(TLSVersionBit::TLS11));
        case 0x0303:
            return static_cast<uint8_t>(1u << static_cast<uint8_t>(TLSVersionBit::TLS12));
        case 0x0304:
            return static_cast<uint8_t>(1u << static_cast<uint8_t>(TLSVersionBit::TLS13));
        default:
            return 0;
    }
}
}

void Processor::beginPacket(const PayloadParams &payloadParams)
{
    m_stepPacketError = false;
    m_stepPacketIsRetransmission = false;
    m_stepContinuationGap = false;
    m_stepContinuationMode = IProcessor::ContinuationMode::None;
    captureStepPacketMetadata(payloadParams);
    m_pSegmentBuffer = &m_initialSegment;
    m_pExtensionLocators = m_pSegmentBuffer->getLocatorsArray();
    m_pSegmentBuffer->setState(SegmentBuffer::State::InitialNonSegmented);
    m_pDigest = &m_pSegmentBuffer->getDigest();
    setState(ProcessorState::SingleSegment);

    // Always keep the live packet cursor available for the current packet.
    m_parserLocationPtr = m_packetPayloadPtr;
    m_lengthLeftToParse = m_packetPayloadLen;

    const bool resumed = resumeOrDiscardStepContinuation(payloadParams);
    if (resumed)
    {
        if (!m_stepPacketIsRetransmission && !m_stepContinuationGap)
        {
            m_records.clear();
            m_record = Record{};
        }
        return;
    }

    if (m_stepPacketError)
    {
        m_stepPacketInitialized = false;
        return;
    }

    resetStepState();
}

void Processor::captureStepPacketMetadata(const PayloadParams &payloadParams)
{
    // Copy out only the scalar values we need, by value - never retain a pointer/reference
    // to the caller's PayloadParams beyond this call (it may be a temporary).
    m_packetPayloadPtr = payloadParams.getPayload();
    m_packetPayloadLen = payloadParams.getLength();
    m_sessionIdx = payloadParams.getSessionIdx();
    m_nextExpectedSeq = payloadParams.getTcpSeq() + payloadParams.getLength();
    m_stepPacketInitialized = true;
}

void Processor::resetStepState()
{
    m_stepNextRecordOrdinal = 0;
    m_stepCurrentRecordType = 0;
    m_stepCurrentRecordLength = 0;
    m_stepCurrentRecordOrdinal = 0;
    resetStepClientHelloState();
    m_bSniExtensionParsed = false;
    m_clientHelloParsingStatus = IClientHelloDataViewer::ClientHelloParsingStatus::None;
    m_sni.clear();
    m_records.clear();
    m_fp = Fingerprint();
    lastVersion = 0;
    m_numOfCipherSuites = 0;
    m_numOfExtensions = 0;
    m_numOfCompressions = 0;
    m_lastError = ParsingError::None;
    m_record = Record{};
    m_stepPacketError = false;
    m_stepContinuationGap = false;
    m_stepContinuationMode = IProcessor::ContinuationMode::None;
    m_currentRecordIsSegmented = false;
}

bool Processor::resumeOrDiscardStepContinuation(const PayloadParams &payloadParams)
{
    bool resumed = false;
    bool hasPendingBuffer = segmentBufferManager->access(payloadParams.getSessionIdx(), [&](auto& segmentBufferHand)
    {
        const uint32_t start = payloadParams.getTcpSeq();
        const uint32_t end = start + payloadParams.getLength();
        const uint32_t expected = segmentBufferHand.getNextSequence();

        if (start == expected)
        {
            resumed = resumeStepFromSegmentBuffer(payloadParams, segmentBufferHand, 0);
        }
        else if (serialLessOrEqual(end, expected))
        {
            m_stepPacketIsRetransmission = true;
            m_diagnosticsCounters->incEvent(DiagnosticsEvents::ContinuationFullRetransmission);
            resumed = true;
        }
        else if (serialLess(start, expected) && serialGreater(end, expected))
        {
            const uint32_t overlap = expected - start;
            m_parserLocationPtr += overlap;
            m_lengthLeftToParse -= static_cast<uint16_t>(overlap);
            m_diagnosticsCounters->incEvent(DiagnosticsEvents::ContinuationPartialOverlap);
            resumed = resumeStepFromSegmentBuffer(payloadParams, segmentBufferHand,
                                                  static_cast<uint16_t>(overlap));
        }
        else
        {
            m_stepContinuationGap = true;
            switch (segmentBufferHand.getContinuationMode())
            {
                case SegmentBuffer::ContinuationMode::OpaqueRecordTracking:
                    m_stepContinuationMode = IProcessor::ContinuationMode::OpaqueRecordTracking;
                    break;
                case SegmentBuffer::ContinuationMode::ClientHelloExtensionReassembly:
                    m_stepContinuationMode = IProcessor::ContinuationMode::ClientHelloExtensionReassembly;
                    break;
                case SegmentBuffer::ContinuationMode::AlertRecordReassembly:
                    m_stepContinuationMode = IProcessor::ContinuationMode::AlertRecordReassembly;
                    break;
                case SegmentBuffer::ContinuationMode::None:
                default:
                    m_stepContinuationMode = IProcessor::ContinuationMode::None;
                    break;
            }
            m_diagnosticsCounters->incEvent(DiagnosticsEvents::ContinuationGap);
            if (segmentBufferHand.getContinuationMode() == SegmentBuffer::ContinuationMode::OpaqueRecordTracking)
            {
                m_diagnosticsCounters->incEvent(DiagnosticsEvents::OpaqueRecordContinuationGap);
            }
            else if (segmentBufferHand.getContinuationMode() == SegmentBuffer::ContinuationMode::ClientHelloExtensionReassembly)
            {
                m_diagnosticsCounters->incEvent(DiagnosticsEvents::ClientHelloContinuationGap);
            }
        }
    });

    if (!hasPendingBuffer)
    {
        return false;
    }

    if (m_stepContinuationGap)
    {
        segmentBufferManager->deallocate(payloadParams.getSessionIdx());
        m_diagnosticsCounters->incEvent(DiagnosticsEvents::SegmentedStateEntriesFree);
        resetContinuationPointers();
        return true;
    }

    if (resumed)
    {
        return true;
    }

    segmentBufferManager->deallocate(payloadParams.getSessionIdx());
    resetContinuationPointers();
    return false;
}

IProcessor::TLSStepResult Processor::processRecord(const TLSData &data)
{
    if (!m_stepPacketInitialized)
    {
        return IProcessor::TLSStepResult::Stop;
    }

    if (m_stepContinuationGap)
    {
        return IProcessor::TLSStepResult::Unsupported;
    }

    if (m_lengthLeftToParse == 0)
    {
        return IProcessor::TLSStepResult::EndOfPacket;
    }

    if (m_stepPacketIsRetransmission)
    {
        return IProcessor::TLSStepResult::NeedMoreData;
    }


    // Delegate to the legacy single-record processing flow and translate the result.
    RecordProcessResult r = processSingleRecord(data);
    switch (r)
    {
        case RecordProcessResult::ContinueProcessing:
            return IProcessor::TLSStepResult::RecordParsed;
        case RecordProcessResult::Segmented:
            return IProcessor::TLSStepResult::NeedMoreData;
        case RecordProcessResult::Unsupported:
            return IProcessor::TLSStepResult::Unsupported;
        case RecordProcessResult::EndOfPacket:
            return IProcessor::TLSStepResult::EndOfPacket;
        case RecordProcessResult::Error:
        case RecordProcessResult::StopProcessing:
        default:
            return IProcessor::TLSStepResult::Stop;
    }
}

void Processor::resetStepClientHelloState()
{
    clearStepClientHelloValues();
    m_lastError = ParsingError::None;
}

void Processor::clearStepClientHelloValues()
{
    m_supportedVersionsBitMask = 0;
    m_supportedVersionsParsed = false;
    m_earlyDataOffered = false;
}

bool Processor::resumeStepFromSegmentBuffer(const PayloadParams& payloadParams,
                                             SegmentBuffer& segmentBuffer,
                                             uint16_t overlap)
{
    m_pSegmentBuffer = &segmentBuffer;
    m_pDigest = &m_pSegmentBuffer->getDigest();
    m_pExtensionLocators = m_pSegmentBuffer->getLocatorsArray();
    if (segmentBuffer.getContinuationMode() == SegmentBuffer::ContinuationMode::ClientHelloExtensionReassembly)
    {
        setState(ProcessorState::LastSegment);
        m_stepContinuationMode = IProcessor::ContinuationMode::ClientHelloExtensionReassembly;
    }
    else if (segmentBuffer.getContinuationMode() == SegmentBuffer::ContinuationMode::AlertRecordReassembly)
    {
        setState(ProcessorState::SingleSegment);
        m_stepContinuationMode = IProcessor::ContinuationMode::AlertRecordReassembly;
    }
    else if (segmentBuffer.getContinuationMode() == SegmentBuffer::ContinuationMode::OpaqueRecordTracking)
    {
        setState(ProcessorState::SingleSegment);
        m_stepContinuationMode = IProcessor::ContinuationMode::OpaqueRecordTracking;
    }
    else
    {
        setState(ProcessorState::SingleSegment);
        m_stepContinuationMode = IProcessor::ContinuationMode::None;
    }
    m_nextExpectedSeq = payloadParams.getTcpSeq() + payloadParams.getLength();
    if (segmentBuffer.getContinuationMode() == SegmentBuffer::ContinuationMode::ClientHelloExtensionReassembly &&
        payloadParams.getLength() > overlap)
    {
        segmentBuffer.setClientHelloSegmentCount(segmentBuffer.getClientHelloSegmentCount() + 1);
    }
    if (overlap != 0)
    {
        m_stepPacketIsRetransmission = false;
    }
    return true;
}

void Processor::resetContinuationPointers()
{
    m_pSegmentBuffer = &m_initialSegment;
    m_pDigest = &m_initialSegment.getDigest();
    m_pExtensionLocators = m_initialSegment.getLocatorsArray();
    setState(ProcessorState::SingleSegment);
}

void Processor::deallocateSegmentBuff()
{
    if (m_pSegmentBuffer != &m_initialSegment)
    {
        if (!segmentBufferManager->deallocate(m_sessionIdx))
        {
            DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_ERROR_DEALLOCATE_SEGSTATE);
        }
        m_diagnosticsCounters->incEvent(DiagnosticsEvents::SegmentedStateEntriesFree);
        resetContinuationPointers();
    }
}

bool Processor::prepareSegmentBufferForContinuation(SegmentBuffer::ContinuationMode mode, bool &allocated)
{
    allocated = false;

    if (m_pSegmentBuffer != &m_initialSegment)
    {
        if (m_pSegmentBuffer->getContinuationMode() != SegmentBuffer::ContinuationMode::None)
        {
            return false;
        }

        m_pSegmentBuffer->resetContinuationState();
        m_pSegmentBuffer->setContinuationMode(mode);
        m_pExtensionLocators = m_pSegmentBuffer->getLocatorsArray();
        return true;
    }

    bool found = false;
    bool reused = false;
    segmentBufferManager->access(m_sessionIdx, [&](auto& segmentBufferHand)
    {
        found = true;
        if (segmentBufferHand.getContinuationMode() == SegmentBuffer::ContinuationMode::None)
        {
            segmentBufferHand.resetContinuationState();
            segmentBufferHand.setContinuationMode(mode);
            m_pSegmentBuffer = &segmentBufferHand;
            m_pExtensionLocators = m_pSegmentBuffer->getLocatorsArray();
            reused = true;
        }
    });

    if (reused)
    {
        return true;
    }

    if (found)
    {
        return false;
    }

    if (!segmentBufferManager->allocate(m_sessionIdx, [&](auto& segmentBufferHand)
    {
        segmentBufferHand.init();
        segmentBufferHand.setContinuationMode(mode);
        m_pSegmentBuffer = &segmentBufferHand;
        m_pExtensionLocators = m_pSegmentBuffer->getLocatorsArray();
    }))
    {
        return false;
    }

    allocated = true;
    m_diagnosticsCounters->incEvent(DiagnosticsEvents::SegmentedStateEntriesAlloc);
    return true;
}

void Processor::finishSegmentBufferContinuation()
{
    if (m_pSegmentBuffer == &m_initialSegment)
    {
        return;
    }

    if (m_pSegmentBuffer->getAlertAlignmentOffset() == 0)
    {
        deallocateSegmentBuff();
        return;
    }

    const uint8_t alignmentOffset = m_pSegmentBuffer->getAlertAlignmentOffset();
    m_pSegmentBuffer->resetContinuationState();
    m_pSegmentBuffer->setAlertAlignmentOffset(alignmentOffset);
    m_pSegmentBuffer->setNextSequence(m_nextExpectedSeq);
    m_pDigest = &m_pSegmentBuffer->getDigest();
    m_pExtensionLocators = m_pSegmentBuffer->getLocatorsArray();
    setState(ProcessorState::SingleSegment);
}

Processor::RecordProcessResult Processor::processSingleRecord(const TLSData &data)
{
    if (m_pSegmentBuffer->isAlertRecordReassembly())
    {
        return processSegmentedAlertContinuation();
    }

    if (m_pSegmentBuffer->getContinuationMode() == SegmentBuffer::ContinuationMode::OpaqueRecordTracking)
    {
        return processSegmentedOpaqueContinuation();
    }

    if (m_pSegmentBuffer->getContinuationMode() == SegmentBuffer::ContinuationMode::ClientHelloExtensionReassembly)
    {
        return processSegmentedClientHelloContinuation();
    }

    return processNewRecord(data);
}

Processor::RecordProcessResult Processor::processSegmentedClientHelloContinuation()
{
    const uint16_t extensionRemaining = m_pSegmentBuffer->getExtensionRemainingLength();
    uint16_t totalRecordLength = m_pSegmentBuffer->getTotalRecordLength();
    const uint16_t bytesToCopy = std::min(extensionRemaining, m_lengthLeftToParse);

    m_numOfCipherSuites = m_pSegmentBuffer->getNumOfCiphers();
    m_numOfCompressions = m_pSegmentBuffer->getNumOfCompressions();
    lastVersion = m_pSegmentBuffer->getLastVersion();
    m_clientHelloParsingStatus = IClientHelloDataViewer::ClientHelloParsingStatus::Partial;
    if (bytesToCopy != 0)
    {
        std::memcpy(m_pSegmentBuffer->getBuffer().data() + m_pSegmentBuffer->getBufferUsed(),
                    m_parserLocationPtr, bytesToCopy);
        m_pSegmentBuffer->setBufferUsed(m_pSegmentBuffer->getBufferUsed() + bytesToCopy);
        m_pSegmentBuffer->setExtensionRemainingLength(extensionRemaining - bytesToCopy);
        m_pSegmentBuffer->setContinuationRemainingLength(
            m_pSegmentBuffer->getContinuationRemainingLength() - bytesToCopy);
        m_parserLocationPtr += bytesToCopy;
        m_lengthLeftToParse -= bytesToCopy;
        m_pSegmentBuffer->setNextSequence(m_nextExpectedSeq);
    }

    if (m_pSegmentBuffer->getExtensionRemainingLength() != 0)
    {
        return RecordProcessResult::Segmented;
    }

    m_pSegmentBuffer->setNextLength(0);
    auto innerProcessResult = processExtensions();
    if (RecordProcessResult::ContinueProcessing != innerProcessResult)
    {
        if (RecordProcessResult::Error == innerProcessResult)
        {
            m_diagnosticsCounters->incEvent(DiagnosticsEvents::ExtensionParserFailures);
        }
        deallocateSegmentBuff();
        return innerProcessResult;
    }

    if (!m_supportedVersionsParsed)
    {
        m_supportedVersionsBitMask = tlsVersionMaskFromWireVersion(m_parsers->clientHello->getVersion());
    }

    const uint32_t clientHelloSegmentCount = m_pSegmentBuffer->getClientHelloSegmentCount();
    m_pDigest->finalize(m_fp);
    m_initialSegment.getDigest().copy(*m_pDigest);
    finishSegmentBufferContinuation();
    m_clientHelloParsingStatus = IClientHelloDataViewer::ClientHelloParsingStatus::Complete;

    // Add the ClientHello record to the records list
    m_record = Record{};
    m_record.recordType = static_cast<uint8_t>(RecordTypesE::handshakeContent);
    m_record.handshakeType = static_cast<uint8_t>(HandShakeTypesE::client_hello);
    m_record.recordLength = totalRecordLength;
    if(m_records.size() >= maxRecords){
        m_lastError = ParsingError::RecordsExceedDataViewerCapacity;
        return RecordProcessResult::StopProcessing;
    }
    m_records.push_back(m_record);

    if (clientHelloSegmentCount == 2)
    {
        m_diagnosticsCounters->incEvent(DiagnosticsEvents::TwoSegmentClientHelloCompleted);
    }
    else if (clientHelloSegmentCount >= 3)
    {
        m_diagnosticsCounters->incEvent(DiagnosticsEvents::ThreeOrMoreSegmentClientHelloCompleted);
    }

    // Reset state for processing any remaining records
    setState(ProcessorState::SingleSegment);

    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::processSegmentedOpaqueContinuation()
{
    const uint16_t remaining = m_pSegmentBuffer->getContinuationRemainingLength();
    const uint16_t consumed = std::min(remaining, m_lengthLeftToParse);

    m_parserLocationPtr += consumed;
    m_lengthLeftToParse -= consumed;
    m_pSegmentBuffer->setContinuationRemainingLength(remaining - consumed);
    m_pSegmentBuffer->setNextSequence(m_nextExpectedSeq);

    if (remaining > consumed)
    {
        return RecordProcessResult::Segmented;
    }

    m_record = Record{};
    m_record.recordType = m_pSegmentBuffer->getContinuationRecordType();
    m_record.handshakeType = m_pSegmentBuffer->getContinuationHandshakeType();
    m_record.recordLength = m_pSegmentBuffer->getContinuationRecordLength();
    if (m_records.size() >= maxRecords)
    {
        m_lastError = ParsingError::RecordsExceedDataViewerCapacity;
        finishSegmentBufferContinuation();
        return RecordProcessResult::StopProcessing;
    }
    m_records.push_back(m_record);
    finishSegmentBufferContinuation();
    return RecordProcessResult::ContinueProcessing;
}

bool Processor::startSegmentedAlert(const TLSData &data)
{
    bool allocated = false;
    if (!prepareSegmentBufferForContinuation(SegmentBuffer::ContinuationMode::AlertRecordReassembly, allocated))
    {
        return false;
    }

    auto& segmentBuffer = *m_pSegmentBuffer;
    const uint16_t bytesReceived = m_lengthLeftToParse;
    const uint16_t bytesToStore = std::min<uint16_t>(bytesReceived, TLSRecordHeaderSize + 2);
    if (bytesToStore != 0)
    {
        std::memcpy(segmentBuffer.getBuffer().data(), m_parserLocationPtr, bytesToStore);
    }
    segmentBuffer.setBufferUsed(bytesToStore);
    segmentBuffer.setNextSequence(m_nextExpectedSeq);
    segmentBuffer.setContinuationMode(SegmentBuffer::ContinuationMode::AlertRecordReassembly);
    segmentBuffer.setContinuationRecordType(static_cast<uint8_t>(RecordTypesE::alertContent));
    segmentBuffer.setContinuationHandshakeType(static_cast<uint8_t>(HandShakeTypesE::hello_request_RESERVED));
    segmentBuffer.setContinuationRecordLength(0);
    segmentBuffer.setContinuationRemainingLength(0);
    segmentBuffer.setAlertRecordReassembly(true);
    segmentBuffer.setAlertPlaintextVisible(
        data.hsState == TLSFlow::HsState::PreHandshake ||
        data.hsState == TLSFlow::HsState::InProgress);
    segmentBuffer.setAlertRecordBytesReceived(bytesReceived);
    segmentBuffer.setAlertHeaderParsed(false);
    m_stepContinuationMode = IProcessor::ContinuationMode::AlertRecordReassembly;
    return true;
}

bool Processor::startSegmentedAlert(const TLSData &data, uint16_t recordPayloadLength)
{
    bool allocated = false;
    if (!prepareSegmentBufferForContinuation(SegmentBuffer::ContinuationMode::AlertRecordReassembly, allocated))
    {
        return false;
    }

    auto& segmentBuffer = *m_pSegmentBuffer;
    const uint16_t bytesReceived = m_lengthLeftToParse;
    const uint16_t bytesToStore = std::min<uint16_t>(bytesReceived, TLSRecordHeaderSize + 2);
    if (bytesToStore != 0)
    {
        std::memcpy(segmentBuffer.getBuffer().data(), m_parserLocationPtr, bytesToStore);
    }
    segmentBuffer.setBufferUsed(bytesToStore);
    segmentBuffer.setNextSequence(m_nextExpectedSeq);
    segmentBuffer.setContinuationMode(SegmentBuffer::ContinuationMode::AlertRecordReassembly);
    segmentBuffer.setContinuationRecordType(static_cast<uint8_t>(RecordTypesE::alertContent));
    segmentBuffer.setContinuationHandshakeType(static_cast<uint8_t>(HandShakeTypesE::hello_request_RESERVED));
    segmentBuffer.setContinuationRecordLength(recordPayloadLength);
    segmentBuffer.setContinuationRemainingLength(TLSRecordHeaderSize + recordPayloadLength - bytesReceived);
    segmentBuffer.setAlertRecordReassembly(true);
    segmentBuffer.setAlertPlaintextVisible(
        data.hsState == TLSFlow::HsState::PreHandshake ||
        data.hsState == TLSFlow::HsState::InProgress);
    segmentBuffer.setAlertRecordBytesReceived(bytesReceived);
    segmentBuffer.setAlertHeaderParsed(true);
    m_stepContinuationMode = IProcessor::ContinuationMode::AlertRecordReassembly;
    return true;
}

Processor::RecordProcessResult Processor::processSegmentedAlertContinuation()
{
    auto& segmentBuffer = *m_pSegmentBuffer;

    while (true)
    {
        if (!segmentBuffer.isAlertHeaderParsed())
        {
            const uint16_t bytesReceived = segmentBuffer.getAlertRecordBytesReceived();
            const uint16_t bytesNeeded = TLSRecordHeaderSize - bytesReceived;
            const uint16_t bytesToConsume = std::min(bytesNeeded, m_lengthLeftToParse);
            if (bytesToConsume != 0)
            {
                std::memcpy(segmentBuffer.getBuffer().data() + bytesReceived,
                            m_parserLocationPtr,
                            bytesToConsume);
                m_parserLocationPtr += bytesToConsume;
                m_lengthLeftToParse -= bytesToConsume;
                segmentBuffer.setAlertRecordBytesReceived(bytesReceived + bytesToConsume);
                segmentBuffer.setBufferUsed(bytesReceived + bytesToConsume);
                segmentBuffer.setNextSequence(m_nextExpectedSeq);
            }

            const uint16_t updatedBytesReceived = segmentBuffer.getAlertRecordBytesReceived();
            if (updatedBytesReceived < TLSRecordHeaderSize)
            {
                segmentBuffer.setContinuationRemainingLength(TLSRecordHeaderSize - updatedBytesReceived);
                return RecordProcessResult::Segmented;
            }

            uint16_t consumed = 0;
            const uint8_t *header = reinterpret_cast<const uint8_t *>(segmentBuffer.getBuffer().data());
            if (m_parsers->tlsRecord->parse(header, TLSRecordHeaderSize, consumed) != ParsingStatus::Complete)
            {
                m_lastError = ParsingError::MalformedTlsRecord;
                finishSegmentBufferContinuation();
                return RecordProcessResult::Error;
            }

            if (!m_parsers->tlsRecord->isSupportedRecordType())
            {
                m_lastError = ParsingError::UnsupportedRecordType;
                finishSegmentBufferContinuation();
                return RecordProcessResult::StopProcessing;
            }

            const uint16_t recordPayloadLength = m_parsers->tlsRecord->getTlsRecordLength();
            if (recordPayloadLength > MaxTlsRecordPayloadSize)
            {
                m_lastError = ParsingError::RecordLengthExceedsData;
                finishSegmentBufferContinuation();
                return RecordProcessResult::Error;
            }
            if (m_clientHelloParsingStatus == IClientHelloDataViewer::ClientHelloParsingStatus::None)
            {
                lastVersion = m_parsers->tlsRecord->getTlsVersion();
            }

            const uint16_t totalRecordLength = TLSRecordHeaderSize + recordPayloadLength;
            if (updatedBytesReceived > totalRecordLength)
            {
                m_lastError = ParsingError::MalformedTlsRecord;
                finishSegmentBufferContinuation();
                return RecordProcessResult::Error;
            }

            segmentBuffer.setContinuationRecordType(m_parsers->tlsRecord->getRecordType());
            segmentBuffer.setContinuationRecordLength(recordPayloadLength);
            segmentBuffer.setAlertHeaderParsed(true);
            segmentBuffer.setContinuationRemainingLength(totalRecordLength - updatedBytesReceived);
        }

        const uint16_t remaining = segmentBuffer.getContinuationRemainingLength();
        if (remaining == 0)
        {
            break;
        }

        const uint16_t bytesToConsume = std::min(remaining, m_lengthLeftToParse);
        if (bytesToConsume == 0)
        {
            return RecordProcessResult::Segmented;
        }

        const uint16_t bytesReceived = segmentBuffer.getAlertRecordBytesReceived();
        if (bytesReceived < TLSRecordHeaderSize + 2)
        {
            const uint16_t bytesToStore = std::min<uint16_t>(bytesToConsume,
                                                              TLSRecordHeaderSize + 2 - bytesReceived);
            if (bytesToStore != 0)
            {
                std::memcpy(segmentBuffer.getBuffer().data() + bytesReceived,
                            m_parserLocationPtr,
                            bytesToStore);
                segmentBuffer.setBufferUsed(bytesReceived + bytesToStore);
            }
        }

        m_parserLocationPtr += bytesToConsume;
        m_lengthLeftToParse -= bytesToConsume;
        segmentBuffer.setAlertRecordBytesReceived(bytesReceived + bytesToConsume);
        segmentBuffer.setContinuationRemainingLength(remaining - bytesToConsume);
        segmentBuffer.setNextSequence(m_nextExpectedSeq);

        if (remaining > bytesToConsume)
        {
            return RecordProcessResult::Segmented;
        }
    }

    m_record = Record{};
    m_record.recordType = segmentBuffer.getContinuationRecordType();
    m_record.handshakeType = segmentBuffer.getContinuationHandshakeType();
    m_record.recordLength = segmentBuffer.getContinuationRecordLength();

    if (segmentBuffer.isAlertPlaintextVisible() &&
        segmentBuffer.getAlertAlignmentOffset() == 0 &&
        m_record.recordLength == 2 &&
        segmentBuffer.getBufferUsed() >= TLSRecordHeaderSize + 2)
    {
        uint16_t consumed = 0;
        const uint8_t *alertPayload = reinterpret_cast<const uint8_t *>(segmentBuffer.getBuffer().data()) + TLSRecordHeaderSize;
        if (m_parsers->alert->parse(alertPayload, 2, consumed) == ParsingStatus::Complete && consumed == 2)
        {
            m_record.alertLevel = m_parsers->alert->getAlertLevel();
            m_record.alertDescription = m_parsers->alert->getAlertDescription();
            m_record.readableAlert = true;
        }
    }

    const RecordProcessResult alignmentResult = updateAlertAlignment(
        segmentBuffer.isAlertPlaintextVisible(), m_record.recordLength);
    if (alignmentResult != RecordProcessResult::ContinueProcessing)
    {
        return alignmentResult;
    }

    if (m_records.size() >= maxRecords)
    {
        m_lastError = ParsingError::RecordsExceedDataViewerCapacity;
        finishSegmentBufferContinuation();
        return RecordProcessResult::StopProcessing;
    }
    m_records.push_back(m_record);
    finishSegmentBufferContinuation();
    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::processAlert(const TLSData &data)
{
    const bool plaintextVisible =
        data.hsState == TLSFlow::HsState::PreHandshake ||
        data.hsState == TLSFlow::HsState::InProgress;

    if (plaintextVisible &&
        m_record.recordLength == 2 &&
        m_pSegmentBuffer->getAlertAlignmentOffset() == 0 &&
        m_currentRecordPayloadLeft == 2 &&
        m_parsers->alert != nullptr)
    {
        uint16_t consumed = 0;
        if (m_parsers->alert->parse(m_currentRecordPayloadPtr, 2, consumed) == ParsingStatus::Complete &&
            consumed == 2)
        {
            m_record.alertLevel = m_parsers->alert->getAlertLevel();
            m_record.alertDescription = m_parsers->alert->getAlertDescription();
            m_record.readableAlert = true;
        }
    }

    return updateAlertAlignment(plaintextVisible, m_record.recordLength);
}

Processor::RecordProcessResult Processor::updateAlertAlignment(bool plaintextVisible, uint16_t recordLength)
{
    if (!plaintextVisible)
    {
        return RecordProcessResult::ContinueProcessing;
    }

    if ((recordLength & 1u) == 0)
    {
        if (m_pSegmentBuffer != &m_initialSegment &&
            m_pSegmentBuffer->getAlertAlignmentOffset() != 0)
        {
            m_pSegmentBuffer->setNextSequence(m_nextExpectedSeq);
        }
        return RecordProcessResult::ContinueProcessing;
    }

    if (m_pSegmentBuffer == &m_initialSegment)
    {
        bool allocated = false;
        if (!prepareSegmentBufferForContinuation(SegmentBuffer::ContinuationMode::None, allocated))
        {
            m_lastError = ParsingError::SegmentBufferAllocationFailure;
            return RecordProcessResult::StopProcessing;
        }
    }

    const uint8_t alignmentOffset = static_cast<uint8_t>(m_pSegmentBuffer->getAlertAlignmentOffset() ^ 1u);
    m_pSegmentBuffer->setAlertAlignmentOffset(alignmentOffset);
    if (alignmentOffset == 0)
    {
        deallocateSegmentBuff();
    }
    else
    {
        m_pSegmentBuffer->setNextSequence(m_nextExpectedSeq);
    }
    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::processNewRecord(const TLSData &data)
{
    bool unsegmentedClientHelloCompleted = false;

    if (data.hsState == TLSFlow::HsState::Unsupported)
    {
        return RecordProcessResult::EndOfPacket;
    }

    if (m_lengthLeftToParse < TLSRecordHeaderSize &&
        m_lengthLeftToParse != 0 &&
        m_parserLocationPtr != nullptr &&
        *m_parserLocationPtr == static_cast<uint8_t>(RecordTypesE::alertContent))
    {
        if (!startSegmentedAlert(data))
        {
            m_lastError = ParsingError::SegmentBufferAllocationFailure;
            return RecordProcessResult::StopProcessing;
        }
        return RecordProcessResult::Segmented;
    }

    // Parse TLS record header (uses m_parserLocationPtr, m_lengthLeftToParse but doesn't modify them)
    auto innerProcessResult = processTlsRecord();
    if (RecordProcessResult::ContinueProcessing != innerProcessResult)
    {
        return innerProcessResult;
    }

    // Get record type and length (use raw uint8_t to capture all record types including unknown)
    uint8_t recordType = m_parsers->tlsRecord->getRecordType();
    uint16_t recordPayloadLen = m_parsers->tlsRecord->getTlsRecordLength();
    uint16_t totalRecordLen = TLSRecordHeaderSize + recordPayloadLen;
    m_currentRecordIsSegmented = m_lengthLeftToParse < totalRecordLen;

    // Initialize record-level tracking
    m_currentRecordPayloadPtr = m_parserLocationPtr + TLSRecordHeaderSize;
    const uint16_t availablePayloadLength = static_cast<uint16_t>(m_lengthLeftToParse - TLSRecordHeaderSize);
    m_currentRecordPayloadLeft = std::min(recordPayloadLen, availablePayloadLength);

    m_record = Record{};
    m_record.recordType = recordType;
    m_record.recordLength = recordPayloadLen;

    if (recordType == static_cast<uint8_t>(RecordTypesE::alertContent))
    {
        if (m_currentRecordIsSegmented)
        {
            if (!startSegmentedAlert(data, recordPayloadLen))
            {
                m_lastError = ParsingError::SegmentBufferAllocationFailure;
                return RecordProcessResult::StopProcessing;
            }
            return RecordProcessResult::Segmented;
        }

        innerProcessResult = processAlert(data);
        if (RecordProcessResult::ContinueProcessing != innerProcessResult)
        {
            return innerProcessResult;
        }
    }

    const bool opaqueRecord =
        (data.hsState == TLSFlow::HsState::PreHandshake &&
         recordType == static_cast<uint8_t>(RecordTypesE::applicationDataContent)) ||
        (data.hsState == TLSFlow::HsState::InProgress &&
         ((TLSFlow::isTLS13(data.tlsVersionMask) &&
           recordType == static_cast<uint8_t>(RecordTypesE::applicationDataContent)) ||
          recordType == static_cast<uint8_t>(RecordTypesE::changeCipherSpecContent))) ||
        (data.hsState == TLSFlow::HsState::ChangeCipherSpec &&
         recordType == static_cast<uint8_t>(RecordTypesE::handshakeContent));

    if (m_currentRecordIsSegmented && opaqueRecord)
    {
        bool allocated = false;
        if (!prepareSegmentBufferForContinuation(SegmentBuffer::ContinuationMode::OpaqueRecordTracking, allocated))
        {
            m_lastError = ParsingError::SegmentBufferAllocationFailure;
            return RecordProcessResult::StopProcessing;
        }
        m_pSegmentBuffer->setNextSequence(m_nextExpectedSeq);
        m_pSegmentBuffer->setContinuationRemainingLength(totalRecordLen - m_lengthLeftToParse);
        m_pSegmentBuffer->setContinuationRecordType(recordType);
        m_pSegmentBuffer->setContinuationHandshakeType(m_record.handshakeType);
        m_pSegmentBuffer->setContinuationRecordLength(recordPayloadLen);
        m_diagnosticsCounters->incEvent(DiagnosticsEvents::OpaqueRecordContinuationStarted);
        return RecordProcessResult::Segmented;
    }
    
    const bool shouldParseHandshake =
        data.hsState == TLSFlow::HsState::PreHandshake ||
        data.hsState == TLSFlow::HsState::InProgress;
    if (shouldParseHandshake &&
        recordType == static_cast<uint8_t>(RecordTypesE::handshakeContent))
    {
        // Parse handshake header
        innerProcessResult = processHandshake();
        if (RecordProcessResult::ContinueProcessing != innerProcessResult)
        {
            return innerProcessResult;
        }

        // Get handshake type
        uint8_t hsType = m_parsers->handshake->getHandshakeType();
        m_record.handshakeType = hsType;

        if (hsType != static_cast<uint8_t>(HandShakeTypesE::client_hello))
        {
            // Non-ClientHello handshake - log and continue to record it
            DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_TLS_RECORD_NON_CLIENT_HELLO, hsType, m_currentRecordPayloadLeft);
        }
        else // ClientHello processing
        {
            
            if (!m_parsers->tlsRecord->isSupportedVersion())
            {
                DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_TLS_RECORD_UNSUPPORTED_VER, m_parsers->tlsRecord->getTlsVersion(), 0, m_lengthLeftToParse);
                m_diagnosticsCounters->incEvent(DiagnosticsEvents::UnsupportedVersion);
                m_lastError = ParsingError::UnsupportedTlsVersion;
                return RecordProcessResult::Error;
            }

            const uint32_t handshakeRecordLength =
                m_parsers->handshake->getHandshakeLength() + TLSHandshakeHeaderSize;
            const uint16_t recordLength = m_parsers->tlsRecord->getTlsRecordLength();
            if (handshakeRecordLength < recordLength)
            {
                DIAG_LOGGER_INJECT(PROCESSOR_PARSE_FAILED, 
                    "Handshake length mismatch: handshake=%u, record=%u", 
                    handshakeRecordLength,
                    recordLength);

                m_lastError = ParsingError::HandshakeLengthNotMatchingRecordLength;
                return RecordProcessResult::StopProcessing;
            }

            // Multiple ClientHello is anomalous - reject
            if (m_clientHelloParsingStatus == IClientHelloDataViewer::ClientHelloParsingStatus::Complete)
            {
                DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "Multiple ClientHello detected", 0, m_currentRecordPayloadLeft);
                m_lastError = ParsingError::MultipleClientHello;
                return RecordProcessResult::Error;
            }
            
            // ClientHello detected
            m_clientHelloParsingStatus = IClientHelloDataViewer::ClientHelloParsingStatus::Partial;
            
            // Initialize digest (version will be hashed after parsing ClientHello)
            m_pDigest->init();

            // Process ClientHello body
            const bool recordLevelFragmented = handshakeRecordLength > recordLength &&
                                               !m_currentRecordIsSegmented;
            innerProcessResult = processClientHello(recordLevelFragmented);
            if (RecordProcessResult::ContinueProcessing != innerProcessResult)
            {
                // Error or Segmented - return without advancing (segmentation saves state)
                return innerProcessResult;
            }

            if (handshakeRecordLength > recordLength)
            {
                if (recordLevelFragmented)
                {
                    m_diagnosticsCounters->incEvent(DiagnosticsEvents::UnsupportedRecordLevelFragmentation);
                    return RecordProcessResult::Unsupported;
                }

                m_lastError = ParsingError::HandshakeLengthNotMatchingRecordLength;
                return RecordProcessResult::StopProcessing;
            }

            // Hash ClientHello version (now available from ClientHelloParser)
            DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "ClientHello version");
            if (!appendOpenParentheses())
            {
                return RecordProcessResult::StopProcessing;
            }
            if (!calcHash(m_parsers->clientHello->getVersion()))
            {
                DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_HS_UNSUPPORTED_VER, m_parsers->clientHello->getVersion());
                m_diagnosticsCounters->incEvent(DiagnosticsEvents::UnsupportedVersion);
                return RecordProcessResult::StopProcessing;
            }
            if (!appendClosingParentheses())
            {
                return RecordProcessResult::StopProcessing;
            }

            /* Save the version from the ClientHello, may be overridden later if supported_versions extension exists */
            lastVersion = m_parsers->clientHello->getVersion();

            m_numOfCompressions = m_parsers->clientHello->getCompressionLen();

            innerProcessResult = processCiphers();
            if (RecordProcessResult::ContinueProcessing != innerProcessResult)
            {
                if (innerProcessResult == RecordProcessResult::Error)
                {
                    // m_lastError already set by processCiphers
                    return RecordProcessResult::Error;
                }
                // Non-error stop - will record below
            }
            else
            {
                m_numOfCipherSuites = m_parsers->cipherSuites->getNumOfSuites();

                innerProcessResult = processExtensions();
                if (RecordProcessResult::ContinueProcessing != innerProcessResult)
                {
                    if (RecordProcessResult::Segmented != innerProcessResult)
                    {
                        if (RecordProcessResult::Unsupported != innerProcessResult)
                        {
                            m_diagnosticsCounters->incEvent(DiagnosticsEvents::ExtensionParserFailures);
                        }
                        // m_lastError already set by processExtensions
                        return innerProcessResult;
                    }
                    
                    if (m_state == ProcessorState::FirstSegment)
                    {
                        // Extensions are segmented - save state for continuation
                        m_pSegmentBuffer->setNumOfCompressions(m_numOfCompressions);
                        m_pSegmentBuffer->setLastVersion(lastVersion);
                        m_pSegmentBuffer->setNumOfCiphers(m_numOfCipherSuites);
                        m_pSegmentBuffer->setExtensionRemainingLength(
                            m_parsers->clientHello->getExtensionsLen() - m_pSegmentBuffer->getBufferUsed());
                        m_diagnosticsCounters->incEvent(DiagnosticsEvents::FirstSegmentProcessSuccess);
                        return RecordProcessResult::Segmented;
                    }
                    // Non-error, non-segmented stop - will record below
                }
                else
                {
                    if (!m_supportedVersionsParsed)
                    {
                        m_supportedVersionsBitMask = tlsVersionMaskFromWireVersion(m_parsers->clientHello->getVersion());
                    }
                    m_pDigest->finalize(m_fp);
                    m_clientHelloParsingStatus = IClientHelloDataViewer::ClientHelloParsingStatus::Complete;
                    unsegmentedClientHelloCompleted = true;
                }
            }
        }
    }

    // Add record to the list
    if(m_records.size() >= maxRecords){
        m_lastError = ParsingError::RecordsExceedDataViewerCapacity;
        return RecordProcessResult::StopProcessing;
    }
    m_records.push_back(m_record);

    if (unsegmentedClientHelloCompleted)
    {
        m_diagnosticsCounters->incEvent(DiagnosticsEvents::UnsegmentedClientHelloCompleted);
    }
    

    if(m_lengthLeftToParse < totalRecordLen)
    {
        // Incomplete record 
        m_diagnosticsCounters->incEvent(DiagnosticsEvents::FirstSegmentProcessSuccess);
        return RecordProcessResult::Segmented;
    }

    // Advance segment-level pointers past this entire record (single advancement point)
    m_parserLocationPtr += totalRecordLen;
    m_lengthLeftToParse -= totalRecordLen;

    return innerProcessResult;
}

Processor::RecordProcessResult Processor::processExtensions()
{
    const uint8_t* extensionsPtr = nullptr;
    uint16_t extensionsLen = 0;
    
    switch (getState())
    {
        /* Normal single segment processing */
        case ProcessorState::SingleSegment:
            extensionsLen = m_parsers->clientHello->getExtensionsLen();
            extensionsPtr = m_parsers->clientHello->getpExtensions();
            m_pExtensionLocators->reset();
            break;
            
        /* This is the first segment treated, work with first partial part */
        case ProcessorState::FirstSegment:
        {
            /* Reset the extensions array */
            m_pExtensionLocators->reset();

            /* Validate total extension length fits in segment buffer */
            extensionsLen = m_parsers->clientHello->getExtensionsLen();
            if (extensionsLen > SegmentBufferSize)
            {
                m_diagnosticsCounters->incEvent(DiagnosticsEvents::UnsupportedExtensionsExceedBuffer);
                deallocateSegmentBuff();
                return RecordProcessResult::Unsupported;
            }

            uint16_t nextLength = m_pSegmentBuffer->getContinuationRemainingLength();
            if (extensionsLen < nextLength) {
                // Would underflow - invalid state
                m_diagnosticsCounters->incEvent(DiagnosticsEvents::SegmentedStateSecondSegmentTooShort);
                m_lastError = ParsingError::SegmentMismatch;
                deallocateSegmentBuff();
                return RecordProcessResult::StopProcessing;
            }            
            
            /* Calculate length to copy from the partial extensions */
            uint16_t toCopy = extensionsLen - nextLength;
            if(toCopy > m_lengthLeftToParse)
            {
                m_diagnosticsCounters->incEvent(DiagnosticsEvents::SegmentedStateSecondSegmentTooShort);
                m_lastError = ParsingError::SegmentMismatch;
                deallocateSegmentBuff();
                return RecordProcessResult::StopProcessing;
            }
            /* Copy the partial extensions to the segment buffer for further processing */
            std::memcpy(m_pSegmentBuffer->getBuffer().data(), m_parsers->clientHello->getpExtensions(), toCopy);
            m_pSegmentBuffer->setBufferUsed(toCopy);

            return RecordProcessResult::Segmented;
        }

        /* This is the last segment treated, work with the saved segment buffer */
        case ProcessorState::LastSegment:
        {
            /* Copy only the bytes needed for ClientHello completion to the segment buffer */
            uint16_t bytesNeeded = m_pSegmentBuffer->getNextLength();
            std::memcpy(m_pSegmentBuffer->getBuffer().data() + m_pSegmentBuffer->getBufferUsed(), m_parserLocationPtr, bytesNeeded);
            m_pSegmentBuffer->setBufferUsed(m_pSegmentBuffer->getBufferUsed() + bytesNeeded);

            /* Set local pointers to parse from the segment buffer */
            extensionsPtr = reinterpret_cast<const uint8_t*>(m_pSegmentBuffer->getBuffer().data());
            extensionsLen = m_pSegmentBuffer->getBufferUsed();
            break;
        }
        default:
            m_lastError = ParsingError::ExtensionParsingFailure;
            return RecordProcessResult::Error;
    }

    /* Parse extensions */
    auto res = parseAllExtensions(extensionsPtr, extensionsLen);
    if (res != RecordProcessResult::ContinueProcessing)
        return res;

    // Sort the extensions according to type, then length, then value, greased last
    m_pExtensionLocators->sort();
    m_numOfExtensions = m_pExtensionLocators->getNumOfExtensions();
    return feedExtensionsFP();
}

Processor::RecordProcessResult Processor::processSni()
{
    /* Parse SNI extension */
    uint16_t consumedSni;
    if (m_parsers->sni->parse(m_sniExtension.pValue, m_sniExtension.length, consumedSni) != ParsingStatus::Complete)
    {
        DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "SNI", consumedSni, m_sniExtension.length);
        m_lastError = ParsingError::ExtensionParsingFailure;
        return RecordProcessResult::Error;
    }

    /* Validate number of SNIs */
    uint16_t numOfSnis = m_parsers->sni->getNumOfSni();
    if (numOfSnis > 2 || numOfSnis == 0)
    {
        m_lastError = ParsingError::MalformedSniExtension;
        return RecordProcessResult::Error;
    }

    /* Validate SNI type */
    uint16_t sniType = m_parsers->sni->getSniType(0);
    if (static_cast<SniTypeE>(sniType) != SniTypeE::hostName)
    {
        DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_NON_HOST_SNI, sniType);
        return RecordProcessResult::ContinueProcessing;
    }

    /* Validate SNI length */
    uint16_t sniLen = m_parsers->sni->getSniLen(0);
    if (sniLen == 0 || sniLen > max_sni_length)
    {
        DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_SNI_TOO_LONG, sniLen);
        m_lastError = ParsingError::MalformedSniExtension;
        return RecordProcessResult::Error;
    }

    /* Size is consistent */
    auto sni = reinterpret_cast<const char*>(m_parsers->sni->getpSni(0));
    if (sniLen != strnlen(sni, sniLen))
    {
        DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_SNI_INCONSISTENT);
        m_lastError = ParsingError::MalformedSniExtension;
        return RecordProcessResult::Error;
    }

    /* Save the SNI */
    m_sni.set(reinterpret_cast<const char*>(m_parsers->sni->getpSni(0)), sniLen);
    m_bSniExtensionParsed = true;
    return RecordProcessResult::ContinueProcessing;
}

// Go over all sorted extensions and inject the relevant parts into the FP hash
Processor::RecordProcessResult Processor::feedExtensionsFP()
{
    // Add a square parentheses around the extensions part, also in case no extensions exist
    if (!appendOpenSqParentheses()) {
        return RecordProcessResult::StopProcessing;
    }
    const uint8_t *pVal;
    for (uint16_t e = 0; e < m_pExtensionLocators->getNumOfExtensions(); e++)
    {
        const ExtensionLocator& extensionLocator = m_pExtensionLocators->get(e);

        // Add extension type to fp hash
        if (!appendOpenParentheses())
        {
            return RecordProcessResult::StopProcessing;
        }
        DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "ExtensionTypeE");

        if (!calcHash(static_cast<uint16_t>(extensionLocator.type)))
        {
            return RecordProcessResult::StopProcessing;
        }

        if (isExtensionContentIncludedInFp(extensionLocator.type))
        {
            // Add extension Length to fp hash
            DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "extensionLen");
            if (!calcHash(static_cast<uint16_t>(extensionLocator.length)))
            {
                return RecordProcessResult::StopProcessing;
            }
            DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_EXTENSION_REQUIORED, "Extension required");
            switch (extensionLocator.type)
            {
                case ExtensionTypeE::supported_groups:
                { // supported groups
                    auto processRes = feedSuppGroupsFP(extensionLocator);
                    if (processRes != RecordProcessResult::ContinueProcessing)
                    {
                        return processRes;
                    }
                    break;
                }
                case ExtensionTypeE::supported_versions:
                { // supported versions
                    auto processRes = feedSuppVersionsFP(extensionLocator);
                    if (processRes != RecordProcessResult::ContinueProcessing)
                    {
                        return processRes;
                    }
                    break;
                }
                default: DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "General extension");
                    if (!calcHash(extensionLocator.pValue, extensionLocator.length))
                    {
                        return RecordProcessResult::StopProcessing;
                    }
            }
        }

        if (!appendClosingParentheses())
        {
            return RecordProcessResult::StopProcessing;
        }
    }
    // Close the square parentheses
    if (!appendClosingSqParentheses())
    {
        return RecordProcessResult::StopProcessing;
    }
    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::feedSuppVersionsFP(const ExtensionLocator& extensionLocator)
{
    uint16_t consumedVersions;
    if (ParsingStatus::Complete != m_parsers->supportedVersions->parse(extensionLocator.pValue, extensionLocator.length, consumedVersions))
    {
        DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "Supported Versions", consumedVersions, extensionLocator.length);
        m_lastError = ParsingError::ExtensionParsingFailure;
        return RecordProcessResult::Error;
    }
    DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "Length of Supported Versions");
    if (!calcHash(m_parsers->supportedVersions->getLength()))
    {
        return RecordProcessResult::StopProcessing;
    }

    uint16_t supportedVersions = m_parsers->supportedVersions->getNumOfVersions(), i = 0;
    bool versionFound = false;
    m_supportedVersionsParsed = true;
    for (; supportedVersions--; i++)
    {
        // Skip GREASE values when setting the lastVersion
        auto version = m_parsers->supportedVersions->getVersion(i);
        if(!versionFound && !isGreaseValue(version))
        {
            lastVersion = version;
            versionFound = true;
        }
        DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "Supported Version");
        // Add each supported version to the hash
        m_supportedVersionsBitMask |= tlsVersionMaskFromWireVersion(version);
        if (!calcHash(deGrease(version)))
        {
            return RecordProcessResult::StopProcessing;
        }
    }

    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::feedSuppGroupsFP(const ExtensionLocator& extensionLocator)
{
    uint16_t consumedGroup;

    if (ParsingStatus::Complete != m_parsers->supportedGroups->parse(extensionLocator.pValue, extensionLocator.length, consumedGroup))
    {
        DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "Supported Groups", consumedGroup, m_lengthLeftToParse);
        m_lastError = ParsingError::ExtensionParsingFailure;
        return RecordProcessResult::Error;
    }
    DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "Length of Supported Groups");
    if (!calcHash(m_parsers->supportedGroups->getGroupsLen()))
    {
        return RecordProcessResult::StopProcessing;
    }

    uint16_t supportedGroups = m_parsers->supportedGroups->getNumOfGroups(), i = 0;
    for (; supportedGroups--; i++)
    {
        DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "Supported Group");
        if (!calcHash(deGrease(m_parsers->supportedGroups->getGroup(i))))
        {
            return RecordProcessResult::StopProcessing;
        }
    }
    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::parseAllExtensions(const uint8_t* extensionsPtr, uint16_t extensionsLenLeftToParse)
{
    uint16_t consumed;
    uint16_t offset = 0;
    
    // Parse and fill the extensions for sorting them before adding to the hash calculation
    while (extensionsLenLeftToParse > 0)
    {
        /* Parse next extension */
        auto parsingRes = m_parsers->extensions->parse(extensionsPtr + offset, extensionsLenLeftToParse, consumed);
        if (parsingRes != IExtensionsParser::Status::Complete)
        {
            DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "Extensions incomplete result inconsistency", getState(), extensionsLenLeftToParse);
            if(parsingRes == IExtensionsParser::Status::Failure)
            {
                m_lastError = ParsingError::ExtensionParsingFailure;
                return RecordProcessResult::Error;
            }
            m_lastError = ParsingError::ExtensionParsingFailure;
            return RecordProcessResult::Error;
        }

        /* Logic check; consumed bytes must consist with parsed extension size */
        if (m_parsers->extensions->getSingleExtensionTotalLength() != consumed)
        {
            DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_EXTENSIONS_LENGTH_INVALID, m_parsers->extensions->getSingleExtensionTotalLength(), consumed, extensionsLenLeftToParse);
            m_lastError = ParsingError::ExtensionParsingFailure;
            return RecordProcessResult::Error;
        }

        /* Add the parsed extension to the extensions array for later sorting */
        if (!m_pExtensionLocators->add({deGrease(static_cast<uint16_t>(m_parsers->extensions->getExtensionType())), m_parsers->extensions->getExtensionDataLength(), m_parsers->extensions->getExtensionValuePtr()}))
        {
            DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "Extensions array insert failed", consumed, extensionsLenLeftToParse);
            return RecordProcessResult::StopProcessing;
        }

        if (m_pExtensionLocators->getLast().getType() == ExtensionTypeE::early_data)
        {
            m_earlyDataOffered = true;
        }

        /* Check for SNI extension and save its locator for later direct access */
        if (m_pExtensionLocators->getLast().getType() == ExtensionTypeE::server_name)
        {
            m_sniExtension = m_pExtensionLocators->getLast();
            if(processSni() == RecordProcessResult::Error)
            {
                // m_lastError already set by processSni
                return RecordProcessResult::Error;
            }
        }

        /* Move to next extension */
        extensionsLenLeftToParse -= consumed;
        offset += consumed;
    }

    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::processCiphers()
{
    uint16_t consumed;
    const uint8_t *pCipherSuites = m_parsers->clientHello->getpCipherSuites();
    uint16_t cipherSuitesLen = m_parsers->clientHello->getCipherSuitesLen();

    if (cipherSuitesLen)
    {
        if (ParsingStatus::Complete != m_parsers->cipherSuites->parse(pCipherSuites, cipherSuitesLen, consumed))
        {
            DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "Cipher Suites", consumed, cipherSuitesLen);
            m_lastError = ParsingError::CipherParsingFailure;
            return RecordProcessResult::Error;
        }
        if (!appendOpenParentheses())
        {
            return RecordProcessResult::StopProcessing;
        }
        for (int i = 0; cipherSuitesLen -= 2; i++)
        {
            DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALC_ADD, "CipherSuite");
            if (!calcHash(deGrease(m_parsers->cipherSuites->getSuite(i))))
            {
                return RecordProcessResult::StopProcessing;
            }
        }
        if (!appendClosingParentheses())
        {
            return RecordProcessResult::StopProcessing;
        }
    }
    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::processClientHello(bool recordLevelFragmented)
{
    uint16_t consumed;
    uint32_t expectedLength = m_parsers->handshake->getHandshakeLength();
    auto status = m_parsers->clientHello->parse(m_currentRecordPayloadPtr, m_currentRecordPayloadLeft, expectedLength, consumed);

    if (IClientHelloParser::Status::Complete != status)
    {
        if (recordLevelFragmented &&
            (IClientHelloParser::Status::IncompleteClientHelloPrefix == status ||
             IClientHelloParser::Status::IncompleteClientHelloBody == status ||
             IClientHelloParser::Status::IncompleteExtensionSection == status))
        {
            m_diagnosticsCounters->incEvent(DiagnosticsEvents::UnsupportedRecordLevelFragmentation);
            return RecordProcessResult::Unsupported;
        }

        /* TLS is segmented */
        if (IClientHelloParser::Status::IncompleteExtensionSection == status)
        {
            bool allocated = false;
            if (!prepareSegmentBufferForContinuation(SegmentBuffer::ContinuationMode::ClientHelloExtensionReassembly,
                                                      allocated))
            {
                DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_ERROR_ALLOCATE_SEGSTATE);
                m_diagnosticsCounters->incEvent(DiagnosticsEvents::SegmentedStateEntriesAllocationFailures);
                m_lastError = ParsingError::SegmentBufferAllocationFailure;
                return RecordProcessResult::StopProcessing;
            }

            /* Initialize segment buffer */
            setState(ProcessorState::FirstSegment);
            m_pSegmentBuffer->setNextSequence(m_nextExpectedSeq);
            m_pSegmentBuffer->setNextLength(m_parsers->tlsRecord->getTlsRecordLength() + TLSRecordHeaderSize - m_packetPayloadLen);
            m_pSegmentBuffer->setStepBufferedLength(m_packetPayloadLen);
            m_pSegmentBuffer->setTotalRecordLength(m_parsers->tlsRecord->getTlsRecordLength());
            m_pSegmentBuffer->setContinuationMode(SegmentBuffer::ContinuationMode::ClientHelloExtensionReassembly);
            m_pSegmentBuffer->setClientHelloSegmentCount(1);
            m_pSegmentBuffer->setContinuationRemainingLength(
                m_parsers->tlsRecord->getTlsRecordLength() + TLSRecordHeaderSize - m_lengthLeftToParse);
            m_pSegmentBuffer->setContinuationRecordType(static_cast<uint8_t>(RecordTypesE::handshakeContent));
            m_pSegmentBuffer->setContinuationHandshakeType(static_cast<uint8_t>(HandShakeTypesE::client_hello));
            m_pSegmentBuffer->setContinuationRecordLength(m_parsers->tlsRecord->getTlsRecordLength());
            m_pSegmentBuffer->copyDigest(*m_pDigest); // copy
            m_pDigest = &(m_pSegmentBuffer->getDigest());
            m_pExtensionLocators = m_pSegmentBuffer->getLocatorsArray();
            DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_ALLOCATED_SEGMENT, m_pSegmentBuffer->getNextLength(), m_pSegmentBuffer->getNextSequence());
            m_diagnosticsCounters->incEvent(DiagnosticsEvents::ClientHelloContinuationStarted);
        }
        else if ((IClientHelloParser::Status::IncompleteClientHelloPrefix == status ||
              IClientHelloParser::Status::IncompleteClientHelloBody == status) &&
                 m_currentRecordIsSegmented)
        {
            m_diagnosticsCounters->incEvent(DiagnosticsEvents::UnsupportedEarlyClientHelloBody);
            return RecordProcessResult::Unsupported;
        }
        else
        {
            DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "Client-Hello", consumed, m_currentRecordPayloadLeft);
            m_lastError = ParsingError::ClientHelloBodyInvalid;
            return RecordProcessResult::Error;
        }
    }
    
    // Advance record-level pointers
    m_currentRecordPayloadPtr += consumed;
    m_currentRecordPayloadLeft -= consumed;

    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::processHandshake()
{
    uint16_t consumed;
    if (ParsingStatus::Complete != m_parsers->handshake->parse(m_currentRecordPayloadPtr, m_currentRecordPayloadLeft, consumed))
    {
        DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "Handshake", consumed, m_currentRecordPayloadLeft);
        m_lastError = ParsingError::MalformedHandshake;
        return RecordProcessResult::Error;
    }
    // Advance record-level pointers
    m_currentRecordPayloadPtr += consumed;
    m_currentRecordPayloadLeft -= consumed;
    return RecordProcessResult::ContinueProcessing;
}

Processor::RecordProcessResult Processor::processTlsRecord()
{
    uint16_t consumed;
    if (ParsingStatus::Complete != m_parsers->tlsRecord->parse(m_parserLocationPtr, m_lengthLeftToParse, consumed))
    {
        DIAG_LOGGER_DEBUG_MODE_INJECT(PROCESSOR_PARSE_FAILED, "TLS record", consumed, m_lengthLeftToParse);
        m_lastError = ParsingError::MalformedTlsRecord;
        return RecordProcessResult::Error;
    }

    // Validate record payload length against RFC 5246 Section 6.2.1 maximum (2^14 bytes)
    uint16_t recordPayloadLen = m_parsers->tlsRecord->getTlsRecordLength();
    if (recordPayloadLen > MaxTlsRecordPayloadSize) {
        m_lastError = ParsingError::RecordLengthExceedsData;
        return RecordProcessResult::Error;
    }

    if(!m_parsers->tlsRecord->isSupportedRecordType())
    {
        m_lastError = ParsingError::UnsupportedRecordType;
        return RecordProcessResult::StopProcessing;
    }

    /* Save the TLS version from the record layer only if ClientHello not yet processed.
       Once ClientHello is parsed, lastVersion comes from handshake/supported_versions extension. */
    if (m_clientHelloParsingStatus == IClientHelloDataViewer::ClientHelloParsingStatus::None)
    {
        lastVersion = m_parsers->tlsRecord->getTlsVersion();
    }

    // Note: We don't advance segment pointers here - processSingleRecord manages that
    return RecordProcessResult::ContinueProcessing;
}

uint16_t Processor::getTlsVersion() const
{
    return lastVersion;
}

uint8_t Processor::getSupportedVersionBitMask() const
{
    return m_supportedVersionsBitMask;
}

bool Processor::isEarlyDataOffered() const
{
    return m_earlyDataOffered;
}

void Processor::getFingerprint(Fingerprint& fingerprint) const
{
    fingerprint = m_fp;
}

bool Processor::getSni(Sni& sni)
{
    if (m_bSniExtensionParsed)
    {
        sni = m_sni;
        return true;
    }

    return false;
}

FullStringData Processor::getFullString() const
{
    return {m_pDigest->getFsArray(), m_pDigest->getFsLen()};
}

uint32_t Processor::getCipherCount() const
{
    return m_numOfCipherSuites;
}

uint32_t Processor::getExtensionCount() const
{
    return m_numOfExtensions;
}

uint32_t Processor::getCompressionCount() const
{
    return m_numOfCompressions;
}

 bool Processor::isSegmented() const
 {
    return (m_state != ProcessorState::SingleSegment);
 }

Processor::ProcessorState Processor::getState() const { return m_state; }
void Processor::setState(Processor::ProcessorState mState) { m_state = mState; }

const IRecordsDataViewer::Records& Processor::getAllRecords() const
{
    return m_records;
}

const Record& Processor::getLastRecord() const
{
    return m_record;
}

// Step-oriented ClientHello mirror accessors were removed in the conservative
// migration. Remaining viewers expose the canonical (legacy) members.

uint8_t Processor::getRecordType() const
{
    return m_record.recordType;
}

uint16_t Processor::getRecordLength() const
{
    return m_record.recordLength;
}


IClientHelloDataViewer::ClientHelloParsingStatus Processor::getClientHelloParsingStatus() const
{
    return m_clientHelloParsingStatus;
}

ParsingError Processor::getLastError() const
{
    return m_lastError;
}