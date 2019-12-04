#include "GActor.h"
#include "BaseGComponent.h"
#include "ComTransform.h"

bool GActor::CheckType(BaseGComponent* _com, const type_info* _ti)
{
	return _com->IsSameType(_ti);
}

void GActor::AddComponent(BaseGComponent& _com)
{
	_com.SetActor(this);
	_com.SetTypeInfo( );
	_com.InitSceneAndTransform( );
	_com.Init( );
	mListOfComponent.push_back(&_com);
}

void GActor::UpdateReadyChild( )
{
	if ( nullptr != mParentActor )
	{
		mParentActor->mListOfChildActor.push_back(this);
		mbChildReadyFlag = false;
	}
}

void GActor::UpdatePreprocess( )
{
	for ( auto& component : mListOfComponent )
	{
		component->UpdatePreprocess( );
	}

	for ( auto& actor : mListOfChildActor )
	{
		actor->UpdatePreprocess( );
	}
}

void GActor::Update( )
{
	for ( auto& component : mListOfComponent )
	{
		if ( false == component->IsCanUpdate( ) )
		{
			continue;
		}
		component->Update( );
	}

	for ( auto& actor : mListOfChildActor )
	{
		if ( false == actor->IsCanUpdate( ) )
		{
			continue;
		}
		actor->Update( );
	}
}

void GActor::UpdatePostprocess( )
{
	for ( auto& component : mListOfComponent )
	{
		component->UpdatePostprocess( );
	}

	for ( auto& actor : mListOfChildActor )
	{
		actor->UpdatePostprocess( );
	}
}

void GActor::RenderPreprocess( )
{
	for ( auto& component : mListOfComponent )
	{
		component->RenderPreprocess( );
	}

	for ( auto& actor : mListOfChildActor )
	{
		actor->RenderPreprocess( );
	}
}

void GActor::RenderPostprocess( )
{
	for ( auto& component : mListOfComponent )
	{
		component->RenderPostprocess( );
	}

	for ( auto& actor : mListOfChildActor )
	{
		actor->RenderPostprocess( );
	}
}

void GActor::CollidePreprocess( )
{
	for ( auto& component : mListOfComponent )
	{
		component->CollidePreprocess( );
	}

	for ( auto& actor : mListOfChildActor )
	{
		actor->CollidePreprocess( );
	}
}

void GActor::CollidePostprocess( )
{
	for ( auto& component : mListOfComponent )
	{
		component->CollidePostprocess( );
	}

	for ( auto& actor : mListOfChildActor )
	{
		actor->CollidePostprocess( );
	}
}

void GActor::Release( )
{
	auto nextComponent = mListOfComponent.begin( );
	const auto& endComponent = mListOfComponent.end( );
	for ( ; nextComponent != endComponent; )
	{
		(*nextComponent)->Release( );
		if ( true == (*nextComponent)->IsDied( ) )
		{
			nextComponent = mListOfComponent.erase(nextComponent);
		}
		else
		{
			++nextComponent;
		}
	}

	auto nextChildActor = mListOfChildActor.begin( );
	const auto& endChildActor = mListOfChildActor.end( );
	for ( ; nextChildActor != endChildActor; )
	{
		(*nextChildActor)->Release( );
		if ( true == (*nextChildActor)->IsDied( ) )
		{
			nextChildActor = mListOfChildActor.erase(nextChildActor);
		}
		else
		{
			++nextChildActor;
		}
	}
}

void GActor::StartSceneChange( )
{
	for ( auto& component : mListOfComponent )
	{
		component->StartSceneChange( );
	}

	for ( auto& actor : mListOfChildActor )
	{
		actor->StartSceneChange( );
	}
}

void GActor::SetParentActor(SmartPtr<GActor> _parentActor)
{
	if ( nullptr == _parentActor )
	{
		CRASH_PROG;
		return;
	}
	mParentActor = _parentActor;

	if ( nullptr != &GetTransform( ) && nullptr != &mParentActor->GetTransform( ) )
	{
		GetTransform( ).SetParentTransform(&mParentActor->GetTransform( ));
	}

	mbChildReadyFlag = true;
}

GVector4D GActor::GetPos( ) const
{
	return mcTransform->GetLocalPos( );
}

void GActor::SetPos(const GVector4D& _pos)
{
	mcTransform->SetLocalPos(_pos);
}

GVector4D GActor::GetLocalScale( ) const
{
	return mcTransform->GetLocalScale( );
}

void GActor::SetLocalScale(const GVector4D& _scale)
{
	mcTransform->SetLocalScale(_scale);
}

void GActor::SetLocalScaleByRatio(const float _scale)
{
	mcTransform->SetLocalScaleByRatio(_scale);
}

GVector4D GActor::GetRotation( )
{
	return mcTransform->GetLocalRotation( );
}

void GActor::SetRotation(const GVector4D& _rotation)
{
	mcTransform->SetLocalRotation(_rotation);
}