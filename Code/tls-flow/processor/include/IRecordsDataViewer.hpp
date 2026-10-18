#pragma once
#include "Record.hpp"
#include <sh-data-struct/ESTL/FixedVector.hpp>
namespace TLSFlow
{
    namespace NSProcessor
    {
        namespace Engine
        {
            class IRecordsDataViewer
            {
            public:
                using Records = ESTL::FixedVector<Record, false>;
                virtual ~IRecordsDataViewer() = default;
                virtual const Records& getAllRecords() const = 0;

                virtual const Record& getLastRecord() const = 0;
                virtual uint8_t getRecordType() const = 0;
                virtual uint16_t getRecordLength() const = 0;
            }; // class IRecordsDataViewer
        }// namespace Engine
    }// namespace NSProcessor
} // namespace TLSFlow