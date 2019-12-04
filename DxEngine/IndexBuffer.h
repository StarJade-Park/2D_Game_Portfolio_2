#pragma once
#include "BaseBuffer.h"

class IndexBuffer
	: public BaseBuffer
{
public:
	template<typename T>
	bool Create(std::vector<T>& _data, const D3D11_USAGE& _vtxUsage)
	{
		if ( 0 >= _data.size( ) )
		{
			CRASH_PROG;
			return false;
		}

		return Create(T::GetFormat( ), (UINT)_data.size( ) * 3, T::GetMemSize( ), _vtxUsage, &_data[0]);
	}

	bool Create(std::vector<DWORD>& _data, const D3D11_USAGE& _vtxUsage)
	{
		if ( 0 >= _data.size( ) )
		{
			CRASH_PROG;
			return false;
		}

		return Create(DXGI_FORMAT::DXGI_FORMAT_R32_UINT, (UINT)_data.size( ), sizeof(DWORD), _vtxUsage, &_data[0]);
	}

	bool Create(const DXGI_FORMAT& _idxFormat, UINT _iIdxCount, UINT _iIdxSize, const D3D11_USAGE& _idxUsage, void* _Data);

	DXGI_FORMAT& GetFormat( )
	{
		return mFormat;
	}

	DXGI_FORMAT& GetType( )
	{
		return GetFormat( );
	}

	UINT GetIndexSize( ) const
	{
		return mIndexSize;
	}

	UINT GetIndexCount( ) const
	{
		return mIndexCount;
	}

private:
	DXGI_FORMAT mFormat;
	UINT mIndexSize;
	UINT mIndexCount;
};