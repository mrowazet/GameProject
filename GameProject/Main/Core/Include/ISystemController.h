#pragma once
#include "Types.h"
#include "Constants.h"
#include "ISystem.h"

namespace engine
{

class ISystemController
{
public:
	ISystemController() = default;
	virtual ~ISystemController() = default;
};

}