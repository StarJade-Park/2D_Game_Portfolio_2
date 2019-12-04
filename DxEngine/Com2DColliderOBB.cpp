#include "Com2DColliderOBB.h"
#include "ComTransform.h"
#include "DebugRenderer.h"
#include "Com2DColliderSphere.h"

Com2DColliderOBB::Com2DColliderOBB( )
	: mDxOBB(DirectX::BoundingOrientedBox( ))
	, mRotationVector(gconst::inf<float>)
{
	EMPTY_STATEMENT;
}

Com2DColliderOBB::~Com2DColliderOBB( )
{
	EMPTY_STATEMENT;
}

void Com2DColliderOBB::CollidePreprocess( )
{
	UpdateTransform( );

	// note Z축 값에 따라서 충돌하지 않을 수 있다.
	mDxOBB.Center = GetWorldPos( ).dxFloat3;
	mDxOBB.Center.z = 0.0f;

	mDxOBB.Extents = (GetWorldScale( ) * 0.5f).dxFloat3; // 사원수, 복소수 회전행렬
	mDxOBB.Extents.z = 1.0f;

	mRotationVector = GetWorldRotation( );
	DirectX::XMStoreFloat4(
		&mDxOBB.Orientation,
		DirectX::XMQuaternionRotationRollPitchYaw(mRotationVector.m.x, mRotationVector.m.y, mRotationVector.m.z)
	);

	if ( true == DebugRenderer::IsOnDebugMode( ) )
	{
		DebugRenderer::DrawRect2D(ComCamera::mMainCam, GetWorldMatrix( ));
	}
}

bool Com2DColliderOBB::CheckShape(ComCollider* _other)
{
	if ( true == _other->IsSameType<Com2DColliderOBB>( ) )
	{
		return mDxOBB.Intersects(((Com2DColliderOBB*)_other)->GetColliderData( ));
	}
	else if ( true == _other->IsSameType<Com2DColliderSphere>( ) )
	{
		return mDxOBB.Intersects(((Com2DColliderSphere*)_other)->GetColliderData( ));
	}

	return false;
}
