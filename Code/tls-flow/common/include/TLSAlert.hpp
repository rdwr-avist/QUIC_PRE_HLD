#pragma once

#include <cstdint>
#include "TLSData.hpp"

namespace TLSFlow
{
    constexpr uint8_t TLSAlertWarningLevel = 1;
    constexpr uint8_t TLSAlertFatalLevel = 2;
    constexpr uint8_t TLSAlertCloseNotify = 0;
    constexpr uint8_t TLSAlertUserCanceled = 90;

    inline bool isTerminatingAlert(uint8_t tlsVersionMask,
                                   uint8_t alertLevel,
                                   uint8_t alertDescription)
    {
        if (alertLevel != TLSAlertWarningLevel && alertLevel != TLSAlertFatalLevel) {
            return false;
        }

        if (alertDescription == TLSAlertCloseNotify) {
            return true;
        }

        const uint8_t knownVersionMask = static_cast<uint8_t>(TLS12OrLessBitMask | TLS13BitMask);
        const bool tls12OrLessOnly = isTLS12OrLess(tlsVersionMask) &&
                                     !isTLS13(tlsVersionMask) &&
                                     (tlsVersionMask & static_cast<uint8_t>(~knownVersionMask)) == 0;
        const bool tls13Only = tlsVersionMask == TLS13BitMask;

        if (tls13Only) {
            return alertDescription != TLSAlertUserCanceled;
        }

        if (tls12OrLessOnly) {
            return alertLevel == TLSAlertFatalLevel;
        }

        return alertLevel == TLSAlertFatalLevel &&
               alertDescription != TLSAlertUserCanceled;
    }
}
