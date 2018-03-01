#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "TestComponents.h"
#include "ComponentPool.h"

using namespace testing;
using namespace engine;

namespace
{

const PoolSize POOL_SIZE = 10;
const u32 EMPTY = 0u;
const u32 ONE_ELEMENT = 1u;
const u32 TWO_ELEMENTS = 2u;

}

class ComponentPoolTestSuite : public Test
{
public:
	ComponentPoolTestSuite()
		:m_pool(POOL_SIZE)
	{

	}

protected:
	ComponentPool<testComponents::ComponentA> m_pool;
};

TEST_F(ComponentPoolTestSuite, componentsCanBeAddAndRemove)
{
	auto& l_first = m_pool.getComponent();
	EXPECT_EQ(ONE_ELEMENT, m_pool.size());

	auto& l_second = m_pool.getComponent();
	EXPECT_EQ(TWO_ELEMENTS, m_pool.size());

	m_pool.returnComponent(l_second);
	m_pool.returnComponent(l_first);
	EXPECT_EQ(EMPTY, m_pool.size());
}