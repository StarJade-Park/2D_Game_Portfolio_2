#pragma once
#include <vector>

#include <d3d11_4.h>
#pragma comment(lib, "d3d11")

#include <..\psklib\Debug.h>
#include <..\psklib\SmartPtr.h>

#include "ShaderType.h"

class VertexGShader;
class PixelGShader;
class GTexture;
class GSampler;
class GBlender;
class GRasterizer;
class GDepthStencil;

class RenderingPaths
{
public:
	struct SamplerData
	{
		shader_type::TYPE shaderType;
		UINT index;
		SmartPtr<GSampler> sampler;
	};

	struct TextureData
	{
		shader_type::TYPE shaderType;
		UINT index;
		SmartPtr<GTexture> texture;
	};

public:
	RenderingPaths( );
	~RenderingPaths( ) = default;

	RenderingPaths(const RenderingPaths& _other);
	RenderingPaths& operator=(const RenderingPaths& _other);

	void SetBlender(const std::wstring& _name);
	void SetDepthStencil(const std::wstring& _name);
	void SetRasterizer(const std::wstring& _name);
	void SetVtxShader(const std::wstring& _name);
	void SetPixShader(const std::wstring& _name);
	void SetSampler(const std::wstring& _name, const shader_type::TYPE _shaderType, const UINT _index);
	void SetTexture(const std::wstring& _name, const shader_type::TYPE _shaderType, const UINT _index);

	void SetDrawMode(const D3D11_PRIMITIVE_TOPOLOGY _drawMode)
	{
		mTopologyMode = _drawMode;
	}

	void SetTopologyMode(const D3D11_PRIMITIVE_TOPOLOGY _topologyMode)
	{
		mTopologyMode = _topologyMode;
	}

	void Update( );

private:
	SmartPtr<VertexGShader> mVtxShader;
	SmartPtr<PixelGShader> mPixShader;
	std::vector<TextureData> mVecOfTextureData;
	std::vector<SamplerData> mVecOfSamplerData;
	D3D11_PRIMITIVE_TOPOLOGY mTopologyMode;
	SmartPtr<GBlender> mBlender;
	SmartPtr<GRasterizer> mRasterizer;
	SmartPtr<GDepthStencil> mDepthStencil;
};