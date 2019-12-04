#pragma once

#pragma once

// �Ӽ� Ȯ��(���̺귯��, ���� �� ������Ʈ)
// _WIN32�� 32/64-bit ���� �κ� ����
// _WIN64�� 64-bit������ ����
#ifdef _WIN32
#ifdef _WIN64			/// 64-bit  only
#ifdef _DEBUG			/// 64-bit debug
#pragma comment(lib, "..\\ThirdParty\\FW1FontWrapper\\x64\\Debug\\FW1FontWrapper.lib")
#else					/// 64-bit release
#pragma comment(lib, "..\\ThirdParty\\FW1FontWrapper\\x64\\Release\\FW1FontWrapper.lib")
#endif					// 64-bit _DEBUG
#else					/// 32-bit  only
#ifdef _DEBUG			/// 32-bit debug
#pragma comment(lib, "..\\ThirdParty\\FW1FontWrapper\\Win32\\Debug\\FW1FontWrapper.lib") 
#else					/// 32-bit release
#pragma comment(lib, "..\\ThirdParty\\FW1FontWrapper\\Win32\\Release\\FW1FontWrapper.lib")
#endif					// 32-bit _DEBUG
#endif					// _WIN64
/// 32/64 common def
#endif					// _WIN32

#include <..\ThirdParty\FW1FontWrapper\FW1CompileSettings.h>
#include <..\ThirdParty\FW1FontWrapper\FW1FontWrapper.h>

#include "GResource.h"

constexpr float default_font_size = 20.0f;

class FWFont
	: public GResource
{
public:
	FWFont( )
		: mFontFactory(nullptr), mFontWrapper(nullptr)
	{
		EMPTY_STATEMENT;
	}

	~FWFont( )
	{
		if ( nullptr != mFontWrapper )
		{
			mFontWrapper->Release( );
			mFontWrapper = nullptr;
		}

		if ( nullptr != mFontFactory )
		{
			mFontFactory->Release( );
			mFontFactory = nullptr;
		}
	}

	bool Create(const std::wstring& _fontFamilyName);
	// - geometry shader, compute shader���� ������ ���ɼ��� ���� ���������� ���� �ʱ�ȭ�� �ʼ�!
	//void DrawToWorld(const std::wstring& _text, const float _fontSize, GVector4D _pos, GVector4D _color, FW1_TEXT_FLAG _fwFlag = FW1_TEXT_FLAG::FW1_TOP);
	// - geometry shader, compute shader���� ������ ���ɼ��� ���� ���������� ���� �ʱ�ȭ�� �ʼ�!
	void DrawToUI(const std::wstring& _text, const float _fontSize, GVector4D _pos, GVector4D _color, FW1_TEXT_FLAG _fwFlag = FW1_TEXT_FLAG::FW1_TOP);

private:
	IFW1Factory* mFontFactory;
	IFW1FontWrapper* mFontWrapper;
};

