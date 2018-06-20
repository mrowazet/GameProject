#pragma once
#include "Types.h"
#include "Constants.h"
#include "ComponentTypes.h"

namespace engine
{

class AttachedComponents final
{
public:
	AttachedComponents() = default;

	void flip(ComponentType p_type)
	{
		m_flags.flip(static_cast<int>(p_type));
	}

	bool isAttached(ComponentType p_type) const
	{
		return m_flags.test(static_cast<int>(p_type));
	}

	bool hasAny() const
	{
		return m_flags.any();
	}

	u32 getNumOfSetComponents() const
	{
		return static_cast<u32>(m_flags.count());
	}

private:
	ComponentFlags m_flags = NO_COMPONENTS;
};


struct Entity final
{
	Entity() = default;
	~Entity() = default;
	Entity(const Entity&) = default;

	Entity(EntityId p_id)
		:id(p_id)
	{
	}

	EntityId id = UNDEFINED_ENTITY_ID;
	AttachedComponents attachedComponents;
	ComponentPtr components = nullptr;
};



}