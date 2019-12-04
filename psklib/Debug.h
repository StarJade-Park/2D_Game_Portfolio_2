#pragma once

#include <string>
#include <Windows.h>
#include <iostream>
#include <assert.h>
#include <crtdbg.h>

#define FALL_THROUGH (void(0))
#define EMPTY_STATEMENT (void(0))

#define CRASH_PROG assert(false)
#define assert_with_msg(expr, msg) assert(expr && msg)
#define assert_msg(msg) assert(msg && false)

// - Static class for check memory leak
class MemLeakCheker final
{
public:
	MemLeakCheker( ) = delete;
	~MemLeakCheker( ) = delete;

	// 메모리의 누수를 확인할 수 있습니다.
	// _CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
	static void SetLeakCheckFlag( )
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	// - Leak line 찾는 함수
	static void CheckLeakByLine(const long _leakLineNumber)
	{
#ifdef DEBUG
		_CrtSetBreakAlloc(_leakLineNumber);
#else
		_leakLineNumber;
#endif // DEBUG
	}
};

class DebugConsole final
{
public:
	// - 디버그용 콘솔을 열고 stdin, stdout을 생성된 콘솔에 쓰도록 하는 함수
	static void OpenDebugConsole( )
	{
		if ( 0 != AllocConsole( ) )
		{
			FILE *acStreamOut = nullptr;
			FILE *acStreamIn = nullptr;

			freopen_s(&acStreamOut, "CONOUT$", "wt", stdout);
			freopen_s(&acStreamIn, "CONIN$", "r", stdin);

			mbConsoleFlag = true;

			std::wcout.imbue(std::locale("kor"));
			std::wcout << L"---- Debug Console ----" << std::endl;
		}
		else
		{
			assert_msg("FAIL : OpenConsole( )");
		}
	}

	// - 디버그 콘솔에 한 줄 쓰기(줄 내림 포함)
	static void PrintlnConsoleW(const std::wstring& _text)
	{
		PrintlnConsoleW(_text.c_str( ));
	}

	// - 디버그 콘솔에 한 줄 쓰기(줄 내림 포함)
	static void PrintlnConsoleW(const wchar_t* _text)
	{
		if ( false == mbConsoleFlag )
		{
			return;
		}

		std::wcout << _text << std::endl;
	}

	// - 디버그 콘솔에 한 줄 쓰기(줄 내림 미포함)
	static void PrintConsoleW(const std::wstring& _text)
	{
		PrintConsoleW(_text.c_str( ));
	}

	// - 디버그 콘솔에 한 줄 쓰기(줄 내림 미포함)
	static void PrintConsoleW(const wchar_t* _text)
	{
		if ( false == mbConsoleFlag )
		{
			return;
		}
		std::wcout << _text;
	}

	// - 디버그 콘솔에 한 줄 쓰기(줄 내림 포함)
	static void PrintlnConsoleA(const std::string& _text = "")
	{
		PrintlnConsoleA(_text.c_str( ));
	}

	// - 디버그 콘솔에 한 줄 쓰기(줄 내림 포함)
	static void PrintlnConsoleA(const char* _text = "")
	{
		if ( false == mbConsoleFlag )
		{
			return;
		}
		std::cout << _text << std::endl;
	}

	// - 디버그 콘솔에 한 줄 쓰기(줄 내림 미포함)
	static void PrintConsoleA(const std::string& _text)
	{
		PrintConsoleA(_text.c_str( ));
	}

	// - 디버그 콘솔에 한 줄 쓰기(줄 내림 미포함)
	static void PrintConsoleA(const char* _text)
	{
		if ( false == mbConsoleFlag )
		{
			return;
		}
		std::cout << _text;
	}
	
	static void PrintOutputWnd(const std::wstring& _text)
	{
		if ( false == mbConsoleFlag )
		{
			return;
		}
		OutputDebugStringW(std::wstring(_text + L"\n").c_str( ));
	}

	// - 디버그용 콘솔을 닫는 함수
	static void CloseDebugConsole( )
	{
		if ( true == mbConsoleFlag )
		{
			if ( 0 == FreeConsole( ) )
			{
				assert_msg("FAIL : FreeConsole( )");
			}
		}
	}

private:
	DebugConsole( ) = delete;
	~DebugConsole( ) = delete;

private:
	static bool mbConsoleFlag;
};
