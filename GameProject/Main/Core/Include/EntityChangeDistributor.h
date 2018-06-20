#pragma once
#include "IEntityChangeDistributor.h"

namespace engine
{

class EntityChangeDistributor : public IEntityChangeDistributor
{
public:

	void distributeInfoAboutChangeInEntity(EntityId p_id) override;

};

}