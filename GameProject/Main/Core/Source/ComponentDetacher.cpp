#include "ComponentDetacher.h"

namespace engine
{

ComponentDetacher::ComponentDetacher(IComponentController& p_componentController)
	:m_componentController(p_componentController)
{
}

bool ComponentDetacher::detachComponent(Entity& p_entity, ComponentType p_componentType)
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

bool ComponentDetacher::isComponentAlreadyAttached(Entity& p_entity, ComponentType p_componentType) const
{
	return p_entity.attachedComponents.isAttached(p_componentType);
}

void ComponentDetacher::detachComponentFromEntity(Entity& p_entity, ComponentType p_componentType)
{
	ComponentPtr* l_ptrToComponentPosition = &p_entity.components;

	while (*l_ptrToComponentPosition != nullptr)
	{
		auto& l_component = **l_ptrToComponentPosition;
		if (l_component.type == p_componentType)
		{
			*l_ptrToComponentPosition = &(*l_component.nextComponent);
			m_componentController.removeComponent(l_component);
			break;
		}
		else
		{
			l_ptrToComponentPosition = &l_component.nextComponent;
		}
	}
}

bool ComponentDetacher::dettachMultipleComponents(Entity&, const ComponentFlags&)
{
	return false;
}

}