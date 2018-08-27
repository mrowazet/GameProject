#include "ComponentAttacher.h"

namespace engine
{

ComponentAttacher::ComponentAttacher(IComponentController& p_componentController)
	:m_componentController(p_componentController)
{
}

bool ComponentAttacher::attachComponent(Entity& p_entity, ComponentType p_componentType)
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

bool ComponentAttacher::isComponentAlreadyAttached(Entity& p_entity, ComponentType p_componentType)
{
	if (p_entity.components == nullptr)
	{
		return false;
	}

	return p_entity.attachedComponents.isAttached(p_componentType);
}

void ComponentAttacher::attachComponentToEntity(Entity& p_entity, ComponentType p_componentType)
{
	auto& l_newComponent = m_componentController.createComponent(p_componentType);
	addComponentToNextFreePositionInEntity(p_entity, l_newComponent);
}

void ComponentAttacher::addComponentToNextFreePositionInEntity(Entity& p_entity, ComponentBase& p_newComponent)
{
	auto l_positionForNewComponent = getNextFreePositionForComponent(p_entity);
	*l_positionForNewComponent = &p_newComponent;
}

ComponentPtr* ComponentAttacher::getNextFreePositionForComponent(Entity& p_entity)
{
	ComponentPtr* l_ptrToComponentPosition = &p_entity.components;

	while (*l_ptrToComponentPosition != nullptr)
	{
		l_ptrToComponentPosition = &(*l_ptrToComponentPosition)->nextComponent;
	}

	return l_ptrToComponentPosition;
}

}