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
	EntityPool(PoolSize p_poolSize, std::unique_ptr<IIdGuard> p_guard);

	Entity& create() override;
	bool remove(EntityId p_id) override;
	Entity& get(EntityId p_id) override;

	u32 size() const override;
	bool hasId(EntityId p_id) const override;

	void clear() override;

protected:
	ContinuousPool<Entity> m_pool;
	std::unique_ptr<IIdGuard> m_idGuard;
	std::set<EntityId> m_storedIds;
};

}