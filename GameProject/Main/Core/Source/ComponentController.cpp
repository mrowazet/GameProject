#include "ComponentController.h"

namespace engine
{

ComponentController::ComponentController(std::unique_ptr<IComponentProvider> p_componentProvider)
	:m_componentProvider(std::move(p_componentProvider))
{

}

bool ComponentController::attachComponent(Entity& p_entity, ComponentType p_componentType)
{
	if (!isComponentAlreadyAttached(p_entity, p_componentType))
	{
		attachComponentToEntity(p_entity, p_componentType);
		p_entity.attachedComponents.flip(p_componentType);

		return true;
	}
	else
	{
		return false;
	}
}

bool ComponentController::attachMultipleComponents(Entity&, const ComponentIndicators&)
{
	return false;
}

bool ComponentController::detachComponent(Entity& p_entity, ComponentType p_componentType)
{
	if (isComponentAlreadyAttached(p_entity, p_componentType))
	{
		detachComponentFromEntity(p_entity, p_componentType);
		p_entity.attachedComponents.flip(p_componentType);

		return true;
	}
	else
	{
		return false;
	}
}

bool ComponentController::dettachMultipleComponents(Entity&, const ComponentIndicators&)
{
	return false;
}

bool ComponentController::isComponentAlreadyAttached(Entity& p_entity, ComponentType p_componentType) const
{
	return p_entity.attachedComponents.isSet(p_componentType);
}

void ComponentController::attachComponentToEntity(Entity& p_entity, ComponentType p_componentType)
{
	auto& l_newComponent = m_componentProvider->createComponent(p_componentType);
	attachToNextFreePosition(p_entity, l_newComponent);
}

void ComponentController::attachToNextFreePosition(Entity& p_entity, ComponentBase& p_newComponent)
{
	auto l_positionForNewComponent = getNextFreeComponentPosition(p_entity);
	*l_positionForNewComponent = &p_newComponent;
}

ComponentPtr* ComponentController::getNextFreeComponentPosition(Entity& p_entity)
{
	ComponentPtr* l_ptrToComponentPosition = &p_entity.components;

	while (*l_ptrToComponentPosition != nullptr)
	{
		l_ptrToComponentPosition = &(*l_ptrToComponentPosition)->nextComponent;
	}

	return l_ptrToComponentPosition;
}

void ComponentController::detachComponentFromEntity(Entity& p_entity, ComponentType p_componentType)
{
	ComponentPtr* l_ptrToComponentPosition = &p_entity.components;

	while (*l_ptrToComponentPosition != nullptr)
	{
		auto& l_component = **l_ptrToComponentPosition;
		if (l_component.type == p_componentType)
		{
			*l_ptrToComponentPosition = &(*l_component.nextComponent);
			m_componentProvider->removeComponent(l_component);
			break;
		}
		else
		{
			l_ptrToComponentPosition = &l_component.nextComponent;
		}
	}
}

}
