#include "ComponentController.h"

namespace engine
{
	ComponentBase& ComponentController::createComponent(ComponentType p_component)
	{
		return ComponentBase();
	}
}
