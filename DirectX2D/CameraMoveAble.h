#pragma once
#include <..\psklib\MyMath.h>

#include <..\DxEngine\ComCustom.h>

class GActor;

class CameraMoveAble
	: public ComCustom
{
public:
	CameraMoveAble( );
	~CameraMoveAble( );

	void Init( ) override;
	void Update( ) override;

private:
	float mSpeed;
	GVector4D mPos;
	GVector4D mRot;
	GActor* mActor;

	float x;
	float y;
	float z;
};

