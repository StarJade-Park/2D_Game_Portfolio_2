#include "InstanceBuffer.h"

bool InstanceBuffer::Create(UINT _maxCount, const type_info* _typeInfo, size_t _typeSize)
{
	if ( nullptr != mBuffer )
	{
		mBuffer->Release( );
	}

	mDataType = _typeInfo;
	mVertexCount = _maxCount;
	mVertexSize = (UINT)_typeSize;
	mBufferDescription.ByteWidth = mVertexCount * mVertexSize;
	mBufferDescription.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	mBufferDescription.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	mData.resize(mBufferDescription.ByteWidth);

	if ( FAILED(GEngineDevice::MainDevice( ).GetIDevice( ).CreateBuffer(&mBufferDescription, nullptr, &mBuffer)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

void InstanceBuffer::UpdateInstanceData( )
{
	if ( mDataCount > mVertexCount )
	{
		Create(mDataCount, mDataType, mVertexSize);
	}

	D3D11_MAPPED_SUBRESOURCE tMap = D3D11_MAPPED_SUBRESOURCE( );

	GEngineDevice::MainDevice( ).GetIContext( ).Map(mBuffer, NULL, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	memcpy_s(tMap.pData, mBufferDescription.ByteWidth, &mData[0], mData.size( ));
	GEngineDevice::MainDevice( ).GetIContext( ).Unmap(mBuffer, NULL);
}
