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
	const bool COMPARE_WITH_SMART_PTRS = DISABLED;

	const bool allocateWholePoolAndClear = DISABLED;
	const bool allocateWholePoolAndReset = DISABLED;
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

	template<typename T>
	std::vector<T> createVector()
	{
		std::vector<T> l_vector;
		l_vector.resize(POOL_SIZE);

		return l_vector;
	}

	template<typename T>
	void repeatActionForNumberOfLoops(std::vector<T>& p_vector, std::function<void(std::vector<T>&)> p_function)
	{
		for (auto i = 0u; i < LOOPS; i++)
		{
			p_function(p_vector);
		}
	}

	template<typename T>
	void measureActionOnCreatedVector(std::function<void(std::vector<T>&)> p_function)
	{
		auto l_vector = createVector<T>();

		startStopwatch();
		repeatActionForNumberOfLoops(l_vector, p_function);
		stopStopwatch();
	}
	
	template<typename T>
	auto allocateAllElementsInVectorAndThenDeallocate()
	{
		auto l_function = [](auto& p_vector)
		{
			for (int i = 0u; i < POOL_SIZE; i++)
				p_vector[0].reset(new T());

			for (int i = 0u; i < POOL_SIZE; i++)
				p_vector[0].reset();
		};
		
		return l_function;
	}

protected:
	testTool::Stopwatch m_stopwatch;
};

TEST_F(ContinuousPoolPerformanceTestSuite, allocateWholePoolAndClear)
{	
	if (not allocateWholePoolAndClear)
		return;

	auto l_allocateWholePoolAndThenClear = [](auto& p_pool)
	{
		for (int i = 0u; i < POOL_SIZE; i++)
			p_pool.allocate();

		p_pool.clear();
	};

	measureActionOnCreatedPool<Entity8>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity16>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity32>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity64>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity128>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity256>(l_allocateWholePoolAndThenClear);
	measureActionOnCreatedPool<Entity512>(l_allocateWholePoolAndThenClear);

	if (not COMPARE_WITH_SMART_PTRS)
		return;

	std::cout << "std::shared_ptr: \n";
	measureActionOnCreatedVector<std::shared_ptr<Entity8>>(allocateAllElementsInVectorAndThenDeallocate<Entity8>());
	measureActionOnCreatedVector<std::shared_ptr<Entity16>>(allocateAllElementsInVectorAndThenDeallocate<Entity16>());
	measureActionOnCreatedVector<std::shared_ptr<Entity32>>(allocateAllElementsInVectorAndThenDeallocate<Entity32>());
	measureActionOnCreatedVector<std::shared_ptr<Entity64>>(allocateAllElementsInVectorAndThenDeallocate<Entity64>());
	measureActionOnCreatedVector<std::shared_ptr<Entity128>>(allocateAllElementsInVectorAndThenDeallocate<Entity128>());
	measureActionOnCreatedVector<std::shared_ptr<Entity256>>(allocateAllElementsInVectorAndThenDeallocate<Entity256>());
	measureActionOnCreatedVector<std::shared_ptr<Entity512>>(allocateAllElementsInVectorAndThenDeallocate<Entity512>());

	std::cout << "std::unique_ptr: \n";
	measureActionOnCreatedVector<std::unique_ptr<Entity8>>(allocateAllElementsInVectorAndThenDeallocate<Entity8>());
	measureActionOnCreatedVector<std::unique_ptr<Entity16>>(allocateAllElementsInVectorAndThenDeallocate<Entity16>());
	measureActionOnCreatedVector<std::unique_ptr<Entity32>>(allocateAllElementsInVectorAndThenDeallocate<Entity32>());
	measureActionOnCreatedVector<std::unique_ptr<Entity64>>(allocateAllElementsInVectorAndThenDeallocate<Entity64>());
	measureActionOnCreatedVector<std::unique_ptr<Entity128>>(allocateAllElementsInVectorAndThenDeallocate<Entity128>());
	measureActionOnCreatedVector<std::unique_ptr<Entity256>>(allocateAllElementsInVectorAndThenDeallocate<Entity256>());
	measureActionOnCreatedVector<std::unique_ptr<Entity512>>(allocateAllElementsInVectorAndThenDeallocate<Entity512>());
}

TEST_F(ContinuousPoolPerformanceTestSuite, allocateWholePoolAndReset)
{
	if (not allocateWholePoolAndReset)
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