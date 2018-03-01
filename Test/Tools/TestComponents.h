#pragma once
#include "ComponentBase.h"

using namespace engine;

namespace testComponents
{
	struct ComponentA : public ComponentBase
	{
		const int value = 1;
	};

	struct ComponentB : public ComponentBase
	{
		const int value = 2;
	};
}