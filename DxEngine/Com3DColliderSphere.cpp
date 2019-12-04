#include "Com3DColliderSphere.h"
#include "ComTransform.h"
#include "Com3DColliderOBB.h"

Com3DColliderSphere::Com3DColliderSphere( )
	: mDxSphere(DirectX::BoundingSphere( ))
{
	EMPTY_STATEMENT;
}

Com3DColliderSphere::~Com3DColliderSphere( )
{
	EMPTY_STATEMENT;
}

void Com3DColliderSphere::CollidePreprocess( )
{
	mDxSphere.Center = GetTransform( ).GetWorldPos( ).dxFloat3;
	mDxSphere.Radius = GetTransform( ).GetWorldPos( ).m.x;
}

bool Com3DColliderSphere::CheckShape(ComCollider* _other)
{
	if ( true == _other->IsSameType<Com3DColliderSphere>( ) )
	{
		return mDxSphere.Intersects(((Com3DColliderSphere*)_other)->GetColliderData( ));
	}
	else if ( true == _other->IsSameType<Com3DColliderOBB>( ) )
	{
		return mDxSphere.Intersects(((Com3DColliderOBB*)_other)->GetColliderData( ));
	}

	return false;
}
