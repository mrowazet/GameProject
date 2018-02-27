#pragma once
#include "IIdGuard.h"
#include <set>

namespace engine
{

class IdGuard : public IIdGuard
{
public:
	IdGuard(Id p_maxId);

	Id getNextId() override;
	void freeId(Id p_id) override;

private:
	const Id m_maxId;

	Id m_currentId = engine::UNDEFINED_ID;
	std::set<Id> m_freedIds;
	bool m_overflowed = false;

	bool isIdCounterOverflowed();
	bool hasFreedIds();
	Id getNextFreeId();
	Id getNewId();
	Id getIdFromFreed();
};

}
