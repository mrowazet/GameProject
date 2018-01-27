#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "Stopwatch.h"

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
	{

	}

	void startStopwatch()
	{
		m_stopwatch.start();
	}

	void stopStopwatch()
	{
		m_stopwatch.stop();
		std::cout << "\n Measured time: " << m_stopwatch.getElapsedTime().count() << "ms \n";
	}

protected:
	testTool::Stopwatch m_stopwatch;
};
