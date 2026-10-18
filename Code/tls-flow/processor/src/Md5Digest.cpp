//
// Created by YohayG on 15/06/2022.
//

#include "Md5Digest.hpp"


using namespace TLSFlow::NSProcessor::Engine;
using namespace TLSFlow;

constexpr uint32_t Md5Digest::s_max_buffer_size;

// Call init before first part calculation to clean a given fp
bool Md5Digest::init(){
    m_arrayIndex = 0;
    return true;
}
// NOTE! This API does not perform any calculation
// Save a portion of bytes to hashlater
bool Md5Digest::calculate(const uint8_t *const data, size_t len) {
    if (len == 0 ||
       (len + m_arrayIndex >= sizeof(m_array))) {
        return false;
    }
    memcpy(&(m_array[m_arrayIndex]), data, len);
    m_arrayIndex+= len;
    return true;
}

// Finalize iterative hash calculation (add padding if needed)
// This is actually the only place where we calculate the MD5 hash on the complete buffer aggregated until here
bool Md5Digest::finalize(TLSFlow::Fingerprint &fp){
    MD5(m_array, m_arrayIndex, (reinterpret_cast<uint8_t*>(&fp)));
    return true;
}
