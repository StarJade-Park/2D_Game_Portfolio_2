#pragma once
#include <map>
#include <string>

#include <Windows.h>

#include "SmartPtr.h"
#include "BaseName.h"
#include "GVector.h"

class WindowController
	: public ControlBlock, public BaseName
{
	/********** member function **********/
public:
	static void InitWndClass(const HINSTANCE& _inst);

	static bool CreateWndClass(const WNDCLASSEXW& _wcex);
	static WNDCLASSEXW FindWndClass(const std::wstring& _name);
	static SmartPtr<WindowController> FindWindowCtrlOrNull(const std::wstring& _windowName);
	static SmartPtr<WindowController> GetMainWindow( );

	static MSG PeekLoopMsg(void(__stdcall *_LoopFunction)() = nullptr);

	static LRESULT CALLBACK MainWndProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static SmartPtr<WindowController> CreateNewWindowOrNull(const std::wstring& _className, const std::wstring& _windowName, const std::wstring& _titleName, bool _bIsShow = true);
	static SmartPtr<WindowController> CreateNewWindowOrNull(const std::wstring& _windowName, const std::wstring& _titleName, bool _bIsShow = true);
	static SmartPtr<WindowController> CreateNewWindowOrNull(const std::wstring& _titleName, bool _bIsShow = true);

	static bool DestroyHWindow(HWND _hWnd);

	static int GetWndCount( );

	static void UpdateAllWnd( );

	void SetWndSize(int _sizeX, int _sizeY);
	GVector2D GetWndSize( );
	void SetWndPos(int _posX, int _posY);
	void SetWndPosAndSize(int _posX, int _posY, int _sizeX, int _sizeY);

	void CursorOff( );

	void Show( );
	void Hide( );

	HWND GetHWnd( ) const
	{
		return mhWnd;
	}

	void SetHWnd(HWND _hWnd)
	{
		mhWnd = _hWnd;
	}

	HDC GetHDC( ) const
	{
		return mhDC;
	}

	GVector2D GetMousePos( );

	bool IsFullScr( )
	{
		return mbFullScr;
	}

protected:
	static void InsertToWndCtrlMap(WindowController* _wndCtrl);

	WindowController( )
		: mhWnd(nullptr)
		, mhDC(nullptr)
		, mStyle(NULL)
		, mHMenu(nullptr)
		, mWndSize({ 0.0f, 0.0f })
		, mbFullScr(FALSE)
	{
		EMPTY_STATEMENT;
	};

	virtual ~WindowController( ) = default;

	bool InitHWnd(const std::wstring& _class, const std::wstring& _title);
	void Update( );

	/********* member variable *********/
protected:
	static void(__stdcall *mLoopFunction)();
	static SmartPtr<WindowController> mMainWndCtrl;
	
private:
	static HINSTANCE mhMainInst;
	static int mWndCtrlCounter;

	// WndClass 관리 멤버 변수
	static std::map<std::wstring, WNDCLASSEXW> mMapOfWndCLASS;
	// WndClass를 통해 만들어진 Window를 관리하는 멤버변수
	static std::map<std::wstring, SmartPtr<WindowController>> mMapOfWndCtrl;

	HWND mhWnd;
	HDC mhDC; // device context handle variable

	DWORD mStyle;
	// 우리는 메뉴를 쓰지 않을 것이므로
	// 남는 Handle을 원하는 동작을 위한 용도로 변경
	HMENU mHMenu;

	GVector2D mWndSize;
	GVector2D mMousePos;

	BOOL mbFullScr;
};

