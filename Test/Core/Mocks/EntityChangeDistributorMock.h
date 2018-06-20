#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IEntityChangeDistributor.h"

namespace engine
{

class EntityChangeDistributorMock : public IEntityChangeDistributor
{
public:
	MOCK_METHOD1(distributeInfoAboutChangeInEntity, void(EntityId));
};

}