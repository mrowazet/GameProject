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
	Pool(const Pool&) = delete;
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
	friend class ContinuousPool<ElementType>;

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

	ElementType* operator->()
	{
		return m_poolElement;
	}

	const ElementType* operator->() const
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

	ContinuousPoolForwardIterator<ElementType>& operator+=(u32 p_offset)
	{
		m_poolElement += p_offset;
		return *this;
	}

	const ContinuousPoolForwardIterator<ElementType>& operator+=(u32 p_offset) const
	{
		m_poolElement += p_offset;
		return *this;
	}

	ContinuousPoolForwardIterator<ElementType> operator+(u32 p_offset) const
	{
		ElementType* l_elementAddress = m_poolElement + p_offset;
		ContinuousPoolForwardIterator<ElementType> l_iter(*l_elementAddress);
		
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

	ContinuousPoolForwardIterator<ElementType>& operator=(const ContinuousPoolForwardIterator<ElementType>& p_iter)
	{
		m_poolElement = p_iter.m_poolElement;
		return *this;
	}

private:
	mutable ElementType* m_poolElement = nullptr;

};


template<typename TypedContinuousPoolForwardIterator>
class ContinuousPoolForwardConstIterator
{
private:
	template<typename ElementType>
	friend class ContinuousPool;

	ContinuousPoolForwardConstIterator(const TypedContinuousPoolForwardIterator& p_iter)
		:m_iter(p_iter)
	{

	}

public:
	ContinuousPoolForwardConstIterator(const ContinuousPoolForwardConstIterator&) = default;
	~ContinuousPoolForwardConstIterator() = default;

	auto&& operator*() const
	{
		return *m_iter;
	}

	auto&& operator->() const
	{
		return m_iter;
	}

	ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>& operator++()
	{
		m_iter++;
		return *this;
	}

	const ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>& operator++() const
	{
		m_iter++;
		return *this;
	}

	ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator> operator++(int)
	{
		TypedContinuousPoolForwardIterator l_iter(m_iter);
		m_iter++;

		return l_iter;
	}

	const ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator> operator++(int) const
	{
		TypedContinuousPoolForwardIterator l_iter(m_iter);
		m_iter++;

		return l_iter;
	}

	ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>& operator+=(u32 p_offset)
	{
		m_iter += p_offset;
		return *this;
	}

	const ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>& operator+=(u32 p_offset) const
	{
		m_iter += p_offset;
		return *this;
	}

	ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator> operator+(u32 p_offset) const
	{
		TypedContinuousPoolForwardIterator l_iter(m_iter);
		l_iter += p_offset;

		ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>l_citer (l_iter);

		return l_citer;
	}

	ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>& operator=(const ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>& p_iter)
	{
		l_iter = p_iter;
		return *this;
	}

	bool operator!=(const ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>& p_iter) const
	{
		return m_iter != p_iter.m_iter;
	}

	bool operator==(const ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>& p_iter) const
	{
		return m_iter == p_iter.m_iter;
	}

private:
	const TypedContinuousPoolForwardIterator m_iter;
};


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

	PoolSize maxSize() const
	{
		return MAX_NR_OF_ELEMENTS;
	}

	u32 size() const
	{
		return m_nrOfStoredElements;
	}

	void clear()
	{
		m_positionAfterLastElement = getPtrToBeginning();
		m_nrOfStoredElements = 0u;
	}

	bool isEmpty() const
	{
		return m_nrOfStoredElements == 0u;
	}

	ContinuousPoolForwardIterator<ElementType> begin()
	{
		return ContinuousPoolForwardIterator<ElementType>(*getPtrToBeginning());
	}

	ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>> begin() const
	{
		return ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>>(*getPtrToBeginning());
	}

	ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>> cbegin() const
	{
		return ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>>(*getPtrToBeginning());
	}

	ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>> end() const
	{
		return ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>>(*m_positionAfterLastElement);
	}

	ContinuousPoolForwardIterator<ElementType> end()
	{
		return ContinuousPoolForwardIterator<ElementType>(*m_positionAfterLastElement);
	}

	ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>> cend() const
	{
		return ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>>(*m_positionAfterLastElement);
	}

private:
	static const int ELEMENT_SIZE = sizeof(ElementType);
	const PoolSize MAX_NR_OF_ELEMENTS;

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
	
	ElementType* getPtrToBeginning() const
	{
		return reinterpret_cast<ElementType*>(&(m_memoryPool->at(0)));
	}
};


}