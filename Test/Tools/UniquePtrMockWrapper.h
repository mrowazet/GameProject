#pragma once
#include <memory>
#include "assert.h"

namespace testTool
{

template<typename Mock>
class UniquePtrMockWrapper
{
public:
	UniquePtrMockWrapper()
		: m_uniqueMockPtr(std::make_unique<Mock>()),
		  m_mock(*m_uniqueMockPtr)
	{
	}
		 
	auto getPtr()
	{	
		assert(m_uniqueMockPtr && "Ownership is already moved!");
		return std::move(m_uniqueMockPtr);
	}

	Mock& operator*()
	{
		return m_mock;
	}

private:
	std::unique_ptr<Mock> m_uniqueMockPtr;
	Mock& m_mock;

};


}