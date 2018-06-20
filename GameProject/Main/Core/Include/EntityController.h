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
					 IEntityChangeDistributor& p_changeDistributor);

	EntityId createEntity() override;
	EntityId createEntityWithComponents(const ComponentFlags& p_components) override;

	bool removeEntity(EntityId p_id) override;

	bool hasEntity(EntityId p_id) override;
	Entity& getEntity(EntityId p_id) override;

	bool connectSingleComponentToEntity(EntityId p_id, ComponentType p_componentType) override;
	bool disconnectSingleComponentFromEntity(EntityId p_id, ComponentType p_componentType) override;

	bool connectComponentsToEntity(EntityId p_id, const ComponentFlags& p_components) override;
	bool disconnectComponentsFromEntity(EntityId p_id, const ComponentFlags& p_components) override;

protected:
	std::unique_ptr<IEntityPool> m_pool;
	IComponentController& m_componentController;
	IEntityChangeDistributor& m_changeDistributor;

private:
	bool isComponentAlreadyAttachedToEntity(Entity& p_entity, ComponentType p_componentType);
	void attachComponent(Entity& p_entity, ComponentType p_componentType);
	void putComponentToNextFreePositionInEntity(Entity& p_entity, ComponentBase& p_component);
	ComponentPtr* getNextFreePositionForComponent(Entity& p_entity);
};

}