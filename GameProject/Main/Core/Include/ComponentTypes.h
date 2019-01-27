#pragma once
#include "Types.h"

namespace engine
{

enum class ComponentType : u32
{
	POSITION = 0,
	MOVABLE = 1,
	VISIBLE = 2,
	WITH_SI = 3,
};

const u32 LAST_VALID_COMPONENT_INDEX = 3u;

}
