#include "EntityController.h"
#include <iostream>

namespace engine
{

EntityController::EntityController(PoolSize p_maxNrOfEntities,
								   IComponentController& p_componentController,
								   const IEntityChangeDistributor& p_changeDistributor)
	:m_entities(p_maxNrOfEntities),
	 m_idGuard(std::make_unique<IdGuard>(p_maxNrOfEntities)),
	 m_componentController(p_componentController),
	 m_changeDistributor(p_changeDistributor)
{
}

EntityId EntityController::createEntity()
{
	auto& l_entity = m_entities.getNext();
	l_entity.id = m_idGuard->getNextId();

	return l_entity.id;
}

EntityId EntityController::createEntityWithComponents(const ComponentFlags& p_components)
{
	return UNDEFINED_ENTITY_ID;
}

bool EntityController::removeEntity(EntityId p_id)
{
	auto l_entityIter = findEntityById(p_id);

	if (l_entityIter != m_entities.end())
	{
		m_idGuard->freeId(l_entityIter->id);
		m_entities.takeBack(*l_entityIter);
		return true;
	}
	
	return false;
}

Entity& EntityController::getEntity(EntityId p_id)
{
	return *findEntityById(p_id);
}

ContinuousPool<Entity>::Iter EntityController::findEntityById(EntityId p_id)
{
	auto l_iter = m_entities.begin();
	auto l_endIter = m_entities.end();

	for (l_iter; l_iter != l_endIter; l_iter++)
	{
		if (l_iter->id == p_id)
		{
			break;
		}
	}

	if (l_iter == l_endIter)
	{
		std::cerr << "Cant't find Entity with selected id: " << p_id << "!\n";
	}

	return l_iter;
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