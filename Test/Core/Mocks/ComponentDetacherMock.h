#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IComponentDetacher.h"

namespace engine
{

class ComponentDetacherMock : public IComponentDetacher
{
public: 
	MOCK_METHOD2(detachComponent, bool(Entity&, ComponentType));
	MOCK_METHOD2(dettachMultipleComponents, bool(Entity&, const ComponentFlags&));
};

}