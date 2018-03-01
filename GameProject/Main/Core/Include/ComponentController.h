#pragma once
#include "IComponentController.h"

namespace engine
{

class ComponentController : public IComponentController
{
public:
	ComponentBase& createComponent(ComponentType p_component) override;
};

}
