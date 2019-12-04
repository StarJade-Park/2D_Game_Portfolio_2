#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>

#include "GameWindow.h"
#include "GSound.h"

GameWindow* GameWindow::mMainGameWnd = nullptr;
std::map<std::wstring, SmartPtr<GameWindow>> GameWindow::mMapOfGameWnd = std::map<std::wstring, SmartPtr<GameWindow>>( );
MSG GameWindow::mMsg = MSG( );

SmartPtr<GameWindow> GameWindow::FindGWndByNameOrNull(const std::wstring& _wndName)
{
	const auto& find = mMapOfGameWnd.find(_wndName);

	if ( mMapOfGameWnd.end( ) != find )
	{
		return find->second;
	}

	return nullptr;
}

SmartPtr<GameWindow> GameWindow::CreateGameWndOrNull(const std::wstring& _wndName)
{
	if ( nullptr != FindGWndByNameOrNull(_wndName) )
	{
		CRASH_PROG;
		return nullptr;
	}

	GameWindow* newGameWin = new GameWindow( );
	newGameWin->SetName(_wndName);

	if ( false == newGameWin->InitHWnd(L"Default", _wndName) )
	{
		CRASH_PROG;
		return nullptr;
	}

	if ( 0 == mMapOfGameWnd.size( ) )
	{
		mMainWndCtrl = newGameWin;
		mMainGameWnd = newGameWin;
	}

	newGameWin->Show( );
	mMapOfGameWnd.insert(std::map<std::wstring, SmartPtr<GameWindow>>::value_type(_wndName, newGameWin));
	InsertToWndCtrlMap(newGameWin);

	return newGameWin;
}

SmartPtr<GameWindow> GameWindow::CreateGameWndOrNull(const std::wstring& _wndName, HWND _hWnd)
{
	if ( nullptr == _hWnd )
	{
		CRASH_PROG;
		return nullptr;
	}

	GameWindow* newGameWin = new GameWindow( );
	newGameWin->SetName(_wndName);
	newGameWin->SetHWnd(_hWnd);
	RECT Rc;
	GetWindowRect(_hWnd, &Rc);
	GetClientRect(_hWnd, &Rc);
	newGameWin->SetWndSize(Rc.right, Rc.bottom);

	if ( 0 == mMapOfGameWnd.size( ) )
	{
		mMainWndCtrl = newGameWin;
		mMainGameWnd = newGameWin;
	}

	mMapOfGameWnd.insert(std::map<std::wstring, SmartPtr<GameWindow>>::value_type(_wndName, newGameWin));
	InsertToWndCtrlMap(newGameWin);

	return newGameWin;
}

void GameWindow::EngineLoop( )
{
	GameTime::Reset( );
	mMsg = WindowController::PeekLoopMsg(UpdateGWnd);
}

void __stdcall GameWindow::UpdateGWnd( )
{
	GameTime::Update( );
	GameKeyInput::Update( );
	GSound::Update( );

	for ( auto& next : mMapOfGameWnd )
	{
		next.second->Update( );
		next.second->UpdateScene( );
	}
}

void GameWindow::ChangeScene(const std::wstring& _sceneName)
{
	if ( nullptr != mCurScene && _sceneName == mCurScene->GetName( ) )
	{
		return;
	}

	const auto& find = mMapOfScene.find(_sceneName);

	if ( find == mMapOfScene.end( ) )
	{
		CRASH_PROG;
		return;
	}

	mNextScene = find->second;
}

void GameWindow::UpdateScene( )
{
	if ( nullptr != mNextScene )
	{
		if ( mNextScene == mCurScene )
		{
			CRASH_PROG;
			return;
		}

		if ( nullptr != mCurScene )
		{
			mCurScene->EndSceneChange( );
			mNextScene->SetGlobalActorList(mCurScene->GetGlobalActorList(mNextScene->GetName( )));
		}

		mNextScene->CheckLoaded( );
		mCurScene = mNextScene;
		mCurScene->StartSceneChange( );
		mNextScene = nullptr;
	}

	mCurScene->Progress( );
}

GameWindow::GameWindow( )
	: mMapOfScene(std::map<std::wstring, SmartPtr<GScene>>( ))
	, mDevice(nullptr)
	, mCurScene(nullptr), mNextScene(nullptr), mNewScene(nullptr)
{
	EMPTY_STATEMENT;
}


GameWindow::~GameWindow( )
{
	EMPTY_STATEMENT;
}

void GameWindow::CreateDevice( )
{
	mDevice = new GEngineDevice( );
	mDevice->Init(this);
}
