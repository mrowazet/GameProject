#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "EntityController.h"
#include "IdGuardMock.h"
#include "EntityChangeDistributorMock.h"
#include "ComponentControllerMock.h"
#include "TestComponents.h"
#include "EntityPoolMock.h"
#include "ComponentTypes.h"

using namespace testing;
using namespace engine;

namespace
{

constexpr bool ID_EXIST = true;
constexpr bool ID_NOT_EXIST = false;
constexpr bool REMOVED = true;
constexpr bool COMPONENT_CONNECTED = true;

constexpr ComponentType COMPONENT_TYPE = ComponentType::VISIBLE;
constexpr EntityId ENTITY_ID = 1u;
testComponents::ComponentA COMPONENT_A;

constexpr u32 ZERO_COMPONENTS = 0u;
constexpr u32 ONE_COMPONENT = 1u;
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
		: m_entity(ENTITY_ID),
		m_entityController(std::make_unique<StrictMock<EntityPoolMock>>(), m_componentControllerMock, m_changeDistributorMock)
	{
	}

protected:
	Entity m_entity;
	ComponentControllerMock m_componentControllerMock;
	EntityChangeDistributorMock m_changeDistributorMock;
	EntityControllerTestable m_entityController;
};

TEST_F(EntityControllerTestSuite, createEntityShouldCreateElementInPoolAndReturnProperId)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, create()).WillOnce(ReturnRef(m_entity));

	auto l_id = m_entityController.createEntity();
	EXPECT_EQ(ENTITY_ID, l_id);
}

TEST_F(EntityControllerTestSuite, removeEntityShouldRemoveElementFromPool)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, removeEntity(ENTITY_ID)).WillOnce(Return(REMOVED));

	m_entityController.removeEntity(ENTITY_ID);
}

TEST_F(EntityControllerTestSuite, getEntityShouldReturnRefToEntityFromPool)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));

	auto& l_returnedEntity = m_entityController.getEntity(ENTITY_ID);
	EXPECT_EQ(&m_entity, &l_returnedEntity);
}

TEST_F(EntityControllerTestSuite, shouldCallPoolToCheckIfEntityWithIdExist)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, hasId(ENTITY_ID));

	m_entityController.hasEntity(ENTITY_ID);
}

TEST_F(EntityControllerTestSuite, shouldCreateComponentWhenConnectComponentToIdToEntityCalled)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));
	EXPECT_CALL(m_componentControllerMock, createComponent(COMPONENT_TYPE)).WillOnce(ReturnRef(COMPONENT_A));

	auto l_result = m_entityController.connectSingleComponentToEntity(ENTITY_ID, COMPONENT_TYPE);
	EXPECT_TRUE(l_result);
}

TEST_F(EntityControllerTestSuite, shouldNotCallComponentControllerIfMentionedComponentIsAlreadyConnectedWhenConnectComponentCalled)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));
	EXPECT_CALL(m_componentControllerMock, createComponent(COMPONENT_TYPE)).Times(0);

	m_entity.attachedComponents.flip(static_cast<int>(COMPONENT_A.type));

	auto l_result = m_entityController.connectSingleComponentToEntity(ENTITY_ID, COMPONENT_TYPE);
	EXPECT_FALSE(l_result);
}

TEST_F(EntityControllerTestSuite, shouldConfigureEntityWhenComponentIsAdding)
{
	auto& l_poolMock = m_entityController.getPoolMock();
	EXPECT_CALL(l_poolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));
	EXPECT_CALL(m_componentControllerMock, createComponent(COMPONENT_TYPE)).WillOnce(ReturnRef(COMPONENT_A));
	EXPECT_EQ(ZERO_COMPONENTS, m_entity.attachedComponents.count());

	auto l_result = m_entityController.connectSingleComponentToEntity(ENTITY_ID, COMPONENT_TYPE);
	EXPECT_TRUE(l_result);

	EXPECT_THAT(m_entity.components, NotNull());
	EXPECT_EQ(m_entity.components, &COMPONENT_A);
	EXPECT_EQ(ONE_COMPONENT, m_entity.attachedComponents.count());

	auto l_componentBitNumber = static_cast<int>(COMPONENT_A.type);
	EXPECT_EQ(COMPONENT_CONNECTED, m_entity.attachedComponents.test(l_componentBitNumber));
}
