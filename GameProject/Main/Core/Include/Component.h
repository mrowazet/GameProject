#pragma once
#include "IComponent.h"

namespace engine
{

struct Component : public IComponent
{
	Component() = default;
	~Component() = default;

	ComponentPtr nextComonent = nullptr;
};

}