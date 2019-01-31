#pragma once
#include "Types.h"

namespace engine
{
	constexpr Id UNDEFINED_ID = 0u;
	constexpr EntityId UNDEFINED_ENTITY_ID = UNDEFINED_ID;
	constexpr ComponentFlags NO_COMPONENTS = ComponentFlags();
	constexpr u32 LAST_VALID_COMPONENT_INDEX = 3u;
}
