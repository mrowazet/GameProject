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

	MOCK_METHOD1(removeEntity, bool(EntityId));

	MOCK_METHOD1(hasEntity, bool(EntityId));
	MOCK_METHOD1(getEntity, Entity&(EntityId));

	MOCK_METHOD2(connectComponentToEntity, bool(EntityId, ComponentType));
	MOCK_METHOD2(disconnectComponentFromEntity, bool(EntityId, ComponentType));

	MOCK_METHOD2(connectMultipleComponentsToEntity, bool(EntityId, const ComponentFlags&));
	MOCK_METHOD2(disconnectMultipleComponentsFromEntity, bool(EntityId, const ComponentFlags&));
};

}
