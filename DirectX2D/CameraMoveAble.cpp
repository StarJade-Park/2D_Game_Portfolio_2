#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>

#include <..\DxEngine\GActor.h>
#include <..\DxEngine\ComTransform.h>

#include "CameraMoveAble.h"
#include <..\CommonLibrary\GameOption.h>

CameraMoveAble::CameraMoveAble( )
	: mActor(nullptr)
	, mSpeed(GameOption::instance.GetAspectRatio( ) * 10.0f), mPos( ), mRot( )
	// todo 배경 이미지 크기를 비율에 맞게 가져오기
	, x(GameOption::instance.GetWidthAspectRatio( )), y(GameOption::instance.GetHeightAspectRatio( ))
	, z(/*최대 배경이미지 위치까지*/)
{
}

CameraMoveAble::~CameraMoveAble( )
{
}

void CameraMoveAble::Init( )
{
	mActor = &GetActor( ); // todo player를 기준으로 만들기
	mPos = mActor->GetPos( );
	mRot = mActor->GetRotation( );
}

void CameraMoveAble::Update( )
{
	//if ( x <= mPos.m.x )
	//{
	//	return;
	//}

	//if ( y <= mPos.m.y )
	//{
	//	return;
	//}

//#pragma region mPos
	if ( true == GameKeyInput::Press(L"Right") )
	{
		mPos.m.x += mSpeed * GameTime::DeltaTime( );
	}
	if ( true == GameKeyInput::Press(L"Up") )
	{
		mPos.m.y += mSpeed * GameTime::DeltaTime( );
	}
	if ( true == GameKeyInput::Press(L"CamZoomIn") )
	{
		mPos.m.z += mSpeed * GameTime::DeltaTime( );
	}

	if ( true == GameKeyInput::Press(L"Left") )
	{
		mPos.m.x -= mSpeed * GameTime::DeltaTime( );
	}
	if ( true == GameKeyInput::Press(L"Down") )
	{
		mPos.m.y -= mSpeed * GameTime::DeltaTime( );
	}
	if ( true == GameKeyInput::Press(L"CamZoomOut") )
	{
		mPos.m.z -= mSpeed * GameTime::DeltaTime( );
	}
//#pragma endregion
//
//#pragma region mRot
//	if ( true == GameKeyInput::Press(L"mRotX+") )
//	{
//		mRot.m.x += mSpeed * GameTime::DeltaTime( );
//	}
//	if ( true == GameKeyInput::Press(L"mRotY+") )
//	{
//		mRot.m.y += mSpeed * GameTime::DeltaTime( );
//	}
//	if ( true == GameKeyInput::Press(L"mRotZ+") )
//	{
//		mRot.m.z += mSpeed * GameTime::DeltaTime( );
//	}
//
//	if ( true == GameKeyInput::Press(L"mRotX-") )
//	{
//		mRot.m.x -= mSpeed * GameTime::DeltaTime( );
//	}
//	if ( true == GameKeyInput::Press(L"mRotY-") )
//	{
//		mRot.m.y -= mSpeed * GameTime::DeltaTime( );
//	}
//	if ( true == GameKeyInput::Press(L"mRotZ-") )
//	{
//		mRot.m.z -= mSpeed * GameTime::DeltaTime( );
//	}
#pragma endregion
	mActor->SetPos(mPos);
	mActor->SetRotation(mRot);
}
