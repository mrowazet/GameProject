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
	EntityId createEntityWithComponents(const ComponentFlags&) override;

	bool removeEntity(EntityId) override;

	bool hasEntity(EntityId) override;
	Entity& getEntity(EntityId) override;

	bool connectComponentToEntity(EntityId, ComponentType) override;
	bool disconnectComponentFromEntity(EntityId, ComponentType) override;

	bool connectMultipleComponentsToEntity(EntityId, const ComponentFlags&) override;
	bool disconnectMultipleComponentsFromEntity(EntityId, const ComponentFlags&) override;

protected:
	std::unique_ptr<IEntityPool> m_pool;
	std::unique_ptr<IComponentAttacher> m_componentAttacher;
	std::unique_ptr<IComponentDetacher> m_componentDetacher;
	IEntityChangeDistributor& m_changeDistributor;

private:
};

}