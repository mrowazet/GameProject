#pragma once
#include <vector>
#include <memory>
#include <new>
#include "Types.h"
#include "Constants.h"
#include "Entity.h"
#include "Component.h"

namespace engine
{

namespace core
{
	using MemoryAllocationUnit = unsigned int;
	using MemoryPool = std::vector<MemoryAllocationUnit>;

}

template<typename ElementType>
class Pool
{
public:
	Pool() = default;
	virtual ~Pool() = default;

	virtual ElementType& allocate() = 0;
	virtual void deallocate() = 0;

	//todo begin end cbegin cend
	//todo parent class for iterators
};

template<typename>
class ContinuousPool;

template<typename ElementType>
class ContinuousPoolForwardIterator
{
private:
	friend class Pool<ElementType>;


};

template<typename ElementType>
using ContinuousPoolForwardConstIterator = const ContinuousPoolForwardIterator<ElementType>;


template<typename ElementType>
class ContinuousPool : public Pool<ElementType>
{
public:
	ContinuousPool(PoolSize p_size, bool p_useDefaultInit = true)
		:MAX_NR_OF_ELEMENTS(p_size)
	{

	}

private:
	static const int ELEMENT_SIZE = sizeof(ElementType);
	const u32 MAX_NR_OF_ELEMENTS;

	std::unique_ptr<core::MemoryPool> m_memoryPool;
	u32 m_nrOfStoredElements = 0u;

	ElementType* m_positionAfterLastElement;

	ElementType* getPtrToBeginning()
	{
		return reinterpret_cast<ElementType*>(&(m_memoryPool->at(0)));
	}

	const ElementType* getPtrToBeginning() const
	{
		return getPtrToBeginning();
	}
	
	ElementType& allocate() override
	{
		return *getPtrToBeginning();
	}

	void deallocate() override
	{

	}

};


}