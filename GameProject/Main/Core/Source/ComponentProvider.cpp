#include "ComponentProvider.h"

namespace engine
{

ComponentBase& ComponentProvider::createComponent(ComponentType)
{
	return ComponentBase(ComponentType::POSITION);
}

bool ComponentProvider::removeComponent(ComponentBase&)
{
	return false;
}

}