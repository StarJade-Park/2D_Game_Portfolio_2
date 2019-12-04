#pragma once
#include <..\ThirdParty\DirectXTex\DirectXTex.h>

#include "DxTexDef.h"
#include "GResource.h"
#include "ShaderType.h"

class GTexture
	: public GResource
{
public:
	GTexture( );
	~GTexture( );

#pragma region GetSet
	ID3D11RenderTargetView& GetRenderTargetView( )
	{
		return *miRenderTargetView;
	}

	ID3D11DepthStencilView& GetDepthStencilView( )
	{
		return *miDepthStencilView;
	}

	ID3D11ShaderResourceView& GetShaderResourceView( )
	{
		return *miShaderResourceView;
	}

	size_t GetWitdh( )
	{
		return mImage.GetMetadata( ).width;
	}

	size_t GetHeight( )
	{
		return mImage.GetMetadata( ).height;
	}

	float GetRatioW( )
	{
		return (float)GetWitdh( ) / (float)GetHeight( );
	}

	float GetRatioH( )
	{
		return (float)GetHeight( ) / (float)GetWitdh( );
	}
#pragma endregion
	bool Load( );
	void Update(shader_type::TYPE _shaderType, UINT _setIndex);

private:
	DirectX::ScratchImage mImage;
	ID3D11Resource* miTexture;

	ID3D11RenderTargetView* miRenderTargetView; // 우리의 도화지
	ID3D11DepthStencilView* miDepthStencilView; // depth buffer를 만들 수도 있음
	ID3D11ShaderResourceView* miShaderResourceView; // shader를 세팅하기 위한 인터페이스
};

