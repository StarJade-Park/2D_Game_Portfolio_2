#pragma once
#include <vector>

#include "MeshData.h"
#include "GResource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InstanceBuffer.h"

class GMesh
	: public GResource
{
public:
	GMesh( );
	~GMesh( );

	bool Create( )
	{
		return true;
	}

	void UpdateAllVertexBuffer( );
	void UpdateVertexBuffer(const UINT _index);
	void UpdateIndexBuffer(const UINT _index);
	void Update(UINT _vtxBufferIndex, UINT _IdxBufferIndex);

	void RenderInstanced(const UINT _index);

	void Render(UINT _vtxBufferIndex);

	void RenderAutomation( );

	SmartPtr<InstanceBuffer> GetInstanceBuffer( )
	{
		return mInstanceBuffer;
	}

	void AddVertexBuffer(const std::wstring& _name);
	void AddInstanceBuffer(const std::wstring& _name);
	void AddIndexBuffer(const std::wstring& _name);

private:
	SmartPtr<InstanceBuffer> mInstanceBuffer;

	std::vector<SmartPtr<VertexBuffer>> mVecOfVB;
	std::vector<SmartPtr<IndexBuffer>> mVecOfIB;

	std::vector<ID3D11Buffer*> mVecOfVertexBuffer;
	std::vector<UINT> mVecOfVertexBufferSize;
	std::vector<UINT> mVecOfVertexBufferOffset;

	std::vector<ID3D11Buffer*> mVecOfIndexBuffer;
	std::vector<UINT> mVecOfIndexBufferOffset;
};

