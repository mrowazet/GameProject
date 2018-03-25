#pragma once
#include <memory>
#include "IEntityController.h"
#include "IComponentController.h"
#include "EntityChangeDistributor.h"
#include "IdGuard.h"
#include "IEntityPool.h"

namespace engine
{

class EntityController : public IEntityController
{
public:
	EntityController(std::unique_ptr<IEntityPool> p_entityPool,
					 IComponentController& p_componentController,
					 const IEntityChangeDistributor& p_changeDistributor);

	EntityId createEntity() override;
	EntityId createEntityWithComponents(const ComponentFlags& p_components) override;

	bool remove(EntityId p_id) override;
	Entity& get(EntityId p_id) override;

	bool connectComponentsToEntity(EntityId p_id, const ComponentFlags& p_components) override;
	bool disconnectComponentsFromEntity(EntityId p_id, const ComponentFlags& p_components) override;

protected:
	std::unique_ptr<IEntityPool> m_pool;
	IComponentController& m_componentController;
	const IEntityChangeDistributor& m_changeDistributor;
};

}