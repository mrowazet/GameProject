#pragma once
#include "Types.h"
#include "Constants.h"

namespace engine
{

struct Component
{
	Component() = default;
	virtual ~Component() = default;

	ComponentPtr nextComonent = nullptr;
};

}