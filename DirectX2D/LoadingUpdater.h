#pragma once
#include <..\DxEngine\GScene.h>

class ComRenderColorRect;
class Com2dAnimation;

class LoadingUpdater
	: public GScene::GSceneUpdater
{
private:
	enum class FSM
	{
		WARNING, LOADING, LOAD_COMPLETE
	};

public:
	LoadingUpdater( )
		: meState(FSM::WARNING), mWarningImgName(L"WARNING_KR_A.PNG")
		, mLoadingBarImgName(L"CHATR_A.PNG")
		, mVecOfResourcePath(std::vector<stdfs::path>( ))
		, mIterPathNext( ), mIterPathEnd( )
		, mResourceCounter(0), mResourceMaxNum(gconst::inf<size_t>)
		, mWarningSprite(nullptr), mWarning(nullptr), mWarningRender(nullptr)
		, mLoadCgSprite(nullptr), mLoadCg(nullptr), mLoadCgRender(nullptr)
		, mLoadingBarSprite(nullptr), mLoadingBar(nullptr), mLoadingBarRender(nullptr), mLoadingBarAni(nullptr)
		, mBoxForFade(nullptr), mFadeBox(nullptr), mFadeValue(0.0f), mFadeStartTime(0.0f)
	{
		EMPTY_STATEMENT;
	}

	~LoadingUpdater( )
	{
		EMPTY_STATEMENT;
	}

private:
	void LoadScene( ) override;
	void UpdateScene( ) override;
	void StartSceneChange( ) override;
	void EndSceneChange( ) override;

	void ChangeState(FSM _state);
	void SetUpLoadingImg( );
	void Loading( );
	void Warning( );
	void LoadComplete( );

private:
	FSM meState;
	std::wstring mWarningImgName;
	std::wstring mLoadingBarImgName;

	std::vector<stdfs::path> mVecOfResourcePath;
	std::vector<stdfs::path>::iterator mIterPathNext;
	std::vector<stdfs::path>::iterator mIterPathEnd;

	std::vector<stdfs::path> mVecOfSoundPath;

	SmartPtr<GActor> mWarning;
	SmartPtr<GSprite> mWarningSprite;
	SmartPtr<ComRender2DSprite> mWarningRender;

	SmartPtr<GActor> mLoadCg;
	SmartPtr<GSprite> mLoadCgSprite;
	SmartPtr<ComRender2DSprite> mLoadCgRender;

	SmartPtr<GActor> mLoadingBar;
	SmartPtr<GSprite> mLoadingBarSprite;
	SmartPtr<ComRender2DSprite> mLoadingBarRender;
	SmartPtr<Com2dAnimation> mLoadingBarAni;

	SmartPtr<GActor> mFadeBox;
	SmartPtr<ComRenderColorRect> mBoxForFade;
	float mFadeValue;
	float mFadeStartTime;

	size_t mResourceCounter;
	size_t mResourceMaxNum;
};

