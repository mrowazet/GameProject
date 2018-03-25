#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "EntityPool.h"
#include "IdGuardMock.h"

using namespace testing;
using namespace engine;

namespace
{
const PoolSize NR_OF_ENTITIES = 5u;

const PoolSize EMPTY = 0u;
const PoolSize ONE_ELEMENT = 1u;
const PoolSize TWO_ELEMENTS = 2u;

const EntityId ENTITY_ID_1 = 1u;
const EntityId ENTITY_ID_2 = 2u;
}

class EntityPoolTestable : public EntityPool
{
public:
	EntityPoolTestable()
		: EntityPool(NR_OF_ENTITIES, std::make_unique<StrictMock<IdGuardMock>>())
	{

	}

	ContinuousPool<Entity>& getPool()
	{
		return m_pool;
	}

	IdGuardMock& getIdGuardMock()
	{
		return static_cast<IdGuardMock&>(*m_idGuard);
	}
};

class EntityPoolTestSuite : public Test
{
public:
	EntityPoolTestSuite() = default;

protected:
	Entity& addEntityToPool(EntityId p_id);
	bool removeEntityFromPool(EntityId p_id);

	EntityPoolTestable m_entityPool;
};

Entity& EntityPoolTestSuite::addEntityToPool(EntityId p_id)
{
	auto& l_guardMock = m_entityPool.getIdGuardMock();
	EXPECT_CALL(l_guardMock, getNextId()).WillOnce(Return(p_id));

	return m_entityPool.create();
}

bool EntityPoolTestSuite::removeEntityFromPool(EntityId p_id)
{
	auto& l_guardMock = m_entityPool.getIdGuardMock();
	EXPECT_CALL(l_guardMock, freeId(p_id));

	return m_entityPool.remove(p_id);
}

TEST_F(EntityPoolTestSuite, shouldAddEntityToPoolWhenCreateIsCalled)
{
	addEntityToPool(ENTITY_ID_1);
	EXPECT_EQ(ONE_ELEMENT, m_entityPool.size());
}

TEST_F(EntityPoolTestSuite, hasEntityIdshouldReturnFalseIfEntityWasNotCreated)
{
	EXPECT_FALSE(m_entityPool.hasId(ENTITY_ID_1));
}

TEST_F(EntityPoolTestSuite, hasEntityIdshouldReturnTrueIfEntityWasAdded)
{
	addEntityToPool(ENTITY_ID_1);
	EXPECT_TRUE(m_entityPool.hasId(ENTITY_ID_1));
}

TEST_F(EntityPoolTestSuite, shouldReturnProperRefIfEntityExist)
{
	addEntityToPool(ENTITY_ID_1);

	auto& l_entity = m_entityPool.get(ENTITY_ID_1);

	auto& l_internalPool = m_entityPool.getPool();
	auto l_entityInPool = l_internalPool.begin();

	EXPECT_EQ(&l_entity, &(*l_entityInPool));
}

TEST_F(EntityPoolTestSuite, hasEntityShouldReturnFalseIfEntityWasRemoved)
{
	addEntityToPool(ENTITY_ID_1);
	removeEntityFromPool(ENTITY_ID_1);

	EXPECT_FALSE(m_entityPool.hasId(ENTITY_ID_1));
}

TEST_F(EntityPoolTestSuite, removeEntityShouldReturnTrueIfEntityWasStored)
{
	addEntityToPool(ENTITY_ID_1);
	EXPECT_TRUE(removeEntityFromPool(ENTITY_ID_1));
	EXPECT_EQ(EMPTY, m_entityPool.size());
}

TEST_F(EntityPoolTestSuite, removeEntityShouldReturnFalseIfEntityWasNotStored)
{
	EXPECT_FALSE(m_entityPool.remove(ENTITY_ID_1));
}

TEST_F(EntityPoolTestSuite, clearShouldRemoveAllData)
{
	addEntityToPool(ENTITY_ID_1);

	auto& l_guardMock = m_entityPool.getIdGuardMock();
	EXPECT_CALL(l_guardMock, reset());

	m_entityPool.clear();

	EXPECT_FALSE(m_entityPool.hasId(ENTITY_ID_1));
	EXPECT_EQ(EMPTY, m_entityPool.size());
}