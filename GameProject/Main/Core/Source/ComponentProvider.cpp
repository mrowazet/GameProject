#include "ComponentProvider.h"

namespace engine
{

ComponentBase& ComponentProvider::createComponent(ComponentType)
{
	return ComponentBase(ComponentType::POSITION);
}

bool removeComponent(ComponentBase&)
{
	return false;
}

}