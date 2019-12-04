#pragma once
#include "Debug.h"

class ControlBlock
{
	template<typename T>
	friend class SmartPtr;

public:
	ControlBlock( )
		: mRefCount(0)
		, mbFlagDebug(false)
	{
		EMPTY_STATEMENT;
	}

	void OnDebugMode( )
	{
		mbFlagDebug = true;
	}

	void OffDebugMode( )
	{
		mbFlagDebug = false;
	}

	bool IsOnDebugFlag( ) const
	{
		return mbFlagDebug;
	}

protected:
	virtual ~ControlBlock( )
	{
		EMPTY_STATEMENT;
	}

private:
	void AddRef( )
	{
		++mRefCount;
	}

	void SubtractRef( )
	{
		--mRefCount;
		if ( 0 >= mRefCount )
		{
			delete this;
		}
	}

private:
	size_t mRefCount;
	bool mbFlagDebug;
};

