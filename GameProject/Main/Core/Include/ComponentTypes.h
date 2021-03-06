#pragma once
#include "Types.h"

namespace engine
{

enum class ComponentType : ComponentIndex
{
	POSITION = 0,
	MOVABLE = 1,
	VISIBLE = 2,
	WITH_SI = 3,
	UndefinedComponent = 255
};

}
