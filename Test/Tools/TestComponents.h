#pragma once
#include "ComponentBase.h"

using namespace engine;

namespace testComponents
{
	struct ComponentA : public ComponentBase
	{
		int a = 1;
	};

	struct ComponentB : public ComponentBase
	{

	};
}