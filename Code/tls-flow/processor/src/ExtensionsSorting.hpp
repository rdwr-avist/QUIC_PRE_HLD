//
// Created by YohayG on 30/05/2023.
//

#pragma once

#include "ITLSParser.hpp"
#include <common/Definitions.hpp>

using namespace std;
namespace TLSFlow
{
    namespace NSProcessor
    {
        namespace Engine
        {

            class ExtensionLocator {
            public:
                ExtensionLocator() = default;

                ExtensionLocator(ExtensionTypeE i_type, uint16_t i_length, const uint8_t *i_pValue) {
                    type = i_type;
                    length = i_length;
                    pValue = i_pValue;
                }

                ExtensionLocator(uint16_t i_type, uint16_t i_length, const uint8_t *i_pValue) {
                    type = static_cast<ExtensionTypeE>(i_type);
                    length = i_length;
                    pValue = i_pValue;
                }

                void setLengthFirstByte(uint8_t byte) { *(reinterpret_cast<uint8_t*>(&length))  = byte;}
                void setLengthLastByte(uint8_t byte) { *(reinterpret_cast<uint8_t*>(&length) + 1)  = byte;}
                void setTypeLastByte(uint8_t byte) {
                    uint16_t t = to_underlying(type) | byte;
                    type = static_cast<ExtensionTypeE>(t);
                }
                ExtensionTypeE getType() const { return type; }
                void setType(ExtensionTypeE inType) { type = inType; }
                void swapLength() {auto t = length; length = htons(t); }
                uint16_t getLength() const { return length; }
                void setLength(uint16_t inLength) { length = inLength; }
                const uint8_t *getPValue() const { return pValue; }
                void setPValue(const uint8_t *pValueIn) { pValue = pValueIn; }


                ExtensionTypeE type;
                uint16_t length;
                const uint8_t *pValue;

                /* Extensions Sorting function. The order is given by the following:
             * Extension value within compared objects are deGREASEd already (meaning only 0x0a0a is expected)

                1.	Ascending Extension Type value
                2.	Ascending Extension length
                3.	Ascending Extension value
                4.  Greased Extensions last
             */

                static bool extensionsSortingComparison(const ExtensionLocator &a, const ExtensionLocator &b) {
                    //cout << "extensionsSortingComparison START \n\ta.type  ( " << hex << showbase << internal << setfill('0') << setw(6) << static_cast<uint16_t >(a.type) << " ) " << "  \tb.type ( "  << setw(6) << static_cast<uint16_t >(b.type) << " ) " << endl;
                    if (a.type == ExtensionTypeE::NORMALIZED_GREASE) {
                        //cout << "a is grease" << endl;
                        if (b.type == ExtensionTypeE::NORMALIZED_GREASE) {
                            //cout << "b is also grease" << endl;
                            return false;
                        }
                        //cout << " ExtensionTypeE::NORMALIZED_GREASE < b.type ==" << ((ExtensionTypeE::NORMALIZED_GREASE < b.type) ? " true " : " false ") << endl;
                        return ExtensionTypeE::NORMALIZED_GREASE < b.type;
                    } else if (b.type == ExtensionTypeE::NORMALIZED_GREASE) {
                        //cout << "b is grease" << endl;
                        return a.type < ExtensionTypeE::NORMALIZED_GREASE;
                    }
                    if (a.type != b.type) {
                        //cout << "a.type != b.type, return a.type < b.type == " << ((a.type < b.type) ? "true" : "false") << endl;
                        return a.type < b.type;
                    }
                    if (a.length != b.length) {
                        //cout << "a.lenght != b.length, return a.length < b.length == " << ((a.length < b.length) ? "true" : "false") << endl;
                        return a.length < b.length;
                    }
                    return (memcmp(a.pValue, b.pValue, min(a.length, b.length)) < 0);
                }
            };

            class ExtensionsArray {
            public:
                ExtensionsArray() {}

                bool add(const ExtensionLocator &extension) {
                    if (maxSupportedExtensions <= m_numOfExtensions) {
                        return false;
                    }
                    m_array[m_numOfExtensions++] = extension;
                    return true;

                }

                /* ExtensionLocator - assumes input index is within valid range (get from getNumOfExtensions() )*/
                const ExtensionLocator &get(uint16_t index) const { return m_array[index]; }

                void reset() { m_numOfExtensions = 0; }

                uint16_t getNumOfExtensions() const { return m_numOfExtensions; }

                ExtensionLocator &getLast() { return m_array[getNumOfExtensions() == 0 ? getNumOfExtensions() : getNumOfExtensions() - 1]; }

                void sort() {
                    std::sort(m_array.begin(), m_array.begin() + getNumOfExtensions(),
                              TLSFlow::NSProcessor::Engine::ExtensionLocator::extensionsSortingComparison);
                }


            private:
                static constexpr uint32_t maxSupportedExtensions = 1024; // Maximal extensions supported to be parsed and sorted in a single Client Hello message
                uint16_t m_numOfExtensions;
                std::array<ExtensionLocator, maxSupportedExtensions> m_array;

            };
        }
    }
}
