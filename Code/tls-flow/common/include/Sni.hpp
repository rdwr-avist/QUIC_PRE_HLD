//
// Created by YohayG on 21/08/2023.
//

#pragma once

using namespace std;

#include <string>
#include <cstdint>
extern "C"
{
#include "ids/exp/ids.h"
extern UINT_32 idsp_hash_func(const void* key, UINT_32 key_len, UINT_32 header_size);

}

namespace TLSFlow {
    static constexpr uint16_t max_sni_length = 255;

     class Sni {

    public:
        Sni(): m_len{0} {}
        void clear() { sni[0] = 0; m_len = 0;}

        // Constructor from a C-style string
        Sni(const char* sniCStr):m_len{0} {
            if (sniCStr) {
                m_len = strlen(sniCStr);
                if (m_len > max_sni_length) {
                    m_len = max_sni_length;
                }
                memcpy(this->sni, sniCStr, m_len);
            }
            this->sni[m_len] = 0; // Null terminate
        }

        //Copy constructor
        Sni(const Sni &other) {
            m_len = other.m_len;
            memcpy(sni, other.sni, m_len);
            sni[m_len] = 0; // Null terminate
        }

        //Copy assignment operator
        Sni& operator=(const Sni &other) {
            if (this != &other) {
                m_len = other.m_len;
                memcpy(sni, other.sni, m_len);
                sni[m_len] = 0; // Null terminate
            }
            return *this;
        }

        const char* c_str() const { return sni; }

        bool set(const char *ptr, uint16_t len) {

            if (len > max_sni_length){
                return false;
            }
            m_len = len;

            memcpy(sni, ptr, len);
            sni[len] = 0;
            return true;
        }

        bool set(const string &in) {
            return set(in.c_str(), in.length());
        }

        bool operator==(const Sni &other) const { return (m_len == other.m_len) && (memcmp(sni, other.sni, m_len) == 0); }
        void getCopy(char* out) const{
            if (out) {
                memcpy(out, sni, m_len + 1);
         }
        }
        bool isEmpty() const { return (m_len == 0);}
        int  getLen() const { return m_len;}
        friend ostream& operator<<(ostream& output, const Sni& sni) { output << sni.sni; return output; }
        operator string() const { return string(sni, m_len); }
    private:
        char sni[max_sni_length + 1];
        size_t  m_len; // Not including null termination
    };
}

namespace std
{
    template<> struct hash<TLSFlow::Sni>
    {
        size_t operator()(const TLSFlow::Sni& key) const
        {
            char sniStr[TLSFlow::max_sni_length + 1];
            key.getCopy(sniStr);

            // round up to the next 4-byte boundary and pad with zeros
            uint32_t length =  key.getLen();
            length = (length + 3) & ~3;
            memset(sniStr + key.getLen() + 1, 0, length - key.getLen());

            return idsp_hash_func(sniStr, length, MAX_UINT_32);
        }
    };
}