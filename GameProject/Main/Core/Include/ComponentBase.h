#pragma once
#include "Types.h"
#include "Constants.h"

namespace engine
{

struct ComponentBase
{
	ComponentBase() = default;
	virtual ~ComponentBase() = default;

	EntityId connectedEntity = UNDEFINED_ENTITY_ID;
	ComponentPtr nextComponent = nullptr;
};

}