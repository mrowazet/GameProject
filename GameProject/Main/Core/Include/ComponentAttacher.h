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

private:
	IComponentController& m_componentController;

	bool isComponentAlreadyAttached(Entity&, ComponentType);
	void ComponentAttacher::attachComponentToEntity(Entity&, ComponentType);
	void ComponentAttacher::addComponentToNextFreePositionInEntity(Entity&, ComponentBase&);
	ComponentPtr* ComponentAttacher::getNextFreePositionForComponent(Entity&);
};

}