#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IIdGuard.h"

namespace engine
{

class IdGuardMock : public IIdGuard
{
public:
	MOCK_METHOD0(getNextId, Id());
	MOCK_METHOD1(freeId, void(Id));
};

}