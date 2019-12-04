#include <..\DxEngine\GameWindow.h>

#include "Main.h"
#include "Init.h"
#include <..\CommonLibrary\GameOption.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	hPrevInstance; // A handle to the previous instance of the application. This parameter is always NULL. 
	lpCmdLine; // The command line for the application, excluding the program name.
	nCmdShow; // TBD
#ifdef NDEBUG

#elif _DEBUG
	MemLeakCheker::SetLeakCheckFlag( );
	DebugConsole::OpenDebugConsole( );
	//MemLeakCheker::CheckLeakByLine(273280);
#endif

	GEngineDevice::InitPath( );

	GameWindow::InitWndClass(hInstance);
	GameWindow::CreateGameWndOrNull(L"Main_Window");
	GameWindow::MainGameWnd( )->SetWndSize((int)GameOption::instance.GetWndWidth( ), (int)GameOption::instance.GetWndHeight( ));
	GameWindow::MainGameWnd( )->CreateDevice( );

	Init( );

	GameWindow::EngineLoop( );

	MSG msg = GameWindow::mMsg;
	
#ifdef _DEBUG
	DebugConsole::CloseDebugConsole( );
#endif // _DEBUG

	return (int)msg.wParam;
}