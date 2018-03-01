#pragma once
#include "Types.h"
#include "IComponentPool.h"

namespace engine
{

template<typename ComponentStruct>
class ComponentPool : public IComponentPool
{
public:
	ComponentPool(PoolSize p_size)
		:m_pool(p_size)
	{

	}

	~ComponentPool() = default;

	ComponentBase& getComponent()
	{
		return m_pool.getNext();
	}

	void returnComponent(ComponentBase& p_component)
	{
		m_pool.takeBack(static_cast<ComponentStruct&>(p_component));
	}

	u32 size() const override
	{
		return m_pool.size();
	}

private:
	ContinuousPool<ComponentStruct> m_pool;
};

}