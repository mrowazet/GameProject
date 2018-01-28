#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include <functional>
#include "Core.h"
#include "Stopwatch.h"
#include "TestEnities.h"

using namespace testing;
using namespace engine;
using namespace testEntity;

namespace
{
	const bool ENABLED = true;
	const bool DISABLED = false;

	const PoolSize POOL_SIZE = 10000u;
	const u32 LOOPS = 100u;

	//TESTS:
	const bool allocateWholePoolAndClear = ENABLED;
	const bool allocateWholePoolAndReset = ENABLED;
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
		std::cout << "Measured time: " << m_stopwatch.getElapsedTime().count() << "ms \n\n";
	}

	template<typename T>
	ContinuousPool<T> createPool()
	{
		return ContinuousPool<T>(POOL_SIZE);
	}

	template<typename T>
	void repeatActionForNumberOfLoops(ContinuousPool<T>& p_pool, std::function<void(ContinuousPool<T>&)> p_function)
	{
		for (auto i = 0u; i < LOOPS; i++)
		{
			p_function(p_pool);
		}
	}

	template<typename T>
	void measureActionOnCreatedPool(std::function<void(ContinuousPool<T>&)> p_function)
	{
		std::cout << "Size of tested entity: " << sizeof(T) << "\n";
		auto l_pool = createPool<T>();

		startStopwatch();
		repeatActionForNumberOfLoops<T>(l_pool, p_function);
		stopStopwatch();
	}

protected:
	testTool::Stopwatch m_stopwatch;
};

TEST_F(ContinuousPoolPerformanceTestSuite, allocateWholePoolAndClear)
{	
	if (!allocateWholePoolAndClear)
		return;

	auto l_allocateWholePoolAndThenClear = [](auto& p_pool)
	{
		for (int i = 0u; i < POOL_SIZE; i++)
			auto& l_element = p_pool.allocate();

		p_pool.clear();
	};

	measureActionOnCreatedPool<Entity8>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity16>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity32>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity64>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity128>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity256>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity512>(l_allocateWholePoolAndThenClear);
}

TEST_F(ContinuousPoolPerformanceTestSuite, allocateWholePoolAndReset)
{
	if (!allocateWholePoolAndReset)
		return;

	auto l_allocateWholePoolAndThenReset = [](auto& p_pool)
	{
		for (int i = 0u; i < POOL_SIZE; i++)
			auto& l_element = p_pool.allocate();

		p_pool.reset();
	};

	measureActionOnCreatedPool<Entity8>(l_allocateWholePoolAndThenReset);
	measureActionOnCreatedPool<Entity16>(l_allocateWholePoolAndThenReset);
	measureActionOnCreatedPool<Entity32>(l_allocateWholePoolAndThenReset);
	measureActionOnCreatedPool<Entity64>(l_allocateWholePoolAndThenReset);
	measureActionOnCreatedPool<Entity128>(l_allocateWholePoolAndThenReset);
	measureActionOnCreatedPool<Entity256>(l_allocateWholePoolAndThenReset);
	measureActionOnCreatedPool<Entity512>(l_allocateWholePoolAndThenReset);
}