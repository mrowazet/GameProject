#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "EntityController.h"
#include "IdGuardMock.h"
#include "EntityChangeDistributorMock.h"
#include "ComponentControllerMock.h"
#include "TestComponents.h"

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
	EntityControllerTestable(PoolSize p_maxNrOfEntities, IComponentController& p_componentController, const IEntityChangeDistributor& p_distributor)
		: EntityController(p_maxNrOfEntities, p_componentController, p_distributor)
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
		: m_entityController(NR_OF_ENTITIES, m_componentControllerMock, m_changeDistributorMock)
	{
	}

protected:
	ComponentControllerMock m_componentControllerMock;
	EntityChangeDistributorMock m_changeDistributorMock;
	EntityControllerTestable m_entityController;
};

TEST_F(EntityControllerTestSuite, createEntityShouldCreateElementInPool)
{
	auto& l_pool = m_entityController.getPool();
	auto& l_guardMock = m_entityController.getIdGuardMock();

	ASSERT_TRUE(l_pool.isEmpty());
	{
		InSequence seq;
		EXPECT_CALL(l_guardMock, getNextId()).WillOnce(Return(ENTITY_ID_1));
		EXPECT_CALL(l_guardMock, getNextId()).WillOnce(Return(ENTITY_ID_2));
	}

	m_entityController.createEntity();
	m_entityController.createEntity();

	EXPECT_THAT(TWO_ELEMENTS, l_pool.size());
}

TEST_F(EntityControllerTestSuite, createdEntitiesHaveUniqueIds)
{
	auto& l_guardMock = m_entityController.getIdGuardMock();

	{
		InSequence seq;
		EXPECT_CALL(l_guardMock, getNextId()).WillOnce(Return(ENTITY_ID_1));
		EXPECT_CALL(l_guardMock, getNextId()).WillOnce(Return(ENTITY_ID_2));
	}

	auto l_firstEntityId = m_entityController.createEntity();
	auto l_secondEntityId = m_entityController.createEntity();

	EXPECT_NE(UNDEFINED_ENTITY_ID, l_firstEntityId);
	EXPECT_NE(UNDEFINED_ENTITY_ID, l_secondEntityId);

	EXPECT_NE(l_firstEntityId, l_secondEntityId);
}

TEST_F(EntityControllerTestSuite, shouldReturnFalseAfterRemoveIfEntityWithGivenIdIsNotExist)
{
	EXPECT_FALSE(m_entityController.removeEntity(ENTITY_ID_1));
}

TEST_F(EntityControllerTestSuite, shouldReturnTrueIfEntityCorrectlyRemoved)
{
	auto& l_pool = m_entityController.getPool();
	auto& l_guardMock = m_entityController.getIdGuardMock();

	EXPECT_CALL(l_guardMock, getNextId()).WillOnce(Return(ENTITY_ID_1));
	EXPECT_CALL(l_guardMock, freeId(ENTITY_ID_1));

	auto l_entityId = m_entityController.createEntity();
	EXPECT_TRUE(m_entityController.removeEntity(l_entityId));
	
	ASSERT_TRUE(l_pool.isEmpty());
}

TEST_F(EntityControllerTestSuite, shouldWriteToStdcerrAfterGetIfEntityDoesNotExist)
{
	internal::CaptureStderr();
	m_entityController.getEntity(ENTITY_ID_1);

	EXPECT_THAT(internal::GetCapturedStderr(), Not(IsEmpty()));
}

TEST_F(EntityControllerTestSuite, shouldReturnRefToObjectIfExist)
{
	auto& l_pool = m_entityController.getPool();
	auto& l_guardMock = m_entityController.getIdGuardMock();

	EXPECT_CALL(l_guardMock, getNextId()).WillOnce(Return(ENTITY_ID_1));
	
	auto l_entityId = m_entityController.createEntity();
	auto& l_entity = m_entityController.getEntity(l_entityId);
	auto l_firstElementInPool = l_pool.begin();

	EXPECT_EQ(l_firstElementInPool->id, l_entity.id);
}
