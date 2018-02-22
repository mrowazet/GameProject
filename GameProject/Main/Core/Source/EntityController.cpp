#include "EntityController.h"

namespace engine
{

EntityController::EntityController(PoolSize p_maxNrOfEntities)
	:m_entities(p_maxNrOfEntities),
	 m_idGuard(std::make_unique<IdGuard>(p_maxNrOfEntities))
{
}

EntityId EntityController::createEntity()
{
	return UNDEFINED_ENTITY_ID;
}

bool EntityController::removeEntity(EntityId p_id)
{
	return false;
}

Entity& EntityController::getEntity(EntityId p_id)
{
	return Entity();
}

}