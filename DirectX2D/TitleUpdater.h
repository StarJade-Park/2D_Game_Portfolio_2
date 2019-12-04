#pragma once
#include <..\DxEngine\GScene.h>

class Com2dAnimation;
class ComRender2DSprite;
class ComRenderColorRect;

class TitleUpdater
	: public GScene::GSceneUpdater
{
private:
	enum class FSM
	{
		LOGO, TITLE_INIT, TITLE_WAIT, TITLE_INPUT
	};

public:
	TitleUpdater( )
		: titleRender(nullptr), ribbonRender(nullptr), erinaRender(nullptr), bgRender(nullptr), mStartButtonRenderer(nullptr)
		, mainCamera(nullptr), title(nullptr), ribbon(nullptr), erinaStand(nullptr), titleBG(nullptr), start_(nullptr)
		, mTitleSizeX(gconst::inf<float>), mTitleSizeY(gconst::inf<float>), mBreathSize(300.0f)
		, mbTitleSizeFlag(false), mStartMsgAlpha(1.0f), mbStartMsgFlag(true)
		, meFsmState(FSM::LOGO)
		, mFadeBox(nullptr), mFadeRenderer(nullptr), mFadeValue(0.0f), mFadeStartTime(0.0f)
		, mFlinchTime(3.0f), mbFlinchFlag(false)
		, mErinaStandSizeX(gconst::inf<float>), mErinaStandSizeY(gconst::inf<float>)
		, mRibbonStandSizeX(gconst::inf<float>), mRibbonStandSizeY(gconst::inf<float>)
	{
		EMPTY_STATEMENT;
	}

	~TitleUpdater( )
	{
		EMPTY_STATEMENT;
	}

private:
	void LoadScene( ) override;
	void UpdateScene( ) override;
	void StartSceneChange( ) override;
	void EndSceneChange( ) override;

	void ChangeState(TitleUpdater::FSM _state)
	{
		meFsmState = _state;
	}

	void Logo( );
	void TitleInit( );
	void TitleWait( );
	void TitleInput( );

	void BlinkStartButton(const float _blinkFrequency);
	void FlinchStandImage( );
	bool FadeFx( );

private:
	std::vector<stdfs::path> mvec;
	std::vector<stdfs::path>::iterator next;
	std::vector<stdfs::path>::iterator end;
	FSM meFsmState;

	SmartPtr<GActor> mainCamera;
	SmartPtr<GActor> title;
	SmartPtr<GActor> ribbon;
	SmartPtr<GActor> erinaStand;
	SmartPtr<GActor> titleBG;
	SmartPtr<GActor> start_;

	SmartPtr<ComRender2DSprite> titleRender;
	SmartPtr<ComRender2DSprite> ribbonRender;
	SmartPtr<ComRender2DSprite> erinaRender;
	SmartPtr<ComRender2DSprite> bgRender;
	SmartPtr<ComRender2DSprite> mStartButtonRenderer;

	SmartPtr<GActor> mFadeBox;
	SmartPtr<ComRenderColorRect> mFadeRenderer;
	float mFadeValue;
	float mFadeStartTime;

	float mFlinchTime;
	bool mbFlinchFlag;

	float mErinaStandSizeX;
	float mErinaStandSizeY;

	float mRibbonStandSizeX;
	float mRibbonStandSizeY;

	float mTitleSizeX;
	float mTitleSizeY;

	float mBreathSize;

	float mStartMsgAlpha;
	bool mbStartMsgFlag;

	bool mbTitleSizeFlag;

	bool sound = false;
};

