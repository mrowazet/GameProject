#pragma once
#include "IComponentAttacher.h"
#include "IComponentController.h"

namespace engine
{

class ComponentAttacher : public IComponentAttacher
{
public:
	ComponentAttacher(IComponentController&);
	bool attachComponent(Entity&, ComponentType) override;
	bool attachMultipleComponents(Entity&, const ComponentIndicators&) override;

private:
	IComponentController& m_componentController;

	bool isComponentAlreadyAttached(Entity&, ComponentType) const;
	void ComponentAttacher::attachComponentToEntity(Entity&, ComponentType);
	void ComponentAttacher::attachToNextFreePosition(Entity&, ComponentBase&);
	ComponentPtr* ComponentAttacher::getNextFreeComponentPosition(Entity&);
};

}