#pragma once
#include "IEntityChangeDistributor.h"

namespace engine
{

class EntityChangeDistributor : public IEntityChangeDistributor
{
public:

	void entityChanged(EntityId p_id) override;

};

}