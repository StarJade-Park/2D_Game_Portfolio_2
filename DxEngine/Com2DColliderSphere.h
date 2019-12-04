#pragma once
#include "ComCollider.h"

class Com2DColliderSphere
	: public ComCollider
{
public:
	Com2DColliderSphere( );
	~Com2DColliderSphere( );

	void CollidePreprocess( ) override;
	bool CheckShape(ComCollider* _other) override;

	DirectX::BoundingSphere& GetColliderData( )
	{
		return mDxSphere;
	}

private:
	DirectX::BoundingSphere mDxSphere;
};

