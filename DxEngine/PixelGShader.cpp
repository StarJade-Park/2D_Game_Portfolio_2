#include "PixelGShader.h"

bool PixelGShader::Load(UINT _verHigh, UINT _verLow, const char* _hlslFunction)
{
	mVerLigh = _verHigh;
	mVerLow = _verLow;

	UINT flag = NULL;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	sprintf_s(mShaderVerStr, "ps_%d_%d", mVerLigh, mVerLow);

	if ( S_OK != D3DCompileFromFile(
		GetAbsPath( ).c_str( ),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_hlslFunction,
		mShaderVerStr,
		flag,
		NULL,
		&miBlob,
		&miErrBlob
	) )
	{
		std::string ErrorText = (char*)miErrBlob->GetBufferPointer( );
		DebugConsole::PrintlnConsoleA(ErrorText.c_str( ));

		CRASH_PROG;
		return false;
	}

	if ( S_OK != GEngineDevice::MainDevice( ).GetIDevice( ).CreatePixelShader(
		miBlob->GetBufferPointer( ),
		miBlob->GetBufferSize(),
		nullptr,
		&mPixShader
	) )
	{
		CRASH_PROG;
		OutputDebugStringW(L"Fail : CreatePixelShader");
		return false;
	}

	return true;
}

void PixelGShader::Update( )
{
	if ( nullptr != mPixShader )
	{
		GEngineDevice::MainDevice( ).GetIContext( ).PSSetShader(mPixShader, nullptr, NULL);
	}
}
