#pragma once
#include "ControlBlock.h"

template <typename P>
class SmartPtr
{
public:
	SmartPtr( )
		: mPtr(nullptr)
	{
		EMPTY_STATEMENT;
	}

	virtual ~SmartPtr( )
	{
		CheckSubtract( );
	}
	
	SmartPtr(const std::nullptr_t _ptr)
		: mPtr(_ptr)
	{
		EMPTY_STATEMENT;
	}

	SmartPtr(P* const _ptr)
		: mPtr(_ptr)
	{
		CheckAdd( );
	}

	SmartPtr(const SmartPtr& _ptr)
		: mPtr(_ptr.mPtr)
	{
		CheckAdd( );
	}

#pragma region compare >, <
	// - compare this, nullptr
	bool operator<(const std::nullptr_t _ptr) const
	{
		return mPtr < _ptr;
	}

	// - compare this, nullptr
	bool operator>(const std::nullptr_t _ptr) const
	{
		return mPtr > _ptr;
	}

	// - compare this, other ptr
	bool operator<(const SmartPtr& _ptr) const
	{
		return mPtr < _ptr.mPtr;
	}

	// - compare this, other ptr
	bool operator>(const SmartPtr& _ptr) const
	{
		return mPtr > _ptr.mPtr;
	}

	// - compare this, other ptr
	bool operator<(P* const _ptr) const
	{
		return mPtr < _ptr;
	}

	// - compare this, other ptr
	bool operator>(P* const _ptr) const
	{
		return mPtr > _ptr;
	}
#pragma endregion

#pragma region compare ==, !=
	// - compare this, nullptr
	bool operator!=(const std::nullptr_t _ptr) const
	{
		return mPtr != _ptr;
	}

	// - compare this, nullptr
	bool operator==(const std::nullptr_t _ptr) const
	{
		return mPtr == _ptr;
	}

	// - compare this, other ptr
	bool operator!=(const SmartPtr& _ptr) const
	{
		return mPtr != _ptr.mPtr;
	}

	// - compare this, other ptr
	bool operator==(const SmartPtr& _ptr) const
	{
		return mPtr == _ptr.mPtr;
	}

	// - compare this, other ptr
	bool operator!=(P* const _ptr) const
	{
		return mPtr != _ptr;
	}

	// - compare this, other ptr
	bool operator==(P* const _ptr) const
	{
		return mPtr == _ptr;
	}
#pragma endregion

	// - ptr ������
	P* const operator->( ) const
	{
		return mPtr;
	}

	// - ptr ������
	P& operator*( ) const
	{
		return (*mPtr);
	}

	SmartPtr& operator=(P* const _ptr)
	{
		CheckSubtract( );
		mPtr = _ptr;
		CheckAdd( );
		return *this;
	}

	SmartPtr& operator=(const std::nullptr_t _ptr)
	{
		CheckSubtract( );
		mPtr = _ptr;
		return *this;
	}

	SmartPtr& operator=(const SmartPtr& _ptr)
	{
		CheckSubtract( );
		mPtr = _ptr.mPtr;
		CheckAdd( );
		return *this;
	}

	// - �� ��ȯ ������
	operator P*( )
	{
		return mPtr;
	}

	void CheckAdd( )
	{
		if ( nullptr == mPtr )
		{
			return;
		}

		((ControlBlock*)mPtr)->AddRef( );
	}

	void CheckSubtract( )
	{
		if ( nullptr == mPtr )
		{
			return;
		}

		((ControlBlock*)mPtr)->SubtractRef( );
	}

public:
	P* mPtr;
};


// ���� ������ �����ε�

template<typename P>
bool operator==(const std::nullptr_t _ptr, const SmartPtr<P>& _sPtr)
{
	return _ptr == _sPtr.mPtr;
}

template<typename P>
bool operator!=(const std::nullptr_t _ptr, const SmartPtr<P>& _sPtr)
{
	return _ptr != _sPtr.mPtr;
}
