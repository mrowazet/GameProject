#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
//#include "Stopwatch.h"

using namespace testing;
using namespace engine;

namespace
{
	const PoolSize POOL_SIZE = 10000u;
}

class ContinuousPoolPerformanceTestSuite : public Test
{
public:
	ContinuousPoolPerformanceTestSuite()
		:m_pool(POOL_SIZE)
	{

	}

protected:
	ContinuousPool<Entity> m_pool;
	//testTool::Stopwatch m_stopwatch;
};

TEST_F(ContinuousPoolPerformanceTestSuite, aaa)
{
	m_stopwatch.start();
}