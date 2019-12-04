#include "GMesh.h"
#include "GEngineDevice.h"

GMesh::GMesh( )
	: mInstanceBuffer(nullptr)
	, mVecOfVertexBuffer( ), mVecOfVertexBufferSize( ), mVecOfVertexBufferOffset( )
	, mVecOfIndexBuffer( ), mVecOfIndexBufferOffset( )
{
	EMPTY_STATEMENT;
}

GMesh::~GMesh( )
{
	EMPTY_STATEMENT;
}

void GMesh::UpdateAllVertexBuffer( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).IASetVertexBuffers(
		0,
		(UINT)mVecOfVertexBuffer.size( ),
		&mVecOfVertexBuffer[0],
		&mVecOfVertexBufferSize[0],
		&mVecOfVertexBufferOffset[0]
	);
}

void GMesh::UpdateVertexBuffer(const UINT _index)
{
	GEngineDevice::MainDevice( ).GetIContext( ).IASetVertexBuffers(
		0,
		1,
		&mVecOfVertexBuffer[_index],
		&mVecOfVertexBufferSize[_index],
		&mVecOfVertexBufferOffset[_index]
	);
}

void GMesh::UpdateIndexBuffer(const UINT _index)
{
	GEngineDevice::MainDevice( ).GetIContext( ).IASetIndexBuffer(
		mVecOfIndexBuffer[_index],
		mVecOfIB[_index]->GetFormat( ),
		mVecOfIndexBufferOffset[_index]
	);
}

void GMesh::Update(UINT _vtxBufferIndex, UINT _idxBufferIndex)
{
	UpdateVertexBuffer(_vtxBufferIndex);
	UpdateIndexBuffer(_idxBufferIndex);
}

void GMesh::RenderInstanced(const UINT _index)
{
	if ( nullptr == mInstanceBuffer )
	{
		CRASH_PROG;
		return;
	}

	GEngineDevice::MainDevice( ).GetIContext( ).DrawIndexedInstanced(
		mVecOfIB[_index]->GetIndexCount( ),
		mInstanceBuffer->GetDataCurrentCount(),
		0, 0, 0
	);
}

void GMesh::Render(UINT _idxBufferIndex)
{
	GEngineDevice::MainDevice( ).GetIContext( ).DrawIndexed(
		mVecOfIB[_idxBufferIndex]->GetIndexCount( ),
		0, 0
	);
}

void GMesh::RenderAutomation( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).DrawAuto( );
}

void GMesh::AddVertexBuffer(const std::wstring& _name)
{
	SmartPtr<VertexBuffer> foundVtxBuffer = GResource::Find<VertexBuffer>(_name);

	if ( nullptr == foundVtxBuffer )
	{
		CRASH_PROG;
		return;
	}

	mVecOfVertexBuffer.push_back(&foundVtxBuffer->GetBuffer( ));
	mVecOfVertexBufferOffset.push_back(0);
	mVecOfVertexBufferSize.push_back(foundVtxBuffer->GetVertexSize( ));

	mVecOfVB.push_back(foundVtxBuffer);
}

void GMesh::AddInstanceBuffer(const std::wstring& _name)
{
	if ( nullptr != mInstanceBuffer )
	{
		CRASH_PROG;
		return;
	}

	SmartPtr<InstanceBuffer> foundInstBuffer = GResource::Find<InstanceBuffer>(_name);

	if ( nullptr == foundInstBuffer )
	{
		CRASH_PROG;
		return;
	}

	mInstanceBuffer = foundInstBuffer;

	mVecOfVertexBuffer.push_back(&foundInstBuffer->GetBuffer( ));
	mVecOfVertexBufferOffset.push_back(0);
	mVecOfVertexBufferSize.push_back(foundInstBuffer->GetVertexSize( ));

	mVecOfVB.push_back(foundInstBuffer.mPtr);
}

void GMesh::AddIndexBuffer(const std::wstring& _name)
{
	SmartPtr<IndexBuffer> foundIdxBuffer = GResource::Find<IndexBuffer>(_name);

	if ( nullptr == foundIdxBuffer )
	{
		CRASH_PROG;
		return;
	}

	mVecOfIndexBuffer.push_back(&foundIdxBuffer->GetBuffer( ));
	mVecOfIndexBufferOffset.push_back(0);

	mVecOfIB.push_back(foundIdxBuffer);
}
