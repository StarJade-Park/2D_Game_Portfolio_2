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
	// note Z�� ���� ���� �浹���� ���� �� �ִ�.
	mDxOBB.Center = GetTransform( ).GetWorldPos( ).dxFloat3;
	mDxOBB.Extents = GetTransform( ).GetWorldScale( ).dxFloat3; // �����, ���Ҽ� ȸ�����
	
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
