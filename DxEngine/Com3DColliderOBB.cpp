#include "Com3DColliderOBB.h"
#include "ComTransform.h"

Com3DColliderOBB::Com3DColliderOBB( )
	: mDxOBB(DirectX::BoundingOrientedBox( ))
	, mRotationVector(gconst::inf<float>)
{
	EMPTY_STATEMENT;
}

Com3DColliderOBB::~Com3DColliderOBB( )
{
	EMPTY_STATEMENT;
}

void Com3DColliderOBB::CollidePreprocess( )
{
	// note Z축 값에 따라서 충돌하지 않을 수 있다.
	mDxOBB.Center = GetTransform( ).GetWorldPos( ).dxFloat3;
	mDxOBB.Extents = GetTransform( ).GetWorldScale( ).dxFloat3; // 사원수, 복소수 회전행렬
	
	mRotationVector = GetTransform( ).GetWorldRotation( ) * gconst::DegToRad;
	DirectX::XMStoreFloat4(
		&mDxOBB.Orientation,
		DirectX::XMQuaternionRotationRollPitchYaw(mRotationVector.m.x, mRotationVector.m.y, mRotationVector.m.z)
	);
}

bool Com3DColliderOBB::CheckShape(ComCollider* _other)
{
	if ( true == _other->IsSameType<Com3DColliderOBB>( ) )
	{
		return mDxOBB.Intersects(((Com3DColliderOBB*)_other)->GetColliderData( ));
	}

	return false;
}
