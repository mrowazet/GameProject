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

	void set(ComponentType p_type, bool p_isSet = true)
	{
		m_componentFlags.set(static_cast<int>(p_type), p_isSet);
	}

	bool isSet(ComponentType p_type) const
	{
		return m_componentFlags.test(static_cast<int>(p_type));
	}

	bool any() const
	{
		return m_componentFlags.any();
	}

	bool none() const
	{
		return m_componentFlags.none();
	}

	void reset()
	{
		m_componentFlags.reset();
	}

	u32 getNumOfSetComponents() const
	{
		return static_cast<u32>(m_componentFlags.count());
	}

	bool operator==(const ComponentIndicators& p_componentIndicators) const
	{
		return m_componentFlags == p_componentIndicators.m_componentFlags;
	}

	bool operator!=(const ComponentIndicators& p_componentIndicators) const
	{
		return m_componentFlags != p_componentIndicators.m_componentFlags;
	}

	ComponentIndicators& operator=(const ComponentIndicators& p_componentIndicators)
	{
		m_componentFlags = p_componentIndicators.m_componentFlags;
		return *this;
	}

	ComponentIndicators& operator&=(const ComponentIndicators& p_componentIndicators)
	{
		m_componentFlags = m_componentFlags & p_componentIndicators.m_componentFlags;
		return *this;
	}

	ComponentIndicators& operator^=(const ComponentIndicators& p_componentIndicators)
	{
		m_componentFlags = m_componentFlags ^ p_componentIndicators.m_componentFlags;
		return *this;
	}

	ComponentIndicators operator^(const ComponentIndicators& p_componentIndicators) const
	{
		ComponentIndicators l_result;
		l_result.m_componentFlags = m_componentFlags ^ p_componentIndicators.m_componentFlags;
		return l_result;
	}

	ComponentIndicators operator&(const ComponentIndicators& p_componentIndicators) const
	{
		ComponentIndicators l_result;
		l_result.m_componentFlags = m_componentFlags & p_componentIndicators.m_componentFlags;
		return l_result;
	}

private:
	ComponentFlags m_componentFlags = NO_COMPONENTS;
};

}