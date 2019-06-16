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

	virtual bool attachComponent(Entity&, ComponentType) = 0;
	virtual bool attachMultipleComponents(Entity&, const ComponentIndicators&) = 0;

	virtual bool detachComponent(Entity&, ComponentType) = 0;
	virtual bool detachMultipleComponents(Entity&, const ComponentIndicators&) = 0;
};

}