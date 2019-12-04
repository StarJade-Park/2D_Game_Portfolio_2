#include "VertexGShader.h"

bool VertexGShader::Load(const UINT _verHigh, const UINT _verLow, const char* _hlslFunction)
{
	mVerLigh = _verHigh;
	mVerLow = _verLow;

	UINT flag = NULL;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	sprintf_s(mShaderVerStr, "vs_%d_%d", mVerLigh, mVerLow);

	if ( FAILED(D3DCompileFromFile(
		GetAbsPath( ).c_str( ),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_hlslFunction,
		mShaderVerStr,
		flag,
		NULL,
		&miBlob,
		&miErrBlob))
		)
	{
		std::string ErrorText = (char*)miErrBlob->GetBufferPointer( );
		DebugConsole::PrintlnConsoleA(ErrorText.c_str( ));

		CRASH_PROG;
		return false;
	}

	if ( S_OK != GEngineDevice::MainDevice( ).GetIDevice( ).CreateVertexShader(
		miBlob->GetBufferPointer( ),
		miBlob->GetBufferSize( ),
		nullptr,
		&mVtxShader
		) )
	{
		CRASH_PROG;
		OutputDebugStringW(L"Fail : CreateVertexShader");
		return false;
	}

	return true;
}

void VertexGShader::Update( )
{
	if ( nullptr != miInputLayout )
	{
		GEngineDevice::MainDevice( ).GetIContext( ).IASetInputLayout(miInputLayout);
	}

	if ( nullptr != mVtxShader )
	{
		GEngineDevice::MainDevice( ).GetIContext( ).VSSetShader(mVtxShader, nullptr, 0);
	}
}

void VertexGShader::AddLayOut(const char* _smName, UINT _index, DXGI_FORMAT _fmt, UINT _inputSlot, UINT _IDSR, D3D11_INPUT_CLASSIFICATION _input)
{
	if ( mVecOfInLayout.size( ) <= _inputSlot )
	{
		mVecOfInLayout.resize(_inputSlot + 1);
		mVecOfInLayout[_inputSlot] = new InputLayout( );
	}

	D3D11_INPUT_ELEMENT_DESC inputData;

	inputData.SemanticName = _smName;
	inputData.SemanticIndex = _index;
	inputData.Format = _fmt;
	inputData.InputSlot = _inputSlot;
	inputData.InputSlotClass = _input;
	inputData.InstanceDataStepRate = _IDSR;
	inputData.AlignedByteOffset = mVecOfInLayout[_inputSlot]->mOffset;

	UINT fmtSize = GetFormatByteSize(inputData.Format);
	if ( 0 == fmtSize )
	{
		CRASH_PROG;
		OutputDebugStringW(L"Invaild Format!!");
		return;
	}

	mVecOfInLayout[_inputSlot]->mOffset += fmtSize;
	mVecOfInLayout[_inputSlot]->mVecOfInputData.push_back(inputData);
}

void VertexGShader::EndLayOut(const char* _smName, UINT _index, DXGI_FORMAT _fmt, UINT _inputSlot, UINT _IDSR, D3D11_INPUT_CLASSIFICATION _input)
{
	AddLayOut(_smName, _index, _fmt, _inputSlot, _IDSR, _input);

	std::vector<D3D11_INPUT_ELEMENT_DESC> vecOfLayout = std::vector<D3D11_INPUT_ELEMENT_DESC>( );
	vecOfLayout.reserve(mVecOfInLayout.size( ) * mVecOfInLayout[0]->mVecOfInputData.size( ));

	for ( auto& layout : mVecOfInLayout )
	{
		for ( auto& inputData : layout->mVecOfInputData )
		{
			vecOfLayout.push_back(inputData);
		}
	}

	if ( S_OK != GEngineDevice::MainDevice( ).GetIDevice( ).CreateInputLayout(
		&vecOfLayout[0],
		(UINT)vecOfLayout.size( ),
		miBlob->GetBufferPointer( ),
		miBlob->GetBufferSize( ),
		&miInputLayout
		) )
	{
		CRASH_PROG;
		OutputDebugStringW(L"Fail : CreateInputLayout");
		return;
	}

	//mVecOfInLayout.clear( );
}