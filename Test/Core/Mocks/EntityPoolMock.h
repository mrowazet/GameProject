#pragma once
#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IEntityPool.h"

namespace engine
{
	class EntityPoolMock : public IEntityPool
	{
	public:	
		MOCK_METHOD0(create, Entity&());
		MOCK_METHOD1(remove, bool(EntityId));
		MOCK_METHOD1(get, Entity&(EntityId));

		MOCK_CONST_METHOD0(size, u32());
		MOCK_CONST_METHOD1(hasId, bool(EntityId));

		MOCK_METHOD0(clear, void());
	};
}