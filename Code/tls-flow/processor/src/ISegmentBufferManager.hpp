//
// Created by maxg on 8/7/24.
//

#pragma once
#include <cstdint>
#include <functional>
#include "SegmentBuffer.hpp"

namespace TLSFlow
{
	namespace NSProcessor
    {
		namespace Engine
        {
			class ISegmentBufferManager
			{
			public:
				/// --------------------------
				/// Aliases
				/// --------------------------
				using SessionId = uint32_t;
				using Handler = std::function<void(SegmentBuffer&)>;

				/// --------------------------
				/// Interfaces
				/// --------------------------
				/**
				 * @brief
				 * Allocate new segment buffer for session and invoke client code handler for processing.
				 * Fails if segment buffer is already allocated for the session.
				 *
				 * @param id
				 * @param handler
				 */
				virtual bool allocate(const SessionId& id, Handler&& handler) =0;

				/**
				 * @brief
				 * Release a segment buffer for session.
				 * Fail if not segment buffer was allocated for the session.
				 *
				 * @param id
				 */
				virtual bool deallocate(const SessionId& id) =0;

				/**
				 * @brief
				 * Find and accesses the segment buffer for session using the client side implementation of the handler.
				 * Fail if segment buffer was not found.
				 *
				 * @param id
				 * @param handler
				 */
				virtual bool access(const SessionId& id, Handler&& handler) =0;

				virtual ~ISegmentBufferManager() = default;
			};
		} // Engine
	} // NSProcessor
} // TLSFingerprint
