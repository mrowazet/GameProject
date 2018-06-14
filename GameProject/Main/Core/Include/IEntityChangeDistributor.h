#pragma once
#include "Types.h"
#include "Constants.h"
#include "Parameters.h"
#include "Entity.h"

namespace engine
{

class IEntityChangeDistributor
{
public:
	IEntityChangeDistributor() = default;
	virtual ~IEntityChangeDistributor() = default;

	virtual void entityChanged(EntityId p_id) = 0;
};

}