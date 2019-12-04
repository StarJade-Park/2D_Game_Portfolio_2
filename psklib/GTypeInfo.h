#pragma once
#include <typeinfo>

#include "Debug.h"

class GTypeInfo
{
public:
	void SetTypeInfo( );

	template<typename Type>
	bool IsSameType( )
	{
		return mTypeInfo == &typeid(Type);
	}

	bool IsSameType(const type_info* _ti)
	{
		return mTypeInfo == _ti;
	}

	bool IsSame(const GTypeInfo& _otherObj)
	{
		return mTypeInfo == _otherObj.mTypeInfo;
	}

protected:
	GTypeInfo( )
		: mTypeInfo(nullptr)
	{
		EMPTY_STATEMENT;
	}

	virtual ~GTypeInfo( )
	{
		EMPTY_STATEMENT;
	}

private:
	const type_info* mTypeInfo;
};

