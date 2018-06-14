#pragma once
#include "Types.h"
#include "Constants.h"
#include "ComponentTypes.h"

namespace engine
{

struct ComponentBase
{
	ComponentBase(ComponentType p_componentType)
		:type(p_componentType)
	{
	}

	virtual ~ComponentBase() = default;

	const ComponentType type;

	EntityId connectedEntity = UNDEFINED_ENTITY_ID;
	ComponentPtr nextComponent = nullptr;
};

}