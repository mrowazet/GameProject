#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "IdGuard.h"

using namespace testing;
using namespace engine;

namespace
{
const Id MAX_ID = 10u;
const Id ID_1 = 1u;
const Id ID_2 = 2u;
}

class IdGuardTestSuite : public Test
{
public:
	IdGuardTestSuite()
		: m_idGuard(MAX_ID)
	{

	}

protected:
	IdGuard m_idGuard;
};

TEST_F(IdGuardTestSuite, shouldReturnId1WhenCalledFirstTime)
{
	auto l_id = m_idGuard.getNextId();
	EXPECT_EQ(ID_1, l_id);
}

TEST_F(IdGuardTestSuite, shouldReturnNextIdWhenCalled)
{
	auto l_id1 = m_idGuard.getNextId();
	auto l_id2 = m_idGuard.getNextId();

	EXPECT_EQ(ID_1, l_id1);
	EXPECT_EQ(ID_2, l_id2);
}

TEST_F(IdGuardTestSuite, shouldReturnUndefinedIdWhenOverflowed)
{
	IdGuard l_idGuard(ID_2);

	l_idGuard.getNextId();
	l_idGuard.getNextId();
	auto l_idAfterOverflow = l_idGuard.getNextId();

	EXPECT_EQ(engine::UNDEFINED_ID, l_idAfterOverflow);
}

TEST_F(IdGuardTestSuite, shouldReturnReleasedIdIfReturned)
{
	auto l_id = m_idGuard.getNextId();
	m_idGuard.freeId(l_id);

	auto l_nextId = m_idGuard.getNextId();
	EXPECT_EQ(ID_1, l_id);
	EXPECT_EQ(l_id, l_nextId);
}

TEST_F(IdGuardTestSuite, shouldReturnNextFreeIdAfterGetFreeSequence)
{
	auto l_id = m_idGuard.getNextId();
	m_idGuard.freeId(l_id);

	auto l_id1 = m_idGuard.getNextId();
	auto l_id2 = m_idGuard.getNextId();

	EXPECT_EQ(ID_1, l_id1);
	EXPECT_EQ(ID_2, l_id2);
}

TEST_F(IdGuardTestSuite, maxIdCanBeReturned)
{
	IdGuard l_idGuard(ID_1);

	auto l_id = l_idGuard.getNextId();
	auto l_undefId = l_idGuard.getNextId();

	EXPECT_EQ(ID_1, l_id);
	EXPECT_EQ(engine::UNDEFINED_ID, l_undefId);
}

TEST_F(IdGuardTestSuite, shouldBePossibleToFreeIdWhenLastCorrectIdHasBeenReturnedEarlier)
{
	IdGuard l_idGuard(ID_2);

	auto l_id1 = l_idGuard.getNextId();
	l_idGuard.getNextId();
	l_idGuard.freeId(l_id1);

	auto l_freedId = l_idGuard.getNextId();
	EXPECT_EQ(l_id1, l_freedId);
}

TEST_F(IdGuardTestSuite, resetShouldClearGuardState)
{
	auto l_firstId = m_idGuard.getNextId();
	m_idGuard.getNextId();

	m_idGuard.reset();

	EXPECT_EQ(l_firstId, m_idGuard.getNextId());
}

TEST_F(IdGuardTestSuite, resetShouldClearFreedIds)
{
	auto l_firstId = m_idGuard.getNextId();
	auto l_secondId = m_idGuard.getNextId();

	m_idGuard.freeId(l_secondId);
	m_idGuard.reset();

	EXPECT_EQ(l_firstId, m_idGuard.getNextId());
}

TEST_F(IdGuardTestSuite, resetShouldClearOverloadState)
{
	IdGuard l_idGuard(ID_1);

	auto l_firstId = l_idGuard.getNextId();

	auto l_undefId = l_idGuard.getNextId();
	ASSERT_EQ(engine::UNDEFINED_ID, l_undefId);

	l_idGuard.reset();
	EXPECT_EQ(l_firstId, l_idGuard.getNextId());
}

/*
Note:
Guard should not be recovered after oveflow - this situation indicates a problem in other parts!

Example:
	Guard is overflowed but some value returns to the pool by "freeId".
	That means that we have value to return when call "getNextId" but it should not be done since overflow took place earlier.
*/