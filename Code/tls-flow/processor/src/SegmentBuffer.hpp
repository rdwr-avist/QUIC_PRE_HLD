//
// Created by maxg on 8/7/24.
//

#pragma once
#include <array>
#include "../include/Definitions.hpp"
#include "ExtensionsSorting.hpp"

namespace TLSFlow
{
	namespace NSProcessor
    {
		namespace Engine
        {
			class SegmentBuffer
			{

            public:
                enum class ContinuationMode : uint8_t
                {
                    None,
                    OpaqueRecordTracking,
                    ClientHelloExtensionReassembly,
                    AlertRecordReassembly
                };

                /// --------------------------
                /// State Descriptor
                /// --------------------------
                enum class State
                {
                    InitialNonSegmented,
                    IncompleteInitial,           // Initial state identified some part of the extensions are missing in the current segment
                    PendingExtensionTypeStart,   // Where one segment contained a full extension data and next segment will start with a new extension structure
                    PendingExtensionTypeCont,    // Where the segment is cut after one byte of the extension type field
                    PendingExtensionLengthStart, // Where the segment is cut after the complete extension type field
                    PendingExtensionLengthCont,  // Where the segment is cut after one byte of the extension Length field
                    PendingExtensionDataStart,   // Where the segment is cut after the complete extension Length field
                    PendingExtensionDataCont,    // Where the segment is cut in the middle of the extension data
                    Complete
                };


                void init() {
                    setState(SegmentBuffer::State::IncompleteInitial);
                    // For testing add some dirty to the buffer so that tests that may repeat with same content will be reliable
                    // memset(buffer.data(), 0xAD, SegmentBufferSize);
                    setBufferUsed(0);
                    setNextSequence(0);
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
                    setAlertAlignmentOffset(0);
                    setStepBufferedLength(0);
                    setStepNextRecordOrdinal(0);
                    setClientHelloSegmentCount(0);
                    numOfCiphers = 0;
                    numOfCompressions = 0;
                    lastVersion = 0;
                }


				/// --------------------------
				/// Getters & Setters
				/// --------------------------
				const std::array<char, SegmentBufferSize>& getBuffer() const;
				std::array<char, SegmentBufferSize>& getBuffer();
				uint16_t getBufferUsed() const;
				void setBufferUsed(uint16_t bufferUsed);
                ExtensionsArray * getLocatorsArray() ;
				uint32_t getNextSequence() const;
				void setNextSequence(uint32_t nextSequence);
				uint16_t getNextLength() const;
				void setNextLength(uint16_t nextLength);
                uint16_t getStepBufferedLength() const;
                void setStepBufferedLength(uint16_t stepBufferedLength);
                uint32_t getStepNextRecordOrdinal() const;
                void setStepNextRecordOrdinal(uint32_t stepNextRecordOrdinal);
                uint32_t getClientHelloSegmentCount() const;
                void setClientHelloSegmentCount(uint32_t clientHelloSegmentCount);
                Md5Digest& getDigest() ;
                void copyDigest(Md5Digest &inDigest);
                uint16_t getTotalRecordLength() const;
                void setTotalRecordLength(uint16_t length);
                ContinuationMode getContinuationMode() const;
                void setContinuationMode(ContinuationMode mode);
                uint16_t getContinuationRemainingLength() const;
                void setContinuationRemainingLength(uint16_t length);
                uint16_t getExtensionRemainingLength() const;
                void setExtensionRemainingLength(uint16_t length);
                uint8_t getContinuationRecordType() const;
                void setContinuationRecordType(uint8_t recordType);
                uint8_t getContinuationHandshakeType() const;
                void setContinuationHandshakeType(uint8_t handshakeType);
                uint16_t getContinuationRecordLength() const;
                void setContinuationRecordLength(uint16_t recordLength);
                bool isAlertRecordReassembly() const;
                void setAlertRecordReassembly(bool enabled);
                bool isAlertPlaintextVisible() const;
                void setAlertPlaintextVisible(bool visible);
                uint16_t getAlertRecordBytesReceived() const;
                void setAlertRecordBytesReceived(uint16_t bytesReceived);
                bool isAlertHeaderParsed() const;
                void setAlertHeaderParsed(bool parsed);
                uint8_t getAlertAlignmentOffset() const;
                void setAlertAlignmentOffset(uint8_t offset);
                void resetContinuationState();
                void setState(State state);
                State getState() const;
                uint32_t getNumOfCiphers() const;
                void setNumOfCiphers(uint32_t numOfCiphers);
                uint32_t getNumOfCompressions() const;
                void setNumOfCompressions(uint32_t numOfCompressions);
                uint16_t getLastVersion() const;
                void setLastVersion(uint16_t lastVersion);

            private:

                /// --------------------------
                /// Members
                /// --------------------------
                uint32_t numOfCiphers{0};
                uint32_t numOfCompressions{0};
                uint16_t lastVersion{0};
                std::array<char, SegmentBufferSize> buffer;
                uint16_t bufferUsed;
                ExtensionsArray extensionsArray;
                uint32_t nextSequence;
                uint16_t nextLength;
                uint16_t stepBufferedLength;
                uint32_t stepNextRecordOrdinal;
                uint32_t clientHelloSegmentCount{0};
                uint16_t totalRecordLength;
                ContinuationMode continuationMode{ContinuationMode::None};
                uint16_t continuationRemainingLength{0};
                uint16_t extensionRemainingLength{0};
                uint8_t continuationRecordType{0};
                uint8_t continuationHandshakeType{0};
                uint16_t continuationRecordLength{0};
                bool alertRecordReassembly{false};
                bool alertPlaintextVisible{false};
                uint16_t alertRecordBytesReceived{0};
                bool alertHeaderParsed{false};
                uint8_t alertAlignmentOffset{0};
                State state;
                Md5Digest digest;

            };
		} // Engine
	} // NSProcessor
} // TLSFingerprint
