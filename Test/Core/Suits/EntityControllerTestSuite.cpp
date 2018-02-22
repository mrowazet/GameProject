#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "EntityController.h"
#include "IdGuardMock.h"

using namespace testing;
using namespace engine;

namespace
{
const PoolSize NR_OF_ENTITIES = 15u;

const PoolSize ONE_ELEMENT = 1u;
const PoolSize TWO_ELEMENTS = 2u;

const EntityId ENTITY_ID_1 = 1u;
const EntityId ENTITY_ID_2 = 2u;

}

class EntityControllerTestable : public EntityController
{
public:
	EntityControllerTestable(PoolSize p_maxNrOfEntities)
		: EntityController(p_maxNrOfEntities)
	{
		m_idGuard = std::make_unique<IdGuardMock>();
	}

	ContinuousPool<Entity>& getPool()
	{
		return m_entities;
	}

	IdGuardMock& getIdGuardMock()
	{
		auto& l_mock = *m_idGuard;
		return dynamic_cast<IdGuardMock&>(l_mock);
	}
};


class EntityControllerTestSuite : public Test
{
public:
	EntityControllerTestSuite()
		: m_entityController(NR_OF_ENTITIES)
	{

	}

protected:
	EntityControllerTestable m_entityController;
};
/*
TEST_F(EntityControllerTestSuite, createEntityShouldCreateElementInPool)
{
	auto& l_pool = m_entityController.getPool();
	ASSERT_TRUE(l_pool.isEmpty());

	m_entityController.createEntity();
	m_entityController.createEntity();

	EXPECT_THAT(TWO_ELEMENTS, l_pool.size());
}

TEST_F(EntityControllerTestSuite, createdEntitiesHaveUniqueIds)
{
	auto& l_guardMock = m_entityController.getIdGuardMock();

	auto l_firstEntityId = m_entityController.createEntity();
	auto l_secondEntityId = m_entityController.createEntity();

	EXPECT_NE(UNDEFINED_ENTITY_ID, l_firstEntityId);
	EXPECT_NE(UNDEFINED_ENTITY_ID, l_secondEntityId);

	EXPECT_NE(l_firstEntityId, l_secondEntityId);
}

TEST_F(EntityControllerTestSuite, shouldReturnFalseIfEntityWithGivenIdIsNotExist)
{
	EXPECT_FALSE(m_entityController.removeEntity(ENTITY_ID));
}

TEST_F(EntityControllerTestSuite, shouldReturnTrueIfEntityCorrectlyRemoved)
{
	auto l_entityId = m_entityController.createEntity();
	EXPECT_TRUE(m_entityController.removeEntity(l_entityId));
}

TEST_F(EntityControllerTestSuite, shouldReturnUndefEntityIfDoesNotExist)
{
	auto& l_entity = m_entityController.getEntity(ENTITY_ID);
	EXPECT_EQ(UNDEFINED_ENTITY_ID, l_entity.id);
}

TEST_F(EntityControllerTestSuite, shouldReturnRefToObjectIfExist)
{
	auto& l_pool = m_entityController.getPool();
	
	auto l_entityId = m_entityController.createEntity();
	auto& l_entity = m_entityController.getEntity(l_entityId);

	auto l_firstElementInPool = l_pool.begin();

	EXPECT_EQ(l_firstElementInPool->id, l_entity.id);
}
*/