#pragma once
#include "ComponentBase.h"

namespace engine
{

struct Component : public ComponentBase
{
	Component() = default;
	~Component() = default;

	ComponentPtr nextComponent = nullptr;
};

}