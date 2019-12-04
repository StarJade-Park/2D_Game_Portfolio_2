#pragma once
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\GScene.h>

class GActor;
class GTileRenderer;
class ComCollider;

class Erina
{
public:
	Erina(GScene& _curScene);
	~Erina( );

	GActor& GetActor( )
	{
		return *mErinaActor;
	}

	void SetPos(const GVector4D& _pos);
	GVector4D GetPos( ) const;

	void Enter(ComCollider* test1, ComCollider* test2);
	void Stay(ComCollider* test1, ComCollider* test2);
	void Exit(ComCollider* test1, ComCollider* test2);

private:
	void MakePlayer(GScene& _curScene);
	void MakeErinaAnimation( );
	void MakeHammerAnimation( );

private:
	GActor* mErinaActor;
	GActor* mHammerActor;

	ComRender2DSprite* mSpritePlayer;
	ComRender2DSprite* mSpriteHammer;

	Com2dAnimation* mErinaAnimation;
	Com2dAnimation* mHammerAnimation;

	const std::wstring mPlayerImgName;
	const std::wstring mHammerImgName;

	float mDefaultAniSpeed;
};