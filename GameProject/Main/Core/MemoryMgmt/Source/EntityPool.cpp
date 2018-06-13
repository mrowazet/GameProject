#include "EntityPool.h"

namespace engine
{
	EntityPool::EntityPool(PoolSize p_poolSize, std::unique_ptr<IIdGuard> p_guard)
		: m_pool(p_poolSize),
		  m_idGuard(std::move(p_guard))
	{
	}

	Entity& EntityPool::create()
	{
		auto& l_entity = m_pool.getNext();	

		l_entity.id = m_idGuard->getNextId();
		m_storedIds.emplace(l_entity.id);

		return l_entity;
	}

	bool EntityPool::removeEntity(EntityId p_id)
	{
		auto l_iter = m_storedIds.find(p_id);

		if (l_iter != m_storedIds.end())
		{
			m_pool.takeBack(getEntity(p_id));

			m_idGuard->freeId(p_id);
			m_storedIds.erase(l_iter);
			
			return true;
		}
		else
		{
			return false;
		}
	}

	Entity& EntityPool::getEntity(EntityId p_id)
	{
		auto l_iter = m_pool.begin();
		auto l_endIter = m_pool.end();

		for (l_iter; l_iter != l_endIter; l_iter++)
		{
			if (l_iter->id == p_id)
			{
				break;
			}
		}

		return *l_iter;
	}

	u32 EntityPool::size() const
	{
		return m_pool.size();
	}

	bool EntityPool::hasId(EntityId p_id) const
	{
		auto l_iter = m_storedIds.find(p_id);
		return l_iter != m_storedIds.end();
	}

	/*
		Caution!
		Information about stored Ids can be desynchronized!
		If Entity with some Id is stored by create method and after that Id is manually changed outside the class then ids in internal set are 'out of date'!
		Entity Id should NOT be changed manually at all.
	*/

	void EntityPool::clear()
	{
		m_pool.clear();
		m_storedIds.clear();
		m_idGuard->reset();
	}
}