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
using namespace testComponents;

namespace
{
constexpr bool ATTACHED = true;
constexpr bool NOT_ATTACHED = false;
constexpr bool DETACHED = true;
constexpr bool NOT_DETACHED = false;
constexpr bool REMOVED = false;
constexpr EntityId ENTITY_ID = 1u;
}

class EntityControllerTestSuite : public Test
{
public:
	EntityControllerTestSuite()
		: m_entity(ENTITY_ID),
		  m_sut(m_entityPoolMock.getPtr(), m_componentAttacherMock.getPtr(), m_componentDetacherMock.getPtr(), m_changeDistributorMock)
	{
	}

	void expectDistributeChange()
	{
		EXPECT_CALL(m_changeDistributorMock, distributeEntityChange(ENTITY_ID));
	}

	void expectGetEntityFromPool()
	{
		EXPECT_CALL(*m_entityPoolMock, getEntity(ENTITY_ID)).WillOnce(ReturnRef(m_entity));
	}

	void expectAttachComponent(const bool p_result)
	{
		EXPECT_CALL(*m_componentAttacherMock, attachComponent(Ref(m_entity), COMPONENT.type)).WillOnce(Return(p_result));
	}

	void expectDetachComponent(const bool p_result)
	{
		EXPECT_CALL(*m_componentDetacherMock, detachComponent(Ref(m_entity), COMPONENT.type)).WillOnce(Return(p_result));
	}

	bool connectComponent()
	{
		return m_sut.connectComponentToEntity(ENTITY_ID, COMPONENT.type);
	}

	bool  disconnectComponent()
	{
		return m_sut.disconnectComponentFromEntity(ENTITY_ID, COMPONENT.type);
	}

protected:
	testComponents::ComponentA COMPONENT;

	Entity m_entity;
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
	expectGetEntityFromPool();

	auto& l_returnedEntity = m_sut.getEntity(ENTITY_ID);
	EXPECT_EQ(&m_entity, &l_returnedEntity);
}

TEST_F(EntityControllerTestSuite, shouldCallPoolToCheckIfEntityWithIdExist)
{
	EXPECT_CALL(*m_entityPoolMock, hasId(ENTITY_ID));

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
	expectAttachComponent(NOT_ATTACHED);

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
	expectDetachComponent(NOT_DETACHED);

	EXPECT_FALSE(disconnectComponent());
}


