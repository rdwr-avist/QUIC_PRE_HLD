//
// Created by MaxG on 31/5/2022.
//

#pragma once

#include <cstdint>
#include <functional>
#include <misc/U128.hpp>
#include <iomanip>
#include "Definitions.hpp"

using namespace std;
using namespace Misc;

namespace TLSFlow
{
    class Fingerprint
    {
    private:
        /**
         * Members
         */
        U128 m_value;
    public:
        /**
         * Param constructor
         *
         * @param hash
         */
        Fingerprint(): m_value(0, 0) {};
        Fingerprint(const U128& value) : m_value(value) { };

        Fingerprint(const char *fp) {
            m_value = 0;
            string fpStr = fp;
            if (FingerprintByteLength == 16 && fpStr.length() == FingerprintByteLength * 2) {
                std::string high_part = fpStr.substr(0, 16);
                std::string low_part = fpStr.substr(16);
                try {
                    m_value.high = std::stoull(high_part, nullptr, 16);
                }
                catch (const std::exception& e) {m_value = 0;}
                try {
                    m_value.low = std::stoull(low_part, nullptr, 16);
                }
                catch (const std::exception& e) {m_value = 0;}
            }
        }

        /**
         * Getters
         */
        const U128& value() const { return m_value; }
        U128& value() { return m_value; }

        void setValue(const U128 &mValue) {
            m_value = mValue;
        }

        __uint128_t getFingerprintUint128() const {
            __uint128_t val = m_value.high;
            val <<= 64;
            val += m_value.low;
            return val;
        }
        void setFingerprintUint128(const __uint128_t &mFingerprint) {
            m_value.high = static_cast<uint64_t>(mFingerprint >> 64);
            m_value.low = static_cast<uint64_t>(mFingerprint);
        }

        /**
         * Operators
         */
        bool operator==(const Fingerprint& other) const { return m_value == other.m_value; }
        bool operator==(const U128& value) const { return m_value == value; }
        bool operator!=(const Fingerprint& other) const { return m_value != other.m_value; }
        bool operator!=(const U128& value) const { return m_value != value; }
//        void operator=(const Fingerprint& other) { m_value = other.m_value; }
        Fingerprint& operator=(const Fingerprint& other) { m_value = other.m_value; return *this;}
        void operator=(const U128& value) { m_value = value; }
        friend ostream& operator<<(ostream& output, const Fingerprint& fingerprint) { output << std::setw(16) << std::setfill('0') << std::hex << std::uppercase << fingerprint.value().high; output << std::setw(16) << std::setfill('0') << std::hex << std::uppercase << fingerprint.value().low; return output; }
        operator string() const { stringstream output; output << *this; return output.str(); }
    };
};

/*******************************************
 * Hash injection into std namespace
 *******************************************/
namespace std
{
    template<> struct hash<TLSFlow::Fingerprint>
    {
        auto operator()(const TLSFlow::Fingerprint& fingerprint) const
        {
            return hash<size_t>()(hash<uint64_t>()(fingerprint.value().high) + hash<uint64_t>()(fingerprint.value().low));
        }
    };
}
