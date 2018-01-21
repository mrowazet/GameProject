#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include <iostream>
#include "DevTestClass.hpp"
#include "DevTestClassMock.hpp"

using namespace testing;

class DevTestClassTestSuite : public Test
{
public:
	DevTestClassTestSuite()
	{

	}

	~DevTestClassTestSuite()
	{

	}

	virtual void SetUp() override
	{

	}

	virtual void TearDown() override
	{

	}

protected:
	DevTestClass m_sut;
};

TEST_F(DevTestClassTestSuite, proveThatGtestWorks)
{
	EXPECT_EQ(DevTestClass::VALUE, m_sut.getInt());
}

TEST_F(DevTestClassTestSuite, mockCanBeUsed)
{
	DevTestClassMock mock;
	const int expectedValue = 5;

	EXPECT_CALL(mock, getInt()).WillRepeatedly(Return(expectedValue));
	EXPECT_CALL(mock, sayHello());

	mock.sayHello();
	auto result = mock.getInt();

	EXPECT_EQ(expectedValue, result);
	EXPECT_THAT(result, Eq(expectedValue));
}