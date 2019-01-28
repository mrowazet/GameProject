#pragma once
#include "Types.h"
#include "Constants.h"
#include "Parameters.h"
#include "ComponentTypes.h"
#include "ComponentBase.h"

namespace engine
{

class IComponentProvider
{
public:
	IComponentProvider() = default;
	virtual ~IComponentProvider() = default;

	virtual ComponentBase& createComponent(ComponentType) = 0;
	virtual bool removeComponent(ComponentBase&) = 0;
};

}