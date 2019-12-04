#include <..\psklib\GameTime.h>

#include "WindowController.h"

HINSTANCE WindowController::mhMainInst;
int WindowController::mWndCtrlCounter = 0;
void(__stdcall *WindowController::mLoopFunction)() = nullptr;
SmartPtr<WindowController> WindowController::mMainWndCtrl = nullptr;

std::map<std::wstring, WNDCLASSEXW> WindowController::mMapOfWndCLASS;
std::map<std::wstring, SmartPtr<WindowController>> WindowController::mMapOfWndCtrl;

// HINSTANCE�� �޾� WNDCLASSEXW�� �ʱ�ȭ�ϰ� ����մϴ�.
void WindowController::InitWndClass(const HINSTANCE& _inst)
{
	mhMainInst = _inst;

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _inst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Default";
	wcex.hIconSm = NULL;

	CreateWndClass(wcex);
}

bool WindowController::CreateWndClass(const WNDCLASSEXW& _wcex)
{
	// RegisterClassExW�� �����ϸ� 0�� ��ȯ
	if ( 0 == RegisterClassExW(&_wcex) )
	{
		assert_msg("Fail class regist");
		return false;
	}

	mMapOfWndCLASS.insert(
		std::map<std::wstring, WNDCLASSEXW>::value_type(_wcex.lpszClassName, _wcex)
	);

	return true;
}

WNDCLASSEXW WindowController::FindWndClass(const std::wstring& _name)
{
	const auto& findIter = mMapOfWndCLASS.find(_name);

	if ( mMapOfWndCLASS.end( ) == findIter )
	{
		WNDCLASSEXW W;
		W.hInstance = 0;
		return W;
	}
	return findIter->second;
}

SmartPtr<WindowController> WindowController::FindWindowCtrlOrNull(const std::wstring& _windowName)
{
	const auto& findIter = mMapOfWndCtrl.find(_windowName);

	if ( mMapOfWndCtrl.end( ) == findIter )
	{
		return nullptr;
	}

	return findIter->second;
}

SmartPtr<WindowController> WindowController::GetMainWindow( )
{
	return mMainWndCtrl;
}

MSG WindowController::PeekLoopMsg(void(__stdcall *_LoopFunction)())
{
	MSG msg = MSG( );

	while ( 0 != WindowController::GetWndCount( ) )
	{
		if ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/* Game Logic */
			_LoopFunction( );
		}
	}

	return msg;
}

// â�� �޽����� ó���մϴ�.
LRESULT WindowController::MainWndProcedure(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch ( _message )
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		/*HDC hdc = */BeginPaint(_hWnd, &ps);
		EndPaint(_hWnd, &ps); // ��û���� ������ ����ؼ� �׸��� ���� �޽��� ����
		break;
	}
	case WM_DESTROY:
		//PostQuitMessage (0);
		DestroyHWindow(_hWnd);
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}

	return 0;
}

// â�� class, window, title �̸��� ���Ͽ� �����մϴ�.
// _bIsShow�� default�� true�Դϴ�.
SmartPtr<WindowController> WindowController::CreateNewWindowOrNull(const std::wstring& _className, const std::wstring& _windowName, const std::wstring& _titleName, bool _bIsShow /*= true*/)
{
	// �����ϰ��� �ϴ� â�� �̹� �ִ°�
	if ( mMapOfWndCtrl.end( ) != mMapOfWndCtrl.find(_className) )
	{
		assert_msg("already created window");
		return nullptr;
	}
	WindowController* newWnd = new WindowController( );
	newWnd->SetName(_windowName);

	if ( false == newWnd->InitHWnd(_className, _titleName) )
	{
		assert_msg("fail InitHWnd");
		delete newWnd; // ���н� ����� nullptr ��ȯ
		return nullptr;
	}

	if ( 0 == mMapOfWndCtrl.size( ) )
	{
		mMainWndCtrl = newWnd;
	}

	mMapOfWndCtrl.insert(
		std::map<std::wstring, WindowController*>::value_type(_windowName, newWnd)
	);

	if ( true == _bIsShow )
	{
		newWnd->Show( );
	}
	else
	{
		newWnd->Hide( );
	}

	++mWndCtrlCounter;

	return newWnd;
}

// �뵵 : â�� window, title �̸��� ���Ͽ� �����մϴ�.
// _bIsShow�� default�� true�Դϴ�.
// class�� �̸��� Deault�� �˴ϴ�.
SmartPtr<WindowController> WindowController::CreateNewWindowOrNull(const std::wstring& _windowName, const std::wstring& _titleName, bool _bIsShow /*= true*/)
{
	return CreateNewWindowOrNull(L"Default", _windowName, _titleName, _bIsShow);
}

// â�� title �̸��� ���Ͽ� �����մϴ�.
// _bIsShow�� default�� true�Դϴ�.
// class, window�� �̸��� Deault�� �˴ϴ�.
SmartPtr<WindowController> WindowController::CreateNewWindowOrNull(const std::wstring& _titleName, bool _bIsShow /*= true*/)
{
	return CreateNewWindowOrNull(L"Default", _titleName, _titleName, _bIsShow);
}

bool WindowController::DestroyHWindow(HWND _hWnd)
{
	auto Loop = mMapOfWndCtrl.begin( );
	const auto& End = mMapOfWndCtrl.end( );

	for ( ; Loop != End; ++Loop )
	{
		if ( Loop->second->mhWnd == _hWnd )
		{
			if ( nullptr != Loop->second )
			{
				//delete Loop->second;
				Loop->second = nullptr;
			}
			mMapOfWndCtrl.erase(Loop);
			--mWndCtrlCounter;
			return true;
		}
	}

	return true;
}

int WindowController::GetWndCount( )
{
	return mWndCtrlCounter;
}

void WindowController::UpdateAllWnd( )
{
	for ( auto& iter : mMapOfWndCtrl )
	{
		iter.second->Update( );
	}
}

void WindowController::SetWndSize(int _sizeX, int _sizeY)
{
	mWndSize.m.x = (float)_sizeX;
	mWndSize.m.y = (float)_sizeY;

	RECT rc = { 0, 0, _sizeX, _sizeY };
	AdjustWindowRect(&rc, mStyle, false);

	SetWindowPos(
		mhWnd, NULL,
		NULL, NULL,
		rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER
	);
}

GVector2D WindowController::GetWndSize( )
{
	return mWndSize;
}

void WindowController::SetWndPos(int _posX, int _posY)
{
	SetWindowPos(mhWnd, 0, _posX, _posY, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
}

void WindowController::SetWndPosAndSize(int _posX, int _posY, int _sizeX, int _sizeY)
{
	SetWndPos(_posX, _posY);
	SetWndSize(_sizeX, _sizeY);
}

void WindowController::CursorOff( )
{
	SetCursor(NULL);
}

void WindowController::Show( )
{
	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);
}

void WindowController::Hide( )
{
	ShowWindow(mhWnd, SW_HIDE);
	UpdateWindow(mhWnd);
}

GVector2D WindowController::GetMousePos( )
{
	return mMousePos;
}

void WindowController::InsertToWndCtrlMap(WindowController* _wndCtrl)
{
	if ( nullptr != FindWindowCtrlOrNull(_wndCtrl->GetName( ).c_str( )) )
	{
		CRASH_PROG;
		return;
	}

	mMapOfWndCtrl.insert(std::map<std::wstring, WindowController*>::value_type(_wndCtrl->GetName( ), _wndCtrl));
	++mWndCtrlCounter;
}

bool WindowController::InitHWnd(const std::wstring& _class, const std::wstring& _title)
{
	WNDCLASSEXW findClass = FindWndClass(_class);

	if ( 0 == findClass.hInstance )
	{
		assert_msg("hInstance in find class is 0");
	}

	mStyle = WS_OVERLAPPEDWINDOW;
	mHMenu = GetMenu(mhWnd);

	if ( nullptr == findClass.lpszMenuName )
	{
		mHMenu = FALSE;
	}

	mhWnd = CreateWindowW(_class.c_str( ), _title.c_str( ), mStyle,
						  0, 0, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, mhMainInst, nullptr);

	if ( nullptr == mhWnd )
	{
		assert_msg("mhWnd is null");
		return false;
	}

	mhDC = GetDC(mhWnd);

	if ( nullptr == mhDC )
	{
		assert_msg("mhDC is null");
		return false;
	}

	return true;
}

void WindowController::Update( )
{
	//CursorOff( );

	POINT point = POINT( );
	GetCursorPos(&point);
	ScreenToClient(mhWnd, &point);

	mMousePos = GVector2D((float)point.x, (float)point.y);
}
