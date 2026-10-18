#pragma once

#include "ParsingError.hpp"

namespace TLSFlow
{
    namespace NSProcessor
    {
        namespace Engine
        {
            class IParsingErrorViewer
            {
            public:
                virtual ~IParsingErrorViewer() = default;
                virtual ParsingError getLastError() const = 0;
            }; // class IParsingErrorViewer
        } // namespace Engine
    } // namespace NSProcessor
} // namespace TLSFlow
