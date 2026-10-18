//
// Created by YohayG on 14/06/2022.
//
#pragma once
#include "../../../GENERIC/diag/diag_log_inject.h"
#include "../../diagnostics/engine/src/DiagnosticsCounters.hpp"
#include "../include/IClientHelloDataViewer.hpp"
#include "../include/IRecordsDataViewer.hpp"
#include "../include/IParsingErrorViewer.hpp"
#include "ExtensionsSorting.hpp"
#include "ITLSParser.hpp"
#include "Md5Digest.hpp"
#include "SegmentBufferManager.hpp"
#include "application/tls-flow/processor/include/IProcessor.hpp"
#include <memory.h>

#ifdef APP_NDEBUG // NO DEBUG BUILD
#define DIAG_LOGGER_INJECT_DEBUG_ONLY(id, ...) // Empty definition for release build
#else
#define DIAG_LOGGER_INJECT_DEBUG_ONLY(id, ...) DIAG_LOGGER_INJECT(id, ##__VA_ARGS__)
#endif

namespace TLSFlow
{
    namespace NSProcessor
    {
        namespace Engine
        {
            class Processor : public IProcessor, public IClientHelloDataViewer, public IRecordsDataViewer, public IParsingErrorViewer
            {
            enum class RecordProcessResult {
                ContinueProcessing,  ///< Packet processed successfully
                Segmented,           ///< Awaiting next TCP segment
                Unsupported,         ///< Legal ClientHello segmentation not supported for reconstruction
                Error,               ///< Protocol violation - malformed packet (potential attack)
                EndOfPacket,        ///< Packet should be ignored without parsing
                StopProcessing       ///< Edge case - valid packet we can't handle (soft failure)
            };
            public:
                Processor() = default;
                ~Processor() override = default;

				void init(unique_ptr<IParsersContainer> parsers, SessionTable *sessionTable, unique_ptr<NSDiagnostics::Engine::IDiagnosticsCounters> diagnosticsCounters) {
					m_parsers = move(parsers);
					m_diagnosticsCounters = std::move(diagnosticsCounters);
					/* Build segment buffer */
                    segmentBufferManager = make_unique<TLSFlow::NSProcessor::Engine::SegmentBufferManager>(
                        sessionTable, *m_diagnosticsCounters);
				}
                /// --------------------------
                /// IProcessor
                /// --------------------------
                void beginPacket(const PayloadParams &payloadParams) override;
                TLSStepResult processRecord(const TLSData &data) override;
                bool isContinuationGap() const override { return m_stepContinuationGap; }
                ContinuationMode getContinuationMode() const override { return m_stepContinuationMode; }
                /// --------------------------
                /// IClientHelloDataViewer
                /// --------------------------
                uint16_t getTlsVersion() const override;
                bool getSni(Sni &sni) override;
                uint8_t getSupportedVersionBitMask() const override;
                bool isEarlyDataOffered() const override;
                void getFingerprint(Fingerprint &fingerprint) const override;
                FullStringData getFullString() const override;
                uint32_t getCipherCount() const override;
                uint32_t getExtensionCount() const override;
                uint32_t getCompressionCount() const override;
                bool isSegmented() const override;
                IClientHelloDataViewer::ClientHelloParsingStatus getClientHelloParsingStatus() const override;

                /// --------------------------
                /// IRecordsDataViewer
                /// --------------------------
                const Records& getAllRecords() const override;
                const Record& getLastRecord() const override;
                uint8_t getRecordType() const override;
                uint16_t getRecordLength() const override;

                /// --------------------------
                /// IParsingErrorViewer
                /// --------------------------
                ParsingError getLastError() const override;
            private:
                enum class ProcessorState{
                    SingleSegment,         // Non Segmented
                    FirstSegment,          // TCP Segmented, currently handling the first segment
                    LastSegment            // TCP Segmented, currently handling the last segment
                };

                // Resources shared for all processor methods
                Fingerprint m_fp;
                SegmentBuffer m_initialSegment;
                ProcessorState m_state = ProcessorState::SingleSegment;

                unique_ptr<IParsersContainer> m_parsers = nullptr;
                // Pointer to resources whether local class members or allocated from segment state pool
                Md5Digest *m_pDigest = nullptr;
                ExtensionsArray *m_pExtensionLocators = nullptr;
                SegmentBuffer *m_pSegmentBuffer = &m_initialSegment;
                unique_ptr<NSDiagnostics::Engine::IDiagnosticsCounters> m_diagnosticsCounters = nullptr;
                unique_ptr<SegmentBufferManager> segmentBufferManager = nullptr;

            	// SNI handling members
            	ExtensionLocator  m_sniExtension{};
            	bool              m_bSniExtensionParsed = false;
            	Sni               m_sni;

                // Last Version
                uint16_t lastVersion = 0;

                uint32_t m_numOfCipherSuites = 0;
                uint32_t m_numOfExtensions = 0;
                uint32_t m_numOfCompressions = 0;
                IClientHelloDataViewer::ClientHelloParsingStatus m_clientHelloParsingStatus = IClientHelloDataViewer::ClientHelloParsingStatus::None;

                // Step-oriented anomaly and minimal ClientHello metadata
                uint8_t m_supportedVersionsBitMask{};
                bool m_supportedVersionsParsed{false};
                bool m_earlyDataOffered{false};

                /// --------------------------
                /// IRecordsDataViewer members
                /// --------------------------
                Record m_record;
                static constexpr size_t maxRecords = 128;
                ESTL::CTVector<Record, maxRecords, false> m_records;

                /// --------------------------
                /// IParsingErrorViewer members
                /// --------------------------
                ParsingError m_lastError = ParsingError::None;

            public:
                ProcessorState getState() const;

                void setState(ProcessorState mState);
                
            private:
                // Segment-level position and length (entire TCP segment)
                const uint8_t *m_parserLocationPtr{};
                uint16_t m_lengthLeftToParse{};

                // Step-oriented packet-local position and length 
                bool m_stepPacketInitialized{false};
                bool m_stepPacketError{false};
                bool m_stepPacketIsRetransmission{false};
                bool m_stepContinuationGap{false};
                ContinuationMode m_stepContinuationMode{ContinuationMode::None};
                bool m_currentRecordIsSegmented{false};
                const uint8_t *m_packetPayloadPtr{};
                uint16_t m_packetPayloadLen{};
                // Copied out of PayloadParams (by value) so continuation logic never
                // depends on the caller's PayloadParams object outliving beginPacket().
                uint32_t m_sessionIdx{};
                uint32_t m_nextExpectedSeq{};

                // Step-oriented current record metadata 
                uint8_t m_stepCurrentRecordType{};
                uint16_t m_stepCurrentRecordLength{};
                uint32_t m_stepCurrentRecordOrdinal{};
                uint32_t m_stepNextRecordOrdinal{};

                // Record-level position and length (current TLS record payload)
                const uint8_t *m_currentRecordPayloadPtr{};
                uint16_t m_currentRecordPayloadLeft{};

                void resetStepClientHelloState();
                void clearStepClientHelloValues();

                bool calcHash(const uint8_t *data, uint32_t len) const {
                    DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALL_CALC_U8_LEN, len);
                    for (uint32_t i = 0; i < len; i++) {
                        //printf("%02x", data[i]);
                        DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_DATA, data[i]);
                    }
                    return m_pDigest->calculate(data, len);
                };

                bool calcHash(const uint8_t data) const {
                    //printf("%02x", data);
                    DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALL_CALC_U8, data);
                    return m_pDigest->calculate(&data, 1);
                };

                bool calcHash(const uint16_t data) const {
                    uint16_t swapped = htons(data);
                    //printf("%04x", data);
                    DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOR_CALL_CALC_U16, data);
                    return m_pDigest->calculate(reinterpret_cast<uint8_t *>(&swapped), 2);
                }

                bool appendOpenParentheses() {
                    //printf("(");
                    return m_pDigest->calculate(&openParentheses, 1);
                }

                bool appendClosingParentheses() {
                    //printf(")");
                    return m_pDigest->calculate(&closingParentheses, 1);
                }

                bool appendOpenSqParentheses() {
                    //printf("[");
                    return m_pDigest->calculate(&openSqParentheses, 1);
                }

                bool appendClosingSqParentheses() {
                    //printf("]");
                    return m_pDigest->calculate(&closingSqParentheses, 1);
                }

				/**
				 *
				 * @param type input extention type
				 * @return true if the given extension type content is included in the FP creation hash
				 */
				bool isExtensionContentIncludedInFp(ExtensionTypeE type) {
                    // List here each extension that it's value is required to be included in the calculated FP hash
                    switch (type) {
                        case ExtensionTypeE::max_fragment_length:
                        case ExtensionTypeE::status_request:
                        case ExtensionTypeE::client_authz:
                        case ExtensionTypeE::server_authz:
                        case ExtensionTypeE::cert_type:
                        case ExtensionTypeE::supported_groups:
                        case ExtensionTypeE::ec_point_formats:
                        case ExtensionTypeE::signature_algorithms:
                        case ExtensionTypeE::heartbeat:
                        case ExtensionTypeE::application_layer_protocol_negotiation:
                        case ExtensionTypeE::status_request_v2:
                        case ExtensionTypeE::token_binding:
                        case ExtensionTypeE::quic_transport_parameters:
                        case ExtensionTypeE::compress_certificate:
                        case ExtensionTypeE::record_size_limit:
                        case ExtensionTypeE::supported_versions:
                        case ExtensionTypeE::psk_key_exchange_modes:
                        case ExtensionTypeE::signature_algorithms_cert:
                        case ExtensionTypeE::token_binding_prototype_ms:
                        case ExtensionTypeE::type_quic_transport_parameters_draft:
                            return true;
                        default:
                            return false;
                    }
                }

                bool isExtensionContentHasLengthFields(ExtensionTypeE type) {
                    // List here each extension that it's value contains inner length fields
                    switch (type) {
                        case ExtensionTypeE::application_layer_protocol_negotiation: // ALPN
                        case ExtensionTypeE::pre_shared_key:
                        case ExtensionTypeE::signature_algorithms_cert:
                            return true;
                        default:
                            return false;
                    }
                }

                

				/**
				 *
				 * @param type input extention type
				 * @return true if the extension type content is required for usages other than computing the FP hash
				 */
				bool isExtensionContentNeeded(ExtensionTypeE type) {
                    // List here each extension that it's value is required for usages other than computing the FP hash
                    switch (type) {
                    	// SNI extension content is required for getParsedSni() will be able to return SNI also if it arrives segmented
                        case ExtensionTypeE::server_name:
                            return true;
                        default:
                            return false;
                    }
                }

				/**
				 *
				 * @param type input extention type
				 * @return true if the given extension type content is required to be saved while parsing segmented hello messages
				 */
				bool shouldSavePartialExtensionContent(ExtensionTypeE type) {
                	return (isExtensionContentNeeded(type) || isExtensionContentIncludedInFp(type));
                }

                uint16_t deGrease(uint16_t value) {
                    switch (value) {
                        case 0x0a0a:
                        case 0x1a1a:
                        case 0x2a2a:
                        case 0x3a3a:
                        case 0x4a4a:
                        case 0x5a5a:
                        case 0x6a6a:
                        case 0x7a7a:
                        case 0x8a8a:
                        case 0x9a9a:
                        case 0xaaaa:
                        case 0xbaba:
                        case 0xcaca:
                        case 0xdada:
                        case 0xeaea:
                        case 0xfafa:
                            DIAG_LOGGER_INJECT_DEBUG_ONLY(PROCESSOC_DEGREASEED);

                            return 0x0a0a;
                        default:
                            return value;
                    }
                }

                bool isGreaseValue(uint16_t value) {
                    switch (value) {
                        case 0x0a0a:
                        case 0x1a1a:
                        case 0x2a2a:
                        case 0x3a3a:
                        case 0x4a4a:
                        case 0x5a5a:
                        case 0x6a6a:
                        case 0x7a7a:
                        case 0x8a8a:
                        case 0x9a9a:
                        case 0xaaaa:
                        case 0xbaba:
                        case 0xcaca:
                        case 0xdada:
                        case 0xeaea:
                        case 0xfafa:
                            return true;
                        default:
                            return false;
                    }
                }

                static constexpr uint8_t openParentheses = '(';
                static constexpr uint8_t closingParentheses = ')';
                static constexpr uint8_t openSqParentheses = '[';
                static constexpr uint8_t closingSqParentheses = ']';

                RecordProcessResult processTlsRecord();

                RecordProcessResult processHandshake();

                RecordProcessResult processAlert(const TLSData &data);

                RecordProcessResult processSegmentedAlertContinuation();

                RecordProcessResult processClientHello(bool recordLevelFragmented);

                RecordProcessResult processCiphers();

                RecordProcessResult processExtensions();

                RecordProcessResult processSni();
                RecordProcessResult parseAllExtensions(const uint8_t* extensionsPtr, uint16_t extensionsLen);

                RecordProcessResult feedExtensionsFP();

                RecordProcessResult feedSuppGroupsFP(const ExtensionLocator &extensionLocator);

                RecordProcessResult feedSuppVersionsFP(const ExtensionLocator &extensionLocator);

                void deallocateSegmentBuff();

                /**
                 * @brief Process a single TLS record from the current position
                 * Handles both segmented continuation and new record processing.
                 * @return ContinueProcessing to process next record, StopProcessing for segmented,
                 *         Error on failure
                 */
                RecordProcessResult processSingleRecord(const TLSData &data);

                /**
                 * @brief Process a new (non-segmented) TLS record
                 * Parses the record header, determines type, and for handshakes
                 * continues to parse the full message. Adds parsed record to m_records.
                 * @return ContinueProcessing to process next record, Segmented for segmented,
                 *         Error on failure
                 */
                RecordProcessResult processNewRecord(const TLSData &data);

                /**
                 * @brief Handle continuation of a ClientHello split across TCP segments.
                 * Supports repeated extension-byte continuation until reconstruction
                 * completes, then resumes parsing at the next record boundary.
                 * @return RecordProcessResult indicating success or failure
                 */
                RecordProcessResult processSegmentedClientHelloContinuation();

                RecordProcessResult processSegmentedOpaqueContinuation();

                bool prepareSegmentBufferForContinuation(SegmentBuffer::ContinuationMode mode, bool &allocated);

                bool startSegmentedAlert(const TLSData &data);

                bool startSegmentedAlert(const TLSData &data, uint16_t recordPayloadLength);

                RecordProcessResult updateAlertAlignment(bool plaintextVisible, uint16_t recordLength);

                void finishSegmentBufferContinuation();

                void resetContinuationPointers();

                /// --------------------------
                ///Begin Packet helper functions
                /// --------------------------
                /**
                 * @brief Capture the scalar packet/session values needed for step-oriented parsing
                 * Copies out only value data from payloadParams - never retains a pointer/reference to the
                 * caller's object, which may be a temporary.
                 * @param payloadParams Payload parameters describing the current packet
                 */
                void captureStepPacketMetadata(const PayloadParams &payloadParams);

                /**
                 * @brief Reset step-local parsing state before processing a new packet
                 * Clears step-oriented record metadata, ClientHello/SNI/fingerprint data, and the
                 * current error state. The caller sets the packet parser position and length separately.
                 */
                void resetStepState();

                /**
                 * @brief Resume step-oriented parsing from a pending segment buffer, or discard it if stale
                 * Looks up any segment buffer pending for this session. If it matches and can be resumed, the
                 * step parser position and next record ordinal are restored from it. If a buffer exists but does
                 * not match this packet, it is discarded so persistStepContinuation() can allocate a fresh one later.
                 * @param payloadParams Payload parameters describing the current packet
                 * @return true if parsing state was resumed from a pending segment buffer, false otherwise
                 */
                bool resumeOrDiscardStepContinuation(const PayloadParams &payloadParams);

                /**
                 * @brief Attempt to resume step-oriented processing from a pending segment buffer
                 * Isolated from legacy segmentation (does not touch m_pSegmentBuffer/m_state).
                 * Combines any previously buffered bytes with the new payload's bytes when the
                 * TCP sequence matches, pointing m_stepParserLocationPtr/m_stepLengthLeftToParse
                 * at the combined buffer.
                 * @param payloadParams The new payload parameters passed to beginPacket()
                 * @param segmentBuffer The pending segment buffer found for this session
                 * @return true if resumed (combined buffer is ready to parse), false otherwise
                 */
                bool resumeStepFromSegmentBuffer(const PayloadParams& payloadParams,
                                                  SegmentBuffer& segmentBuffer,
                                                  uint16_t overlap);

            };
        }
    }
}

