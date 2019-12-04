#pragma once
#include <..\psklib\MyMath.h>

class GameOption
{
public:
	float GetWndWidth( ) const
	{
		return mWndWidth;
	}

	float GetWndHeight( ) const
	{
		return mWndHeight;
	}

	float GetWidthAspectRatio( ) const
	{
		return mWidthAspectRatio;
	}

	float GetHeightAspectRatio( ) const
	{
		return mHeightAspectRatio;
	}

	float GetAspectRatio( ) const
	{
		return mAspectRatio;
	}

private:
	GameOption( )
		: mWndWidth(1280.0f), mWndHeight(720.0f)
		, mWidthAspectRatio(gconst::inf<float>), mHeightAspectRatio(gconst::inf<float>)
		, mAspectRatio(gconst::inf<float>)
	{
		mAspectRatio = (float)my_math::gcd((UINT)mWndWidth, (UINT)mWndHeight);

		mWidthAspectRatio = mWndWidth / mAspectRatio;
		mHeightAspectRatio = mWndHeight / mAspectRatio;
	}

	~GameOption( )
	{
		EMPTY_STATEMENT;
	}

public:
	static const GameOption instance;

private:
	float mWndWidth;
	float mWndHeight;
	float mWidthAspectRatio;
	float mHeightAspectRatio;
	float mAspectRatio;
};
