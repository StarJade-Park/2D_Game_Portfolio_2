#pragma once
#include <..\DxEngine\ComCustom.h>

class ComTransform;

class MoveAble
	: public ComCustom
{
public:
	MoveAble( );
	~MoveAble( );

	void Init( ) override;
	void Update( ) override;

private:
	GVector4D mPos;
	float mSpeed;
	GActor* mActor;
};

