#pragma once
#include <Windows.h>

class GameTime final
{
public:
#pragma region gametimer_class
	class GameTimer
	{
		friend GameTime;
	public:
		float Update( )
		{
			QueryPerformanceCounter(&mCurTime);

			// delta time = curtime - prevtime
			mdDeltaTime = ((double)mCurTime.QuadPart - \
				(double)mPrevTime.QuadPart) / (double)(mCountTime.QuadPart);

			mfDeltaTime = (float)mdDeltaTime;
			mPrevTime = mCurTime;

			return mfDeltaTime;
		}

		void Reset( )
		{
			// 컴퓨터가 일정하게 세는 숫자
			QueryPerformanceFrequency(&mCountTime);
			// 프로그램 동작 후 얼마나 세었는지 반환
			QueryPerformanceCounter(&mCurTime);
			QueryPerformanceCounter(&mPrevTime);
		}

	private:
		GameTimer( );
		~GameTimer( ) = default;

	private:
		LARGE_INTEGER mCountTime;
		LARGE_INTEGER mCurTime;
		LARGE_INTEGER mPrevTime;
		double mdDeltaTime;
		float mfDeltaTime;
	};
#pragma endregion

public:
	static float DeltaTime( )
	{
		return MainTimer.mfDeltaTime;
	}

	static void Update( )
	{
		MainTimer.Update( );
	}

	static void Reset( )
	{
		MainTimer.Reset( );
	}

private:
	GameTime( ) = default;
	~GameTime( ) = default;

private:
	static GameTimer MainTimer;
};
