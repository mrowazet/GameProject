#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IComponentController.h"

namespace engine
{

class ComponentControllerMock : public IComponentController
{
public:
	MOCK_METHOD2(attachComponent, bool(Entity&, ComponentType));
	MOCK_METHOD2(attachMultipleComponents, bool(Entity&, const ComponentIndicators&));

	MOCK_METHOD2(detachComponent, bool(Entity&, ComponentType));
	MOCK_METHOD2(dettachMultipleComponents, bool(Entity&, const ComponentIndicators&));
};

}