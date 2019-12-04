#include <..\psklib\GameTime.h>
#include <..\psklib\GameKeyInput.h>

#include "GScene.h"
#include "GActor.h"
#include "GEngineDevice.h"

GScene::GScene( )
	: mListOfActors(std::list<SmartPtr<GActor>>( ))
	, mListOfUpdaters(std::list<SmartPtr<GSceneUpdater>>( ))
	, mRendererMGR(RendererManager(this))
	, mCollisionMGR(CollisionManager( ))
{
	EMPTY_STATEMENT;
}

GScene::~GScene( )
{
	EMPTY_STATEMENT;
}

void GScene::Progress( )
{
	UpdateSceneUpdater( );
	CheckChildActorCanUpdate( );

	UpdatePreprocess( );
	Update( );
	UpdatePostprocess( );
	RenderPreprocess( );
	Render( );
	RenderPostprocess( );
	CollidePreprocess( );
	Collide( );
	CollidePostprocess( );
	Release( );
}

void GScene::PushToListOfUpdaters(SmartPtr<GSceneUpdater> _updater)
{
	if ( nullptr == _updater )
	{
		CRASH_PROG;
		return;
	}

	_updater->SetScene(this);
	mListOfUpdaters.push_back(_updater);
}

void GScene::UpdateSceneUpdater( )
{
	for ( auto& iter : mListOfUpdaters )
	{
		iter->UpdateScene( );
	}
}

void GScene::CheckChildActorCanUpdate( )
{
	if ( 0 == mListOfActors.size( ) )
	{
		return;
	}

	auto next = mListOfActors.begin( );
	const auto& end = mListOfActors.end( );

	for ( ; next != end; )
	{
		if ( true == (*next)->IsReadyChild( ) )
		{
			(*next)->UpdateReadyChild( );
			next = mListOfActors.erase(next);
		}
		else
		{
			++next;
		}
	}
}

SmartPtr<GActor> GScene::CreateActor(const std::wstring& _actorName)
{
	GActor* newActor = new GActor( );
	newActor->SetName(_actorName);
	newActor->SetScene(this);
	mListOfActors.push_back(newActor);

	if ( nullptr == newActor->AddComponent<ComTransform>( ) )
	{
		CRASH_PROG;
		return nullptr;
	}

	return newActor;
}

void GScene::UpdatePreprocess( )
{
	for ( auto& iter : mListOfActors )
	{
		iter->UpdatePreprocess( );
	}
}

void GScene::Update( )
{
	for ( auto& iter : mListOfActors )
	{
		if ( false == iter->IsCanUpdate( ) )
		{
			continue;
		}

		iter->Update( );
	}
}

void GScene::UpdatePostprocess( )
{
	for ( auto& iter : mListOfActors )
	{
		if ( false == iter->IsCanUpdate( ) )
		{
			continue;
		}

		iter->UpdatePostprocess( );
	}
}

void GScene::RenderPreprocess( )
{
	for ( auto& iter : mListOfActors )
	{
		iter->RenderPreprocess( );
	}
}

void GScene::Render( )
{
	mRendererMGR.Render( );
}

void GScene::RenderPostprocess( )
{
	for ( auto& iter : mListOfActors )
	{
		iter->RenderPostprocess( );
	}
}

void GScene::CollidePreprocess( )
{
	for ( auto& iter : mListOfActors )
	{
		iter->CollidePreprocess( );
	}
}

void GScene::Collide( )
{
	mCollisionMGR.Collide( );
}

void GScene::CollidePostprocess( )
{
	for ( auto& iter : mListOfActors )
	{
		iter->CollidePostprocess( );
	}
}

void GScene::Release( )
{
	mRendererMGR.Release( );
	mCollisionMGR.Release( );

	auto next = mListOfActors.begin( );
	const auto& end = mListOfActors.end( );

	for ( ; next != end; )
	{
		(*next)->Release( );
		if ( true == (*next)->IsDied( ) )
		{
			next = mListOfActors.erase(next);
		}
		else
		{
			++next;
		}
	}
}

std::list<SmartPtr<GActor>> GScene::GetGlobalActorList(const std::wstring& _nextSceneName)
{
	std::list<SmartPtr<GActor>> listOfGlobalActor = std::list<SmartPtr<GActor>>( );

	auto next = mListOfActors.begin( );
	const auto& end = mListOfActors.end( );

	for ( ; next != end; )
	{
		if ( false == (*next)->IsGlobalActor( ) )
		{
			++next;
			continue;
		}
		else if ( true == (*next)->IsAvailableInNextScene(_nextSceneName) )
		{
			(*next)->OnReadyFlag( );

			listOfGlobalActor.push_back((*next));
			next = mListOfActors.erase(next);
		}
		else
		{
			DebugConsole::PrintlnConsoleW((*next)->GetName( ).append(L" is not global && available in next scene") );
			++next;
		}
	}

	mRendererMGR.SetGlobalRenderer( );
	mCollisionMGR.SetGlobalCollider( );

	return listOfGlobalActor;
}

void GScene::SetGlobalActorList(std::list<SmartPtr<GActor>>&& _list)
{
	for ( auto& next : _list )
	{
		next->SetScene(this);
		next->StartSceneChange( );
		next->OffReadyFlag( );
		mListOfActors.push_back(next);
	}
}

void GScene::EndSceneChange( )
{
	for (auto& next : mListOfUpdaters )
	{
		next->EndSceneChange( );
	}
}

void GScene::StartSceneChange( )
{
	for ( auto& next : mListOfUpdaters )
	{
		next->StartSceneChange( );
	}
}
