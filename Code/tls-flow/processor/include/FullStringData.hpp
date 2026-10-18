//
// Created by maxg on 9/4/25.
//

#pragma once

namespace TLSFlow
{
    namespace NSProcessor
    {
        namespace Engine
        {
            class FullStringData
            {
            private:
                /// --------------------------
                /// Members
                /// --------------------------
                const uint8_t* data;
                size_t length;

            public:
                /// --------------------------
                /// Getters & Setters
                /// --------------------------
                const uint8_t* getData() const { return data; }
                size_t getLength() const { return length; }

                /// --------------------------
                /// Constructors & Destructors
                /// --------------------------
                FullStringData(const uint8_t* data, size_t length) : data(data), length(length) {}
                ~FullStringData() = default;
            };
        } // Engine
    } // NSProcessor
} // TLSFlow
