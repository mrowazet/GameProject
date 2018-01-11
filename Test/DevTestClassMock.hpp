#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "DevTestClass.hpp"

class DevTestClassMock : public DevTestClass
{
public:
	MOCK_CONST_METHOD0(sayHello, void());
	MOCK_CONST_METHOD0(getInt, int());
};