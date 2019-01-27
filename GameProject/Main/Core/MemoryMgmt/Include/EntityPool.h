#pragma once
#include "IEntityPool.h"
#include <set>
#include <memory>
#include "Pool.h"
#include "IIdGuard.h"

namespace engine
{

class EntityPool : IEntityPool
{
public:
	EntityPool(PoolSize, std::unique_ptr<IIdGuard>);

	Entity& create() override;
	bool removeEntity(EntityId) override;
	Entity& getEntity(EntityId) override;

	u32 size() const override;
	bool hasEntity(EntityId) const override;

	void clear() override;

protected:
	ContinuousPool<Entity> m_pool;
	std::unique_ptr<IIdGuard> m_idGuard;
	std::set<EntityId> m_storedIds;
};

}