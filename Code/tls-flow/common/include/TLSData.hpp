#pragma once

#include <cstdint>
#include "TLSViolation.hpp"
#include "../../common/include/Fingerprint.hpp"

namespace TLSFlow
{
  /**
    * @brief TLS handshake lifecycle state.
    */
    enum class HsState : uint8_t
    {
        PreHandshake,
        InProgress,
        ChangeCipherSpec,
        Finished,
		Error,
		TerminatedByAlert,
		Unsupported,
    };

	static_assert(static_cast<uint8_t>(HsState::ChangeCipherSpec) <
	              static_cast<uint8_t>(HsState::Finished),
	              "HsState ordering is required by the encrypted handshake boundary");
	static_assert(static_cast<uint8_t>(HsState::Unsupported) >
			      static_cast<uint8_t>(HsState::Finished),
	              "HsState ordering is required by terminal-state checks");
	static_assert(static_cast<uint8_t>(HsState::TerminatedByAlert) >
	              static_cast<uint8_t>(HsState::Finished),
	              "HsState ordering is required by terminal-state checks");

	inline bool isActiveHandshakeState(HsState state)
	{
		return state == HsState::PreHandshake ||
		       state == HsState::InProgress ||
		       state == HsState::ChangeCipherSpec;
	}

	inline bool isTerminalState(HsState state)
	{
		return state == HsState::Finished ||
		       state == HsState::Error ||
		       state == HsState::Unsupported ||
		       state == HsState::TerminatedByAlert;
	}

	inline bool isSilentTerminalState(HsState state)
	{
		return state == HsState::Finished || state == HsState::Unsupported;
	}

	/**
	* @brief Bit positions for the tlsVersionMask field in TLSData.
	*
	* Each enumerator is the shift amount for the corresponding TLS version bit.
	* Example: (1u << static_cast<uint8_t>(TLSVersionBit::TLS13)) selects the TLS 1.3 bit.
	*/
	enum class TLSVersionBit : uint8_t
	{
		SSL30 = 0,
		TLS10 = 1,
		TLS11 = 2,
		TLS12 = 3,
		TLS13 = 4
	};

	constexpr uint8_t TLS13BitMask = 1u << static_cast<uint8_t>(TLSVersionBit::TLS13);
	constexpr uint8_t TLS12OrLessBitMask = TLS13BitMask - 1u;

	/**
	* @brief Returns true if the TLS 1.3 bit is set in the given version mask.
	*
	* @param versionMask  Bitmask where each bit position is defined by TLSVersionBit.
	*/
	inline bool isTLS13(uint8_t versionMask)
	{
		return versionMask & TLS13BitMask;
	}

	/**
	* @brief Returns true if the TLS 1.2 or lower bit is set in the given version mask.
	*
	* @param versionMask  Bitmask where each bit position is defined by TLSVersionBit.
	*/
	inline bool isTLS12OrLess(uint8_t versionMask)
	{
		return versionMask & TLS12OrLessBitMask;
	}
	
	/**
	* @brief Per-session TLS flow context, owned by the TLS state update layer.
	*
	* Holds all mutable per-session state accumulated during TLS parsing.
	* All fields are reset to their initial defaults by clear().
	*
	* ch = ClientHello
	* ccs = ChangeCipherSpec
	* AppData = ApplicationData
	*/
	struct TLSData
	{
		HsState          hsState;
		uint8_t          tlsVersionMask;
		uint8_t          clientHelloCount;
		uint8_t          earlyDataCount;
		bool             earlyDataOffered;
		uint8_t          preCcsSeen;
		TLSFlow::Fingerprint chFingerprint;
		TLSViolationMask terminalViolationMask;
		unsigned long clientHelloTimeStamp;
	
		TLSData()
			: hsState(HsState::PreHandshake)
			, tlsVersionMask(0)
			, clientHelloCount(0)
			, earlyDataCount(0)
			, earlyDataOffered(false)
			, preCcsSeen(0)
			, chFingerprint()
			, terminalViolationMask(TLSViolationMaskNone),
			clientHelloTimeStamp(0)
		{}
	
		/**
		* @brief Sets the bit for the given TLS version in tlsVersionMask.
		*
		* @param version  Bit position to set, as defined by TLSVersionBit.
		*/
		void enableVersion(TLSVersionBit version)
		{
			tlsVersionMask |= static_cast<uint8_t>(1u << static_cast<uint8_t>(version));
		}
	
		/**
		* @brief Clears the bit for the given TLS version in tlsVersionMask.
		*
		* @param version  Bit position to clear, as defined by TLSVersionBit.
		*/
		void disableVersion(TLSVersionBit version)
		{
			tlsVersionMask &= static_cast<uint8_t>(~(1u << static_cast<uint8_t>(version)));
		}
	
		/**
		* @brief Resets all fields to their initial defaults.
		*
		* Idempotent — calling clear() on an already-default TLSData is safe.
		*/
		void clear()
		{
			hsState               = HsState::PreHandshake;
			tlsVersionMask        = 0;
			clientHelloCount      = 0;
			earlyDataCount        = 0;
			earlyDataOffered     = false;
			preCcsSeen            = 0;
			chFingerprint    = TLSFlow::Fingerprint();
			terminalViolationMask = TLSViolationMaskNone;
			clientHelloTimeStamp   = 0;
		}
	};

	inline bool usesTerminalPayloadPath(HsState state)
	{
		return state == HsState::Error ||
		       state == HsState::TerminatedByAlert;
	}
	
}