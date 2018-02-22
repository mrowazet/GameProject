#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "Core.h"
#include "IdGuard.h"

using namespace testing;
using namespace engine;

namespace
{
const Id MAX_ID = 10u;
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
