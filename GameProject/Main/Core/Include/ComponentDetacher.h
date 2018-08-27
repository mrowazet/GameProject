#pragma once
#include "IComponentDetacher.h"
#include "IComponentController.h"

namespace engine
{

class ComponentDetacher : public IComponentDetacher
{
public:
	ComponentDetacher(IComponentController&);
	bool detachComponent(Entity&, ComponentType) override;

private:
	IComponentController& m_componentController;

	bool isComponentAlreadyAttached(Entity&, ComponentType);
	void detachComponentFromEntity(Entity&, ComponentType);
};

}