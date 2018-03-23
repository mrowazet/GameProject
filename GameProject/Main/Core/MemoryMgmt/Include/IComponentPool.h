#pragma once
#include "ComponentBase.h"

namespace engine
{

class IComponentPool
{
public:
	IComponentPool() = default;
	virtual ~IComponentPool() = default;

	virtual ComponentBase& getComponent() = 0;
	virtual void returnComponent(ComponentBase& p_component) = 0;

	virtual u32 size() const = 0;
};

}