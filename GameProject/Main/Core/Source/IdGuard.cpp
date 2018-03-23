#include "IdGuard.h"

namespace engine
{

IdGuard::IdGuard(Id p_maxId)
	:m_maxId(p_maxId)
{
}

Id IdGuard::getNextId()
{
	if (isIdCounterOverflowed())
	{
		return engine::UNDEFINED_ID;
	}
	else
	{
		return getNextFreeId();
	}
}

bool IdGuard::isIdCounterOverflowed()
{
	return m_overflowed;
}

Id IdGuard::getNextFreeId()
{
	if (hasFreedIds())
	{
		return getIdFromFreed();
	}
	else
	{
		return getNewId();
	}
}

bool IdGuard::hasFreedIds()
{
	return !m_freedIds.empty();
}

Id IdGuard::getNewId()
{
	if (m_currentId == m_maxId)
	{
		m_overflowed = true;
		return engine::UNDEFINED_ID;
	}
	else
	{
		++m_currentId;
		return m_currentId;
	}
}

Id IdGuard::getIdFromFreed()
{
	auto l_iter = m_freedIds.begin();
	auto l_id = *l_iter;
	m_freedIds.erase(l_iter);

	return l_id;
}

void IdGuard::freeId(Id p_id)
{
	m_freedIds.emplace(p_id);
}

void IdGuard::reset()
{
	m_overflowed = false;
	m_currentId = engine::UNDEFINED_ID;
	m_freedIds.clear();
}

}