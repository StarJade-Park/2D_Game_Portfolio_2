#pragma once
#include <d3d11_4.h>
#include <..\psklib\MyMath.h>

namespace MeshData
{
	struct Vtx2D
	{
	public:
		static UINT GetMemSize( )
		{
			return (UINT)sizeof(Vtx2D);
		}

	public:
		GVector4D mPos;
		GVector2D mUv;
		GVector4D mColor;
	};

	struct VtxDEBUG
	{
	public:
		static UINT GetMemSize( )
		{
			return (UINT)sizeof(VtxDEBUG);
		}

	public:
		GVector4D mPos;
	};


	struct Idx32
	{
	public:
		Idx32(UINT _x, UINT _y, UINT _z)
			: _01(_x), _02(_y), _03(_z)
		{
			EMPTY_STATEMENT;
		}

		~Idx32( ) = default;

		static UINT GetMemSize( )
		{
			return (UINT)(sizeof(UINT));
		}

		static DXGI_FORMAT GetFormat( )
		{
			return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		}

	public:
		UINT _01;
		UINT _02;
		UINT _03;
	};
};