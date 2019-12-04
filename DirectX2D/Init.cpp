#include <..\psklib\JsonPaser.h>
#include <..\psklib\GameFileIO.h>
#include <..\psklib\GameFileSystem.h>

#include <..\DxEngine\GameWindow.h>
#include <..\DxEngine\GResource.h>
#include <..\DxEngine\GTexture.h>
#include <..\DxEngine\GSprite.h>
#include <..\DxEngine\GSound.h>

#include <..\CommonLibrary\KeyInit.h>

#include "Init.h"
#include "TitleUpdater.h"
#include "TownUpdater.h"
#include "MainMenuUpdater.h"
#include "LoadingUpdater.h"

#include "RbrbMap04.h"

void Init( )
{
	InitAllKeys( );
	GSound::Init( );

	DebugConsole::PrintConsoleW(L"Root Path : ");
	DebugConsole::PrintlnConsoleW(GFileSystem::GetRootPath( ).c_str( ));

	GameWindow::MainGameWnd( )->CreateScene(L"Loading", new LoadingUpdater( ));
	GameWindow::MainGameWnd( )->CreateScene(L"Title", new TitleUpdater( ));
	//GameWindow::MainGameWnd( )->CreateScene(L"Main_Menu", new MainMenuUpdater( ));
	//GameWindow::MainGameWnd( )->CreateScene(L"Town", new TownUpdater( ));

	GameWindow::MainGameWnd( )->CreateScene(L"RbrbMap04", new RbrbMap04( ));

	GameWindow::MainGameWnd( )->ChangeScene(L"Loading");

	DebugConsole::PrintlnConsoleW(L"Game Start...");
}
