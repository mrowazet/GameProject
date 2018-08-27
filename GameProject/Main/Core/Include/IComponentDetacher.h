#pragma once
#include "Entity.h"
#include "ComponentTypes.h"

namespace engine
{

class IComponentDetacher
{
public:
	virtual bool detachComponent(Entity&, ComponentType) = 0;
};

}