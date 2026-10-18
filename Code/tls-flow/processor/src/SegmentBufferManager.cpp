//
// Created by maxg on 8/7/24.
//

#include "SegmentBufferManager.hpp"
#include <DPEE/DPEEngine.hpp>

using namespace TLSFlow::NSProcessor::Engine;
using namespace TLSFlow::NSDiagnostics::Common;

/// --------------------------
/// Concrete
/// --------------------------
SegmentBufferManager::SegmentBufferManager(
	SessionTable *sessionTable,
	NSDiagnostics::Engine::IDiagnosticsCounters& diagnosticsCounters):
	sessionTable{sessionTable}, diagnosticsCounters{diagnosticsCounters} {}

/// --------------------------
/// ISegmentBufferManager
/// --------------------------
bool SegmentBufferManager::allocate(const SessionId &id, Handler &&handler)
{
	/* Already exist? */
	auto mapItr = map.find(id);
	if(mapItr != map.end())
		return false;

	/* Fifo table is full? */
	if(map.isFull() == true)
	{
		diagnosticsCounters.incEvent(DiagnosticsEvents::SegmentBufferFull);

		/* Get oldest segment buffer */
		mapItr = map.begin();

		/* Fetch associated session of oldest segment buffer */
		auto session = sessionTable->getSessionByIndex(mapItr->first);
		if(session == nullptr)
			return false;

		/* Remove observer from session */
		session->deleteObserver(this, Events::STM_EVENT_AGING);

		/* Release oldest segment buffer */
		map.pop();
	}

	/* Insert into fifo map */
	auto res = map.insert({id, {}});
	if(res.second == false)
		return false;

	/* Fetch the new session */
	auto session = sessionTable->getSessionByIndex(id);
	if(session == nullptr)
		return false;

	/* Register observer to the new session */
	if(session->registerObserver(this, Events::STM_EVENT_AGING, AgingPriorityTLSSegmentBuffer) != 0)
	{
		map.erase(id);
		return false;
	}

	/* Invoke client handler */
	handler(res.first->second);
	return true;
}

bool SegmentBufferManager::deallocate(const SessionId &id)
{
	/* Doesn't exist? */
	auto itr = map.find(id);
	if(itr == map.end())
		return false;

	/* Release */
	if(map.erase(id) == 0)
		return false;

	/* Unregister observer */
	auto session = sessionTable->getSessionByIndex(id);
	if(session == nullptr)
		return false;

	session->deleteObserver(this, Events::STM_EVENT_AGING, AgingPriorityTLSSegmentBuffer);

	return true;
}

bool SegmentBufferManager::access(const SessionId &id, Handler &&handler)
{
	/* Doesn't exist? */
	auto itr = map.find(id);
	if(itr == map.end())
		return false;

	/* Invoke client handler */
	handler(itr->second);
	return true;
}

/// --------------------------
/// Observer
/// --------------------------
int SegmentBufferManager::onEvent(Session *session, DPFrame *packet, int event)
{
	/* Validate event is aging */
	if(event != Events::STM_EVENT_AGING)
		return ObserverResult::RET_OBSERVER_CONTINUE;

	/* Release */
	map.erase(session->getSessionIndex());
	return ObserverResult::RET_OBSERVER_CONTINUE;
}