#include "BaseGComponent.h"
#include "GActor.h"

void BaseGComponent::InitSceneAndTransform( )
{
	mTrans = &mActor->GetTransform( );
}

GScene & BaseGComponent::GetScene( ) const
{
	return mActor->GetScene( );
}

bool BaseGComponent::IsCanUpdate( )
{
	return true == mbUpdateFlag && false == mbDied && true == GetActor( ).IsCanUpdate( );
}

bool BaseGComponent::IsDied( )
{
	return true == mbDied || true == GetActor( ).IsDied( );
}
