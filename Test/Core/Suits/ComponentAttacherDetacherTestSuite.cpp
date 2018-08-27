#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "ComponentAttacher.h"
#include "ComponentDetacher.h"
#include "ComponentControllerMock.h"
#include "TestComponents.h"

using namespace testing;
using namespace engine;
using namespace testComponents;

namespace //check for unused!
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

class ComponentAttacherDetacherBaseTestSuite : public Test
{
public:
	void SetUp() override
	{
		EXPECT_FALSE(m_entity.attachedComponents.hasAny());
		EXPECT_THAT(m_entity.components, IsNull());
	}

	void expectCreateComponent(ComponentBase& p_component)
	{
		EXPECT_CALL(m_componentControllerMock, createComponent(p_component.type)).WillOnce(ReturnRef(p_component));
	}

	void expectRemoveComponent(ComponentBase& p_component)
	{
		EXPECT_CALL(m_componentControllerMock, removeComponent(Ref(p_component))).WillOnce(Return(REMOVED));
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
	ComponentA COMPONENT_A;
	ComponentB COMPONENT_B;
	ComponentC COMPONENT_C;

	Entity m_entity;
	ComponentControllerMock m_componentControllerMock;
};

class ComponentAttacherTestSuite : public ComponentAttacherDetacherBaseTestSuite
{
public:
	void SetUp() override
	{
		ComponentAttacherDetacherBaseTestSuite::SetUp();
	}

protected:
	ComponentAttacher m_sut{ m_componentControllerMock };
};

TEST_F(ComponentAttacherTestSuite, shouldCallCreateComponentAndReturnTrueWhenAttachingComponent)
{
	expectCreateComponent(COMPONENT_A);
	m_sut.attachComponent(m_entity, COMPONENT_A.type);
}

TEST_F(ComponentAttacherTestSuite, shouldConfigureEntityWhenComponentIsAttaching)
{
	expectCreateComponent(COMPONENT_A);
	m_sut.attachComponent(m_entity, COMPONENT_A.type);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(ComponentAttacherTestSuite, shouldConfigureEntityCorrectlyWhenComponentIsAttachingWhenAnotherComponentIsAlreadyCreated)
{
	expectCreateComponent(COMPONENT_A);
	m_sut.attachComponent(m_entity, COMPONENT_A.type);

	expectCreateComponent(COMPONENT_B);
	m_sut.attachComponent(m_entity, COMPONENT_B.type);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}


TEST_F(ComponentAttacherTestSuite, shouldRetrunFalseIfComponentIsAlreadyAttached)
{
	expectCreateComponent(COMPONENT_A);
	m_sut.attachComponent(m_entity, COMPONENT_A.type);

	EXPECT_FALSE(m_sut.attachComponent(m_entity, COMPONENT_A.type));
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

class ComponentDetacherTestSuite : public ComponentAttacherDetacherBaseTestSuite
{
public:
	void SetUp() override
	{
		ComponentAttacherDetacherBaseTestSuite::SetUp();
	}

	void addComponentToEntity(ComponentBase& p_component)
	{
		m_entity.components = &p_component;
		m_entity.attachedComponents.flip(p_component.type);
	}

	void addComponentToComponentA(ComponentBase& p_component)
	{
		COMPONENT_A.nextComponent = &p_component;
		m_entity.attachedComponents.flip(p_component.type);
	}

	void addComponentToComponentB(ComponentBase& p_component)
	{
		COMPONENT_B.nextComponent = &p_component;
		m_entity.attachedComponents.flip(p_component.type);
	}

protected:
	ComponentDetacher m_sut{ m_componentControllerMock };
};

TEST_F(ComponentDetacherTestSuite, shouldReturnFalseIfComponentIsAlreadyDetached)
{
	EXPECT_FALSE(m_sut.detachComponent(m_entity, COMPONENT_A.type));
	checkNumberOfConnectedComponents(ZERO_COMPONENTS);
}

TEST_F(ComponentDetacherTestSuite, shouldCorrectlyDetachComponentFromEntityWhenLastComponentRemoved)
{
	addComponentToEntity(COMPONENT_A);

	expectRemoveComponent(COMPONENT_A);
	m_sut.detachComponent(m_entity, COMPONENT_A.type);

	checkNumberOfConnectedComponents(ZERO_COMPONENTS);
	EXPECT_THAT(m_entity.components, IsNull());
}

TEST_F(ComponentDetacherTestSuite, shouldCorrectlyDisconnectFirstComponentInTheMiddleFromEntity)
{
	addComponentToEntity(COMPONENT_A);
	addComponentToComponentA(COMPONENT_B);

	expectRemoveComponent(COMPONENT_A);
	m_sut.detachComponent(m_entity, COMPONENT_A.type);

	checkComponentConnectedAsFirst(COMPONENT_B);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(ComponentDetacherTestSuite, shouldCorrectlyDetachComponentInTheMiddleFromEntity)
{
	addComponentToEntity(COMPONENT_A);
	addComponentToComponentA(COMPONENT_B);
	addComponentToComponentB(COMPONENT_C);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
	checkComponentConnectedAsThird(COMPONENT_C);

	expectRemoveComponent(COMPONENT_B);
	m_sut.detachComponent(m_entity, COMPONENT_B.type);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_C);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}

TEST_F(ComponentDetacherTestSuite, shouldCorrectlyDetachLastComponentFromEntity)
{
	addComponentToEntity(COMPONENT_A);
	addComponentToComponentA(COMPONENT_B);

	expectRemoveComponent(COMPONENT_B);
	m_sut.detachComponent(m_entity, COMPONENT_B.type);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
	EXPECT_THAT(COMPONENT_A.nextComponent, IsNull());
}