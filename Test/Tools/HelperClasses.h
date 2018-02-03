#pragma once
#include "Core.h"

namespace testTool
{

using namespace engine;

class CtorDtorCounter
{
public:
	CtorDtorCounter() = default;

	u32& getCtorCounter()
	{
		return m_ctorCounter;
	}

	u32& getDtorCounter()
	{
		return m_dtorCounter;
	}

	u32& getCopyCounter()
	{
		return m_copyCounter;
	}

private:
	u32 m_ctorCounter = 0u;
	u32 m_dtorCounter = 0u;
	u32 m_copyCounter = 0u;
};

class TestEntity
{
public:
	TestEntity() = default;

	TestEntity(CtorDtorCounter& p_counters, u32 p_id = 0u)
		:m_counters(&p_counters),
		 m_id(p_id)
	{
		auto& l_counter = m_counters->getCtorCounter();
		l_counter++;
	}

	TestEntity(const TestEntity& p_testEntity)
	{
		m_id = p_testEntity.m_id;
		m_counters = p_testEntity.m_counters;

		auto& l_counter = m_counters->getCopyCounter();
		l_counter++;
	}

	~TestEntity()
	{
		auto& l_counter = m_counters->getDtorCounter();
		l_counter++;
	}

	u32 getId() const
	{
		return m_id;
	}

private:
	u32 m_id = 0u;
	CtorDtorCounter* m_counters = nullptr;
};

}
