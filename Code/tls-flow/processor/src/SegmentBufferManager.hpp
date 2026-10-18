//
// Created by maxg on 8/7/24.
//

#pragma once
#include <stm/include/Observer.hpp>
#include <sh-data-struct/FIFOHashMap/FIFOSimpleMap.hpp>
#include "ISegmentBufferManager.hpp"
#include "../../diagnostics/engine/include/IDiagnosticsCounters.hpp"
#include <memory>

namespace TLSFlow
{
	namespace NSProcessor
    {
		namespace Engine
        {
			class SegmentBufferManager : public ISegmentBufferManager, public Observer
			{
			private:
				/// --------------------------
				/// Members
				/// --------------------------
				FIFOSimpleMap<SessionId, SegmentBuffer, MaxNumberOfSegmentBuffers> map;
				SessionTable* sessionTable;
				NSDiagnostics::Engine::IDiagnosticsCounters& diagnosticsCounters;
			public:
				/// --------------------------
				/// Constructors & Destructors
				/// --------------------------
				SegmentBufferManager(
					SessionTable *sessionTable,
					NSDiagnostics::Engine::IDiagnosticsCounters& diagnosticsCounters);

				/// --------------------------
				/// ISegmentBufferManager
				/// --------------------------
				/**
				 * @brief
				 * Allocates new segment buffer for session and register to the sessions aging event.
				 * If there are no free segment buffers remained, we release the last accessed segment buffer and allocate it for the new session.
				 * This is achieved using fifo structure.
				 * Fails if segment buffer is already allocated for the session.
				 *
				 * @param id
				 * @param handler
				 */
				bool allocate(const SessionId &id, Handler &&handler) override;

				/**
				 * @brief
				 * Release segment buffer for session and unregister from sessions aging event.
				 * Fail if not segment buffer was allocated for the session.
				 *
				 * @param id
				 */
				bool deallocate(const SessionId &id) override;

				/**
				 * @brief
				 * Find segment buffer for session.
				 * Re-insert segment buffer from/into the fifo structure to prevent recently accessed entries from being released in case we reach full capacity.
				 * Fail if segment buffer was not found.
				 *
				 * @param id
				 * @param handler
				 */
				bool access(const SessionId &id, Handler &&handler) override;

				/// --------------------------
				/// Observer
				/// --------------------------
				/**
				 * @brief
				 * Handles aging events of sessions that have segment buffers allocated for them.
				 * Releases the allocated segment buffer for the session.
				 */
				int onEvent(Session *session, DPFrame *packet, int event) override;
			};
		} // Engine
	} // NSProcessor
} // TLSFingerprint
