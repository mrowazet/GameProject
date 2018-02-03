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
	using MemoryAllocationUnit = std::uintptr_t;
	using MemoryPool = std::vector<MemoryAllocationUnit>;
}

template<typename>
class ContinuousPool;

template<typename ElementType>
class ContinuousPoolForwardIterator
{
private:
	template<typename ElementType>
	friend class ContinuousPool;

	ContinuousPoolForwardIterator(ElementType& p_element)
	{
		m_poolElement = &p_element;
	}

public:
	using Iter = ContinuousPoolForwardIterator<ElementType>;

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

	Iter& operator++()
	{
		m_poolElement++;
		return *this;
	}

	const Iter& operator++() const
	{
		m_poolElement++;
		return *this;
	}

	Iter operator++(int)
	{
		Iter l_iter(*this);
		m_poolElement++;

		return l_iter;
	}

	const Iter operator++(int) const
	{
		Iter l_iter(*this);
		m_poolElement++;

		return l_iter;
	}

	Iter& operator+=(u32 p_offset)
	{
		m_poolElement += p_offset;
		return *this;
	}

	const Iter& operator+=(u32 p_offset) const
	{
		m_poolElement += p_offset;
		return *this;
	}

	Iter operator+(u32 p_offset) const
	{
		ElementType* l_elementAddress = m_poolElement + p_offset;
		Iter l_iter(*l_elementAddress);
		
		return l_iter;
	}

	bool operator!=(const Iter& p_iter) const
	{
		return m_poolElement != p_iter.m_poolElement;
	}

	bool operator==(const Iter& p_iter) const
	{
		return m_poolElement == p_iter.m_poolElement;
	}

	Iter& operator=(const Iter& p_iter)
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
	using CIter = ContinuousPoolForwardConstIterator<TypedContinuousPoolForwardIterator>;

	ContinuousPoolForwardConstIterator(const ContinuousPoolForwardConstIterator&) = default;
	~ContinuousPoolForwardConstIterator() = default;

	auto&& operator*() const
	{
		return *m_iter;
	}

	const auto operator->() const
	{
		return m_iter.operator->();
	}

	CIter& operator++()
	{
		m_iter++;
		return *this;
	}

	const CIter& operator++() const
	{
		m_iter++;
		return *this;
	}

	CIter operator++(int)
	{
		TypedContinuousPoolForwardIterator l_iter(m_iter);
		m_iter++;

		return l_iter;
	}

	const CIter operator++(int) const
	{
		TypedContinuousPoolForwardIterator l_iter(m_iter);
		m_iter++;

		return l_iter;
	}

	CIter& operator+=(u32 p_offset)
	{
		m_iter += p_offset;
		return *this;
	}

	const CIter& operator+=(u32 p_offset) const
	{
		m_iter += p_offset;
		return *this;
	}

	CIter operator+(u32 p_offset) const
	{
		TypedContinuousPoolForwardIterator l_iter(m_iter);
		l_iter += p_offset;

		CIter l_citer (l_iter);

		return l_citer;
	}

	CIter& operator=(const CIter& p_iter)
	{
		l_iter = p_iter;
		return *this;
	}

	bool operator!=(const CIter& p_iter) const
	{
		return m_iter != p_iter.m_iter;
	}

	bool operator==(const CIter& p_iter) const
	{
		return m_iter == p_iter.m_iter;
	}

private:
	const TypedContinuousPoolForwardIterator m_iter;
};

enum class InitMode
{
	NO_PRE_INIT,
	PRE_INIT
};

template<typename ElementType>
class ContinuousPool
{
public:
	using Iter = ContinuousPoolForwardIterator<ElementType>;
	using CIter = ContinuousPoolForwardConstIterator<ContinuousPoolForwardIterator<ElementType>>;

	template<typename ...Args>
	ContinuousPool(PoolSize p_size, InitMode p_initMode = InitMode::NO_PRE_INIT, Args&&... args)
		:MAX_NR_OF_ELEMENTS(p_size)
	{
		assert(isElementSizeEnough());
		initMemory();
		assert(isDataAligned());

		if (p_initMode == InitMode::PRE_INIT)
		{
			reset();
			preInitPoolElements(std::forward<Args>(args)...);
		}	

		reset();
	}

	ContinuousPool(const ContinuousPool<ElementType>&) = delete;
	ContinuousPool(ContinuousPool<ElementType>&&) = default;

	~ContinuousPool()
	{
		clear();
	}

	template<typename ...Args>
	ElementType& allocate(Args&&... args)
	{
		ElementType* l_nextElement = &getNext();
		new (l_nextElement) ElementType(std::forward<Args>(args)...);

		return *l_nextElement;
	}

	void deallocate(ElementType& p_element)
	{	
		p_element.~ElementType();
		takeBack(p_element);
	}

	ElementType& getNext()
	{
		ElementType* l_nextElement = m_positionAfterLastElement;

		m_positionAfterLastElement++;
		m_nrOfStoredElements++;

		return *l_nextElement;
	}

	void takeBack(ElementType& p_element)
	{
		ElementType* l_element = &p_element;

		m_positionAfterLastElement--;
		m_nrOfStoredElements--;

		if (l_element != m_positionAfterLastElement)
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
		for (auto& element : *this)
			element.~ElementType();

		reset();
	}

	void reset()
	{
		m_positionAfterLastElement = getPtrToBeginning();
		m_nrOfStoredElements = 0u;
	}

	bool isEmpty() const
	{
		return m_nrOfStoredElements == 0u;
	}

	bool isMyObject(const ElementType& p_element) const
	{
		for (const auto& element : *this)
		{
			if (&element == &p_element)
			{
				return true;
			}
		}

		return false;
	}

	Iter begin()
	{
		return Iter(*getPtrToBeginning());
	}

	CIter begin() const
	{
		return cbegin();
	}

	CIter cbegin() const
	{
		return CIter(*getPtrToBeginning());
	}

	Iter end()
	{
		return Iter(*m_positionAfterLastElement);
	}

	CIter end() const
	{
		return cend();
	}

	CIter cend() const
	{
		return CIter(*m_positionAfterLastElement);
	}

private:
	static const int ELEMENT_SIZE = sizeof(ElementType);
	const PoolSize MAX_NR_OF_ELEMENTS;

	std::unique_ptr<core::MemoryPool> m_memoryPool;
	u32 m_nrOfStoredElements = 0u;

	ElementType* m_positionAfterLastElement = nullptr;

	void initMemory()
	{
		m_memoryPool = std::make_unique<core::MemoryPool>();
		m_memoryPool->resize(MAX_NR_OF_ELEMENTS * (ELEMENT_SIZE / sizeof(core::MemoryAllocationUnit)));
	}

	template<typename ...Args>
	void preInitPoolElements(Args&&... args)
	{
		for (auto i = 0u; i < MAX_NR_OF_ELEMENTS; i++)
		{
			allocate(std::forward<Args>(args)...);
		}
	}
	
	ElementType* getPtrToBeginning() const
	{
		return reinterpret_cast<ElementType*>(&(m_memoryPool->at(0)));
	}

	bool isElementSizeEnough()
	{
		return sizeof(ElementType) >= sizeof(core::MemoryAllocationUnit);
	}

	bool isDataAligned()
	{
		auto l_dataBeginning = getPtrToBeginning();
		auto l_alignedPtr = reinterpret_cast<const core::MemoryAllocationUnit>(l_dataBeginning);

		return !(l_alignedPtr % alignof(ElementType));
	}
};

}