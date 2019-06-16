#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "ComponentController.h"
#include "TestComponents.h"
#include "ComponentProviderMock.h"
#include "UniquePtrMockWrapper.h"

using namespace testing;
using namespace testTool;
using namespace engine;
using namespace testComponents;

namespace
{

constexpr bool REMOVED = true;
constexpr u32 ZERO_COMPONENTS = 0u;
constexpr u32 ONE_COMPONENT = 1u;
constexpr u32 TWO_COMPONENTS = 2u;
constexpr u32 THREE_COMPONENTS = 3u;

}

class ComponentControllerTestSuite : public Test
{
public:
	ComponentControllerTestSuite()
		: m_sut(m_componentProviderMock.getPtr())
	{

	}

	void SetUp() override
	{
		EXPECT_FALSE(m_entity.attachedComponents.any());
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

	void expectCreateComponent(ComponentBase& p_component)
	{
		EXPECT_CALL(*m_componentProviderMock, createComponent(p_component.type)).WillOnce(ReturnRef(p_component));
	}

	void attachComponent(ComponentBase& p_component)
	{
		expectCreateComponent(p_component);
		m_sut.attachComponent(m_entity, p_component.type);
	}

	void addOneComponent()
	{
		addComponentToEntity(COMPONENT_A);
		checkComponentConnectedAsFirst(COMPONENT_A);
	}

	void addTwoComponents()
	{
		addOneComponent();

		addComponentToComponentA(COMPONENT_B);
		checkComponentConnectedAsSecond(COMPONENT_B);
	}

	void addThreeComponents()
	{
		addTwoComponents();

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
		EXPECT_CALL(*m_componentProviderMock, removeComponent(Ref(p_component))).WillOnce(Return(REMOVED));
	}

	void detachComponent(ComponentBase& p_component)
	{
		expectRemoveComponent(p_component);
		m_sut.detachComponent(m_entity, p_component.type);
		EXPECT_FALSE(isComponentBitIsSet(p_component));
	}

	bool attachMultipleComponents(const ComponentIndicators& p_componentsToAttach)
	{
		return m_sut.attachMultipleComponents(m_entity, p_componentsToAttach);
	}

	bool detachMultipleComponents(const ComponentIndicators& p_componentsToDetach)
	{
		return m_sut.detachMultipleComponents(m_entity, p_componentsToDetach);
	}

	ComponentIndicators createIndicatorsWithThreeComponents()
	{
		auto l_componentsToProcess = createIndicatorsWithTwoComponents();
		l_componentsToProcess.flip(COMPONENT_C.type);

		return l_componentsToProcess;
	}

	ComponentIndicators createIndicatorsWithTwoComponents()
	{
		auto l_componentsToProcess = createIndicatorsWithOneComponent();
		l_componentsToProcess.flip(COMPONENT_B.type);

		return l_componentsToProcess;
	}

	ComponentIndicators createIndicatorsWithOneComponent()
	{
		ComponentIndicators l_componentsToProcess;
		l_componentsToProcess.flip(COMPONENT_A.type);

		return l_componentsToProcess;
	}

protected:
	ComponentA COMPONENT_A;
	ComponentB COMPONENT_B;
	ComponentC COMPONENT_C;

	Entity m_entity;
	UniqueStrictMock<ComponentProviderMock> m_componentProviderMock;
	
	ComponentController m_sut;
};

TEST_F(ComponentControllerTestSuite, shouldCallCreateComponentAndReturnTrueWhenAttachingComponent)
{
	expectCreateComponent(COMPONENT_A);
	EXPECT_TRUE(m_sut.attachComponent(m_entity, COMPONENT_A.type));
}

TEST_F(ComponentControllerTestSuite, shouldConfigureEntityWhenComponentIsAttaching)
{
	attachComponent(COMPONENT_A);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(ComponentControllerTestSuite, shouldConfigureEntityCorrectlyWhenComponentIsAttachingWhenAnotherComponentIsAlreadyCreated)
{
	attachComponent(COMPONENT_A);
	attachComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}

TEST_F(ComponentControllerTestSuite, shouldRetrunFalseIfComponentIsAlreadyAttached)
{
	attachComponent(COMPONENT_A);

	EXPECT_FALSE(m_sut.attachComponent(m_entity, COMPONENT_A.type));
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(ComponentControllerTestSuite, shouldReturnFalseIfComponentIsAlreadyDetached)
{
	EXPECT_FALSE(m_sut.detachComponent(m_entity, COMPONENT_A.type));
	checkNumberOfConnectedComponents(ZERO_COMPONENTS);
}

TEST_F(ComponentControllerTestSuite, shouldCorrectlyDetachComponentFromEntityWhenLastComponentRemoved)
{
	addOneComponent();

	detachComponent(COMPONENT_A);

	checkNumberOfConnectedComponents(ZERO_COMPONENTS);
	EXPECT_THAT(m_entity.components, IsNull());
}

TEST_F(ComponentControllerTestSuite, shouldCorrectlyDetachFirstComponentFromEntity)
{
	addTwoComponents();

	detachComponent(COMPONENT_A);

	checkComponentConnectedAsFirst(COMPONENT_B);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
}

TEST_F(ComponentControllerTestSuite, shouldCorrectlyDetachComponentInTheMiddleFromEntity)
{
	addThreeComponents();

	detachComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_C);
	checkNumberOfConnectedComponents(TWO_COMPONENTS);
}

TEST_F(ComponentControllerTestSuite, shouldCorrectlyDetachLastComponentFromEntity)
{
	addTwoComponents();

	detachComponent(COMPONENT_B);

	checkComponentConnectedAsFirst(COMPONENT_A);
	checkNumberOfConnectedComponents(ONE_COMPONENT);
	EXPECT_THAT(COMPONENT_A.nextComponent, IsNull());
}

TEST_F(ComponentControllerTestSuite, shouldReturnFalseIfRequestedComponentIndicatorsAreEmpty)
{
	EXPECT_FALSE(m_sut.attachMultipleComponents(m_entity, ComponentIndicators()));
}

TEST_F(ComponentControllerTestSuite, shouldReturnFalseIfAllRequestedComponentsAreAlreadyAttached)
{
	addTwoComponents();
	EXPECT_FALSE(attachMultipleComponents(createIndicatorsWithTwoComponents()));
}

TEST_F(ComponentControllerTestSuite, shouldAttachRequestedComponentsToEmptyEntity)
{
	expectCreateComponent(COMPONENT_A);
	expectCreateComponent(COMPONENT_B);

	EXPECT_TRUE(attachMultipleComponents(createIndicatorsWithTwoComponents()));

	checkNumberOfConnectedComponents(TWO_COMPONENTS);
	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
}

TEST_F(ComponentControllerTestSuite, shouldAttachSingleRequestedComponentsToEntityWhichIsNotEmpty)
{
	addOneComponent();
	expectCreateComponent(COMPONENT_B);

	EXPECT_TRUE(attachMultipleComponents(createIndicatorsWithTwoComponents()));

	checkNumberOfConnectedComponents(TWO_COMPONENTS);
	checkComponentConnectedAsSecond(COMPONENT_B);
}

TEST_F(ComponentControllerTestSuite, shouldAttachAllRequestedComponentsToEntityWhichIsNotEmpty)
{
	addOneComponent();
	expectCreateComponent(COMPONENT_B);
	expectCreateComponent(COMPONENT_C);

	EXPECT_TRUE(attachMultipleComponents(createIndicatorsWithThreeComponents()));

	checkNumberOfConnectedComponents(THREE_COMPONENTS);
	checkComponentConnectedAsFirst(COMPONENT_A);
	checkComponentConnectedAsSecond(COMPONENT_B);
	checkComponentConnectedAsThird(COMPONENT_C);
}

TEST_F(ComponentControllerTestSuite, shouldRetrunFalseIfEntityIsEmpty)
{
	EXPECT_FALSE(detachMultipleComponents(createIndicatorsWithTwoComponents()));
}

TEST_F(ComponentControllerTestSuite, shouldRetrunFalseIfNothingToDetach)
{
	addComponentToEntity(COMPONENT_B);
	EXPECT_FALSE(detachMultipleComponents(createIndicatorsWithOneComponent()));
}

TEST_F(ComponentControllerTestSuite, shouldDetachSingleRequestedComponentFromEntityWithOneComponent)
{
	addTwoComponents();
	expectRemoveComponent(COMPONENT_A);

	EXPECT_TRUE(detachMultipleComponents(createIndicatorsWithOneComponent()));

	checkNumberOfConnectedComponents(ONE_COMPONENT);
	checkComponentConnectedAsFirst(COMPONENT_B);
}

TEST_F(ComponentControllerTestSuite, shouldDetachRequestedComponentsFromEntity)
{
	addThreeComponents();
	expectRemoveComponent(COMPONENT_A);
	expectRemoveComponent(COMPONENT_B);

	EXPECT_TRUE(detachMultipleComponents(createIndicatorsWithTwoComponents()));
	checkNumberOfConnectedComponents(ONE_COMPONENT);
	checkComponentConnectedAsFirst(COMPONENT_C);
}

TEST_F(ComponentControllerTestSuite, shouldDetachAllComponentsConnectedToEntity)
{
	addTwoComponents();
	expectRemoveComponent(COMPONENT_A);
	expectRemoveComponent(COMPONENT_B);

	EXPECT_TRUE(detachMultipleComponents(createIndicatorsWithThreeComponents()));
	checkNumberOfConnectedComponents(ZERO_COMPONENTS);
	EXPECT_THAT(m_entity.components, IsNull());
}