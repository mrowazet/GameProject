#pragma once
#include "Types.h"
#include "Constants.h"

namespace engine
{

class IComponent
{
public:
	IComponent() = default;
	virtual ~IComponent() = default;
};

}