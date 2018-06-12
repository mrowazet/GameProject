#pragma once
#include <vector>
#include <memory>
#include <new>
#include "assert.h"
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
class ContinuousPoolIterator
{
private:
	template<typename ElementType>
	friend class ContinuousPool;

public:
	using Iter = ContinuousPoolIterator<ElementType>;

	ContinuousPoolIterator(const ContinuousPoolIterator&) = default;
	~ContinuousPoolIterator() = default;

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

	Iter operator++(int)
	{
		Iter l_iter(*this);
		m_poolElement++;

		return l_iter;
	}

	Iter& operator--()
	{
		m_poolElement--;
		return *this;
	}

	Iter operator--(int)
	{
		Iter l_iter(*this);
		m_poolElement--;

		return l_iter;
	}

	Iter& operator+=(u32 p_offset)
	{
		m_poolElement += p_offset;
		return *this;
	}

	Iter& operator-=(u32 p_offset)
	{
		m_poolElement -= p_offset;
		return *this;
	}

	Iter operator+(u32 p_offset) const
	{
		ElementType* l_elementAddress = m_poolElement + p_offset;
		Iter l_iter(*l_elementAddress);
		
		return l_iter;
	}

	Iter operator-(u32 p_offset) const
	{
		ElementType* l_elementAddress = m_poolElement - p_offset;
		Iter l_iter(*l_elementAddress);

		return l_iter;
	}

	Iter& operator=(const Iter& p_iter)
	{
		m_poolElement = p_iter.m_poolElement;
		return *this;
	}

	bool operator==(const Iter& p_iter) const
	{
		return m_poolElement == p_iter.m_poolElement;
	}

	bool operator!=(const Iter& p_iter) const
	{
		return m_poolElement != p_iter.m_poolElement;
	}

private:
	mutable ElementType* m_poolElement = nullptr;

	ContinuousPoolIterator(ElementType& p_element)
	{
		m_poolElement = &p_element;
	}
};

template<typename TypedContinuousPoolIterator>
class ContinuousPoolConstIterator
{
private:
	template<typename ElementType>
	friend class ContinuousPool;

public:
	using CIter = ContinuousPoolConstIterator<TypedContinuousPoolIterator>;

	ContinuousPoolConstIterator(const ContinuousPoolConstIterator&) = default;
	~ContinuousPoolConstIterator() = default;

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

	CIter operator++(int)
	{
		CIter l_iter(m_iter);
		m_iter++;

		return l_iter;
	}

	CIter& operator--()
	{
		m_iter--;
		return *this;
	}

	CIter operator--(int)
	{
		CIter l_iter(m_iter);
		m_iter--;

		return l_iter;
	}

	CIter& operator+=(u32 p_offset)
	{
		m_iter += p_offset;
		return *this;
	}

	CIter& operator-=(u32 p_offset)
	{
		m_iter -= p_offset;
		return *this;
	}

	CIter operator+(u32 p_offset) const
	{
		return CIter(m_iter + p_offset);
	}

	CIter operator-(u32 p_offset) const
	{
		return CIter(m_iter - p_offset);
	}

	CIter& operator=(const CIter& p_iter)
	{
		l_iter = p_iter;
		return *this;
	}

	bool operator==(const CIter& p_iter) const
	{
		return m_iter == p_iter.m_iter;
	}

	bool operator!=(const CIter& p_iter) const
	{
		return !(*this == p_iter);
	}

private:
	TypedContinuousPoolIterator m_iter;

	ContinuousPoolConstIterator(const TypedContinuousPoolIterator& p_iter)
		:m_iter(p_iter)
	{
	}
};

template<typename ElementType>
class SafeIterator
{
private:
	template<typename ElementType>
	friend class ContinuousPool;

	using TypedContinuousPool = ContinuousPool<ElementType>;
	using Iter = typename TypedContinuousPool::Iter;

public:
	SafeIterator(const SafeIterator& p_safeIter)
		:m_parentPool(p_safeIter.m_parentPool),
		 m_iter(p_safeIter.m_iter),
		 m_isValid(p_safeIter.m_isValid)
	{
		m_parentPool.registerSafeIterator(*this);
	}

	~SafeIterator()
	{
		m_parentPool.deregisterSafeIterator(*this);
	}

	Iter& getIter()
	{
		return m_iter;
	}

	bool isValid()
	{
		return m_isValid;
	}

	bool operator==(const SafeIterator& p_iter) const
	{
		return m_iter == p_iter.m_iter;
	}

	bool operator!=(const SafeIterator& p_iter) const
	{
		return !(*this == p_iter);
	}

private:
	TypedContinuousPool& m_parentPool;
	Iter m_iter;
	bool m_isValid = true;

	SafeIterator(TypedContinuousPool& p_pool, Iter& p_iter)
		:m_parentPool(p_pool),
		m_iter(p_iter)
	{
		m_parentPool.registerSafeIterator(*this);

		if (m_parentPool.isEmpty())
		{
			m_isValid = false;
		}
	}

	void invalidate()
	{
		m_isValid = false;
	}
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
	using Iter = ContinuousPoolIterator<ElementType>;
	using CIter = ContinuousPoolConstIterator<ContinuousPoolIterator<ElementType>>;

	using TypedSafeIter = SafeIterator<ElementType>;
	using SafeItersContainer = std::vector<TypedSafeIter*>;

private:
	template<typename T>
	friend class SafeIterator;

public:
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
		//clear() should not be called here - it will cause a crash if getNext was used earlier
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
		invalidateSafeIteratorsWhichPointToRemovedElement(p_element);

		if (!isLastElement(p_element))
		{
			copyLastElementToPositionOfRemovedElement(p_element);
			updateSafeIteratorsWhichPointedToLastElement(p_element);
		}

		m_positionAfterLastElement--;
		m_nrOfStoredElements--;
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
		{
			element.~ElementType();
		}

		reset();
	}

	void reset()
	{
		m_positionAfterLastElement = getPtrToBeginning();
		m_nrOfStoredElements = 0u;

		invalidateAllSafeIterators();
	}

	bool isEmpty() const
	{
		return m_nrOfStoredElements == 0u;
	}

	bool isMyObject(const ElementType& p_element) const
	{
		for (const auto& l_element : *this)
		{
			if (isTheSameElement(l_element, p_element))
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

	TypedSafeIter makeSafeIter()
	{
		return TypedSafeIter(*this, begin());
	}

	TypedSafeIter makeSafeIter(Iter& p_iter)
	{
		return TypedSafeIter(*this, p_iter);
	}

private:
	static const int ELEMENT_SIZE = sizeof(ElementType);
	const PoolSize MAX_NR_OF_ELEMENTS;

	std::unique_ptr<core::MemoryPool> m_memoryPool;
	u32 m_nrOfStoredElements = 0u;

	ElementType* m_positionAfterLastElement = nullptr;
	SafeItersContainer m_safeIters;

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

	void registerSafeIterator(TypedSafeIter& p_safeIter)
	{
		auto& l_ptr = m_safeIters.emplace_back();
		l_ptr = &p_safeIter;
	}

	void deregisterSafeIterator(TypedSafeIter& p_safeIter)
	{
		for (auto l_iter = m_safeIters.begin(); l_iter != m_safeIters.end(); l_iter++)
		{
			auto& l_safeIterator = **l_iter;

			if (l_safeIterator == p_safeIter)
			{
				m_safeIters.erase(l_iter);
				break;
			}
		}
	}

	void invalidateSafeIteratorsWhichPointToRemovedElement(const ElementType& p_removedElement)
	{
		for (auto l_iterPtr : m_safeIters)
		{
			auto& l_pointedElement = *l_iterPtr->getIter();

			if(isTheSameElement(l_pointedElement, p_removedElement))
			{
				l_iterPtr->invalidate();
			}
		}
	}

	void invalidateAllSafeIterators()
	{
		for (auto l_iterPtr : m_safeIters)
		{
			l_iterPtr->invalidate();
		}
	}

	void updateSafeIteratorsWhichPointedToLastElement(ElementType& p_newPosition)
	{
		for (auto l_iterPtr : m_safeIters)
		{
			auto& l_element = *l_iterPtr->getIter();

			if (isLastElement(l_element))
			{
				l_iterPtr->getIter().m_poolElement = &p_newPosition;
			}
		}
	}

	bool isLastElement(const ElementType& p_element) const
	{
		return isTheSameElement(p_element, getLastElement());
	}

	bool isTheSameElement(const ElementType& p_firstElement, const ElementType& p_secondElement) const
	{
		return &p_firstElement == &p_secondElement;
	}

	ElementType& getLastElement() const
	{
		return *(m_positionAfterLastElement - 1);
	}

	void copyLastElementToPositionOfRemovedElement(ElementType& p_removedElement)
	{
		std::memcpy(&p_removedElement, &getLastElement(), ELEMENT_SIZE);
	}
};

}