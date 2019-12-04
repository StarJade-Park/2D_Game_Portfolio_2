#include "ComCustom.h"
#include "GActor.h"
#include "GScene.h"

ComCustom::ComCustom( )
{
}

ComCustom::~ComCustom( )
{
}

GScene& ComCustom::GetScene( ) const
{
	return GetActor( ).GetScene( );
}

SmartPtr<GActor> ComCustom::CreateObject(const wchar_t* _name)
{
	return GetScene( ).CreateActor(_name);
}
