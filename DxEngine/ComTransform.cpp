#include "ComTransform.h"
#include "GActor.h"

void ComTransform::Init( )
{
	if ( nullptr != &GetActor( ).GetTransform( ) )
	{
		CRASH_PROG;
		return;
	}

	GetActor( ).SetTransform(this);
}

void ComTransform::RenderPreprocess( )
{
	UpdateTransform( );
}

void ComTransform::RenderPostprocess( )
{
	mbUpdateFlag = false;
}

void ComTransform::UpdateTransform( )
{
	if ( true == mbScaleFlag )
	{
		mMatLocalScale.Scale(mvLocalScale);

		mbUpdateFlag = true;
		mbScaleFlag = false;
	}

	if ( true == mbRotationFlag )
	{
		//mMatLocalRotation.RotByDeg(mvLocalRotation);
		mMatLocalRotation.RotQuaternionByDeg(mvLocalRotation);

		mbUpdateFlag = true;
		mbRotationFlag = false;
	}

	if ( true == mbPositionFlag )
	{
		mMatLocalPosition.Translate(mvLocalPosition);

		mbUpdateFlag = true;
		mbPositionFlag = false;
	}

	// 크기와 회전은 분리해 내기 힘들다. 
	if ( true == mbUpdateFlag && nullptr == mParentTransform )
	{
		mMatWorld = mMatLocalScale * mMatLocalRotation * mMatLocalPosition;
	}
	else if ( nullptr != mParentTransform && true == mParentTransform->mbUpdateFlag )
	{
		mMatParentWorld = mParentTransform->GetWorldMatrix( );
		mMatWorld = mMatLocalScale * mMatLocalRotation * mMatLocalPosition * mMatParentWorld;
	}

	if ( nullptr != mParentTransform && true == mParentTransform->mbUpdateFlag )
	{
		mvWorldPosition = mMatWorld.VecArr[3];
		mvWorldWRotation = mParentTransform->GetLocalRotation( ) + GetLocalRotation( );
		mvWorldScale = mParentTransform->GetLocalScale( ) * GetLocalScale( );
		mbUpdateFlag = true;
	}
	else if ( true == mbUpdateFlag && nullptr == mParentTransform )
	{
		mvWorldPosition = mMatWorld.VecArr[3];
		mvWorldWRotation = GetLocalRotation( );
		mvWorldScale = GetLocalScale( );
		mbUpdateFlag = true;
	}
}
