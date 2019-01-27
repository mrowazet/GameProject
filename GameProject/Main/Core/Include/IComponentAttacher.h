#pragma once
#include "Entity.h"
#include "ComponentTypes.h"

namespace engine
{

class IComponentAttacher
{
public:
	virtual ~IComponentAttacher() = default;
	virtual bool attachComponent(Entity&, ComponentType) = 0;
	virtual bool attachMultipleComponents(Entity&, const ComponentFlags&) = 0;
};

}