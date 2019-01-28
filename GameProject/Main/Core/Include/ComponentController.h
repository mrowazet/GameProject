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
	ComponentPtr* getNextFreeComponentPosition(Entity&);
	void detachComponentFromEntity(Entity&, ComponentType);

	std::unique_ptr<IComponentProvider> m_componentProvider;
};

}
