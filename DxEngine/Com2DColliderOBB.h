#pragma once
#include "ComCollider.h"

class Com2DColliderOBB
	: public ComCollider
{
public:
	Com2DColliderOBB( );
	~Com2DColliderOBB( );
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

