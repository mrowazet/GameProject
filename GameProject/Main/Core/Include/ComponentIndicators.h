#pragma once
#include "Types.h"
#include "Constants.h"
#include "ComponentTypes.h"

namespace engine
{

class ComponentIndicators final
{
public:
	ComponentIndicators() = default;

	void flip(ComponentType p_type)
	{
		m_componentFlags.flip(static_cast<int>(p_type));
	}

	bool isSet(ComponentType p_type) const
	{
		return m_componentFlags.test(static_cast<int>(p_type));
	}

	bool hasAny() const
	{
		return m_componentFlags.any();
	}

	u32 getNumOfSetComponents() const
	{
		return static_cast<u32>(m_componentFlags.count());
	}

	bool operator==(const ComponentIndicators& p_componentIndicators)
	{
		return m_componentFlags == p_componentIndicators.m_componentFlags;
	}

	bool operator!=(const ComponentIndicators& p_componentIndicators)
	{
		return m_componentFlags != p_componentIndicators.m_componentFlags;
	}

	ComponentIndicators& operator=(const ComponentIndicators& p_componentIndicators)
	{
		m_componentFlags = p_componentIndicators.m_componentFlags;
		return *this;
	}

private:
	ComponentFlags m_componentFlags = NO_COMPONENTS;
};

}