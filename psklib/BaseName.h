#pragma once
#include <string>

#include "Debug.h"

class BaseName
{
public:
	std::wstring GetName( ) const
	{
		return mName;
	}

	void SetName(const std::wstring& _name)
	{
		mName = _name;
	}

protected:
	BaseName( ) = default;
	virtual ~BaseName( ) = default;

private:
	std::wstring mName;
};