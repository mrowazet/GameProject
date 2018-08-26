#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IComponentController.h"

namespace engine
{

class ComponentControllerMock : public IComponentController
{
public:
	MOCK_METHOD1(createComponent, ComponentBase&(ComponentType p_component));
	MOCK_METHOD1(removeComponent, bool(ComponentBase&));
};

}