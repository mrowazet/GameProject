#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "TestEnities.h"
#include "HelperClasses.h"
#include "TestComponents.h"
#include "Core.h"

using namespace testing;
using namespace testTool;
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

	constexpr unsigned int ALIGNMENT4096 = 4096u;
	struct alignas(ALIGNMENT4096) Aligned4096 
	{
	};
}

class PoolTestSuite : public Test
{
public:
	PoolTestSuite()
		:m_sut(POOL_SIZE)
	{
	}

protected:
	void addThreeElementsToPool();
	Entity& addElementToPool(EntityId p_entityId);

	ContinuousPool<Entity> m_sut;
	SpecialFuncCounter m_counters;
};

void PoolTestSuite::addThreeElementsToPool()
{
	addElementToPool(ENTITY_ID_1);
	addElementToPool(ENTITY_ID_2);
	addElementToPool(ENTITY_ID_3);
}

Entity& PoolTestSuite::addElementToPool(EntityId p_entityId)
{
	auto& l_element = m_sut.allocate();
	l_element.id = p_entityId;

	return l_element;
}

TEST_F(PoolTestSuite, ConstructorAllocatesEnoughMemoryForNumberOfElements)
{
	EXPECT_EQ(POOL_SIZE, m_sut.maxSize());
}

TEST_F(PoolTestSuite, PoolIsEmptyAfterInitialization)
{
	EXPECT_EQ(NO_ELEMENTS, m_sut.size());
	EXPECT_TRUE(m_sut.isEmpty());
}

TEST_F(PoolTestSuite, ResetShouldClearInternalDataWithoutCallingDtors)
{
	ContinuousPool<TestEntity> l_pool(POOL_SIZE);

	l_pool.allocate(m_counters);
	l_pool.allocate(m_counters);

	ASSERT_EQ(TWO_ELEMENTS, l_pool.size());
	ASSERT_FALSE(l_pool.isEmpty());

	l_pool.reset();

	EXPECT_EQ(NO_ELEMENTS, l_pool.size());
	EXPECT_TRUE(l_pool.isEmpty());
	EXPECT_EQ(ZERO, m_counters.getDtorCounter());
}

TEST_F(PoolTestSuite, BeginAndEndIterAreEqualIfPoolIsEmpty)
{
	ASSERT_TRUE(m_sut.isEmpty());

	auto l_begin = m_sut.begin();
	auto l_end = m_sut.end();

	EXPECT_EQ(l_begin, l_end);
}

TEST_F(PoolTestSuite, CanIterateOverPoolUsingIterator)
{
	addThreeElementsToPool();

	auto l_iter = m_sut.begin();

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

	for (auto& l_element : m_sut)
	{
		l_nrOfLoops++;
	}

	EXPECT_EQ(THREE_ELEMENTS, l_nrOfLoops);
}

TEST_F(PoolTestSuite, ElementCanBeAddedToPool)
{
	m_sut.allocate();

	EXPECT_EQ(ONE_ELEMENT, m_sut.size());
	EXPECT_FALSE(m_sut.isEmpty());
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

	ASSERT_EQ(TWO_ELEMENTS, m_sut.size());
	m_sut.deallocate(l_elementToRemove);

	EXPECT_EQ(ONE_ELEMENT, m_sut.size());
}

TEST_F(PoolTestSuite, ElementsAreCorrectlyLocatedInPoolAfterAddRemoveAddSequence)
{
	auto& l_elementOnPosition0 = addElementToPool(ENTITY_ID_1);
	auto& l_elementOnPosition1 = addElementToPool(ENTITY_ID_2);
	auto& l_elementOnPosition2 = addElementToPool(ENTITY_ID_3);

	m_sut.deallocate(l_elementOnPosition0);
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

	auto l_iter = m_sut.begin();
	l_iter += ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_3, l_iter->id);
}

TEST_F(PoolTestSuite, NewIterCanBeCreatedByOperatorPlus)
{
	addThreeElementsToPool();

	auto l_begin = m_sut.begin();
	auto l_iter = l_begin + ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_1, l_begin->id);
	EXPECT_EQ(ENTITY_ID_3, l_iter->id);
}

TEST_F(PoolTestSuite, ConstItersCanBeCreated)
{
	auto l_cbegin = m_sut.cbegin();
	auto l_cend = m_sut.cend();
}

TEST_F(PoolTestSuite, ConstIterCanBeUsed)
{
	addThreeElementsToPool();

	auto l_cbegin = m_sut.cbegin();

	EXPECT_EQ(ENTITY_ID_1, (*l_cbegin).id);
	EXPECT_EQ(ENTITY_ID_1, l_cbegin->id);
}

TEST_F(PoolTestSuite, ElementIsNotCopiedWhenIterReturned)
{
	addThreeElementsToPool();

	auto l_cbegin = m_sut.cbegin();
	auto l_begin = m_sut.begin();

	l_begin->id = ENTITY_ID_3;
	const auto& l_element = *l_cbegin;

	l_begin->id = ENTITY_ID_4;

	EXPECT_EQ(ENTITY_ID_4, (*l_cbegin).id);
	EXPECT_EQ(ENTITY_ID_4, l_cbegin->id);
	EXPECT_EQ(ENTITY_ID_4, l_element.id);
}

TEST_F(PoolTestSuite, PrefixPostfixIncrementationCanBeUsedWithConstIter)
{
	addThreeElementsToPool();

	auto l_cbegin = m_sut.cbegin();

	++l_cbegin;
	EXPECT_EQ(ENTITY_ID_2, l_cbegin->id);

	l_cbegin++;
	EXPECT_EQ(ENTITY_ID_3, l_cbegin->id);
}

TEST_F(PoolTestSuite, ConstIterAreEqualsIfPoolIsEmpty)
{
	auto l_cbegin = m_sut.cbegin();
	auto l_cend = m_sut.cend();

	EXPECT_EQ(l_cbegin, l_cend);

	l_cbegin++;
	EXPECT_NE(l_cbegin, l_cend);
}

TEST_F(PoolTestSuite, ConstIteratorCanBeMovedWithOperatorPlusEqual)
{
	addThreeElementsToPool();

	auto l_citer = m_sut.cbegin();
	l_citer += ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_3, l_citer->id);
}

TEST_F(PoolTestSuite, NewConstIterCanBeCreatedByOperatorPlus)
{
	addThreeElementsToPool();

	auto l_cbegin = m_sut.cbegin();
	auto l_citer = l_cbegin + ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_1, l_cbegin->id);
	EXPECT_EQ(ENTITY_ID_3, l_citer->id);
}

TEST_F(PoolTestSuite, CanUseRangeBasedForLoopToIterateOverPoolUsingConstIter)
{
	addThreeElementsToPool();
	u32 l_nrOfLoops = 0u;

	const auto& l_pool = m_sut;

	for (const auto& l_element : l_pool)
	{
		l_nrOfLoops++;
	}

	EXPECT_EQ(THREE_ELEMENTS, l_nrOfLoops);
}

TEST_F(PoolTestSuite, CanForwardArgumentsToCtorWhenAllocate)
{
	auto& l_element = m_sut.allocate(ENTITY_ID_4);
	EXPECT_EQ(ENTITY_ID_4, l_element.id);
}

TEST_F(PoolTestSuite, ShouldDeathIfDataNotAligned)
{
	EXPECT_DEATH(ContinuousPool<Aligned4096> l_pool(POOL_SIZE), "");
}

TEST_F(PoolTestSuite, ShuldDeathIfDataSizeTooSmall)
{
	EXPECT_DEATH(ContinuousPool<testEntity::Entity4> l_pool(POOL_SIZE), "");
}

TEST_F(PoolTestSuite, DealocateShouldCallClassDtor)
{
	ContinuousPool<TestEntity> l_pool(POOL_SIZE);

	auto& l_element = l_pool.allocate(m_counters);
	l_pool.deallocate(l_element);

	EXPECT_EQ(ONE_ELEMENT, m_counters.getDtorCounter());
}

TEST_F(PoolTestSuite, ClearShouldCallDtorsOnAllObjectsAndClearInternalData)
{
	ContinuousPool<TestEntity> l_pool(POOL_SIZE);

	l_pool.allocate(m_counters);
	l_pool.allocate(m_counters);
	l_pool.allocate(m_counters);

	l_pool.clear();

	EXPECT_EQ(THREE_ELEMENTS, m_counters.getDtorCounter());
	EXPECT_EQ(ZERO, l_pool.size());

}

TEST_F(PoolTestSuite, isMyElementReturnsCorrectValues)
{
	auto& l_myElement = m_sut.allocate();
	Entity l_otherObject;

	EXPECT_TRUE(m_sut.isMyObject(l_myElement));
	EXPECT_FALSE(m_sut.isMyObject(l_otherObject));
}

TEST_F(PoolTestSuite, elementsAreNotPreInitedByDefault)
{
	auto l_pool = ContinuousPool<TestEntity>(POOL_SIZE);
	EXPECT_EQ(ZERO, m_counters.getCtorCounter());
}

TEST_F(PoolTestSuite, elementsArePreConsturctedWhenCtorWithPreInitIsUsed)
{
	auto l_pool = ContinuousPool<TestEntity>(POOL_SIZE, InitMode::PRE_INIT, m_counters);
	EXPECT_EQ(POOL_SIZE, m_counters.getCtorCounter());
	EXPECT_TRUE(l_pool.isEmpty());
}

TEST_F(PoolTestSuite, possibleToGetNextElementWithoutCallingCtor)
{
	auto l_pool = ContinuousPool<TestEntity>(POOL_SIZE, InitMode::PRE_INIT, m_counters);

	auto& l_element1 = l_pool.getNext();
	auto& l_element2 = l_pool.getNext();

	EXPECT_EQ(POOL_SIZE, m_counters.getCtorCounter());
	EXPECT_FALSE(l_pool.isEmpty());
	EXPECT_EQ(TWO_ELEMENTS, l_pool.size());

	auto l_addres1 = reinterpret_cast<std::uintptr_t>(&l_element1);
	auto l_addres2 = reinterpret_cast<std::uintptr_t>(&l_element2);
	auto l_elementSizeInPool = l_addres2 - l_addres1;

	EXPECT_EQ(sizeof(TestEntity), l_elementSizeInPool);
}

TEST_F(PoolTestSuite, possibleToReturnElementWithoutCallingDtor)
{
	auto l_pool = ContinuousPool<TestEntity>(POOL_SIZE);

	auto& l_element1 = l_pool.allocate(m_counters, ENTITY_ID_1);
	auto& l_element2 = l_pool.allocate(m_counters, ENTITY_ID_2);

	ASSERT_EQ(TWO_ELEMENTS, l_pool.size());
	ASSERT_EQ(TWO_ELEMENTS, m_counters.getCtorCounter());

	l_pool.takeBack(l_element1);

	// l_elements1 still point to first position in pool where elements from second position should be move
	EXPECT_EQ(ONE_ELEMENT, l_pool.size());
	EXPECT_EQ(ZERO, m_counters.getDtorCounter());
	EXPECT_EQ(ENTITY_ID_2, l_element1.getId());
}

TEST_F(PoolTestSuite, CanIterateBackOverPoolUsingIterator)
{
	addThreeElementsToPool();

	auto l_iter = m_sut.end();

	--l_iter;
	EXPECT_EQ(ENTITY_ID_3, l_iter->id);

	--l_iter;
	EXPECT_EQ(ENTITY_ID_2, l_iter->id);

	l_iter--;
	EXPECT_EQ(ENTITY_ID_1, l_iter->id);
}

TEST_F(PoolTestSuite, IteratorCanBeMovedWithOperatorMinusEqual)
{
	addThreeElementsToPool();

	auto l_iter = m_sut.end();
	l_iter -= ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_2, l_iter->id);
}

TEST_F(PoolTestSuite, NewIterCanBeCreatedByOperatorMinus)
{
	addThreeElementsToPool();

	auto l_end = m_sut.end();
	--l_end;

	auto l_iter = l_end - ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_3, l_end->id);
	EXPECT_EQ(ENTITY_ID_1, l_iter->id);
}

TEST_F(PoolTestSuite, PrefixPostfixDecrementationCanBeUsedWithConstIter)
{
	addThreeElementsToPool();

	auto l_cend = m_sut.cend();

	--l_cend;
	EXPECT_EQ(ENTITY_ID_3, l_cend->id);

	l_cend--;
	EXPECT_EQ(ENTITY_ID_2, l_cend->id);
}

TEST_F(PoolTestSuite, ConstIteratorCanBeMovedWithOperatorMinusEqual)
{
	addThreeElementsToPool();

	auto l_citer = m_sut.cend();
	l_citer -= ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_2, l_citer->id);
}

TEST_F(PoolTestSuite, NewConstIterCanBeCreatedByOperatorMinus)
{
	addThreeElementsToPool();

	auto l_cend = m_sut.cend();
	--l_cend;

	auto l_citer = l_cend - ITER_OFFSET;

	EXPECT_EQ(ENTITY_ID_3, l_cend->id);
	EXPECT_EQ(ENTITY_ID_1, l_citer->id);
}

TEST_F(PoolTestSuite, SafeIterFromEmptyPoolIsNotValid)
{
	auto l_safeIter = m_sut.makeSafeIter();
	EXPECT_FALSE(l_safeIter.isValid());
}

TEST_F(PoolTestSuite, SafeIterIsValidWhenCreatedFromNonEmptyPool)
{
	addThreeElementsToPool();
	
	auto l_safeIter = m_sut.makeSafeIter();
	EXPECT_TRUE(l_safeIter.isValid());
}

TEST_F(PoolTestSuite, SafeIterShouldPointToTheSameElementAfterMoveInternally)
{
	auto& l_firstElement = addElementToPool(ENTITY_ID_1);
	addElementToPool(ENTITY_ID_2);
	addElementToPool(ENTITY_ID_3);

	auto l_safeIter = m_sut.makeSafeIter();
	auto& l_iter = l_safeIter.getIter();
	l_iter += TWO_ELEMENTS;

	ASSERT_EQ(ENTITY_ID_3, l_iter->id);
	m_sut.deallocate(l_firstElement);
	addElementToPool(ENTITY_ID_4);

	EXPECT_TRUE(l_safeIter.isValid());
	EXPECT_EQ(ENTITY_ID_3, l_iter->id);
}

TEST_F(PoolTestSuite, SafeIterShouldBeInvalidIfTrackedElementIsDealloacted)
{
	auto& l_firstElement = addElementToPool(ENTITY_ID_3);
	addElementToPool(ENTITY_ID_4);

	auto l_safeIter = m_sut.makeSafeIter();
	auto& l_iter = l_safeIter.getIter();

	ASSERT_TRUE(l_safeIter.isValid());
	m_sut.deallocate(l_firstElement);

	EXPECT_FALSE(l_safeIter.isValid());
}

TEST_F(PoolTestSuite, SafeItersShouldBeInvalidAfterReset)
{
	addThreeElementsToPool();

	auto l_safeIter = m_sut.makeSafeIter();
	
	ASSERT_TRUE(l_safeIter.isValid());
	m_sut.reset();

	ASSERT_FALSE(l_safeIter.isValid());
}

TEST_F(PoolTestSuite, SafeItersShouldBeInvalidAfterClear)
{
	addThreeElementsToPool();

	auto l_safeIter = m_sut.makeSafeIter();

	ASSERT_TRUE(l_safeIter.isValid());
	m_sut.clear();

	ASSERT_FALSE(l_safeIter.isValid());
}

TEST_F(PoolTestSuite, SafeItersAreEqualWhenPointsToTheSameElement)
{
	addThreeElementsToPool();

	auto l_safeIter1 = m_sut.makeSafeIter();
	auto l_safeIter2 = m_sut.makeSafeIter();

	EXPECT_EQ(l_safeIter1, l_safeIter2);

	auto& l_iter = l_safeIter1.getIter();
	l_iter++;

	EXPECT_NE(l_safeIter1, l_safeIter2);
}

TEST_F(PoolTestSuite, SafeIterIsCorectllyCopied)
{
	addThreeElementsToPool();

	auto l_safeIter1 = m_sut.makeSafeIter();
	auto l_safeIter2 = l_safeIter1;

	EXPECT_EQ(l_safeIter1.isValid(), l_safeIter2.isValid());
	
	//corecttly registered SafeIter should be invalidate after pool reset
	m_sut.reset();
	EXPECT_EQ(l_safeIter1.isValid(), l_safeIter2.isValid());
}

TEST_F(PoolTestSuite, SafeIterCanBeCreatedFromIter)
{
	addThreeElementsToPool();

	auto l_iter = m_sut.begin();
	l_iter++;

	EXPECT_NE(m_sut.begin(), l_iter);

	auto l_safeIter = m_sut.makeSafeIter(l_iter);
	EXPECT_TRUE(l_safeIter.isValid());
	EXPECT_NE(m_sut.begin(), l_safeIter.getIter());
	EXPECT_EQ(l_iter, l_safeIter.getIter());
}
