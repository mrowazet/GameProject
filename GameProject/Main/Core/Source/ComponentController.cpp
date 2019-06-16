#include "ComponentController.h"

namespace engine
{

ComponentController::ComponentController(std::unique_ptr<IComponentProvider> p_componentProvider)
	:m_componentProvider(std::move(p_componentProvider))
{

}

bool ComponentController::attachComponent(Entity& p_entity, ComponentType p_componentType)
{
	if (not isComponentAlreadyAttached(p_entity, p_componentType))
	{
		attachComponentToEntity(p_entity, p_componentType);
		return true;
	}
	else
	{
		return false;
	}
}

bool ComponentController::attachMultipleComponents(Entity& p_entity, const ComponentIndicators& p_componentsRequestedToAttach)
{
	if (auto l_componentsToAttach = getComponentsWhichAreNotAlreadyAttached(p_entity, p_componentsRequestedToAttach); l_componentsToAttach.any())
	{
		attachMultipleComponentsToEntity(p_entity, l_componentsToAttach);
		return true;
	}
	else
	{
		return false;
	}
}

void ComponentController::attachMultipleComponentsToEntity(Entity& p_entity, const ComponentIndicators& p_componentsToAttach)
{
	if (hasOnlyOneComponentToProcess(p_componentsToAttach))
	{
		auto l_component = getSingleSetComponent(p_componentsToAttach);
		attachComponentToEntity(p_entity, l_component);
	}
	else
	{
		attachRequestedComponentsToEntity(p_entity, p_componentsToAttach);
	}
}

bool ComponentController::hasOnlyOneComponentToProcess(const ComponentIndicators& p_componentsToAttach) const
{
	return p_componentsToAttach.getNumOfSetComponents() == 1;
}

ComponentType ComponentController::getSingleSetComponent(const ComponentIndicators& p_componentsToAttach) const
{
	for (auto i = 0u; i < LAST_VALID_COMPONENT_INDEX; i++)
	{
		if (auto l_componentType = convertIndexToComponentType(i); p_componentsToAttach.isSet(l_componentType))
		{
			return l_componentType;
		}
	}

	return ComponentType::UndefinedComponent;
}

ComponentType ComponentController::convertIndexToComponentType(ComponentIndex p_index) const
{
	return static_cast<ComponentType>(p_index);
}

void ComponentController::attachRequestedComponentsToEntity(Entity& p_entity, const ComponentIndicators& p_componentsToAttach)
{
	auto l_positionForNextComponent = getNextFreeComponentPosition(p_entity);

	for (auto i = 0u; i < LAST_VALID_COMPONENT_INDEX; i++)
	{
		if (auto l_componentType = convertIndexToComponentType(i); p_componentsToAttach.isSet(l_componentType))
		{
			auto& l_newComponent = m_componentProvider->createComponent(l_componentType);
			attachComponentToPosition(l_newComponent, l_positionForNextComponent);
			p_entity.attachedComponents.flip(l_componentType);

			l_positionForNextComponent = getNextComponentPosition(l_newComponent);
		}
	}
}

ComponentIndicators ComponentController::getComponentsWhichAreNotAlreadyAttached(const Entity& p_entity, const ComponentIndicators& p_requestedComponents) const
{
	auto l_componentsToAttach = p_requestedComponents;
	l_componentsToAttach = l_componentsToAttach ^ p_entity.attachedComponents;
	l_componentsToAttach = l_componentsToAttach & p_requestedComponents;

	return l_componentsToAttach;
}

bool ComponentController::detachComponent(Entity& p_entity, ComponentType p_componentType)
{
	if (isComponentAlreadyAttached(p_entity, p_componentType))
	{
		detachComponentFromEntity(p_entity, p_componentType);
		return true;
	}
	else
	{
		return false;
	}
}

bool ComponentController::detachMultipleComponents(Entity& p_entity, const ComponentIndicators& p_componentsRequestedToDetach)
{
	if (auto l_componentsToDetach = getComponentsToDetach(p_entity, p_componentsRequestedToDetach); l_componentsToDetach.any())
	{
		detachMultipleComponentsFromEntity(p_entity, l_componentsToDetach);
		return true;
	}
	else
	{
		return false;
	}
}

void ComponentController::detachMultipleComponentsFromEntity(Entity& p_entity, const ComponentIndicators& p_componentsToDetach)
{
	if (hasOnlyOneComponentToProcess(p_componentsToDetach))
	{
		auto l_component = getSingleSetComponent(p_componentsToDetach);
		detachComponentFromEntity(p_entity, l_component);
	}
	else
	{
		detachRequestedComponentsFromEntity(p_entity, p_componentsToDetach);
	}
}

void ComponentController::detachRequestedComponentsFromEntity(Entity& p_entity, const ComponentIndicators& p_componentsToDetach)
{
	for (auto i = 0u; i < LAST_VALID_COMPONENT_INDEX; i++)
	{
		if (auto l_componentType = convertIndexToComponentType(i); p_componentsToDetach.isSet(l_componentType))
		{
			detachComponentFromEntity(p_entity, l_componentType);
		}
	}
}

ComponentIndicators ComponentController::getComponentsToDetach(const Entity& p_entity, const ComponentIndicators& p_requestedComponents) const
{
	return p_entity.attachedComponents & p_requestedComponents;
}

bool ComponentController::isComponentAlreadyAttached(Entity& p_entity, ComponentType p_componentType) const
{
	return p_entity.attachedComponents.isSet(p_componentType);
}

void ComponentController::attachComponentToEntity(Entity& p_entity, ComponentType p_componentType)
{
	auto& l_newComponent = m_componentProvider->createComponent(p_componentType);
	attachToNextFreePosition(p_entity, l_newComponent);
	p_entity.attachedComponents.flip(p_componentType);
}

void ComponentController::attachToNextFreePosition(Entity& p_entity, ComponentBase& p_newComponent)
{
	auto l_positionForNewComponent = getNextFreeComponentPosition(p_entity);
	attachComponentToPosition(p_newComponent, l_positionForNewComponent);
}

ComponentPtr* ComponentController::getNextFreeComponentPosition(Entity& p_entity) const
{
	ComponentPtr* l_ptrToComponentPosition = &p_entity.components;

	while (*l_ptrToComponentPosition != nullptr)
	{
		l_ptrToComponentPosition = getNextComponentPosition(**l_ptrToComponentPosition);
	}

	return l_ptrToComponentPosition;
}

ComponentPtr* ComponentController::getNextComponentPosition(ComponentBase& p_component) const
{
	return &p_component.nextComponent;
}

void ComponentController::attachComponentToPosition(ComponentBase& p_componentToAttach, ComponentPtr* p_position)
{
	*p_position = &p_componentToAttach;
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
			l_ptrToComponentPosition = getNextComponentPosition(l_component);
		}
	}

	p_entity.attachedComponents.flip(p_componentType);
}

}
