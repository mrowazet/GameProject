#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IComponentProvider.h"

namespace engine
{

class ComponentProviderMock : public IComponentProvider
{
public:
	MOCK_METHOD1(createComponent, ComponentBase&(ComponentType p_component));
	MOCK_METHOD1(removeComponent, bool(ComponentBase&));
};

}