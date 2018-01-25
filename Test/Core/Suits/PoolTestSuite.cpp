#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"

using namespace testing;
using namespace engine;

namespace
{
	const PoolSize POOL_SIZE	= 10u;

	const u32 ZERO				= 0u;
	const u32 NO_ELEMENTS		= 0u;
	const u32 ONE_ELEMENT		= 1u;
	const u32 TWO_ELEMENTS		= 2u;
	const u32 THREE_ELEMENTS	= 3u;

	const EntityId ENTITY_ID_1	= 1u;
	const EntityId ENTITY_ID_2	= 2u;
	const EntityId ENTITY_ID_3	= 3u;
	const EntityId ENTITY_ID_4	= 4u;

	const u32 ITER_OFFSET		= 2u;

	constexpr unsigned int ALIGNMENT1024 = 1024u;
	struct alignas(ALIGNMENT1024) Aligned1024 
	{
	};

	class DtorCounter
	{
	public:
		DtorCounter(unsigned int& p_destructionsCounter)
			:m_counter(p_destructionsCounter)
		{

		}

		~DtorCounter()
		{
			m_counter++;
		}

	private:
		unsigned int& m_counter;
	};
}

class PoolTestSuite : public Test
{
public:
	PoolTestSuite()
		:m_pool(POOL_SIZE)
	{
	}

protected:
	void addThreeElementsToPool();
	Entity& addElementToPool(EntityId p_entityId);

	ContinuousPool<Entity> m_pool;
};

void PoolTestSuite::addThreeElementsToPool()
{
	addElementToPool(ENTITY_ID_1);
	addElementToPool(ENTITY_ID_2);
	addElementToPool(ENTITY_ID_3);
}

Entity& PoolTestSuite::addElementToPool(EntityId p_entityId)
{
	auto& l_element = m_pool.allocate();
	l_element.id = p_entityId;

	return l_element;
}

TEST_F(PoolTestSuite, ConstructorAllocatesEnoughMemoryForNumberOfElements)
{
	EXPECT_EQ(POOL_SIZE, m_pool.maxSize());
}

TEST_F(PoolTestSuite, PoolIsEmptyAfterInitialization)
{
	EXPECT_EQ(NO_ELEMENTS, m_pool.size());
	EXPECT_TRUE(m_pool.isEmpty());
}

TEST_F(PoolTestSuite, ResetShouldClearInternalDataWithoutCallingDtors)
{
	ContinuousPool<DtorCounter> l_pool(POOL_SIZE);
	auto l_destructions = 0u;

	l_pool.allocate(l_destructions);
	l_pool.allocate(l_destructions);

	ASSERT_EQ(TWO_ELEMENTS, l_pool.size());
	ASSERT_FALSE(l_pool.isEmpty());

	l_pool.reset();

	EXPECT_EQ(NO_ELEMENTS, l_pool.size());
	EXPECT_TRUE(l_pool.isEmpty());
	EXPECT_EQ(ZERO, l_destructions);
}

TEST_F(PoolTestSuite, BeginAndEndIterAreEqualIfPoolIsEmpty)
{
	ASSERT_TRUE(m_pool.isEmpty());

	auto l_begin = m_pool.begin();
	auto l_end = m_pool.end();

	EXPECT_EQ(l_begin, l_end);
}

TEST_F(PoolTestSuite, CanIterateOverPoolUsingIterator)
{
	addThreeElementsToPool();

	auto l_iter = m_pool.begin();

	EXPECT_EQ(ENTITY_ID_1, l_iter->id);

	++l_iter;
	EXPECT_EQ(ENTITY_ID_2, l_iter->id);

	++l_iter;
	EXPECT_EQ(ENTITY_ID_3, l_iter->id);
}

TEST_F(PoolTestSuite, CanUseRangeBasedForLoopToIterateOverPool)
{
	addThreeElementsToPool();
	u32 l_nrOfLoops = 0u;

	for (auto l_element : m_pool)
	{
		l_nrOfLoops++;
	}

	EXPECT_EQ(THREE_ELEMENTS, l_nrOfLoops);
}

TEST_F(PoolTestSuite, ElementCanBeAddedToPool)
{
	m_pool.allocate();

	EXPECT_EQ(ONE_ELEMENT, m_pool.size());
	EXPECT_FALSE(m_pool.isEmpty());
}

TEST_F(PoolTestSuite, ElementsAreCorrectlyAddedToPool)
{
	auto& l_element1 = addElementToPool(ENTITY_ID_1);
	auto& l_element2 = addElementToPool(ENTITY_ID_2);

	EXPECT_EQ(ENTITY_ID_1, l_element1.id);
	EXPECT_EQ(ENTITY_ID_2, l_element2.id);

	auto l_addres1 = reinterpret_cast<std::uintptr_t>(&l_element1);
	auto l_addres2 = reinterpret_cast<std::uintptr_t>(&l_element2);
	auto l_elementSizeInPool = l_addres2 - l_addres1;

	EXPECT_EQ(sizeof(Entity), l_elementSizeInPool);
}

TEST_F(PoolTestSuite, ElementCanBeRemoveFromPool)
{
	auto& l_elementToRemove = addElementToPool(ENTITY_ID_1);
	addElementToPool(ENTITY_ID_2);

	ASSERT_EQ(TWO_ELEMENTS, m_pool.size());
	m_pool.deallocate(l_elementToRemove);

	EXPECT_EQ(ONE_ELEMENT, m_pool.size());
}

TEST_F(PoolTestSuite, ElementsAreCorrectlyLocatedInPoolAfterAddRemoveAddSequence)
{
	auto& l_elementOnPosition0 = addElementToPool(ENTITY_ID_1);
	auto& l_elementOnPosition1 = addElementToPool(ENTITY_ID_2);
	auto& l_elementOnPosition2 = addElementToPool(ENTITY_ID_3);

	m_pool.deallocate(l_elementOnPosition0);
	addElementToPool(ENTITY_ID_4);

	//Note: references still point to the same address on memory but internal struct of data 
	//		has changed so we can check if order of elements is correctly organized

	EXPECT_EQ(ENTITY_ID_3, l_elementOnPosition0.id);
	EXPECT_EQ(ENTITY_ID_2, l_elementOnPosition1.id);
	EXPECT_EQ(ENTITY_ID_4, l_elementOnPosition2.id);
}

TEST_F(PoolTestSuite, IteratorCanBeMovedWithOperatorPlusEqual)
{
	addThreeElementsToPool();

	auto l_iter = m_pool.begin();
	l_iter += ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_3, l_iter->id);
}

TEST_F(PoolTestSuite, NewIterCanBeCreatedByOperatorPlus)
{
	addThreeElementsToPool();

	auto l_begin = m_pool.begin();
	auto l_iter = l_begin + ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_1, l_begin->id);
	EXPECT_EQ(ENTITY_ID_3, l_iter->id);
}

TEST_F(PoolTestSuite, ConstItersCanBeCreated)
{
	auto l_cbegin = m_pool.cbegin();
	auto l_cend = m_pool.cend();
}

TEST_F(PoolTestSuite, ConstIterCanBeUsed)
{
	addThreeElementsToPool();

	auto l_cbegin = m_pool.cbegin();

	EXPECT_EQ(ENTITY_ID_1, (*l_cbegin).id);
	EXPECT_EQ(ENTITY_ID_1, l_cbegin->id);
}

TEST_F(PoolTestSuite, ElementIsNotCopiedWhenIterReturned)
{
	addThreeElementsToPool();

	auto l_cbegin = m_pool.cbegin();
	auto l_begin = m_pool.begin();

	l_begin->id = ENTITY_ID_4;

	EXPECT_EQ(ENTITY_ID_4, (*l_cbegin).id);
	EXPECT_EQ(ENTITY_ID_4, l_cbegin->id);
}

TEST_F(PoolTestSuite, PrefixPostfixIncrementationCanBeUsedWithConstIter)
{
	addThreeElementsToPool();

	auto l_cbegin = m_pool.cbegin();

	++l_cbegin;
	EXPECT_EQ(ENTITY_ID_2, l_cbegin->id);

	l_cbegin++;
	EXPECT_EQ(ENTITY_ID_3, l_cbegin->id);
}

TEST_F(PoolTestSuite, ConstIterAreEqualsIfPoolIsEmpty)
{
	auto l_cbegin = m_pool.cbegin();
	auto l_cend = m_pool.cend();

	EXPECT_EQ(l_cbegin, l_cend);

	l_cbegin++;
	EXPECT_NE(l_cbegin, l_cend);
}

TEST_F(PoolTestSuite, ConstIteratorCanBeMovedWithOperatorPlusEqual)
{
	addThreeElementsToPool();

	auto l_citer = m_pool.cbegin();
	l_citer += ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_3, l_citer->id);
}

TEST_F(PoolTestSuite, NewConstIterCanBeCreatedByOperatorPlus)
{
	addThreeElementsToPool();

	auto l_cbegin = m_pool.cbegin();
	auto l_citer = l_cbegin + ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_1, l_cbegin->id);
	EXPECT_EQ(ENTITY_ID_3, l_citer->id);
}

TEST_F(PoolTestSuite, CanUseRangeBasedForLoopToIterateOverPoolUsingConstIter)
{
	addThreeElementsToPool();
	u32 l_nrOfLoops = 0u;

	const auto& l_pool = m_pool;

	for (const auto& l_element : l_pool)
	{
		l_nrOfLoops++;
	}

	EXPECT_EQ(THREE_ELEMENTS, l_nrOfLoops);
}

TEST_F(PoolTestSuite, CanForwardArgumentsToCtorWhenAllocate)
{
	auto& l_element = m_pool.allocate(ENTITY_ID_4);
	EXPECT_EQ(ENTITY_ID_4, l_element.id);
}

TEST_F(PoolTestSuite, ShouldDeathIfDataNotAligned)
{
	EXPECT_DEATH(ContinuousPool<Aligned1024> l_pool(POOL_SIZE), "");
}

//TEST_F(PoolTestSuite, DealocateShouldCallClassDtor)
//{
//	ContinuousPool<DtorCounter> l_pool(POOL_SIZE);
//	auto l_destructions = 0u;
//
//	auto& l_element = l_pool.allocate(l_destructions);
//
//	EXPECT_EQ(ONE_ELEMENT, l_destructions);
//}
//
//TEST_F(PoolTestSuite, ClearShouldCallDtorsOnAllObjectsAndClearInternalData)
//{
//
//}