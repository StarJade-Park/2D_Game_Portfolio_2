#include <..\DxEngine\ComCamera.h>
#include <..\DxEngine\GSprite.h>
#include <..\DxEngine\GTexture.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\ComRenderColorRect.h>
#include <..\DxEngine\GActor.h>
#include <..\DxEngine\Com2DColliderOBB.h>

#include <..\psklib\GameKeyInput.h>

#include "TownUpdater.h"
#include <..\CommonLibrary\GameOption.h>
#include "Background.h"
#include "CameraMoveAble.h"
#include "Erina.h"


class Test 
{
private:
	int HP = 1000000;

public:
	void Enter(ComCollider* _This, ComCollider* _Other)
	{
		std::cout << "충돌했습니다." << std::endl;
	}

	void Exit(ComCollider* _This, ComCollider* _Other)
	{
		std::cout << "충돌 끝났습니다." << std::endl;
	}

	void Stay(ComCollider* _This, ComCollider* _Other)
	{
		--HP;
		std::cout << "충돌 중입니다." << HP << std::endl;
	}
};

TownUpdater::TownUpdater( )
{
}

TownUpdater::~TownUpdater( )
{
}

void TownUpdater::LoadScene( )
{
	Erina player = Erina(GetScene( ));

	GetScene( ).mCollisionMGR.LinkGroup(COLLIDER_GROUP::COL_PLAYER, COLLIDER_GROUP::COL_ETC_OBJECT);

	mMainCamera = GetScene( ).CreateActor(L"mainCamera");
	mMainCamera->GetTransform( ).SetWorldPos(GVector4D(0.f, 0.f, -5.f));
	//mMainCamera->AddComponent<CameraMoveAble>( );
	{
		SmartPtr<ComCamera> comCam = mMainCamera->AddComponent<ComCamera>(RENDER_GROUP::GROUP_PLAYER, RENDER_GROUP::GROUP_ETC_OBJECT);
		comCam->SetProjMode(PROJECTION_MODE::ORTHOGONAL);
		comCam->SetWidthAndHeight(40.0f * 20.0f, 40.0f * 11.0f);
	}

	{
		SmartPtr<GActor> mMainCamera = GetScene( ).CreateActor(L"backGround");
		mMainCamera->GetTransform( ).SetWorldPos(GVector4D(0.f, 0.f, -5.f));
		SmartPtr<ComCamera> comCam = mMainCamera->AddComponent<ComCamera>(RENDER_GROUP::GROUP_BACKGROUND);
		comCam->SetProjMode(PROJECTION_MODE::PERSPECTIVE);
		comCam->SetWidthAndHeight(GameOption::instance.GetWidthAspectRatio( ), GameOption::instance.GetHeightAspectRatio( ));
	}
	//Test* NewTest = new Test( );

	//Erina player = Erina(&GetScene( ));
	//SmartPtr<Com2DColliderOBB> colPtr = player.mActor->GetComponentOrNull<Com2DColliderOBB>( );
	//colPtr->RegistEnterFunction(&Test::Enter, NewTest);
	//colPtr->RegistStayFunction(&Test::Stay, NewTest);
	//colPtr->RegistExitFunction(&Test::Exit, NewTest);
	Background bg = Background(&GetScene( ), L"ALIUS3_A.PNG");

	// note 충돌 처리
	//{
	//	SmartPtr<GActor> colTest = GetScene( ).CreateActor(L"ColTest_1");
	//	colTest->GetTransform( ).SetWorldPos({ 10.f, 0.0f, 0.0F });
	//	colTest->GetTransform( ).SetWorldScale(10.0f);

	//	SmartPtr<ComRenderColorRect> Render = colTest->AddComponent<ComRenderColorRect>(RENDER_GROUP::GROUP_ETC_OBJECT);
	//	Render->SetColor(GVector4D::Red);

	//	colTest->AddComponent<Com2DColliderOBB>(COLLIDER_GROUP::COL_ETC_OBJECT);
	//}

	//{
	//	SmartPtr<GActor> colTest = GetScene( ).CreateActor(L"ColTest_2");
	//	colTest->GetTransform( ).SetWorldPos({ -10.f, 0.0f, 0.0F });
	//	colTest->GetTransform( ).SetWorldScale(10.0f);

	//	SmartPtr<ComRenderColorRect> Render = colTest->AddComponent<ComRenderColorRect>(RENDER_GROUP::GROUP_ETC_OBJECT);
	//	Render->SetColor(GVector4D::Green);

	//	colTest->AddComponent<Com2DColliderOBB>(COLLIDER_GROUP::COL_ETC_OBJECT);
	//}
}

void TownUpdater::UpdateScene( )
{
	//player->Update( );
}

void TownUpdater::StartSceneChange( )
{
}

void TownUpdater::EndSceneChange( )
{

}
