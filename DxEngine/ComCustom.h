#pragma once
#include "BaseGComponent.h"

class GScene;

class ComCustom
	: public BaseGComponent
{
public:
	ComCustom( );
	~ComCustom( );

	GScene& GetScene( ) const;
	SmartPtr<GActor> CreateObject(const wchar_t* _name = L"Default Actor Name");
};

