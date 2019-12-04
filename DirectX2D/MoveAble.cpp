#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>

#include <..\DxEngine\GActor.h>
#include <..\DxEngine\ComTransform.h>

#include "MoveAble.h"
#include <..\CommonLibrary\GameOption.h>

MoveAble::MoveAble( )
	: mSpeed(GameOption::instance.GetAspectRatio( ) * 2.0f), mPos(gconst::inf<float>)
{
	EMPTY_STATEMENT;
}

MoveAble::~MoveAble( )
{
	EMPTY_STATEMENT;
}

void MoveAble::Init( )
{
	mActor = &GetActor( );
	mPos = mActor->GetPos( );
}

void MoveAble::Update( )
{
	mPos = mActor->GetPos( );

	//if ( true == GameKeyInput::Press(L"Up") )
	//{
	//	mPos.m.y += mSpeed * GameTime::DeltaTime( );
	//}

	//if ( true == GameKeyInput::Press(L"Down") )
	//{
	//	mPos.m.y -= mSpeed * GameTime::DeltaTime( );
	//}

	if ( true == GameKeyInput::Press(L"Right") )
	{
		mPos.m.x += mSpeed * GameTime::DeltaTime( );
	}

	if ( true == GameKeyInput::Press(L"Left") )
	{
		mPos.m.x -= mSpeed * GameTime::DeltaTime( );
	}

	mActor->SetPos(mPos);
}
