#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "EntityPool.h"
#include "IdGuardMock.h"
#include "UniquePtrMockWrapper.h"

using namespace testing;
using namespace testTool;
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
	EntityPoolTestable(std::unique_ptr<IIdGuard> p_guard)
		: EntityPool(NR_OF_ENTITIES, std::move(p_guard))
	{

	}

	ContinuousPool<Entity>& getPool()
	{
		return m_pool;
	}
};

class EntityPoolTestSuite : public Test
{
public:
	EntityPoolTestSuite()
		:m_sut(m_idGuardMock.getPtr())
	{
	}

protected:
	Entity& addEntityToPool(EntityId p_id);
	bool removeEntityFromPool(EntityId p_id);

	UniquePtrMockWrapper<StrictMock<IdGuardMock>> m_idGuardMock;
	EntityPoolTestable m_sut;
};

Entity& EntityPoolTestSuite::addEntityToPool(EntityId p_id)
{
	EXPECT_CALL(*m_idGuardMock, getNextId()).WillOnce(Return(p_id));

	return m_sut.create();
}

bool EntityPoolTestSuite::removeEntityFromPool(EntityId p_id)
{
	EXPECT_CALL(*m_idGuardMock, freeId(p_id));

	return m_sut.removeEntity(p_id);
}

TEST_F(EntityPoolTestSuite, shouldAddEntityToPoolWhenCreateIsCalled)
{
	addEntityToPool(ENTITY_ID_1);
	EXPECT_EQ(ONE_ELEMENT, m_sut.size());
}

TEST_F(EntityPoolTestSuite, hasEntityIdshouldReturnFalseIfEntityWasNotCreated)
{
	EXPECT_FALSE(m_sut.hasId(ENTITY_ID_1));
}

TEST_F(EntityPoolTestSuite, hasEntityIdshouldReturnTrueIfEntityWasAdded)
{
	addEntityToPool(ENTITY_ID_1);
	EXPECT_TRUE(m_sut.hasId(ENTITY_ID_1));
}

TEST_F(EntityPoolTestSuite, shouldReturnProperRefIfEntityExist)
{
	addEntityToPool(ENTITY_ID_1);

	auto& l_entity = m_sut.getEntity(ENTITY_ID_1);

	auto& l_internalPool = m_sut.getPool();
	auto l_entityInPool = l_internalPool.begin();

	EXPECT_EQ(&l_entity, &(*l_entityInPool));
}

TEST_F(EntityPoolTestSuite, hasEntityShouldReturnFalseIfEntityWasRemoved)
{
	addEntityToPool(ENTITY_ID_1);
	removeEntityFromPool(ENTITY_ID_1);

	EXPECT_FALSE(m_sut.hasId(ENTITY_ID_1));
}

TEST_F(EntityPoolTestSuite, removeEntityShouldReturnTrueIfEntityWasStored)
{
	addEntityToPool(ENTITY_ID_1);
	EXPECT_TRUE(removeEntityFromPool(ENTITY_ID_1));
	EXPECT_EQ(EMPTY, m_sut.size());
}

TEST_F(EntityPoolTestSuite, removeEntityShouldReturnFalseIfEntityWasNotStored)
{
	EXPECT_FALSE(m_sut.removeEntity(ENTITY_ID_1));
}

TEST_F(EntityPoolTestSuite, clearShouldRemoveAllData)
{
	addEntityToPool(ENTITY_ID_1);

	EXPECT_CALL(*m_idGuardMock, reset());

	m_sut.clear();

	EXPECT_FALSE(m_sut.hasId(ENTITY_ID_1));
	EXPECT_EQ(EMPTY, m_sut.size());
}