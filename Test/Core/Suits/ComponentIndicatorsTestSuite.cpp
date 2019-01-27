#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"

using namespace testing;
using namespace engine;

namespace
{
	const u32 TWO_COMPONENTS = 2u;
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
	EXPECT_FALSE(m_sut.hasAny());
}

TEST_F(ComponentIndicatorsTestSuite, shouldReturnNumberOfAttachedComponentsWhenFunctionCalled)
{
	m_sut.flip(ComponentType::MOVABLE);
	m_sut.flip(ComponentType::POSITION);

	EXPECT_EQ(TWO_COMPONENTS, m_sut.getNumOfSetComponents());
}

TEST_F(ComponentIndicatorsTestSuite, shouldReplaceStoredFlagsByNewWhenAssignOperatorIsCalled)
{
	ComponentIndicators l_componentIndicators;
	l_componentIndicators.flip(ComponentType::VISIBLE);

	m_sut = l_componentIndicators;

	EXPECT_EQ(m_sut.getNumOfSetComponents(), l_componentIndicators.getNumOfSetComponents());
	EXPECT_TRUE(m_sut == l_componentIndicators);
}

TEST_F(ComponentIndicatorsTestSuite, shouldReturnFalseIfComponentNotSetAndTrueIfSet)
{
	EXPECT_FALSE(m_sut.isSet(ComponentType::VISIBLE));
	m_sut.flip(ComponentType::VISIBLE);
	EXPECT_TRUE(m_sut.isSet(ComponentType::VISIBLE));
}

TEST_F(ComponentIndicatorsTestSuite, compareOperatorsShouldReturnProperResult)
{
	ComponentIndicators l_componentIndicators;

	EXPECT_TRUE(m_sut == l_componentIndicators);
	EXPECT_FALSE(m_sut != l_componentIndicators);

	m_sut.flip(ComponentType::VISIBLE);

	EXPECT_FALSE(m_sut == l_componentIndicators);
	EXPECT_TRUE(m_sut != l_componentIndicators);
}
