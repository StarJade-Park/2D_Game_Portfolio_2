#pragma once

#pragma once

// 속성 확인(라이브러리, 게임 각 프로젝트)
// _WIN32는 32/64-bit 공통 부분 정의
// _WIN64는 64-bit에서만 정의
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
	// - geometry shader, compute shader등을 설정할 가능성에 따라 파이프라인 슬롯 초기화는 필수!
	//void DrawToWorld(const std::wstring& _text, const float _fontSize, GVector4D _pos, GVector4D _color, FW1_TEXT_FLAG _fwFlag = FW1_TEXT_FLAG::FW1_TOP);
	// - geometry shader, compute shader등을 설정할 가능성에 따라 파이프라인 슬롯 초기화는 필수!
	void DrawToUI(const std::wstring& _text, const float _fontSize, GVector4D _pos, GVector4D _color, FW1_TEXT_FLAG _fwFlag = FW1_TEXT_FLAG::FW1_TOP);

private:
	IFW1Factory* mFontFactory;
	IFW1FontWrapper* mFontWrapper;
};

