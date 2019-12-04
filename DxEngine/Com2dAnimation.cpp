#include "Com2dAnimation.h"
#include "ComRender2DSprite.h"
#include "GActor.h"
#include "GSprite.h"

Com2dAnimation::Com2dAnimation( )
	: mRender(nullptr)
	, mMapOfAnimation(std::map<std::wstring, SmartPtr<GAnimation>>( ))
	, mCurAnimation(nullptr)
{
	EMPTY_STATEMENT;
}

Com2dAnimation::~Com2dAnimation( )
{
	EMPTY_STATEMENT;
}

void Com2dAnimation::Init( )
{
	mRender = GetActor( ).GetComponentOrNull<ComRender2DSprite>( );
	if ( nullptr == mRender )
	{
		CRASH_PROG;
		return;
	}
}

void Com2dAnimation::RenderPreprocess( )
{
	if ( nullptr == mCurAnimation )
	{
		// note nullptr -> animation stop
		return;
	}

	mCurAnimation->Update( );
	mRender->SetSpriteIdx(mCurAnimation->GetCurFrameIndex( ));
}

void Com2dAnimation::Stop( )
{
	mCurAnimation = nullptr;
}

bool Com2dAnimation::ChangeAnimation(const std::wstring& _aniName)
{
	GAnimation* changeAnimationInfo = FindAnimation(_aniName);
	if ( nullptr == changeAnimationInfo )
	{
		CRASH_PROG;
		return false;
	}

	if ( mCurAnimation == changeAnimationInfo )
	{
		return false;
	}

	mCurAnimation = changeAnimationInfo;
	mCurAnimation->Reset( );

	return true;
}

void Com2dAnimation::PlaySameAnimation( )
{
	mCurAnimation->Reset( );
}

bool Com2dAnimation::CreateAnimation(const std::wstring& _spriteName, const std::wstring& _aniName, UINT _start, UINT _end, float _playTime, bool _loopAble)
{
	if ( nullptr != FindAnimation(_aniName) )
	{
		CRASH_PROG;
		return false;
	}

	if ( _start > _end )
	{
		CRASH_PROG;
		return false;
	}

	SmartPtr<GSprite> sprite = GResource::Find<GSprite>(_spriteName);

	if ( _end > sprite->GetSpritesCount( ) )
	{
		CRASH_PROG;
		return false;
	}

	Com2dAnimation::GAnimation* newAni = new Com2dAnimation::GAnimation( );
	newAni->SetName(_aniName);
	newAni->mbLoopFlag = _loopAble;
	newAni->Init(sprite, _start, _end, _playTime);

	mMapOfAnimation.insert(
		std::map<std::wstring, SmartPtr<GAnimation>>::value_type(_aniName, newAni)
	);

	if ( 1 == mMapOfAnimation.size( ) )
	{
		mCurAnimation = newAni;
	}

	return true;
}

SmartPtr<Com2dAnimation::GAnimation> Com2dAnimation::FindAnimation(std::wstring _aniName)
{
	auto iterFind = mMapOfAnimation.find(_aniName);
	if ( mMapOfAnimation.end( ) == iterFind )
	{
		return nullptr;
	}

	return iterFind->second;
}

