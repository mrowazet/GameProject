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
#include "ComponentAttacherMock.h"
#include "ComponentDetacherMock.h"

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

constexpr u32 ZERO_COMPONENTS = 0u;
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

	void connectComponent(ComponentBase& p_component)
	{
		expectConnectComponent(p_component);
		m_sut.connectComponentToEntity(ENTITY_ID, p_component.type);
	}

	void expectConnectComponent(ComponentBase& p_component)
	{
		EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));
		EXPECT_CALL(m_componentControllerMock, createComponent(p_component.type)).WillOnce(ReturnRef(p_component));
		EXPECT_CALL(m_changeDistributorMock, distributeEntityChange(ENTITY_ID));
	}

	void disconnectComponent(ComponentBase& p_component)
	{
		expectDisconnectComponent(p_component);
		m_sut.disconnectComponentFromEntity(ENTITY_ID, p_component.type);
	}

	void expectDisconnectComponent(ComponentBase& p_component)
	{
		EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));
		EXPECT_CALL(m_componentControllerMock, removeComponent(Ref(p_component))).WillOnce(Return(REMOVED));
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

	void checkComponentConnectedAsThird(ComponentBase& p_component)
	{
		checkComponentBitIsSet(p_component);
		EXPECT_EQ(m_entity.components->nextComponent->nextComponent, &p_component);
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
	testComponents::ComponentA COMPONENT_A;
	testComponents::ComponentB COMPONENT_B;
	testComponents::ComponentC COMPONENT_C;

	Entity m_entity;
	StrictMock<ComponentControllerMock> m_componentControllerMock;
	StrictMock<EntityChangeDistributorMock> m_changeDistributorMock;
	UniqueStrictMock<EntityPoolMock> m_entityPoolMock;
	UniqueStrictMock<ComponentAttacherMock> m_componentAttacherMock;
	UniqueStrictMock<ComponentDetacherMock> m_componentDetacherMock;

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

TEST_F(EntityControllerTestSuite, shouldCreateComponentWhenConnectComponentToIdToEntityCalled) //update needed
{
	expectConnectComponent(COMPONENT_A);
	EXPECT_TRUE(m_sut.connectComponentToEntity(ENTITY_ID, COMPONENT_A.type));
}

TEST_F(EntityControllerTestSuite, shouldNotCallComponentControllerIfMentionedComponentIsAlreadyConnectedWhenConnectComponentCalled) //skipped update?
{
	connectComponent(COMPONENT_A);

	EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));

	EXPECT_FALSE(m_sut.connectComponentToEntity(ENTITY_ID, COMPONENT_A.type));
}

TEST_F(EntityControllerTestSuite, shouldConfigureEntityWhenComponentIsAdding) //ok
{
	connectComponent(COMPONENT_A);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(EntityControllerTestSuite, shouldCorrectlyAddComponentToEntityIfAnotherOneIsAlreadyConnected) //ok
{
	connectComponent(COMPONENT_A);
	connectComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}

TEST_F(EntityControllerTestSuite, shouldRetrunFalseIfComponentIsAlreadyConnected) //ok
{
	connectComponent(COMPONENT_A);
	EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));

	EXPECT_FALSE(m_sut.connectComponentToEntity(ENTITY_ID, COMPONENT_A.type));
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(EntityControllerTestSuite, shouldReturnFalseIfComponentIsAlreadyDisconnected) //ok
{
	EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));

	EXPECT_FALSE(m_sut.disconnectComponentFromEntity(ENTITY_ID, COMPONENT_A.type));
	checkNumberOfConnectedComponents(ZERO_COMPONENTS);
}

TEST_F(EntityControllerTestSuite, shouldCorrectlyDisconnectComponentFromEntityWhenLastComponentRemoved) //ok
{
	connectComponent(COMPONENT_A);
	disconnectComponent(COMPONENT_A);
	checkNumberOfConnectedComponents(ZERO_COMPONENTS);
	EXPECT_THAT(m_entity.components, IsNull());
}

TEST_F(EntityControllerTestSuite, shouldCorrectlyDisconnectFirstComponentInTheMiddleFromEntity) //ok
{
	connectComponent(COMPONENT_A);
	connectComponent(COMPONENT_B);

	disconnectComponent(COMPONENT_A);

	checkComponentConnectedAsFirst(COMPONENT_B);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(EntityControllerTestSuite, shouldCorrectlyDisconnectComponentInTheMiddleFromEntity) //ok
{
	connectComponent(COMPONENT_A);
	connectComponent(COMPONENT_B);
	connectComponent(COMPONENT_C);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
	checkComponentConnectedAsThird(COMPONENT_C);

	disconnectComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_C);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}

TEST_F(EntityControllerTestSuite, shouldCorrectlyDisconnectLastComponentFromEntity) //ok
{
	connectComponent(COMPONENT_A);
	connectComponent(COMPONENT_B);

	disconnectComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
	EXPECT_THAT(COMPONENT_A.nextComponent, IsNull());
}
