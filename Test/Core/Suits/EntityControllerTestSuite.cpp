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
#include "UniquePtrMockWrapper.h"

using namespace testing;
using namespace testTool;
using namespace engine;

namespace
{

constexpr bool ID_EXIST = true;
constexpr bool ID_NOT_EXIST = false;
constexpr bool REMOVED = true;
constexpr bool COMPONENT_CONNECTED = true;

constexpr EntityId ENTITY_ID = 1u;
testComponents::ComponentA COMPONENT_A;
testComponents::ComponentB COMPONENT_B;

constexpr u32 ONE_COMPONENT = 1u;
constexpr u32 TWO_COMPONENTS = 2u;
}


class EntityControllerTestSuite : public Test
{
public:
	EntityControllerTestSuite()
		: m_entity(ENTITY_ID),
		  m_sut(m_entityPoolMock.getPtr(), m_componentControllerMock, m_changeDistributorMock)
	{
	}

	void SetUp() override
	{
		EXPECT_FALSE(m_entity.attachedComponents.hasAny());
		EXPECT_THAT(m_entity.components, IsNull());
	}

	void addComponent(ComponentBase& p_component)
	{
		expectAddComponent(p_component);
		m_sut.connectSingleComponentToEntity(ENTITY_ID, p_component.type);
	}

	void expectAddComponent(ComponentBase& p_component)
	{
		EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));
		EXPECT_CALL(m_componentControllerMock, createComponent(p_component.type)).WillOnce(ReturnRef(p_component));
		EXPECT_CALL(m_changeDistributorMock, distributeEntityChange(ENTITY_ID));
	}

	void checkComponentConnectedAsFirst(ComponentBase& p_component)
	{
		checkComponentBitIsSet(p_component);
		EXPECT_EQ(m_entity.components, &p_component);
	}

	void checkComponentConnectedAsSecond(ComponentBase& p_component)
	{
		checkComponentBitIsSet(p_component);
		EXPECT_EQ(m_entity.components->nextComponent, &p_component);
	}

	void checkComponentBitIsSet(ComponentBase& p_component)
	{
		EXPECT_TRUE(m_entity.attachedComponents.isAttached(p_component.type));
	}

	void checkNumberOfConnectedComponents(u32 p_nrOfComponents)
	{
		EXPECT_EQ(p_nrOfComponents, m_entity.attachedComponents.getNumOfSetComponents());
	}

protected:
	Entity m_entity;
	StrictMock<ComponentControllerMock> m_componentControllerMock;
	StrictMock<EntityChangeDistributorMock> m_changeDistributorMock;
	UniquePtrMockWrapper<StrictMock<EntityPoolMock>> m_entityPoolMock;

	EntityController m_sut;
};

TEST_F(EntityControllerTestSuite, createEntityShouldCreateElementInPoolAndReturnProperId)
{
	EXPECT_CALL(*m_entityPoolMock, create()).WillOnce(ReturnRef(m_entity));

	auto l_id = m_sut.createEntity();
	EXPECT_EQ(ENTITY_ID, l_id);
}

TEST_F(EntityControllerTestSuite, removeEntityShouldRemoveElementFromPool)
{
	EXPECT_CALL(*m_entityPoolMock, removeEntity(ENTITY_ID)).WillOnce(Return(REMOVED));

	m_sut.removeEntity(ENTITY_ID);
}

TEST_F(EntityControllerTestSuite, getEntityShouldReturnRefToEntityFromPool)
{
	EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));

	auto& l_returnedEntity = m_sut.getEntity(ENTITY_ID);
	EXPECT_EQ(&m_entity, &l_returnedEntity);
}

TEST_F(EntityControllerTestSuite, shouldCallPoolToCheckIfEntityWithIdExist)
{
	EXPECT_CALL(*m_entityPoolMock, hasId(ENTITY_ID));

	m_sut.hasEntity(ENTITY_ID);
}

TEST_F(EntityControllerTestSuite, shouldCreateComponentWhenConnectComponentToIdToEntityCalled)
{
	expectAddComponent(COMPONENT_A);
	EXPECT_TRUE(m_sut.connectSingleComponentToEntity(ENTITY_ID, COMPONENT_A.type));
}

TEST_F(EntityControllerTestSuite, shouldNotCallComponentControllerIfMentionedComponentIsAlreadyConnectedWhenConnectComponentCalled)
{
	addComponent(COMPONENT_A);

	EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));

	EXPECT_FALSE(m_sut.connectSingleComponentToEntity(ENTITY_ID, COMPONENT_A.type));
}

TEST_F(EntityControllerTestSuite, shouldConfigureEntityWhenComponentIsAdding)
{
	addComponent(COMPONENT_A);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(EntityControllerTestSuite, shouldCorrectlyAddComponentToEntityIfAnotherOneIsAlreadyConnected)
{
	addComponent(COMPONENT_A);
	addComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}
