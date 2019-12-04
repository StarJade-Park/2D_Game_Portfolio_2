#include "VertexBuffer.h"

bool VertexBuffer::Create(UINT _iVtxCount, UINT _iVtxSize, const D3D11_USAGE& _vtxUsage, void * _data)
{
	mVertexCount = _iVtxCount; // 4개
	mVertexSize = _iVtxSize; // 16바이트
	mBufferDescription.ByteWidth = _iVtxCount * _iVtxSize;
	mBufferDescription.Usage = _vtxUsage;

	if ( D3D11_USAGE::D3D11_USAGE_DYNAMIC == mBufferDescription.Usage )
	{
		mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	}

	mBufferDescription.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA DataStruct = D3D11_SUBRESOURCE_DATA( );
	DataStruct.pSysMem = _data;

	if ( FAILED(GEngineDevice::MainDevice( ).GetIDevice( ).CreateBuffer(&mBufferDescription, &DataStruct, &mBuffer)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}
