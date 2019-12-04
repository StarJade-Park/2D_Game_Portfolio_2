#include <..\psklib\GameFileSystem.h>
#include <..\psklib\GameKeyInput.h>

#include <..\DxEngine\GameWindow.h>
#include <..\DxEngine\ComCamera.h>
#include <..\DxEngine\GSprite.h>
#include <..\DxEngine\GTexture.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\ComRenderColorRect.h>
#include <..\DxEngine\GActor.h>

#include "MainMenuUpdater.h"
#include <..\CommonLibrary\GameOption.h>

MainMenuUpdater::MainMenuUpdater( )
{
}

MainMenuUpdater::~MainMenuUpdater( )
{
}

void MainMenuUpdater::LoadScene( )
{
	std::wstring menuImgName = L"MAINMENU2_A.PNG";
	// create sprite
	SmartPtr<GSprite> bgSprite = GResource::Create<GSprite>(menuImgName, menuImgName, 1, 5, 0, 5);

	/*********** cam ***********/
	SmartPtr<GActor> mainCamera = GetScene( ).CreateActor(L"mainCamera");
	mainCamera->GetTransform( ).TranslateWorld(GVector4D(0.f, 0.f, -5.f));
	SmartPtr<ComCamera> comCam = mainCamera->AddComponent<ComCamera>(RENDER_GROUP::GROUP_UI);
	comCam->SetProjMode(PROJECTION_MODE::ORTHOGONAL);
	comCam->SetWidthAndHeight(GameOption::instance.GetWidthAspectRatio( ), GameOption::instance.GetHeightAspectRatio( ));

	/*************** UI **************/
	SmartPtr<GActor> titleBG = GetScene( ).CreateActor(L"Main Menu BG");
	titleBG->GetTransform( ).SetWorldPos({ 0.0f, 0.0f, 0.0f });
	titleBG->GetTransform( ).SetWorldScale({ GameOption::instance.GetWidthAspectRatio( ), GameOption::instance.GetHeightAspectRatio( ) });
	SmartPtr<ComRender2DSprite> testRender = titleBG->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	testRender->SetSprite(menuImgName);

	SmartPtr<Com2dAnimation> testAni = titleBG->AddComponent<Com2dAnimation>( );
	testAni->CreateAnimation(menuImgName, L"Menu", 0, 5, 0.5f, true);
	// note 실시간으로 alpha 바꿔주면서 ani 현재, 이후를 출력해줘야 함?
}

void MainMenuUpdater::UpdateScene( )
{
	if ( true == GameKeyInput::Down(L"Enter") )
	{
		GameWindow::MainGameWnd( )->ChangeScene(L"Town");
	}
}

void MainMenuUpdater::StartSceneChange( )
{
}

void MainMenuUpdater::EndSceneChange( )
{
}
