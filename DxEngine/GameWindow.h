#pragma once
#include <..\psklib\WindowController.h>

#include "GEngineDevice.h"
#include "GScene.h"

class GameWindow final
	: public WindowController
{
public:
	static SmartPtr<GameWindow> FindGWndByNameOrNull(const std::wstring& _wndName);
	static SmartPtr<GameWindow> CreateGameWndOrNull(const std::wstring& _wndName);
	static SmartPtr<GameWindow> CreateGameWndOrNull(const std::wstring& _wndName, HWND _hWnd);
	static void EngineLoop( );
	static void __stdcall UpdateGWnd( );

	static SmartPtr<GameWindow> MainGameWnd( )
	{
		return mMainGameWnd;
	}

	void ChangeScene(const std::wstring& _sceneName);

	template<typename ...Rest>
	void CreateScene(const std::wstring& _sceneName, Rest ..._arg)
	{
		mNewScene = new GScene( );
		mNewScene->SetWnd(this);
		mNewScene->SetName(_sceneName);
		PushToSceneMap(_arg...);
	}

	template<typename ...Rest>
	void PushToSceneMap(GScene::GSceneUpdater* _updater, Rest ..._arg)
	{
		mNewScene->PushToListOfUpdaters(_updater);
		PushToSceneMap(_arg...);
	}

	void PushToSceneMap( )
	{
		mMapOfScene.insert(std::map<std::wstring, SmartPtr<GScene>>::value_type(mNewScene->GetName( ), mNewScene));
		mNewScene = nullptr;
	}

	void UpdateScene( );

	GameWindow( );
	~GameWindow( );

	void CreateDevice( );

public:
	static MSG mMsg;

private:
	static GameWindow* mMainGameWnd;
	static std::map<std::wstring, SmartPtr<GameWindow>> mMapOfGameWnd;

	std::map<std::wstring, SmartPtr<GScene>> mMapOfScene;
	SmartPtr<GEngineDevice> mDevice;
	SmartPtr<GScene> mCurScene;
	SmartPtr<GScene> mNextScene;
	SmartPtr<GScene> mNewScene;
};

