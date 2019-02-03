#pragma once
#include "IComponentController.h"
#include "IComponentProvider.h"

namespace engine
{

class ComponentController : public IComponentController
{
public:
	ComponentController(std::unique_ptr<IComponentProvider>);

	bool attachComponent(Entity&, ComponentType) override;
	bool attachMultipleComponents(Entity&, const ComponentIndicators&) override;

	bool detachComponent(Entity&, ComponentType) override;
	bool dettachMultipleComponents(Entity&, const ComponentIndicators&) override;

private:
	bool isComponentAlreadyAttached(Entity&, ComponentType) const;
	void attachComponentToEntity(Entity&, ComponentType);
	void attachToNextFreePosition(Entity&, ComponentBase&);
	ComponentPtr* getNextComponentPosition(ComponentBase&) const;
	void attachComponentToPosition(ComponentBase&, ComponentPtr*);
	ComponentPtr* getNextFreeComponentPosition(Entity&) const;
	void detachComponentFromEntity(Entity&, ComponentType);
	ComponentIndicators getComponentsWhichAreNotAlreadyAttached(const Entity&, const ComponentIndicators&) const;
	bool hasOnlyOneComponentToAttach(const ComponentIndicators&) const;
	ComponentType getSingleSetComponent(const ComponentIndicators&) const;
	void attachMultipleComponentsToEntity(Entity&, const ComponentIndicators&);
	void attachRequestedComponentsToEntity(Entity&, const ComponentIndicators&);
	ComponentType convertIndexToComponentType(ComponentIndex) const;

	std::unique_ptr<IComponentProvider> m_componentProvider;
};

}
