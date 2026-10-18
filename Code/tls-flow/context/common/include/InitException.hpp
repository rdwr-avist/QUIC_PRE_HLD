//
// Created by talc on 7/22/25.
//

#pragma once

#include <string>

namespace TLSFlow
{
    namespace Context
    {
        class InitException
        {
        private:
            /// --------------------------
            /// Members
            /// --------------------------
            std::string phase;
            std::string module;
            std::string message;

        public:
            /// --------------------------
            /// Constructor
            /// --------------------------
            InitException(const std::string& phase, const std::string& module, const std::string& message): phase(phase), module(module), message(message) {};

            /// --------------------------
            /// Getters
            /// --------------------------
            std::string getPhase() const { return phase; };
            std::string getModule() const { return module; };
            std::string getMessage() const { return message; };
        };
    } // Context
} // TLSFlow