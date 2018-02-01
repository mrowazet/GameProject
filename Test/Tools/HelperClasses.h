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

private:
	u32 m_ctorCounter = 0u;
	u32 m_dtorCounter = 0u;
};

class TestEntity
{
public:
	TestEntity(CtorDtorCounter& p_counters)
		:m_counters(p_counters)
	{
		auto& l_counter = m_counters.getCtorCounter();
		l_counter++;
	}

	~TestEntity()
	{
		auto& l_counter = m_counters.getDtorCounter();
		l_counter++;
	}

private:
	CtorDtorCounter& m_counters;
};

}
