#pragma once
#include <vector>
#include <map>

#include "BaseGComponent.h"

class GSprite;
class ComRender2DSprite;

class Com2dAnimation
	: public BaseGComponent
{
private:
	struct AnimationData
	{
		AnimationData(UINT _idx, float _playTime)
			: mSpriteIdx(_idx), mPlayTime(_playTime)
		{
			EMPTY_STATEMENT;
		}

		UINT mSpriteIdx;
		float mPlayTime;
	}; 

	class GAnimation
		: public BaseName, public ControlBlock
	{
	public:
		GAnimation( );
		~GAnimation( );
		void Init(SmartPtr<GSprite> _sprite, UINT _start, UINT _end, float _playTime);
		void Update( );
		void Reset( );

		UINT GetCurFrameIndex( )
		{
			return mVecOfFrameData[mCurFrame].mSpriteIdx;
		}

		UINT GetCurFrame( )
		{
			return mCurFrame;
		}

	public:
		std::vector<AnimationData> mVecOfFrameData;
		SmartPtr<GSprite> mSprite;
		UINT mCurFrame;
		float mCurPlayTime;
		bool mbLoopFlag;
		bool mbCurAniEndedFlag;
	};

public:
	Com2dAnimation( );
	~Com2dAnimation( );

	void Init( ) override;
	void RenderPreprocess( ) override;

	bool IsAnimationEnded( )
	{
		return mCurAnimation->mbCurAniEndedFlag;
	}

	UINT GetAnimationCurFrame( )
	{
		return mCurAnimation->GetCurFrame( );
	}

	void Stop( );
	bool ChangeAnimation(const std::wstring& _aniName);
	bool CreateAnimation(
		const std::wstring& _spriteName, const std::wstring& _aniName,
		UINT _start, UINT _end,
		float _playTime = 0.1f, bool _loopAble = true
	);

	void PlaySameAnimation( );

private:
	SmartPtr<GAnimation> FindAnimation(std::wstring _aniName);

private:
	ComRender2DSprite* mRender;
	std::map<std::wstring, SmartPtr<GAnimation>> mMapOfAnimation;
	SmartPtr<Com2dAnimation::GAnimation> mCurAnimation;
};

