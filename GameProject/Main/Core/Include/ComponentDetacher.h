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
	bool dettachMultipleComponents(Entity&, const ComponentFlags&) override;

private:
	IComponentController& m_componentController;

	bool isComponentAlreadyAttached(Entity&, ComponentType) const;
	void detachComponentFromEntity(Entity&, ComponentType);
};

}