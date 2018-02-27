#pragma once
#include <memory>
#include "IEntityController.h"
#include "IComponentController.h"
#include "IdGuard.h"
#include "Pool.h"

namespace engine
{

class EntityController : public IEntityController
{
public:
	EntityController(PoolSize p_maxNrOfEntities);

	EntityId createEntity() override;
	EntityId createEntityWithComponents(const ComponentFlags& p_components) override;

	bool removeEntity(EntityId p_id) override;
	Entity& getEntity(EntityId p_id) override;

	bool connectComponentsToEntity(EntityId p_id, const ComponentFlags& p_components) override;
	bool disconnectComponentsFromEntity(EntityId p_id, const ComponentFlags& p_components) override;

protected:
	ContinuousPool<Entity> m_entities;
	std::unique_ptr<IIdGuard> m_idGuard;

	ContinuousPool<Entity>::Iter findEntityById(EntityId p_id);
};

}