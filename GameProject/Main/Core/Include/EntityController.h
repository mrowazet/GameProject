#pragma once
#include <memory>
#include "IEntityController.h"
#include "EntityChangeDistributor.h"
#include "IdGuard.h"
#include "IEntityPool.h"
#include "IComponentAttacher.h"
#include "IComponentDetacher.h"

namespace engine
{

class EntityController : public IEntityController
{
public:
	EntityController(std::unique_ptr<IEntityPool> p_entityPool,
					 std::unique_ptr<IComponentAttacher> p_componentAttacher,
					 std::unique_ptr<IComponentDetacher> p_componentDetacher,
					 IEntityChangeDistributor& p_changeDistributor);

	EntityId createEntity() override;
	EntityId createEntityWithComponents(const ComponentFlags& p_components) override;

	bool removeEntity(EntityId p_id) override;

	bool hasEntity(EntityId p_id) override;
	Entity& getEntity(EntityId p_id) override;

	bool connectComponentToEntity(EntityId p_id, ComponentType p_componentType) override;
	bool disconnectComponentFromEntity(EntityId p_id, ComponentType p_componentType) override;

	bool connectMultipleComponentsToEntity(EntityId p_id, const ComponentFlags& p_components) override;
	bool disconnectMultipleComponentsFromEntity(EntityId p_id, const ComponentFlags& p_components) override;

protected:
	std::unique_ptr<IEntityPool> m_pool;
	std::unique_ptr<IComponentAttacher> m_componentAttacher;
	std::unique_ptr<IComponentDetacher> m_componentDetacher;
	IEntityChangeDistributor& m_changeDistributor;

private:
};

}