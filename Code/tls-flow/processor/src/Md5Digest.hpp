//
// Created by YohayG on 15/06/2022.
//

#pragma once

#include "Md5Digest.hpp"
#include "openssl/md5.h"
#include <netinet/in.h>
#include "application/tls-flow/common/include/Fingerprint.hpp"

namespace TLSFlow {
    namespace NSProcessor {
        namespace Engine {
            class Md5Digest {
            public:
                Md5Digest() {};

                ~Md5Digest() = default;

                bool init() ;

                bool calculate(const uint8_t *data, size_t len) ;

                bool finalize(Fingerprint &fp) ;

                void copy(const Md5Digest& from) {
                    memcpy(m_array, from.m_array, std::min(static_cast<uint32_t >(from.m_arrayIndex), s_max_buffer_size));
                    m_arrayIndex = from.m_arrayIndex;
                }

                size_t getFsLen() {return m_arrayIndex;}
                uint8_t *getFsArray() {return m_array;}

            private:
                static constexpr uint32_t s_max_buffer_size = 2870; // To hold two tcp segments (1460 * 2) - 50 which is the minimum tls record length before any extension
                uint8_t m_array[s_max_buffer_size];      // A buffer used to store the string representation byte values before computing the hash on it
                size_t m_arrayIndex = 0;   // Length of used bytes in the buffer
            };
        }
    }
}


