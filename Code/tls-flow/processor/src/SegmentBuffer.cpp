//
// Created by maxg on 8/7/24.
//

#include "SegmentBuffer.hpp"

using namespace TLSFlow::NSProcessor::Engine;

/// --------------------------
/// Implementations
/// --------------------------
const std::array<char, SegmentBufferSize> & SegmentBuffer::getBuffer() const { return buffer; }
std::array<char, SegmentBufferSize> & SegmentBuffer::getBuffer() { return buffer; }

uint16_t SegmentBuffer::getBufferUsed() const { return bufferUsed; }
void SegmentBuffer::setBufferUsed(const uint16_t used) { this->bufferUsed = used; }

uint32_t SegmentBuffer::getNextSequence() const { return nextSequence; }
void SegmentBuffer::setNextSequence(const uint32_t seq) { this->nextSequence = seq; }

uint16_t SegmentBuffer::getNextLength() const { return nextLength; }
void SegmentBuffer::setNextLength(const uint16_t len) { this->nextLength = len; }

uint16_t SegmentBuffer::getStepBufferedLength() const { return stepBufferedLength; }
void SegmentBuffer::setStepBufferedLength(const uint16_t len) { stepBufferedLength = len; }

uint32_t SegmentBuffer::getStepNextRecordOrdinal() const { return stepNextRecordOrdinal; }
void SegmentBuffer::setStepNextRecordOrdinal(const uint32_t ordinal) { stepNextRecordOrdinal = ordinal; }

uint32_t SegmentBuffer::getClientHelloSegmentCount() const { return clientHelloSegmentCount; }
void SegmentBuffer::setClientHelloSegmentCount(const uint32_t count) { clientHelloSegmentCount = count; }

Md5Digest& SegmentBuffer::getDigest()  {return digest;}

void SegmentBuffer::copyDigest(Md5Digest &inDigest) {this->digest.copy(inDigest);} // Copy

ExtensionsArray *SegmentBuffer::getLocatorsArray()  {
    return &extensionsArray;
}

SegmentBuffer::State SegmentBuffer::getState() const { return state; }

void SegmentBuffer::setState(SegmentBuffer::State st) { SegmentBuffer::state = st; }

uint16_t SegmentBuffer::getTotalRecordLength() const { return totalRecordLength; }

void SegmentBuffer::setTotalRecordLength(uint16_t len) { totalRecordLength = len; }

SegmentBuffer::ContinuationMode SegmentBuffer::getContinuationMode() const { return continuationMode; }

void SegmentBuffer::setContinuationMode(ContinuationMode mode) { continuationMode = mode; }

uint16_t SegmentBuffer::getContinuationRemainingLength() const { return continuationRemainingLength; }

void SegmentBuffer::setContinuationRemainingLength(uint16_t len) { continuationRemainingLength = len; }

uint16_t SegmentBuffer::getExtensionRemainingLength() const { return extensionRemainingLength; }

void SegmentBuffer::setExtensionRemainingLength(uint16_t len) { extensionRemainingLength = len; }

uint8_t SegmentBuffer::getContinuationRecordType() const { return continuationRecordType; }

void SegmentBuffer::setContinuationRecordType(uint8_t type) { continuationRecordType = type; }

uint8_t SegmentBuffer::getContinuationHandshakeType() const { return continuationHandshakeType; }

void SegmentBuffer::setContinuationHandshakeType(uint8_t type) { continuationHandshakeType = type; }

uint16_t SegmentBuffer::getContinuationRecordLength() const { return continuationRecordLength; }

void SegmentBuffer::setContinuationRecordLength(uint16_t length) { continuationRecordLength = length; }

bool SegmentBuffer::isAlertRecordReassembly() const { return alertRecordReassembly; }

void SegmentBuffer::setAlertRecordReassembly(bool enabled) { alertRecordReassembly = enabled; }

bool SegmentBuffer::isAlertPlaintextVisible() const { return alertPlaintextVisible; }

void SegmentBuffer::setAlertPlaintextVisible(bool visible) { alertPlaintextVisible = visible; }

uint16_t SegmentBuffer::getAlertRecordBytesReceived() const { return alertRecordBytesReceived; }

void SegmentBuffer::setAlertRecordBytesReceived(uint16_t bytesReceived) { alertRecordBytesReceived = bytesReceived; }

bool SegmentBuffer::isAlertHeaderParsed() const { return alertHeaderParsed; }

void SegmentBuffer::setAlertHeaderParsed(bool parsed) { alertHeaderParsed = parsed; }

uint8_t SegmentBuffer::getAlertAlignmentOffset() const { return alertAlignmentOffset; }

void SegmentBuffer::setAlertAlignmentOffset(uint8_t offset) { alertAlignmentOffset = offset & 1u; }

void SegmentBuffer::resetContinuationState()
{
    setState(SegmentBuffer::State::InitialNonSegmented);
    setBufferUsed(0);
    setNextLength(0);
    setTotalRecordLength(0);
    setContinuationMode(ContinuationMode::None);
    setContinuationRemainingLength(0);
    setExtensionRemainingLength(0);
    setContinuationRecordType(0);
    setContinuationHandshakeType(0);
    setContinuationRecordLength(0);
    setAlertRecordReassembly(false);
    setAlertPlaintextVisible(false);
    setAlertRecordBytesReceived(0);
    setAlertHeaderParsed(false);
    setStepBufferedLength(0);
    setStepNextRecordOrdinal(0);
    setClientHelloSegmentCount(0);
    numOfCiphers = 0;
    numOfCompressions = 0;
    lastVersion = 0;
}

uint32_t SegmentBuffer::getNumOfCiphers() const { return numOfCiphers; }
void SegmentBuffer::setNumOfCiphers(uint32_t ciphers) { numOfCiphers = ciphers; }

uint32_t SegmentBuffer::getNumOfCompressions() const { return numOfCompressions; }
void SegmentBuffer::setNumOfCompressions(uint32_t compressions) { numOfCompressions = compressions; }

uint16_t SegmentBuffer::getLastVersion() const { return lastVersion; }
void SegmentBuffer::setLastVersion(uint16_t version) { lastVersion = version; }