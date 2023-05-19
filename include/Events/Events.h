#pragma once

#include "EventSinks/CombatManager.h"
#include "EventSinks/CrosshairRefMgr.h"
#include "EventSinks/LifeStateMgr.h"
#include "EventSinks/LockedContainerMgr.h"

#ifdef GetObject
#undef GetObject
#endif

namespace Events {
	inline void Register() {
		CrosshairRefManager::Register();
		//LifeStateManager::Register();
		//LockedContainerManager::Register();
		//CombatManager::Register();

		logger::info("Registered all event handlers"sv);
	}
}
