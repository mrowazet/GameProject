#pragma once
#include "Types.h"

namespace engine
{

class IEntityPool
{
public:
	IEntityPool() = default;
	virtual ~IEntityPool() = default;

	virtual Entity& create() = 0;
	virtual bool removeEntity(EntityId) = 0;
	virtual Entity& getEntity(EntityId) = 0;

	virtual u32 size() const = 0;
	virtual bool hasEntity(EntityId) const = 0;

	virtual void clear() = 0;
};

}