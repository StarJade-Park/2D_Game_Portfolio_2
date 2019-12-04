#include "GameTime.h"

GameTime::GameTimer::GameTimer( )
	: mCountTime(LARGE_INTEGER( ))
	, mCurTime(LARGE_INTEGER( ))
	, mPrevTime(LARGE_INTEGER( ))
	, mdDeltaTime(-999.9f)
	, mfDeltaTime(-999.9f)
{
	Reset( );
}

GameTime::GameTimer GameTime::MainTimer = GameTime::GameTimer( );
