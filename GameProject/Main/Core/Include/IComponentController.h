#pragma once
#include "Types.h"
#include "Constants.h"
#include "Parameters.h"
#include "ComponentTypes.h"
#include "ComponentBase.h"
#include "Entity.h"

namespace engine
{

class IComponentController
{
public:
	IComponentController() = default;
	~IComponentController() = default;
};

}