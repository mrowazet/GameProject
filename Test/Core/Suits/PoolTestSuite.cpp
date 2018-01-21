#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"

using namespace testing;
using namespace engine;

namespace
{

}

class PoolTestSuite : public Test
{
public:
	PoolTestSuite() = default;
};

TEST_F(PoolTestSuite, test)
{
	ContinuousPool<Entity> l_pool(10);
}