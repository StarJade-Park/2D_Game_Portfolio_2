#include "FWFont.h"

bool FWFont::Create(const std::wstring& _fontFamilyName)
{
	if ( FAILED(FW1CreateFactory(FW1_VERSION, &mFontFactory)) )
	{
		CRASH_PROG;
		return false;
	}

	if ( FAILED(mFontFactory->CreateFontWrapper(&GEngineDevice::MainDevice( ).GetIDevice( ), _fontFamilyName.c_str( ), &mFontWrapper)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

//void FWFont::DrawToWorld(const std::wstring& _text, const float _fontSize, GVector4D _pos, GVector4D _color, FW1_TEXT_FLAG _fwFlag)
//{
//	if ( nullptr == mFontWrapper )
//	{
//		CRASH_PROG;
//		return;
//	}
//
//	// todo 필요하면 만들기
//	//mFontWrapper->DrawGeometry(&GEngineDevice::MainDevice( ).GetIContext( ), );
//}

void FWFont::DrawToUI(const std::wstring& _text, const float _fontSize, GVector4D _pos, GVector4D _color, FW1_TEXT_FLAG _fwFlag)
{
	if ( nullptr == mFontWrapper )
	{
		CRASH_PROG;
		return;
	}

	mFontWrapper->DrawString(
		&GEngineDevice::MainDevice( ).GetIContext( ),
		_text.c_str( ), _fontSize,
		_pos.m.x, -_pos.m.y,
		_color.ToUIntRGBA( ),
		_fwFlag
	);
}
