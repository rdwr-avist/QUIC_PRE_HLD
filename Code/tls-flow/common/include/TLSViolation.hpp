#pragma once

#include <cstdint>

namespace TLSFlow
{
    enum class TLSViolation : uint8_t
    {
        None = 0,  // no violation; verdict is non-terminal

        // ── TLS-Enforcement violations ─────────────────────────────────────────
        TLSEnforcementMalformedTLSHandshake,    // generic handshake failure
        TLSEnforcementWrongVersion,             // negotiated TLS version does not meet policy requirement
        TLSEnforcementMaxCiphers,               // cipher suite count exceeds configured maximum
        TLSEnforcementMaxCompression,           // compression method count exceeds configured maximum
        TLSEnforcementMaxExtensions,            // extension count exceeds configured maximum
        TLSEnforcementPrematureClosure,         // TCP-Reset immediately after CH
        TLSEnforcementIncompleteHandshake,      // session closed before handshake completed
        TLSEnforcementEarlyDataExceeded,        // AppData before CH exceeds allowed count (TLS 1.3)
        TLSEnforcementUnexpectedPreCCS,         // excess CCS records before CH
        TLSEnforcementAlertDuringHandshake,     // clear Alert record seen during HS
        TLSEnforcementMalformedClientHello,     // ClientHello structure could not be parsed
        TLSEnforcementUnexpectedRecord,         // record type not permitted in current HS state
        TLSEnforcementMultipleClientHello,      // second ClientHello not permitted (TLS ≤1.2)

        TLSMaxViolationTypes // Must be last. not a real violation
        
    };

    using TLSViolationMask = uint32_t;
    constexpr TLSViolationMask TLSViolationMaskNone = 0u;

    static_assert(static_cast<uint8_t>(TLSViolation::TLSMaxViolationTypes) < 32u,
                  "TLSViolationMask width must fit all TLSViolation values");

    inline bool isValidTLSViolation(TLSViolation violation) noexcept
    {
        return violation != TLSViolation::None &&
               violation != TLSViolation::TLSMaxViolationTypes;
    }

    inline TLSViolationMask toTLSViolationMask(TLSViolation violation) noexcept
    {
        if (!isValidTLSViolation(violation)) {
            return TLSViolationMaskNone;
        }

        return static_cast<TLSViolationMask>(1u << static_cast<uint8_t>(violation));
    }

    inline bool hasTLSViolation(TLSViolationMask mask, TLSViolation violation) noexcept
    {
        return (mask & toTLSViolationMask(violation)) != 0u;
    }

    inline TLSViolation highestTLSViolation(TLSViolationMask mask) noexcept
    {
        for (int violation = static_cast<int>(TLSViolation::TLSMaxViolationTypes) - 1;
             violation > static_cast<int>(TLSViolation::None);
             --violation) {
            const TLSViolation current = static_cast<TLSViolation>(violation);
            if (hasTLSViolation(mask, current)) {
                return current;
            }
        }

        return TLSViolation::None;
    }
} // TLSFlow
