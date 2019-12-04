#pragma once
#include "BaseBuffer.h"

class VertexBuffer
	: public BaseBuffer
{
public:
	VertexBuffer( )
		: mVertexCount(gconst::inf<UINT>), mVertexSize(gconst::inf<UINT>)
	{
		EMPTY_STATEMENT;
	}

	~VertexBuffer( )
	{
		EMPTY_STATEMENT;
	}

	void SetVertexCount(const UINT _vtxCount)
	{
		mVertexCount = _vtxCount;
	}

	UINT GetVertexCount( ) const
	{
		return mVertexCount;
	}

	void SetVertexSize(const UINT _vtxSize)
	{
		mVertexSize = _vtxSize;
	}

	UINT GetVertexSize( ) const
	{
		return mVertexSize;
	}

	template<typename VBTYPE>
	bool Create(std::vector<VBTYPE>& _vtxData, const D3D11_USAGE& _vtxUsage)
	{
		bool result = Create((UINT)_vtxData.size( ), VBTYPE::GetMemSize( ), _vtxUsage, &_vtxData[0]);

		if ( false == result )
		{
			CRASH_PROG;
			return false;
		}

		return true;
	}

	bool Create(UINT _iVtxCount, UINT _iVtxSize, const D3D11_USAGE& _vtxUsage, void* _data);

protected:
	UINT mVertexSize; // Vertex(Á¡)ÀÇ size
	UINT mVertexCount; // VertexÀÇ °¹¼ö
};