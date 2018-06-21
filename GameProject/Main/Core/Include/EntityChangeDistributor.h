#pragma once
#include "IEntityChangeDistributor.h"

namespace engine
{

class EntityChangeDistributor : public IEntityChangeDistributor
{
public:

	void distributeEntityChange(EntityId p_id) override;

};

}