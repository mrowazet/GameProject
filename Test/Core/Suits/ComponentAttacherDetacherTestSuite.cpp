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

namespace
{

constexpr bool REMOVED = true;
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

	void checkComponentConnectedAsFirst(ComponentBase& p_component)
	{
		EXPECT_TRUE(isComponentBitIsSet(p_component));
		EXPECT_EQ(m_entity.components, &p_component);
	}

	void checkComponentConnectedAsSecond(ComponentBase& p_component)
	{
		EXPECT_TRUE(isComponentBitIsSet(p_component));
		EXPECT_EQ(m_entity.components->nextComponent, &p_component);
	}

	void checkComponentConnectedAsThird(ComponentBase& p_component)
	{
		EXPECT_TRUE(isComponentBitIsSet(p_component));
		EXPECT_EQ(m_entity.components->nextComponent->nextComponent, &p_component);
	}

	bool isComponentBitIsSet(ComponentBase& p_component)
	{
		return m_entity.attachedComponents.isSet(p_component.type);
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

	void expectCreateComponent(ComponentBase& p_component)
	{
		EXPECT_CALL(m_componentControllerMock, createComponent(p_component.type)).WillOnce(ReturnRef(p_component));
	}

	void attachComponent(ComponentBase& p_component)
	{
		expectCreateComponent(p_component);
		m_sut.attachComponent(m_entity, p_component.type);
	}

protected:
	ComponentAttacher m_sut{ m_componentControllerMock };

};

TEST_F(ComponentAttacherTestSuite, shouldCallCreateComponentAndReturnTrueWhenAttachingComponent)
{
	expectCreateComponent(COMPONENT_A);
	EXPECT_TRUE(m_sut.attachComponent(m_entity, COMPONENT_A.type));
}

TEST_F(ComponentAttacherTestSuite, shouldConfigureEntityWhenComponentIsAttaching)
{
	attachComponent(COMPONENT_A);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(ComponentAttacherTestSuite, shouldConfigureEntityCorrectlyWhenComponentIsAttachingWhenAnotherComponentIsAlreadyCreated)
{
	attachComponent(COMPONENT_A);
	attachComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}


TEST_F(ComponentAttacherTestSuite, shouldRetrunFalseIfComponentIsAlreadyAttached)
{
	attachComponent(COMPONENT_A);

	EXPECT_FALSE(m_sut.attachComponent(m_entity, COMPONENT_A.type));
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(ComponentAttacherTestSuite, shouldReturnFalseIfNoComponentsToAttach)
{
	ComponentIndicators l_emptyFlags;
	EXPECT_FALSE(m_sut.attachMultipleComponents(m_entity, l_emptyFlags));
}

TEST_F(ComponentAttacherTestSuite, shouldReturnFalseIfAllRequestedComponentsAreAlreadyAttached)
{
	//todo
}

TEST_F(ComponentAttacherTestSuite, shouldReturnTrueIfAtLeastOneComponentHasBeenAttached)
{
	//todo
}

class ComponentDetacherTestSuite : public ComponentAttacherDetacherBaseTestSuite
{
public:
	void SetUp() override
	{
		ComponentAttacherDetacherBaseTestSuite::SetUp();
	}

	void addOneComponent()
	{
		addComponentToEntity(COMPONENT_A);
		checkComponentConnectedAsFirst(COMPONENT_A);
	}

	void addTwoComponents()
	{
		addComponentToEntity(COMPONENT_A);
		checkComponentConnectedAsFirst(COMPONENT_A);

		addComponentToComponentA(COMPONENT_B);
		checkComponentConnectedAsSecond(COMPONENT_B);
	}

	void addThreeComponents()
	{
		addComponentToEntity(COMPONENT_A);
		checkComponentConnectedAsFirst(COMPONENT_A);

		addComponentToComponentA(COMPONENT_B);
		checkComponentConnectedAsSecond(COMPONENT_B);

		addComponentToComponentB(COMPONENT_C);
		checkComponentConnectedAsThird(COMPONENT_C);
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

	void expectRemoveComponent(ComponentBase& p_component)
	{
		EXPECT_CALL(m_componentControllerMock, removeComponent(Ref(p_component))).WillOnce(Return(REMOVED));
	}

	void detachComponent(ComponentBase& p_component)
	{
		expectRemoveComponent(p_component);
		m_sut.detachComponent(m_entity, p_component.type);
		EXPECT_FALSE(isComponentBitIsSet(p_component));
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
	addOneComponent();

	detachComponent(COMPONENT_A);

	checkNumberOfConnectedComponents(ZERO_COMPONENTS);
	EXPECT_THAT(m_entity.components, IsNull());
}

TEST_F(ComponentDetacherTestSuite, shouldCorrectlyDisconnectFirstComponentInTheMiddleFromEntity)
{
	addTwoComponents();

	detachComponent(COMPONENT_A);

	checkComponentConnectedAsFirst(COMPONENT_B);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(ComponentDetacherTestSuite, shouldCorrectlyDetachComponentInTheMiddleFromEntity)
{
	addThreeComponents();

	detachComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_C);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}

TEST_F(ComponentDetacherTestSuite, shouldCorrectlyDetachLastComponentFromEntity)
{
	addTwoComponents();

	detachComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
	EXPECT_THAT(COMPONENT_A.nextComponent, IsNull());
}