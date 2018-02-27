#pragma once
#include "Types.h"
#include "Constants.h"
#include "Parameters.h"
#include "ComponentTypes.h"
#include "Entity.h"

namespace engine
{

class IEntityController
{
public:
	IEntityController() = default;
	~IEntityController() = default;

	virtual EntityId createEntity() = 0;
	virtual EntityId createEntityWithComponents(const ComponentFlags& p_components) = 0;

	virtual bool removeEntity(EntityId p_id) = 0;
	virtual Entity& getEntity(EntityId p_id) = 0;
	
	virtual bool connectComponentsToEntity(EntityId p_id, const ComponentFlags& p_components) = 0;
	virtual bool disconnectComponentsFromEntity(EntityId p_id, const ComponentFlags& p_components) = 0;
};

}
