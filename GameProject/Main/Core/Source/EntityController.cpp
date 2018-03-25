#include "EntityController.h"
#include <iostream>

namespace engine
{

EntityController::EntityController(std::unique_ptr<IEntityPool> p_entityPool,
								   IComponentController& p_componentController,
								   const IEntityChangeDistributor& p_changeDistributor)
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

bool EntityController::remove(EntityId p_id)
{
	return m_pool->remove(p_id);
}

Entity& EntityController::get(EntityId p_id)
{
	if (!m_pool->hasId(p_id))
	{
		std::cerr << "Requestd ID does NOT exist in the pool! ID: " << p_id;
	}

	return m_pool->get(p_id);
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