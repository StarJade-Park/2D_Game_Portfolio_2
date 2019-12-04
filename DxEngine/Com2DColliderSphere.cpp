#include "DebugRenderer.h"
#include "Com2DColliderSphere.h"
#include "ComTransform.h"
#include "Com2DColliderOBB.h"
#include "ComCamera.h"

Com2DColliderSphere::Com2DColliderSphere( )
	: mDxSphere(DirectX::BoundingSphere( ))
{
	EMPTY_STATEMENT;
}

Com2DColliderSphere::~Com2DColliderSphere( )
{
	EMPTY_STATEMENT;
}

void Com2DColliderSphere::CollidePreprocess( )
{
	mDxSphere.Center = GetWorldPos( ).dxFloat3;
	mDxSphere.Center.z = 0.0f;

	mDxSphere.Radius = GetWorldScale( ).m.x * 0.5f;

	if ( true == DebugRenderer::IsOnDebugMode( ) )
	{
		DebugRenderer::DrawCirCle2D(ComCamera::mMainCam, GetWorldMatrix( ));
	}
}

bool Com2DColliderSphere::CheckShape(ComCollider* _other)
{
	if ( true == _other->IsSameType<Com2DColliderSphere>( ) )
	{
		return mDxSphere.Intersects(((Com2DColliderSphere*)_other)->GetColliderData( ));
	}
	else if ( true == _other->IsSameType<Com2DColliderOBB>( ) )
	{
		return mDxSphere.Intersects(((Com2DColliderOBB*)_other)->GetColliderData( ));
	}

	return false;
}
