#pragma once
#include <map>
#include <list>

#include <..\psklib\BaseName.h>
#include <..\psklib\SmartPtr.h>
#include <..\psklib\WindowController.h>

#include "BaseUpdater.h"
#include "ComTransform.h"
#include "GActor.h"
#include "RendererManager.h"
#include "CollisionManager.h"

class GScene final
	: public BaseName, public ControlBlock, public BaseUpdater
{
	friend class GameWindow;
#pragma region SceneUpdater
public:
	// - virtual function list
	// void LoadScene( ) override; // pure virtual
	// void UpdateScene( ) override;
	// void StartSceneChange( ) override;
	// void EndSceneChange( ) override;
	// note HSCENECOM
	class GSceneUpdater
		: public ControlBlock
	{
		friend GScene;
	public:
		GScene& GetScene( )
		{
			return *mScene;
		}

		void SetScene(GScene* _scene)
		{
			mScene = _scene;
		}

	private:
		virtual void LoadScene( ) = 0;

		virtual void UpdateScene( )
		{
			EMPTY_STATEMENT;
		}

		virtual void StartSceneChange( )
		{
			EMPTY_STATEMENT;
		}

		virtual void EndSceneChange( )
		{
			EMPTY_STATEMENT;
		}

	protected:
		GSceneUpdater( )
			: mScene(nullptr), mbLoaded(false)
		{
			EMPTY_STATEMENT;
		}

		~GSceneUpdater( ) = default;

	private:
		GScene* mScene;
		bool mbLoaded;
	};
#pragma endregion
public:
	template<typename Updater>
	static void PushToListOfUpdaters( )
	{
		Updater* newUpdater = new Updater( );
		PushToListOfUpdaters(newUpdater);
	}

	WindowController& GetWnd( ) const
	{
		return *mWindowCtrl;
	}

	SmartPtr<GActor> CreateActor(const std::wstring& _actorName = L"Default Actor Name");

	// note transform이 필요없는 actor 생성시 만들 것
	//SmartPtr<GActor> CreateNoneTransformActor(const wchar_t* _actorName = L"Default Actor Name");

	void EndSceneChange( );
	void StartSceneChange( );

private:
	virtual void CheckLoaded( )
	{
		for ( auto& iter : mListOfUpdaters )
		{
			if ( false == iter->mbLoaded )
			{
				iter->LoadScene( );
				iter->mbLoaded = true;
			}
		}
	}

	GScene( );
	~GScene( );

	void SetWnd(WindowController* _window)
	{
		mWindowCtrl = _window;
	}

	void PushToListOfUpdaters(SmartPtr<GSceneUpdater> _updater);

	void UpdateSceneUpdater( );
	void CheckChildActorCanUpdate( );

	void Progress( );

	void UpdatePreprocess( ) override;
	void Update( ) override;
	void UpdatePostprocess( ) override;

	void RenderPreprocess( ) override;
	void Render( ); // note RendererManager에서 처리함
	void RenderPostprocess( ) override;

	void CollidePreprocess( ) override;
	void Collide( ); // note CollisionManager에서 처리함
	void CollidePostprocess( ) override;

	void Release( ) override;

	std::list<SmartPtr<GActor>> GetGlobalActorList(const std::wstring& _NextSceneName);
	void SetGlobalActorList(std::list<SmartPtr<GActor>>&& _List);

public:
	RendererManager mRendererMGR;
	CollisionManager mCollisionMGR;

private:
	std::list<SmartPtr<GSceneUpdater>> mListOfUpdaters;
	std::list<SmartPtr<GActor>> mListOfActors;

	WindowController* mWindowCtrl;
};

