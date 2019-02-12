#pragma once
#include <memory>
#include "IEntityController.h"
#include "EntityChangeDistributor.h"
#include "IdGuard.h"
#include "IEntityPool.h"
#include "IComponentController.h"

namespace engine
{

class EntityController : public IEntityController
{
public:
	EntityController(std::unique_ptr<IEntityPool>,
					 std::unique_ptr<IComponentController>,
					 IEntityChangeDistributor&);

	EntityId createEntity() override;
	EntityId createEntityWithComponents(const ComponentIndicators&) override;

	bool removeEntity(EntityId) override;

	bool hasEntity(EntityId) override;
	Entity& getEntity(EntityId) override;

	bool connectComponentToEntity(EntityId, ComponentType) override;
	bool disconnectComponentFromEntity(EntityId, ComponentType) override;

	bool connectMultipleComponentsToEntity(EntityId, const ComponentIndicators&) override;
	bool disconnectMultipleComponentsFromEntity(EntityId, const ComponentIndicators&) override;

private:
	void disconnectAllComponentsFromEntity(EntityId);
	bool connectMultipleComponents(Entity&, const ComponentIndicators&);
	bool disconnectMultipleComponents(Entity&, const ComponentIndicators&);

	std::unique_ptr<IEntityPool> m_pool;
	std::unique_ptr<IComponentController> m_componentController;
	IEntityChangeDistributor& m_changeDistributor;

};

}