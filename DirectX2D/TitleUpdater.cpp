#include <..\psklib\GameFileSystem.h>
#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>

#include <..\DxEngine\GameWindow.h>
#include <..\DxEngine\ComCamera.h>
#include <..\DxEngine\GSprite.h>
#include <..\DxEngine\GTexture.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\ComRenderColorRect.h>
#include <..\DxEngine\Com2DColliderOBB.h>
#include <..\DxEngine\GActor.h>
#include <..\DxEngine\GSound.h>

#include "TitleUpdater.h"
#include <..\CommonLibrary\GameOption.h>

void TitleUpdater::LoadScene( )
{
	// 제작사 로고 페이드 인/아웃하면서 보여줌
	
	// 좌->
	// 타이틀, press..., 2015...
	// 우->
	// 캐릭터 이미지
	
	// 배경 구름은 어느 정도 확대 후 카메라 움직임(랜덤)
	// 타이틀 숨쉬는 효과
	// press 깜빡임 효과
	// 캐릭터 움찔(?) 효과 

	// 키 누르면 페이드 아웃되면서 타이틀, 캐릭터 왔던 방향으로 사라짐
	// 메인 메뉴로 전환
	const std::wstring& menuImgName = L"TITLE3_A.PNG";
	const std::wstring& titleNameImg = L"RABIRIBI_A.PNG";
	const std::wstring& erinaImg = L"ERINA_A.PNG";
	const std::wstring& ribbonImg = L"RIBBON_A.PNG";
	const std::wstring& startImg = L"START_BUTTON.PNG";

	// create sprite
	SmartPtr<GSprite> bgSprite = GResource::Create<GSprite>(menuImgName, menuImgName, 1, 1, 0, 1);
	SmartPtr<GSprite> titleNameSprite = GResource::Create<GSprite>(titleNameImg, titleNameImg, 1, 1, 0, 1);
	SmartPtr<GSprite> erinaSprite = GResource::Create<GSprite>(erinaImg, erinaImg, 1, 1, 0, 1);
	SmartPtr<GSprite> ribbonSprite = GResource::Create<GSprite>(ribbonImg, ribbonImg, 1, 1, 0, 1);
	SmartPtr<GSprite> startSprite = GResource::Create<GSprite>(startImg, startImg, 1, 3, 0, 3);

	/*********** CREATE : camera ***********/
	mainCamera = GetScene( ).CreateActor(L"mainCamera");
	mainCamera->GetTransform( ).TranslateLocal(GVector4D(0.f, 0.f, -5.f));
	SmartPtr<ComCamera> comCam = mainCamera->AddComponent<ComCamera>(RENDER_GROUP::GROUP_UI, RENDER_GROUP::GROUP_FADE);
	comCam->SetProjMode(PROJECTION_MODE::ORTHOGONAL);
	comCam->SetWidthAndHeight(GameOption::instance.GetWndWidth( ), GameOption::instance.GetWndHeight( ));

	/************** LOAD : Title Image **************/
	title = GetScene( ).CreateActor(L"Rabi-Ribi title");
	title->SetPos({ -250.0f, 100.0f, 0.0f });
	mTitleSizeX = (float)titleNameSprite->GetTexture( ).GetWitdh( );
	mTitleSizeY = (float)titleNameSprite->GetTexture( ).GetHeight( );
	title->SetLocalScale({ mTitleSizeX, mTitleSizeY });
	titleRender = title->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	titleRender->SetSprite(titleNameImg);
	titleRender->SetSampler(GResource::Find<GSampler>(L"Sampler_Linear"));

	/************** LOAD : ribbon stand image **************/
	ribbon = GetScene( ).CreateActor(L"ribbon stand");
	ribbon->SetPos({ 200.0f, -50.0f, 1.0f });
	mRibbonStandSizeX = (float)ribbonSprite->GetTexture( ).GetWitdh( );
	mRibbonStandSizeY = (float)ribbonSprite->GetTexture( ).GetHeight( );
	ribbon->SetLocalScale({ mRibbonStandSizeX, mRibbonStandSizeY });
	ribbonRender = ribbon->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	ribbonRender->SetSprite(ribbonImg);
	ribbonRender->SetSampler(GResource::Find<GSampler>(L"Sampler_Linear"));

	/************** LOAD : erina stand image **************/
	erinaStand = GetScene( ).CreateActor(L"erina stand");
	erinaStand->SetPos({ 250.0f, -50.0f, 2.0f });
	mErinaStandSizeX = (float)erinaSprite->GetTexture( ).GetWitdh( );
	mErinaStandSizeY = (float)erinaSprite->GetTexture( ).GetHeight( );
	erinaStand->SetLocalScale({ mErinaStandSizeX, mErinaStandSizeY });
	erinaRender = erinaStand->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	erinaRender->SetSprite(erinaImg);
	erinaRender->SetSampler(GResource::Find<GSampler>(L"Sampler_Linear"));

	/************** LOAD : Press Start Button **************/
	start_ = GetScene( ).CreateActor(L"Start");
	start_->SetPos({ -250.0f, -150.0f, 0.0f });
	start_->SetLocalScale({ (float)startSprite->GetTexture( ).GetWitdh( ), (float)startSprite->GetTexture( ).GetHeight( ) / 3.0f });
	start_->SetLocalScaleByRatio(1.5f);
	mStartButtonRenderer = start_->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	mStartButtonRenderer->SetSprite(startImg, 2);
	mStartButtonRenderer->SetSampler(GResource::Find<GSampler>(L"Sampler_Linear"));

	/********************** LOAD : Fade FX Render ***********************/
	mFadeBox = GetScene( ).CreateActor(L"Fade Effect Box");
	mFadeBox->SetPos({ 0.0f, 0.0f, -5.0f });
	mFadeBox->SetLocalScale({ GameOption::instance.GetWndWidth( ), GameOption::instance.GetWndHeight( ) });
	mFadeRenderer = mFadeBox->AddComponent<ComRenderColorRect>(RENDER_GROUP::GROUP_FADE);
	mFadeRenderer->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });

	/************** LOAD : Back Ground **************/
	titleBG = GetScene( ).CreateActor(L"Main Title BG");
	titleBG->SetPos({ 0.0f, 0.0f, 40.0f });
	titleBG->SetLocalScale({ (float)bgSprite->GetTexture( ).GetWitdh( ), (float)bgSprite->GetTexture( ).GetHeight( ) });
	bgRender = titleBG->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	bgRender->SetSprite(menuImgName);
	bgRender->SetSampler(GResource::Find<GSampler>(L"Sampler_Linear"));
}

float breathSpeed = 15.0f;

void TitleUpdater::UpdateScene( )
{
	if ( false == mbTitleSizeFlag )
	{
		mBreathSize -= breathSpeed * GameTime::DeltaTime( );
		if ( 285.0f >= mBreathSize )
		{
			mbTitleSizeFlag = true;
		}
	}
	else
	{
		mBreathSize += breathSpeed * GameTime::DeltaTime( );
		if ( 300.0f <= mBreathSize )
		{
			mbTitleSizeFlag = false;
		}
	}

	title->GetTransform( ).
		SetLocalScaleAddScala({ (mTitleSizeX / mTitleSizeY) * mBreathSize, (mTitleSizeY / mTitleSizeY) * mBreathSize });

	if ( true == GameKeyInput::Down(L"JustDoIt") )
	{
		GameWindow::MainGameWnd( )->ChangeScene(L"Town"); // debug : 바로 전환하고자 하는 scene명 적으면 됨
	}

	switch ( meFsmState )
	{
	case TitleUpdater::FSM::LOGO:
		Logo( );
		break;
	case TitleUpdater::FSM::TITLE_INIT:
		TitleInit( );
		break;
	case TitleUpdater::FSM::TITLE_WAIT:
		TitleWait( );
		break;
	case TitleUpdater::FSM::TITLE_INPUT:
		TitleInput( );
		break;
	default:
		CRASH_PROG;
		break;
	}
}

void TitleUpdater::StartSceneChange( )
{
	EMPTY_STATEMENT;
}

void TitleUpdater::EndSceneChange( )
{
	EMPTY_STATEMENT;
}

void TitleUpdater::Logo( )
{
	// todo 로고 이미지 추가
	ChangeState(FSM::TITLE_WAIT);
}

void TitleUpdater::TitleInit( )
{
	// todo 해당 위치까지 이동시키기
	// title->SetPos({ -250.0f, 100.0f, 0.0f });
	// ribbon->SetPos({ 200.0f, -50.0f, 1.0f });
	// erinaStand->SetPos({ 250.0f, -50.0f, 2.0f });
	// start_->SetPos({ -250.0f, -150.0f, 0.0f });
}

void TitleUpdater::TitleWait( )
{
	if ( false == sound )
	{
		GSound::Play(L"TITLE", L"BGM1.OGG", 10);
		sound = true;
	}

	if ( true == GameKeyInput::Down(L"Jump") || true == GameKeyInput::Down(L"Enter") )
	{
		ChangeState(FSM::TITLE_INPUT);
		mFadeValue = 0.0f;
		return;
	}

	BlinkStartButton(1.5f);
	FlinchStandImage( );
}

void TitleUpdater::TitleInput( )
{
	BlinkStartButton(5.0f);

	mFadeStartTime += GameTime::DeltaTime( );
	if ( 1.0f >= mFadeStartTime )
	{
		return;
	}

	if ( true == FadeFx( ) )
	{
		GSound::Stop(L"TITLE");
		GSound::Play(L"PAGE.WAV");
		GameWindow::MainGameWnd( )->ChangeScene(L"RbrbMap04");
	}
}

void TitleUpdater::BlinkStartButton(const float _blinkFrequency)
{
	if ( false == mbStartMsgFlag )
	{
		mStartMsgAlpha -= _blinkFrequency * GameTime::DeltaTime( );
		if ( 0.0f >= mStartMsgAlpha )
		{
			mbStartMsgFlag = true;
		}
	}
	else
	{
		mStartMsgAlpha += _blinkFrequency * GameTime::DeltaTime( );
		if ( 1.0f <= mStartMsgAlpha )
		{
			mbStartMsgFlag = false;
		}
	}

	mStartButtonRenderer->SetColor({ 1.0f, 1.0f, 1.0f, mStartMsgAlpha });
}

//float test = 0.0f;

void TitleUpdater::FlinchStandImage( )
{
	// todo
	//if ( false == mbFlinchFlag )
	//{
	//	mFlinchTime -= GameTime::DeltaTime( );
	//	ribbon->SetLocalScale({ mRibbonStandSizeX, mRibbonStandSizeY });
	//	erinaStand->SetLocalScale({ mErinaStandSizeX, mErinaStandSizeY });
	//	if ( 0.0f >= mFlinchTime )
	//	{
	//		mbFlinchFlag = true;
	//	}
	//}
	//else
	//{
	//	test += 10.0f * GameTime::DeltaTime( );
	//	GVector4D scale = ribbon->GetLocalScale( );
	//	ribbon->SetLocalScale({ scale.m.x, scale.m.y - 0.75f, scale.m.z });
	//	scale = erinaStand->GetLocalScale( );
	//	erinaStand->SetLocalScale({ scale.m.x, scale.m.y - 0.75f, scale.m.z });

	//	if ( 0.5f <= test )
	//	{
	//		mbFlinchFlag = false;
	//		mFlinchTime = 3.0f;
	//		test = 0.0f;
	//	}
	//}
}

bool TitleUpdater::FadeFx( )
{
	if ( 1.0f >= mFadeValue )
	{
		mFadeValue += GameTime::DeltaTime( );
	}
	else
	{
		return true;
	}

	mFadeRenderer->SetColor({ 1.0f, 1.0f, 1.0f, mFadeValue });
	return false;
}
