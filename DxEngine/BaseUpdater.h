#pragma once
#include <..\psklib\Debug.h>

class BaseUpdater
{
public:
	virtual void UpdatePreprocess( );
	virtual void Update( );
	virtual void UpdatePostprocess( );

	virtual void RenderPreprocess( );
	virtual void RenderPostprocess( );

	virtual void CollidePreprocess( );
	virtual void CollidePostprocess( );

	virtual void Release( );

	virtual void StartSceneChange( );

	virtual bool IsCanUpdate( )
	{
		return true == mbUpdateFlag && false == mbDied;
	}

	virtual void OffUpdate( )
	{
		mbUpdateFlag = false;
	}

	virtual void OnUpdate( )
	{
		mbUpdateFlag = true;
	}

	virtual void SwitchUpdateFlag( )
	{
		mbUpdateFlag = !mbUpdateFlag;
	}

	virtual bool IsDied( )
	{
		return mbDied;
	}

	void Death( )
	{
		mbDied = true;
	}

	virtual bool IsGlobal( )
	{
		return mbGlobalFlag;
	}

	virtual void OnGlobalFlag( )
	{
		mbGlobalFlag = true;
	}

	virtual void OffGlobalFlag( )
	{
		mbGlobalFlag = false;
	}

protected:
	BaseUpdater( )
		: mbUpdateFlag(true), mbDied(false), mbGlobalFlag(false)
	{
		EMPTY_STATEMENT;
	}

	virtual ~BaseUpdater( ) = 0;

protected:
	bool mbUpdateFlag;
	bool mbDied;
	bool mbGlobalFlag;
};