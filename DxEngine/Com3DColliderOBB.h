#pragma once
#include "ComCollider.h"

// - DirectX::BoundingOrientedBox
class Com3DColliderOBB
	: public ComCollider
{
public:
	Com3DColliderOBB( );
	~Com3DColliderOBB( );
	void CollidePreprocess( ) override;

	bool CheckShape(ComCollider* _other) override;
	DirectX::BoundingOrientedBox& GetColliderData( )
	{
		return mDxOBB;
	}

private:
	DirectX::BoundingOrientedBox mDxOBB;
	GVector4D mRotationVector;
};

