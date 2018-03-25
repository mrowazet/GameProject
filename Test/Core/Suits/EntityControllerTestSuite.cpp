#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "EntityController.h"
#include "IdGuardMock.h"
#include "EntityChangeDistributorMock.h"
#include "ComponentControllerMock.h"
#include "TestComponents.h"
#include "EntityPoolMock.h"

using namespace testing;
using namespace engine;

namespace
{

const EntityId ENTITY_ID = 1u;
Entity ENTITY(ENTITY_ID);

}

class EntityControllerTestable : public EntityController
{
public:
	EntityControllerTestable(std::unique_ptr<IEntityPool> p_entityPool, IComponentController& p_componentController, const IEntityChangeDistributor& p_distributor)
		: EntityController(std::move(p_entityPool), p_componentController, p_distributor)
	{

	}

	EntityPoolMock& getPoolMock()
	{
		return static_cast<EntityPoolMock&>(*m_pool);
	}
};

class EntityControllerTestSuite : public Test
{
public:
	EntityControllerTestSuite()
		: m_entityController(std::make_unique<StrictMock<EntityPoolMock>>(), m_componentControllerMock, m_changeDistributorMock)
	{
	}

protected:
	ComponentControllerMock m_componentControllerMock;
	EntityChangeDistributorMock m_changeDistributorMock;
	EntityControllerTestable m_entityController;
};

TEST_F(EntityControllerTestSuite, createEntityShouldCreateElementInPoolAndReturnProperId)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, create()).WillOnce(ReturnRef(ENTITY));

	auto l_id = m_entityController.createEntity();
	EXPECT_EQ(ENTITY_ID, l_id);
}

TEST_F(EntityControllerTestSuite, removeEntityShouldRemoveElementFromPool)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, remove(ENTITY_ID)).WillOnce(Return(true));

	m_entityController.remove(ENTITY_ID);
}

TEST_F(EntityControllerTestSuite, getEntityShouldReturnRefToEntityFromPool)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, get(ENTITY_ID)).WillOnce(ReturnRef(ENTITY));
	EXPECT_CALL(l_poolMock, hasId(ENTITY_ID)).WillOnce(Return(true));

	auto& l_returnedEntity = m_entityController.get(ENTITY_ID);
	EXPECT_EQ(&ENTITY, &l_returnedEntity);
}

TEST_F(EntityControllerTestSuite, shouldWriteToStdcerrAfterGetIfEntityDoesNotExist)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, get(ENTITY_ID)).WillOnce(ReturnRef(ENTITY));
	EXPECT_CALL(l_poolMock, hasId(ENTITY_ID)).WillOnce(Return(false));

	internal::CaptureStderr();
	m_entityController.get(ENTITY_ID);

	EXPECT_THAT(internal::GetCapturedStderr(), Not(IsEmpty()));
}
