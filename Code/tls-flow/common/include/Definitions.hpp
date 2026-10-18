//
// Created by talc on 7/27/25.
//

#pragma once
#include <cstddef>

extern "C" {
#include "Definitions.h"
}

namespace TLSFlow
{
  namespace Extension
  {
    namespace Engine
    {
      template <std::size_t MainCapacity,
                std::size_t ReclaimableCapacity = MainCapacity / 2>
      class TLSSessionExtensionTable;
    }
  }

  /// --------------------------
  /// Constants
  /// --------------------------
  static constexpr uint32_t FingerprintByteLength = FINGERPRINT_BYTE_LENGTH;
  static constexpr std::size_t SessionExtensionTableCapacity = 200000;

  using TlsSessionExtensionTable =
      Extension::Engine::TLSSessionExtensionTable<SessionExtensionTableCapacity>;

}