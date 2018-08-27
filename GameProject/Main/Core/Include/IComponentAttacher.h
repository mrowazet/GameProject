#pragma once
#include "Entity.h"
#include "ComponentTypes.h"

namespace engine
{

class IComponentAttacher
{
public:
	virtual bool attachComponent(Entity&, ComponentType) = 0;
};

}