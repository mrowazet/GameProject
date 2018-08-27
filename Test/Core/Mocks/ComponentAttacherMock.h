#pragma once
#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include "IComponentAttacher.h"

namespace engine
{

class ComponentAttacherMock : public IComponentAttacher
{
public: 
	MOCK_METHOD2(attachComponent, bool(Entity&, ComponentType));
};

}