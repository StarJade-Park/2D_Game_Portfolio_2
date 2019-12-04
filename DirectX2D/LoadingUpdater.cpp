#include <..\psklib\GameTime.h>
#include <..\psklib\Well512Random.h>

#include <..\DxEngine\GResource.h>
#include <..\DxEngine\GTexture.h>
#include <..\DxEngine\GSprite.h>
#include <..\DxEngine\GameWindow.h>
#include <..\DxEngine\ComRender2DSprite.h.>
#include <..\DxEngine\ComCamera.h>
#include <..\DxEngine\ComRenderColorRect.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\GSound.h>

#include "LoadingUpdater.h"
#include <..\CommonLibrary\GameOption.h>

void LoadingUpdater::ChangeState(FSM _state)
{
	meState = _state;
}

void LoadingUpdater::LoadScene( )
{
	// Get all img files path
	mVecOfResourcePath = GFileSystem::GetAllImgFiles( );
	mVecOfSoundPath = GFileSystem::GetAllSoundFiles( );

	mIterPathNext = mVecOfResourcePath.begin( );
	mIterPathEnd = mVecOfResourcePath.end( );
	mResourceMaxNum = mVecOfResourcePath.size( );

	/********************** CREATE : Load Scene Camera ***********************/
	SmartPtr<GActor> mainCamera = GetScene( ).CreateActor(L"mainCamera");
	mainCamera->GetTransform( ).TranslateLocal(GVector4D(0.f, 0.f, -10.f));
	SmartPtr<ComCamera> comCam = mainCamera->AddComponent<ComCamera>(RENDER_GROUP::GROUP_UI, RENDER_GROUP::GROUP_FADE);
	comCam->SetProjMode(PROJECTION_MODE::ORTHOGONAL);
	comCam->SetWidthAndHeight(GameOption::instance.GetWndWidth( ), GameOption::instance.GetWndHeight( ));

	/********************** LOAD : WarningImage ***********************/
	GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"Loading").GetAbsPath( ), mWarningImgName));
	mWarningSprite = GResource::Create<GSprite>(mWarningImgName, mWarningImgName, 1, 1, 0, 1);

	mWarning = GetScene( ).CreateActor(L"Warning Image");
	mWarning->SetLocalScale({ (float)mWarningSprite->GetTexture( ).GetWitdh( ), (float)mWarningSprite->GetTexture( ).GetHeight( ) });
	mWarningRender = mWarning->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	mWarningRender->SetSprite(mWarningSprite);
	mWarningRender->SetSampler(GResource::Find<GSampler>(L"Sampler_Linear"));

	/********************** LOAD : Load CG ***********************/
	mLoadCg = GetScene( ).CreateActor(L"Load CG");
	mLoadCgRender = mLoadCg->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	mLoadCgRender->SetSprite(mWarningSprite); // note 임시로 지정한 스프라이트임
	mLoadCgRender->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	mLoadCgRender->SetSampler(GResource::Find<GSampler>(L"Sampler_Linear"));

	/********************** LOAD : Loading Bar ***********************/
	GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"Loading").GetAbsPath( ), mLoadingBarImgName));
	mLoadingBarSprite = GResource::Create<GSprite>(mLoadingBarImgName, mLoadingBarImgName, 15, 1, 0, 14);

	mLoadingBar = GetScene( ).CreateActor(L"Erina");
	mLoadingBar->SetPos({ (GameOption::instance.GetWndWidth( ) / 2) - 65.0f, -(GameOption::instance.GetWndHeight( ) / 2) + 65.0f, -3.0f });
	mLoadingBar->SetLocalScale({ (float)mLoadingBarSprite->GetTexture( ).GetWitdh( ) / 15, (float)mLoadingBarSprite->GetTexture( ).GetHeight( ) });

	mLoadingBarRender = mLoadingBar->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_UI);
	mLoadingBarRender->SetSprite(mLoadingBarSprite);
	mLoadingBarRender->SetPixShader(GResource::Find<PixelGShader>(L"LoadingBar.hlsl"));
	mLoadingBarRender->SetVtxShader(GResource::Find<VertexGShader>(L"LoadingBar.hlsl"));
	mLoadingBarRender->SetConstBufferFillData(GResource::Find<GConstBuffer>(L"ConstBufferCutData"));
	mLoadingBarRender->SetFillData(GVector4D(0.0f, 0.0f));
	mLoadingBarRender->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	mLoadingBarAni = mLoadingBar->AddComponent<Com2dAnimation>( );
	mLoadingBarAni->CreateAnimation(mLoadingBarImgName, L"Loading", 0, 14, 0.05f, true);

	/********************** LOAD : Fade FX Render ***********************/
	mFadeBox = GetScene( ).CreateActor(L"Fade Effect Box");
	mFadeBox->SetPos({ 0.0f, 0.0f, -5.0f });
	mFadeBox->SetLocalScale({ GameOption::instance.GetWndWidth( ), GameOption::instance.GetWndHeight( ) });
	mBoxForFade = mFadeBox->AddComponent<ComRenderColorRect>(RENDER_GROUP::GROUP_FADE);
	mBoxForFade->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
}

void LoadingUpdater::UpdateScene( )
{
	switch ( meState )
	{
	case LoadingUpdater::FSM::WARNING:
		Warning( );
		break;
	case LoadingUpdater::FSM::LOADING:
		Loading( );
		break;
	case LoadingUpdater::FSM::LOAD_COMPLETE:
		LoadComplete( );
		break;
	default:
		break;
	}
}

void LoadingUpdater::StartSceneChange( )
{
	EMPTY_STATEMENT;
}

void LoadingUpdater::EndSceneChange( )
{
	mWarning->Death( );
	mLoadCg->Death( );
	mLoadingBar->Death( );
	mFadeBox->Death( );
}

void LoadingUpdater::SetUpLoadingImg( )
{
	auto VecOfLoadCgPath = GFileSystem::GetAllImgFiles(GFileSystem::FindPathOrNull(L"LOADCG").GetAbsPath( ));
	UINT idx = Well512Random::Instance( ).GetValue(0, (UINT)VecOfLoadCgPath.size( ));

	GResource::Load<GTexture>(VecOfLoadCgPath[idx]);

	mLoadCgSprite = GResource::Create<GSprite>(VecOfLoadCgPath[idx].filename( ), VecOfLoadCgPath[idx].filename( ), 1, 1, 0, 1);

	mLoadCg->SetLocalScale({ (float)mLoadCgSprite->GetTexture( ).GetWitdh( ), (float)mLoadCgSprite->GetTexture( ).GetHeight( ) });
	mLoadCgRender->SetSprite(mLoadCgSprite);
	mLoadCgRender->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

void LoadingUpdater::Loading( )
{
	if ( 0.0f <= mFadeValue )
	{
		mFadeValue -= GameTime::DeltaTime( );
	}

	mBoxForFade->SetColor({ 0.0f, 0.0f, 0.0f, mFadeValue });

	// todo NOWLOADING 글씨가 파도치는 효과, 무지개 순서대로 글씨 칠하기

	if ( mIterPathEnd == mIterPathNext )
	{
		for ( auto& soundPath : mVecOfSoundPath )
		{
			auto sound = GResource::Load<GSound>(soundPath.c_str( ));
			if ( nullptr == sound )
			{
				CRASH_PROG;
				return;
			}
		}

		ChangeState(FSM::LOAD_COMPLETE);
		return;
	}

	auto tex = GResource::Load<GTexture>((*mIterPathNext).c_str( ));
	if ( nullptr == tex )
	{
		CRASH_PROG;
		return;
	}

	++mIterPathNext;
	++mResourceCounter;
	// 로딩 바 채우기
	mLoadingBarRender->SetFillData({ 1.0f , ((float)mResourceCounter / (float)mVecOfResourcePath.size( )) });
}

void LoadingUpdater::Warning( )
{
	mFadeStartTime += GameTime::DeltaTime( );
	if ( 3.0f >= mFadeStartTime )
	{
		return;
	}

	// 페이드 아웃
	mFadeValue += GameTime::DeltaTime( );
	mBoxForFade->SetColor({ 0.0f, 0.0f, 0.0f, mFadeValue });

	if ( 1.0f <= mFadeValue )
	{
		ChangeState(FSM::LOADING);
		mWarningRender->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		mLoadingBarRender->SetColor({ 0.52f, 0.52f, 0.52f, 1.0f }); // gray color
		SetUpLoadingImg( );
		return;
	}
}

void LoadingUpdater::LoadComplete( )
{
	GameWindow::MainGameWnd( )->ChangeScene(L"Title");
	return;
}
