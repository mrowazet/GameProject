#include "EntityController.h"
#include <iostream>

namespace engine
{

EntityController::EntityController(std::unique_ptr<IEntityPool> p_entityPool,
								   IComponentController& p_componentController,
								   IEntityChangeDistributor& p_changeDistributor)
	:m_pool(std::move(p_entityPool)),
	 m_componentController(p_componentController),
	 m_changeDistributor(p_changeDistributor)
{
}

EntityId EntityController::createEntity()
{
	auto& l_entity = m_pool->create();
	return l_entity.id;
}

EntityId EntityController::createEntityWithComponents(const ComponentFlags& p_components)
{
	return UNDEFINED_ENTITY_ID;
}

bool EntityController::removeEntity(EntityId p_id)
{
	return m_pool->removeEntity(p_id);
}

bool EntityController::hasEntity(EntityId p_id)
{
	return m_pool->hasId(p_id);
}

Entity& EntityController::getEntity(EntityId p_id)
{
	return m_pool->getEntity(p_id);
}

bool EntityController::connectComponentToEntity(EntityId p_id, ComponentType p_componentType)
{
	auto& l_entity = m_pool->getEntity(p_id);
	
	if(!isComponentAlreadyAttachedToEntity(l_entity, p_componentType))
	{
		attachComponent(l_entity, p_componentType);
		m_changeDistributor.distributeEntityChange(p_id);
		
		return true;
	}
	else
	{
		return false;
	}
}

bool EntityController::isComponentAlreadyAttachedToEntity(Entity& p_entity, ComponentType p_componentType)
{
	if (p_entity.components == nullptr)
	{
		return false;
	}

	return p_entity.attachedComponents.isAttached(p_componentType);
}

void EntityController::attachComponent(Entity& p_entity, ComponentType p_componentType)
{
	p_entity.attachedComponents.flip(p_componentType);

	auto& l_component = m_componentController.createComponent(p_componentType);
	putComponentToNextFreePositionInEntity(p_entity, l_component);
}

void EntityController::putComponentToNextFreePositionInEntity(Entity& p_entity, ComponentBase& p_component)
{
	auto l_freeComponentPosition = getNextFreePositionForComponent(p_entity);
	*l_freeComponentPosition = &p_component;
}

ComponentPtr* EntityController::getNextFreePositionForComponent(Entity& p_entity)
{
	ComponentPtr* l_ptrToComponentPosition = &p_entity.components;

	while (*l_ptrToComponentPosition != nullptr)
	{
		l_ptrToComponentPosition = &(*l_ptrToComponentPosition)->nextComponent;
	}

	return l_ptrToComponentPosition;
}

bool EntityController::disconnectComponentFromEntity(EntityId p_id, ComponentType p_componentType)
{
	auto& l_entity = m_pool->getEntity(p_id);

	if (isComponentAlreadyAttachedToEntity(l_entity, p_componentType))
	{
		detachComponent(l_entity, p_componentType);
		m_changeDistributor.distributeEntityChange(p_id);
		return true;
	}
	else
	{
		return false;
	}
}

void EntityController::detachComponent(Entity& p_entity, ComponentType p_componentType)
{
	p_entity.attachedComponents.flip(p_componentType);

	ComponentPtr* l_ptrToComponentPosition = &p_entity.components;

	while(*l_ptrToComponentPosition != nullptr)
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

bool EntityController::connectMultipleComponentsToEntity(EntityId p_id, const ComponentFlags& p_components)
{
	return false;
}

bool EntityController::disconnectMultipleComponentsFromEntity(EntityId p_id, const ComponentFlags& p_components)
{
	return false;
}

}