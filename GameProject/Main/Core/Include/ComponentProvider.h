#pragma once
#include "IComponentProvider.h"

namespace engine
{

class ComponentProvider : public IComponentProvider
{
public:
	ComponentBase& createComponent(ComponentType) override;
	bool removeComponent(ComponentBase&) override;
};

}