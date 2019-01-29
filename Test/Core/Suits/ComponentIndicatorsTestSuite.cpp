#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"

using namespace testing;
using namespace engine;

namespace
{

const u32 TWO_COMPONENTS = 2u;
const bool SET = true;
const ComponentType COMPONENT_1 = ComponentType::POSITION;
const ComponentType COMPONENT_2 = ComponentType::MOVABLE;
const ComponentType COMPONENT_3 = ComponentType::VISIBLE;
const ComponentType COMPONENT_4 = ComponentType::WITH_SI;

}
class ComponentIndicatorsTestSuite : public Test
{
public:
	ComponentIndicatorsTestSuite() = default;

protected:
	ComponentIndicators m_sut;
};

TEST_F(ComponentIndicatorsTestSuite, newAttachedComponentsShouldBeEmpty)
{
	EXPECT_FALSE(m_sut.any());
	EXPECT_TRUE(m_sut.none());
}

TEST_F(ComponentIndicatorsTestSuite, shouldReturnNumberOfAttachedComponentsWhenFunctionCalled)
{
	m_sut.flip(COMPONENT_1);
	m_sut.flip(COMPONENT_2);

	EXPECT_EQ(TWO_COMPONENTS, m_sut.getNumOfSetComponents());
}

TEST_F(ComponentIndicatorsTestSuite, shouldReplaceStoredFlagsByNewWhenAssignOperatorIsCalled)
{
	ComponentIndicators l_componentIndicators;
	l_componentIndicators.flip(COMPONENT_1);

	m_sut = l_componentIndicators;

	EXPECT_EQ(m_sut.getNumOfSetComponents(), l_componentIndicators.getNumOfSetComponents());
	EXPECT_TRUE(m_sut == l_componentIndicators);
}

TEST_F(ComponentIndicatorsTestSuite, flipShouldSetAndUnsetProperComponent)
{
	m_sut.flip(COMPONENT_1);
	EXPECT_TRUE(m_sut.isSet(COMPONENT_1));
	m_sut.flip(COMPONENT_1);
	EXPECT_FALSE(m_sut.isSet(COMPONENT_1));
}

TEST_F(ComponentIndicatorsTestSuite, compareOperatorsShouldReturnProperResult)
{
	ComponentIndicators l_componentIndicators;

	EXPECT_TRUE(m_sut == l_componentIndicators);
	EXPECT_FALSE(m_sut != l_componentIndicators);

	m_sut.flip(COMPONENT_1);

	EXPECT_FALSE(m_sut == l_componentIndicators);
	EXPECT_TRUE(m_sut != l_componentIndicators);
}

TEST_F(ComponentIndicatorsTestSuite, resetShouldClearAllIndicators)
{
	m_sut.flip(COMPONENT_1);
	m_sut.flip(COMPONENT_2);

	m_sut.reset();
	EXPECT_TRUE(m_sut.none());
}

TEST_F(ComponentIndicatorsTestSuite, setShouldSetProperValueForComponent)
{
	m_sut.set(COMPONENT_1, not SET);
	EXPECT_FALSE(m_sut.isSet(COMPONENT_1));

	m_sut.set(COMPONENT_1, SET);
	EXPECT_TRUE(m_sut.isSet(COMPONENT_1));
}

TEST_F(ComponentIndicatorsTestSuite, andOperatorShouldStoreProperResultOnCalledObject)
{
	m_sut.set(COMPONENT_1);
	m_sut.set(COMPONENT_2);
	m_sut.set(COMPONENT_3);

	ComponentIndicators l_anotherComponentIndicators;
	l_anotherComponentIndicators.set(COMPONENT_1);
	l_anotherComponentIndicators.set(COMPONENT_3);

	m_sut = (m_sut &= l_anotherComponentIndicators);
	EXPECT_EQ(TWO_COMPONENTS, m_sut.getNumOfSetComponents());
	EXPECT_FALSE(m_sut.isSet(COMPONENT_2));
}

TEST_F(ComponentIndicatorsTestSuite, xorOperatorShouldStoreProperResultOnCalledObject)
{
	m_sut.set(COMPONENT_1);
	m_sut.set(COMPONENT_2);
	m_sut.set(COMPONENT_3);

	ComponentIndicators l_anotherComponentIndicators;
	l_anotherComponentIndicators.set(COMPONENT_2);

	m_sut = (m_sut ^= l_anotherComponentIndicators);
	EXPECT_EQ(TWO_COMPONENTS, m_sut.getNumOfSetComponents());
	EXPECT_FALSE(m_sut.isSet(COMPONENT_2));
}
