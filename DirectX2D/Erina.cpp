#include <..\DxEngine\GameWindow.h>
#include <..\DxEngine\ComCamera.h>
#include <..\DxEngine\GSprite.h>
#include <..\DxEngine\GTexture.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\ComRenderColorRect.h>
#include <..\DxEngine\Com2DColliderOBB.h>
#include <..\DxEngine\Com2DColliderSphere.h>
#include <..\DxEngine\GActor.h>

#include "Erina.h"
#include "MoveAble.h"
#include "CameraMoveAble.h"
#include <..\CommonLibrary\GameOption.h>
#include <..\CommonLibrary\GTileRenderer.h>

#include "ErinaFSM.h"

// NOTE 망치 : 원래 이미지의 2배 크기의 rectangle를 만들고 원래 망치, 망치 2배 크기의 이미지를 일단 랜더링

Erina::Erina(GScene& _curScene)
	: mPlayerImgName(L"PLAYER_A.PNG"), mHammerImgName(L"HAMMER_A.PNG")
	, mErinaActor(nullptr), mSpritePlayer(nullptr), mErinaAnimation(nullptr)
	, mDefaultAniSpeed(0.08f), mSpriteHammer(nullptr)
{
	MakePlayer(_curScene);
}

Erina::~Erina( )
{
	EMPTY_STATEMENT;
}

void Erina::MakePlayer(GScene& _curScene)
{
	GResource::Create<GSprite>(mPlayerImgName, mPlayerImgName, 10, 21, 0, 210);
	GResource::Create<GSprite>(mHammerImgName, mHammerImgName, 64.0f, 64.0f);

	mErinaActor = _curScene.CreateActor(L"Erina");
	mErinaActor->SetPos(0.0f);
	mErinaActor->SetLocalScale({ 100.0f, 100.0f, 1.0f });
	mErinaActor->SetSceneAvailable(L"Town");

	mSpritePlayer = mErinaActor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_PLAYER);
	mSpritePlayer->SetSprite(mPlayerImgName);

	mErinaAnimation = mErinaActor->AddComponent<Com2dAnimation>( );

	mHammerActor = _curScene.CreateActor(L"Hammer");
	mHammerActor->SetParentActor(mErinaActor);

	mSpriteHammer = mHammerActor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_PLAYER_HAMMER);
	mSpriteHammer->SetSprite(mHammerImgName);

	mHammerAnimation = mHammerActor->AddComponent<Com2dAnimation>( );

	MakeErinaAnimation( );

	Com2DColliderSphere* col = mErinaActor->AddComponent<Com2DColliderSphere>(COLLIDER_GROUP::COL_PLAYER);
	col->SetLocalScale(col->GetLocalScale( ) * 0.1f);
	SmartPtr<ErinaFSM> fsm = mErinaActor->AddComponent<ErinaFSM>( );
	fsm->SetHammerActor(mHammerActor);

	MakeHammerAnimation( );

	//mActor->AddComponent<ShootAble>( );
	col->RegistEnterFunction(&Erina::Enter, this);
	col->RegistStayFunction(&Erina::Stay, this);
	col->RegistExitFunction(&Erina::Exit, this);
}

void Erina::MakeErinaAnimation( )
{
	// -> 구간 반복 애니메이션 따로 만들고 change하는 방식으로 갑시다
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Idle", 0, 8, mDefaultAniSpeed, true);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Walk", 10, 15, 0.1f, true);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"WalkAfterSlide", 10, 15, 0.05f, true);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Jump_Up", 20, 23, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Bunny_Spin", 30, 35, 0.05f, true);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Jump_Down", 40, 43, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Slide", 50, 53, mDefaultAniSpeed, false);

	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Carrot_Bomb", 60, 69, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Hammer_Commbo_3", 70, 79, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Use_Heal_Item", 80, 87, mDefaultAniSpeed, false);
	//mAnimation->CreateAnimation(mPlayerImgName, L"", 90, 90, mDefaultAniSpeed, true); // NOTE INDEX ERR
	//mAnimation->CreateAnimation(mPlayerImgName, L"Wake_Up", 91, 95, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Hammer_Attack", 100, 105, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Hammer_Commbo_2", 110, 116, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Hammer_Commbo_4", 120, 124, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Hammer_Commbo_5", 122, 124, mDefaultAniSpeed, false);
	//mAnimation->CreateAnimation(mPlayerImgName, L"_______", 130, 133, 0.5f, true);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Hammer_Drill_Down", 140, 144, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Hammer_Drill_Up", 150, 154, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Hammer_Attack_While_Walk", 160, 165, mDefaultAniSpeed, false);
	//mAnimation->CreateAnimation(mPlayerImgName, L"Focus", 170, 175, mDefaultAniSpeed, false);
	//mAnimation->CreateAnimation(mPlayerImgName, L"Knock_Down", 180, 196, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Bunny_Strike", 202, 207, mDefaultAniSpeed, false);
	mErinaAnimation->CreateAnimation(mPlayerImgName, L"Bunny_Strike_Continue", 205, 207, mDefaultAniSpeed, true);

	mErinaAnimation->ChangeAnimation(L"Idle");
}

void Erina::MakeHammerAnimation( )
{
	// 10 x 17
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Full", 0, 170, mDefaultAniSpeed, true);
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Idle", 30, 39, mDefaultAniSpeed, true);

	mHammerAnimation->CreateAnimation(mHammerImgName, L"Hammer_Attack", 100, 106, mDefaultAniSpeed, false);
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Hammer_Commbo_2", 110, 117, mDefaultAniSpeed, false);
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Hammer_Commbo_3", 0, 9, mDefaultAniSpeed, false);
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Hammer_Commbo_4", 100, 106, mDefaultAniSpeed, false);
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Hammer_Commbo_5", 120, 125, 0.05f, false);
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Hammer_Commbo_5_Continue", 122, 125, 0.05f, true);
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Hammer_Drill_Down", 140, 145, 0.05f, true);
	mHammerAnimation->CreateAnimation(mHammerImgName, L"Hammer_Drill_Up", 150, 155, 0.05f, true);

	mHammerAnimation->ChangeAnimation(L"Idle");
}

void Erina::SetPos(const GVector4D& _pos)
{
	mErinaActor->SetPos(_pos);
}

GVector4D Erina::GetPos( ) const
{
	return mErinaActor->GetPos( );
}

void Erina::Enter(ComCollider* test1, ComCollider* test2)
{
	DebugConsole::PrintlnConsoleW(L"Erina enter");
}

void Erina::Stay(ComCollider* test1, ComCollider* test2)
{
	DebugConsole::PrintlnConsoleW(L"Erina stay");
}

void Erina::Exit(ComCollider * test1, ComCollider * test2)
{
	DebugConsole::PrintlnConsoleW(L"Erina Exit");
}
