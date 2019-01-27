#pragma once
#include "Entity.h"
#include "ComponentTypes.h"

namespace engine
{

class IComponentDetacher
{
public:
	virtual ~IComponentDetacher() = default;
	virtual bool detachComponent(Entity&, ComponentType) = 0;
	virtual bool dettachMultipleComponents(Entity&, const ComponentIndicators&) = 0;
};

}