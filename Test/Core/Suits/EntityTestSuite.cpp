#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"

using namespace testing;
using namespace engine;

namespace
{
	const EntityId UNDEFINED_ID = UNDEFINED_ENTITY_ID;
	const EntityId ENTITY_ID = 33u;
}

class EntityTestSuite : public Test
{
public:
	EntityTestSuite() = default;
};

TEST_F(EntityTestSuite, defaultCtorSetIdToZero)
{
	Entity l_entity;
	EXPECT_EQ(UNDEFINED_ID, l_entity.id);
}

TEST_F(EntityTestSuite, constructorSetProperId)
{
	Entity l_entity(ENTITY_ID);
	EXPECT_EQ(ENTITY_ID, l_entity.id);
}

