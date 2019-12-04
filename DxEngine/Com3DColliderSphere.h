#pragma once
#include "ComCollider.h"

class Com3DColliderSphere
	: public ComCollider
{
public:
	Com3DColliderSphere( );
	~Com3DColliderSphere( );

	void CollidePreprocess( ) override;
	bool CheckShape(ComCollider* _other) override;

	DirectX::BoundingSphere& GetColliderData( )
	{
		return mDxSphere;
	}

private:
	DirectX::BoundingSphere mDxSphere;
};

