#pragma once
#include "Types.h"
#include "Constants.h"
#include "ComponentIndicators.h"

namespace engine
{

struct Entity final
{
	Entity() = default;
	~Entity() = default;
	Entity(const Entity&) = default;

	Entity(EntityId p_id)
		:id(p_id)
	{
	}

	EntityId id = UNDEFINED_ENTITY_ID;
	ComponentIndicators attachedComponents;
	ComponentPtr components = nullptr;
};

}