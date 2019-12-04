#include <..\psklib\MathConstDef.h>
#include <..\psklib\GameTime.h>

#include "Com2dAnimation.h"

Com2dAnimation::GAnimation::GAnimation( )
	: mVecOfFrameData(std::vector<AnimationData>( ))
	, mSprite(nullptr)
	, mCurFrame(gconst::inf<UINT>), mCurPlayTime(0.0f)
	, mbLoopFlag(true)
	, mbCurAniEndedFlag(false)
{
}

Com2dAnimation::GAnimation::~GAnimation( )
{
	EMPTY_STATEMENT;
}

void Com2dAnimation::GAnimation::Init(SmartPtr<GSprite> _sprite, UINT _start, UINT _end, float _playTime)
{
	if ( nullptr == _sprite )
	{
		CRASH_PROG;
		return;
	}
	mSprite = _sprite;

	for ( UINT idx = _start; idx < _end; ++idx )
	{
		mVecOfFrameData.push_back(AnimationData(idx, _playTime));
	}
}

void Com2dAnimation::GAnimation::Update( )
{
	mCurPlayTime -= GameTime::DeltaTime( );

	if ( 0.0f >= mCurPlayTime )
	{
		++mCurFrame;

		if ( (UINT)mVecOfFrameData.size( ) <= mCurFrame )
		{
			if ( true == mbLoopFlag )
			{
				Reset( );
			}
			else if ( false == mbLoopFlag )
			{
				mCurFrame = (UINT)mVecOfFrameData.size( ) - 1;
				mbCurAniEndedFlag = true;
			}
		}

		mCurPlayTime = mVecOfFrameData[mCurFrame].mPlayTime;
	}
}

void Com2dAnimation::GAnimation::Reset( )
{
	mCurFrame = 0;
	mCurPlayTime = mVecOfFrameData[mCurFrame].mPlayTime;
	mbCurAniEndedFlag = false;
}