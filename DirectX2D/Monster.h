#pragma once
#include <..\DxEngine\GScene.h>
#include <..\DxEngine\GActor.h>
#include <..\DxEngine\ComCollider.h>
#include <..\DxEngine\Com2DColliderOBB.h>
#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\Com2dAnimation.h>

class Monster
{
public:
	Monster(GScene& _scene, const std::wstring& _imgName, const float imgCutSize);
	virtual ~Monster( );

	void SetPos(const GVector4D& _pos);
	GVector4D GetPos( ) const;

	GActor& GetActor( )
	{
		return *mActor;
	}

protected:
	virtual void CreateAnimation( ) = 0;
	virtual void CreateCollider( ) = 0;
	virtual void CreateFSM( ) = 0;

	virtual void Enter(ComCollider* own, ComCollider* other) = 0;
	virtual void Stay(ComCollider* own, ComCollider* other) = 0;
	virtual void Exit(ComCollider* own, ComCollider* other) = 0;

private:
	void Init( );

protected:
	GActor* mActor;
	ComRender2DSprite* mSprite;
	Com2dAnimation* mAnimation;
	const float mImgCutSize;
	const float mDefaultAniSpeed;

	const float mDefaultJumpPower;
	const float mDefaultGravityAcceleration;
	float mGravityAcceleration;
	float mJumpPower;

	std::wstring mImgName;
};

