//
// Created by maxg on 4/18/23.
//

#pragma once

#include <cstdint>

namespace TLSFlow {
    namespace NSProcessor {
        namespace Engine {
            class PayloadParams {
            private:
                /**
                 * Members
                 */
                uint8_t *m_pPayload;
                uint16_t m_length;
                uint32_t m_sessionIdx;
                uint32_t m_tcpSeq;
            public:
                /**
                 * Default constructor
                 */
                PayloadParams() = default;

                /**
                 * Basic Param constructor for bac
                 *
                 * @param payload
                 * @param length
                 * @param sessionIdx
                 * @param tcpSeq
                 */
                PayloadParams(uint8_t *payload, uint16_t length, uint32_t sessionIdx = 0xffffffff, uint32_t tcpSeq = 0)
                        : m_pPayload(payload), m_length(length), m_sessionIdx(sessionIdx), m_tcpSeq(tcpSeq) {}


                /**
                 * Getters & Setters
                 */
                uint8_t *getPayload() const { return m_pPayload; }

                void setPayload(uint8_t *payload) { m_pPayload = payload; }

                uint16_t getLength() const { return m_length; }

                void setLength(uint16_t length) { m_length = length; }

                uint32_t getSessionIdx() const { return m_sessionIdx; }

                void setContext(uint32_t sessionIdx) { m_sessionIdx = sessionIdx; }

                uint32_t getTcpSeq() const { return m_tcpSeq; }

                void setTcpSeq(uint32_t tcpSeq) { m_tcpSeq = tcpSeq; }
            };
        }
    }
}

