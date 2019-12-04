#include "IndexBuffer.h"

bool IndexBuffer::Create(const DXGI_FORMAT& _idxFormat, UINT _iIdxCount, UINT _iIdxSize, const D3D11_USAGE& _idxUsage, void* _data)
{
	mIndexCount = _iIdxCount; // 4개
	mIndexSize = _iIdxSize; // 16바이트
	mFormat = _idxFormat;
	mBufferDescription.ByteWidth = mIndexCount * mIndexSize;
	mBufferDescription.Usage = _idxUsage;

	if ( D3D11_USAGE::D3D11_USAGE_DYNAMIC == mBufferDescription.Usage )
	{
		mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	}

	mBufferDescription.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResData = D3D11_SUBRESOURCE_DATA( );
	subResData.pSysMem = _data;

	if ( FAILED(GEngineDevice::MainDevice( ).GetIDevice( ).CreateBuffer(&mBufferDescription, &subResData, &mBuffer)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}
