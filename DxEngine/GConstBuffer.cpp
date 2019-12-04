#include "GConstBuffer.h"

bool GConstBuffer::Create(shader_type::TYPE _shaderType, size_t _size, UINT _registerNumber)
{
	meShaderType = _shaderType;
	mReg = _registerNumber;

	switch ( meShaderType )
	{
	case shader_type::TYPE::VS:
		ShUpdateFunction = &GConstBuffer::VSUpdate;
		break;
	case shader_type::TYPE::HS:
		ShUpdateFunction = &GConstBuffer::HSUpdate;
		break;
	case shader_type::TYPE::DS:
		ShUpdateFunction = &GConstBuffer::DSUpdate;
		break;
	case shader_type::TYPE::GS:
		ShUpdateFunction = &GConstBuffer::GSUpdate;
		break;
	case shader_type::TYPE::PS:
		ShUpdateFunction = &GConstBuffer::PSUpdate;
		break;
	case shader_type::TYPE::NONE:
	default:
		CRASH_PROG;
		return false;
	}

	mData = new char[_size];

	mBufDesc.ByteWidth = (UINT)_size;
	mBufDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	if ( D3D11_USAGE::D3D11_USAGE_DYNAMIC == mBufDesc.Usage )
	{
		mBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	}
	mBufDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER; // 다른 flag와 엮을 수 없음
	mBufDesc.MiscFlags = 0; // 0 is unused

	mSubResData.pSysMem = mData;
	mMappedSubRes.pData = mData;

	if ( FAILED(GEngineDevice::MainDevice( ).GetIDevice( ).CreateBuffer(&mBufDesc, &mSubResData, &mBuffer)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

void GConstBuffer::SetData(void* _data, UINT _size)
{
	// note 구조 바꾸면서 지워질 코드
	GEngineDevice::MainDevice( ).GetIContext( ).Map(
		mBuffer,
		0,
		D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
		0,
		&mMappedSubRes
	);
	memcpy_s(mMappedSubRes.pData, mBufDesc.ByteWidth, _data, _size);
	GEngineDevice::MainDevice( ).GetIContext( ).Unmap(mBuffer, 0);
}

void GConstBuffer::Update( )
{
	(this->*ShUpdateFunction)( );
}

void GConstBuffer::VSUpdate( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).VSSetConstantBuffers(mReg, 1, &mBuffer);
}

void GConstBuffer::HSUpdate( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).HSSetConstantBuffers(mReg, 1, &mBuffer);
}

void GConstBuffer::DSUpdate( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).DSSetConstantBuffers(mReg, 1, &mBuffer);
}

void GConstBuffer::GSUpdate( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).GSSetConstantBuffers(mReg, 1, &mBuffer);
}

void GConstBuffer::PSUpdate( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).PSSetConstantBuffers(mReg, 1, &mBuffer);
}
