#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IEntityController.h"

namespace engine
{

class EntityControllerMock : public IEntityController
{
public:
	MOCK_METHOD0(createEntity, EntityId());
	MOCK_METHOD1(createEntityWithComponents, EntityId(const ComponentFlags&));

	MOCK_METHOD1(remove, bool(EntityId));
	MOCK_METHOD1(get, Entity&(EntityId));

	MOCK_METHOD2(connectComponentsToEntity, bool(EntityId, const ComponentFlags&));
	MOCK_METHOD2(disconnectComponentsFromEntity, bool(EntityId, const ComponentFlags&));
};

}
