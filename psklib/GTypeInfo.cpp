#include "GTypeInfo.h"

void GTypeInfo::SetTypeInfo( )
{
	mTypeInfo = &typeid(*this);
}
