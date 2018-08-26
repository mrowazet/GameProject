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
	virtual ~IComponentController() = default;

	virtual ComponentBase& createComponent(ComponentType) = 0;
	virtual bool removeComponent(ComponentBase&) = 0;
};

}