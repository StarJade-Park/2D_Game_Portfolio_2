#pragma once
#include <..\psklib\MyMath.h>
#include <..\psklib\BaseName.h>
#include <..\psklib\GTypeInfo.h>
#include <..\psklib\SmartPtr.h>

#include "BaseUpdater.h"

class GActor;
class GScene;
class ComTransform;

class BaseGComponent
	: public ControlBlock, public BaseUpdater, public GTypeInfo
{
	friend GActor;
public:
	virtual void Init( ) = 0;
	void InitSceneAndTransform( );

	GActor& GetActor( ) const
	{
		return *mActor;
	}

	GScene& GetScene( ) const;

	ComTransform& GetTransform( ) const
	{
		return *mTrans;
	}

	void CreateData( )
	{
		EMPTY_STATEMENT;
	}

	bool IsCanUpdate( ) override;
	bool IsDied( ) override;
	
protected:
	BaseGComponent( )
		: mActor(nullptr), mTrans(nullptr)
	{
		EMPTY_STATEMENT;
	}

	~BaseGComponent( ) = default;

private:
	void SetActor(GActor* _actor)
	{
		mActor = _actor;
	}

private:
	GActor* mActor;
	ComTransform* mTrans;
};

