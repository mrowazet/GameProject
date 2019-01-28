#include "EntityController.h"
#include <iostream>

namespace engine
{

EntityController::EntityController(std::unique_ptr<IEntityPool> p_entityPool,
								   std::unique_ptr<IComponentController> p_componentController,
								   IEntityChangeDistributor& p_changeDistributor)
	:m_pool(std::move(p_entityPool)),
     m_componentController(std::move(p_componentController)),
	 m_changeDistributor(p_changeDistributor)
{
}

EntityId EntityController::createEntity()
{
	auto& l_entity = m_pool->create();
	return l_entity.id;
}

EntityId EntityController::createEntityWithComponents(const ComponentIndicators& p_components)
{
	return UNDEFINED_ENTITY_ID;
}

bool EntityController::removeEntity(EntityId p_id)
{
	return m_pool->removeEntity(p_id);
}

bool EntityController::hasEntity(EntityId p_id)
{
	return m_pool->hasEntity(p_id);
}

Entity& EntityController::getEntity(EntityId p_id)
{
	return m_pool->getEntity(p_id);
}

bool EntityController::connectComponentToEntity(EntityId p_id, ComponentType p_componentType)
{
	auto& l_entity = m_pool->getEntity(p_id);
	
	if(m_componentController->attachComponent(l_entity, p_componentType))
	{
		m_changeDistributor.distributeEntityChange(p_id);
		return true;
	}
	else
	{
		return false;
	}
}

bool EntityController::disconnectComponentFromEntity(EntityId p_id, ComponentType p_componentType)
{
	auto& l_entity = m_pool->getEntity(p_id);

	if (m_componentController->detachComponent(l_entity, p_componentType))
	{
		m_changeDistributor.distributeEntityChange(p_id);
		return true;
	}
	else
	{
		return false;
	}
}

bool EntityController::connectMultipleComponentsToEntity(EntityId p_id, const ComponentIndicators& p_components)
{
	auto& l_entity = m_pool->getEntity(p_id);

	if (m_componentController->attachMultipleComponents(l_entity, p_components))
	{
		m_changeDistributor.distributeEntityChange(p_id);
		return true;
	}
	else
	{
		return false;
	}
}

bool EntityController::disconnectMultipleComponentsFromEntity(EntityId p_id, const ComponentIndicators& p_components)
{
	auto& l_entity = m_pool->getEntity(p_id);

	if (m_componentController->dettachMultipleComponents(l_entity, p_components))
	{
		m_changeDistributor.distributeEntityChange(p_id);
		return true;
	}
	else
	{
		return false;
	}
}

}