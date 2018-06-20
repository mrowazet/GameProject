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

bool EntityController::connectSingleComponentToEntity(EntityId p_id, ComponentType p_componentType)
{
	auto& l_entity = m_pool->getEntity(p_id);
	
	if(!isComponentAlreadyAttachedToEntity(l_entity, p_componentType))
	{
		attachComponent(l_entity, p_componentType);
		m_changeDistributor.distributeInfoAboutChangeInEntity(p_id);
		
		return true;
	}
	else
	{
		return false;
	}
}

bool EntityController::isComponentAlreadyAttachedToEntity(Entity& p_entity, ComponentType p_componentType)
{
	return p_entity.attachedComponents.test(static_cast<int>(p_componentType));
}

void EntityController::attachComponent(Entity& p_entity, ComponentType p_componentType)
{
	p_entity.attachedComponents.flip(static_cast<int>(p_componentType));

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
	ComponentPtr* l_componentPtr = &p_entity.components;

	while (*l_componentPtr != nullptr)
	{
		l_componentPtr = &(*l_componentPtr)->nextComponent;
	}

	return l_componentPtr;
}

bool EntityController::disconnectSingleComponentFromEntity(EntityId p_id, ComponentType p_componentType)
{
	return false;
}

bool EntityController::connectComponentsToEntity(EntityId p_id, const ComponentFlags& p_components)
{
	return false;
}

bool EntityController::disconnectComponentsFromEntity(EntityId p_id, const ComponentFlags& p_components)
{
	return false;
}

}