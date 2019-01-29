#pragma once
#include "ComponentBase.h"

using namespace engine;

namespace testComponents
{
	struct ComponentA : public ComponentBase
	{
		ComponentA()
			:ComponentBase(ComponentType::POSITION)
		{
		}

		const int value = 1;
	};

	struct ComponentB : public ComponentBase
	{
		ComponentB()
			:ComponentBase(ComponentType::MOVABLE)
		{
		}

		const int value = 2;
	};

	struct ComponentC : public ComponentBase
	{
		ComponentC()
			:ComponentBase(ComponentType::VISIBLE)
		{
		}

		const int value = 3;
	};
}