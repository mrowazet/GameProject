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
	virtual void deallocate(ElementType& p_element) = 0;

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

	ContinuousPoolForwardIterator(ElementType& p_element)
	{
		m_poolElement = &p_element;
	}

public:
	ContinuousPoolForwardIterator(const ContinuousPoolForwardIterator&) = default;
	~ContinuousPoolForwardIterator() = default;

	ElementType& operator*()
	{
		return *m_poolElement;
	}

	const ElementType& operator*() const
	{
		return *m_poolElement;
	}

	ElementType& operator->()
	{
		return m_poolElement;
	}

	const ElementType& operator->() const
	{

		return m_poolElement;
	}

	ContinuousPoolForwardIterator<ElementType>& operator++()
	{
		m_poolElement++;
		return *this;
	}

	const ContinuousPoolForwardIterator<ElementType>& operator++() const
	{
		m_poolElement++;
		return *this;
	}

	ContinuousPoolForwardIterator<ElementType> operator++(int)
	{
		ContinuousPoolForwardIterator<ElementType> l_iter(*this);
		m_poolElement++;

		return l_iter;
	}

	const ContinuousPoolForwardIterator<ElementType> operator++(int) const
	{
		ContinuousPoolForwardIterator<ElementType> l_iter(*this);
		m_poolElement++;

		return l_iter;
	}

	bool operator!=(const ContinuousPoolForwardIterator<ElementType>& p_iter) const
	{
		return m_poolElement != p_iter.m_poolElement;
	}

	bool operator==(const ContinuousPoolForwardIterator<ElementType>& p_iter) const
	{
		return m_poolElement == p_iter.m_poolElement;
	}

	ContinuousPoolForwardIterator<ElementType>& operator=(const ContinuousPoolForwardIterator& p_iter)
	{
		m_poolElement = p_iter.m_poolElement;
		return *this;
	}

private:
	mutable ElementType* m_poolElement = nullptr;

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
		m_memoryPool = std::make_unique<core::MemoryPool>();
		m_memoryPool->resize(p_size * (ELEMENT_SIZE / sizeof(core::MemoryAllocationUnit)));

		m_positionAfterLastElement = getPtrToBeginning();

		if (p_useDefaultInit)
			initElements();
	}

	ElementType& allocate() override
	{
		ElementType* l_newElement = nullptr;

		l_newElement = new (m_positionAfterLastElement) ElementType();

		m_positionAfterLastElement++;
		m_nrOfStoredElements++;

		return *l_newElement;
	}

	void deallocate(ElementType& p_element) override
	{
		ElementType* l_element = &p_element;

		m_positionAfterLastElement--;
		m_nrOfStoredElements--;

		std::memcpy(l_element, m_positionAfterLastElement, ELEMENT_SIZE);
	}

	ContinuousPoolForwardIterator<ElementType> begin()
	{
		return ContinuousPoolForwardIterator<ElementType>(getPtrToBeginning());
	}

	ContinuousPoolForwardConstIterator<ElementType> cbegin() const
	{
		return ContinuousPoolForwardConstIterator<ElementType>(getPtrToBeginning());
	}

	ContinuousPoolForwardIterator<ElementType> end()
	{
		return ContinuousPoolForwardIterator<ElementType>(*m_positionAfterLastElement);
	}

	ContinuousPoolForwardConstIterator<ElementType> cend() const
	{
		return ContinuousPoolForwardConstIterator<ElementType>(*m_positionAfterLastElement);
	}

private:
	static const int ELEMENT_SIZE = sizeof(ElementType);
	const u32 MAX_NR_OF_ELEMENTS;

	std::unique_ptr<core::MemoryPool> m_memoryPool;
	u32 m_nrOfStoredElements = 0u;

	ElementType* m_positionAfterLastElement = nullptr;

	void initElements()
	{
		ElementType* l_element = nullptr;

		for (auto i = 0u; i < MAX_NR_OF_ELEMENTS; i++)
		{
			l_element = new (m_positionAfterLastElement) ElementType();
			m_positionAfterLastElement++;
		}

		m_positionAfterLastElement = getPtrToBeginning();
	}
	
	ElementType* getPtrToBeginning()
	{
		return reinterpret_cast<ElementType*>(&(m_memoryPool->at(0)));
	}

	const ElementType* getPtrToBeginning() const
	{
		return getPtrToBeginning();
	}
};


}