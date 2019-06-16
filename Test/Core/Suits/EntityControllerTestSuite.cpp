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
using namespace testComponents;

namespace
{
constexpr bool ATTACHED = true;
constexpr bool DETACHED = true;
constexpr bool REMOVED = false;
constexpr EntityId ENTITY_ID = 1u;
}

class EntityControllerTestSuite : public Test
{
public:
	EntityControllerTestSuite()
		: m_entity(ENTITY_ID),
		  m_sut(m_entityPoolMock.getPtr(), m_componentControllerMock.getPtr(), m_changeDistributorMock)
	{
	}

	void expectDistributeChange()
	{
		EXPECT_CALL(m_changeDistributorMock, distributeEntityChange(ENTITY_ID));
	}

	void expectCreateEntity()
	{
		EXPECT_CALL(*m_entityPoolMock, create()).WillOnce(ReturnRef(m_entity));
	}

	void expectGetEntityFromPool()
	{
		EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));
	}

	void expectAttachComponent(const bool p_result)
	{
		EXPECT_CALL(*m_componentControllerMock, attachComponent(Ref(m_entity), COMPONENT.type)).WillOnce(Return(p_result));
	}

	void expectDetachComponent(const bool p_result)
	{
		EXPECT_CALL(*m_componentControllerMock, detachComponent(Ref(m_entity), COMPONENT.type)).WillOnce(Return(p_result));
	}

	void expectAttachMultipleComponents(const bool p_result)
	{
		EXPECT_CALL(*m_componentControllerMock, attachMultipleComponents(Ref(m_entity), m_componentIndicators)).WillOnce(Return(p_result));
	}

	void expectDettachMultipleComponents(const bool p_result)
	{
		EXPECT_CALL(*m_componentControllerMock, detachMultipleComponents(Ref(m_entity), m_componentIndicators)).WillOnce(Return(p_result));
	}

	bool connectComponent()
	{
		return m_sut.connectComponentToEntity(ENTITY_ID, COMPONENT.type);
	}

	bool  disconnectComponent()
	{
		return m_sut.disconnectComponentFromEntity(ENTITY_ID, COMPONENT.type);
	}

	bool connectMultipleComponents()
	{
		return m_sut.connectMultipleComponentsToEntity(ENTITY_ID, m_componentIndicators);
	}

	bool disconnectMultipleComponents()
	{
		return m_sut.disconnectMultipleComponentsFromEntity(ENTITY_ID, m_componentIndicators);
	}

protected:
	testComponents::ComponentA COMPONENT;

	Entity m_entity;
	ComponentIndicators m_componentIndicators;

	StrictMock<EntityChangeDistributorMock> m_changeDistributorMock;
	UniqueStrictMock<EntityPoolMock> m_entityPoolMock;
	UniqueStrictMock<ComponentControllerMock> m_componentControllerMock;

	EntityController m_sut;
};

TEST_F(EntityControllerTestSuite, createEntityShouldCreateElementInPoolAndReturnProperId)
{
	expectCreateEntity();

	auto l_id = m_sut.createEntity();
	EXPECT_EQ(ENTITY_ID, l_id);
}

TEST_F(EntityControllerTestSuite, removeEnityWithMultipleComponentsShouldRemoveThemViaEntityControllerAndDistributeChange)
{
	expectGetEntityFromPool();
	expectDettachMultipleComponents(DETACHED);
	EXPECT_CALL(*m_entityPoolMock, removeEntity(ENTITY_ID)).WillOnce(Return(REMOVED));
	expectDistributeChange();

	m_sut.removeEntity(ENTITY_ID);
}

TEST_F(EntityControllerTestSuite, removeEnityWithMultipleComponentsShouldNotTriggerChangeDistributionIfNoComponentWasDetached)
{
	expectGetEntityFromPool();
	expectDettachMultipleComponents(not DETACHED);
	EXPECT_CALL(*m_entityPoolMock, removeEntity(ENTITY_ID)).WillOnce(Return(REMOVED));

	m_sut.removeEntity(ENTITY_ID);
}

TEST_F(EntityControllerTestSuite, getEntityShouldReturnRefToEntityFromPool)
{
	expectGetEntityFromPool();

	auto& l_returnedEntity = m_sut.getEntity(ENTITY_ID);
	EXPECT_EQ(&m_entity, &l_returnedEntity);
}

TEST_F(EntityControllerTestSuite, shouldCallPoolToCheckIfEntityWithIdExist)
{
	EXPECT_CALL(*m_entityPoolMock, hasEntity(ENTITY_ID));

	m_sut.hasEntity(ENTITY_ID);
}

TEST_F(EntityControllerTestSuite, shouldReturnTrueAndDistributeChangeIfComponentCorrectlyCreated) 
{
	expectGetEntityFromPool();
	expectAttachComponent(ATTACHED);
	expectDistributeChange();

	EXPECT_TRUE(connectComponent());
}

TEST_F(EntityControllerTestSuite, shouldReturnFalseIfComponentNotCorrectlyCreated)
{
	expectGetEntityFromPool();
	expectAttachComponent(not ATTACHED);

	EXPECT_FALSE(connectComponent());
}

TEST_F(EntityControllerTestSuite, shouldReturnTrueAndDistributeChangeIfComponentCorrectlyRemoved)
{
	expectGetEntityFromPool();
	expectDetachComponent(DETACHED);
	expectDistributeChange();

	EXPECT_TRUE(disconnectComponent());
}

TEST_F(EntityControllerTestSuite, shouldReturnFalseIfComponentNotCorrectlyRemoved)
{
	expectGetEntityFromPool();
	expectDetachComponent(not DETACHED);

	EXPECT_FALSE(disconnectComponent());
}

TEST_F(EntityControllerTestSuite, shouldReturnTrueAndDistributeChangeIfAtLeastOneComponentHasBeenConnectedToEntity)
{
	expectGetEntityFromPool();
	expectAttachMultipleComponents(ATTACHED);
	expectDistributeChange();

	EXPECT_TRUE(connectMultipleComponents());
}

TEST_F(EntityControllerTestSuite, shouldReturnFalseIfNoComponentHasNotBeenConnectedToEntity)
{
	expectGetEntityFromPool();
	expectAttachMultipleComponents(not ATTACHED);

	EXPECT_FALSE(connectMultipleComponents());
}

TEST_F(EntityControllerTestSuite, shouldReturnTrueAndDistributeChangeIfAtLeastOneComponentHasBeenDisconnectedFromEntity)
{
	expectGetEntityFromPool();
	expectDettachMultipleComponents(DETACHED);
	expectDistributeChange();

	EXPECT_TRUE(disconnectMultipleComponents());
}

TEST_F(EntityControllerTestSuite, shouldReturnFalseIfNoComponentHasNotBeenDisconnectedFromEntity)
{
	expectGetEntityFromPool();
	expectDettachMultipleComponents(not DETACHED);

	EXPECT_FALSE(disconnectMultipleComponents());
}

TEST_F(EntityControllerTestSuite, shouldCreateEnityWithConnectedComponentsAndDistributeChangeWhenFunctionCalled)
{
	expectCreateEntity();
	expectAttachMultipleComponents(ATTACHED);
	expectDistributeChange();

	m_sut.createEntityWithComponents(m_componentIndicators);
}

TEST_F(EntityControllerTestSuite, shouldCreateEnityFunctionCalledWithoutRequestedComponents)
{
	expectCreateEntity();
	expectAttachMultipleComponents(not ATTACHED);

	m_sut.createEntityWithComponents(m_componentIndicators);
}