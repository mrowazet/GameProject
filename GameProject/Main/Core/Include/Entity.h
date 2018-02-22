#pragma once
#include "Types.h"
#include "Constants.h"

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
	ComponentFlags attachedComponents = NO_COMPONENTS;
	ComponentPtr components = nullptr;
};

}