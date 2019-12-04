#include "RenderingPaths.h"

#include "VertexGShader.h"
#include "PixelGShader.h"
#include "GTexture.h"
#include "GSampler.h"
#include "GBlender.h"
#include "GRasterizer.h"
#include "GDepthStencil.h"

RenderingPaths::RenderingPaths( )
	: mVtxShader(nullptr), mPixShader(nullptr)
	, mVecOfSamplerData(std::vector<SamplerData>( )), mVecOfTextureData(std::vector<TextureData>( ))
	, mBlender(nullptr), mRasterizer(nullptr), mDepthStencil(nullptr)
	, mTopologyMode(D3D11_PRIMITIVE_TOPOLOGY( ))
{
	EMPTY_STATEMENT;
}

RenderingPaths::RenderingPaths(const RenderingPaths& _other)
{
	memcpy_s(this, sizeof(RenderingPaths), &_other, sizeof(RenderingPaths));
}

RenderingPaths& RenderingPaths::operator=(const RenderingPaths& _other)
{
	memcpy_s(this, sizeof(RenderingPaths), &_other, sizeof(RenderingPaths));
	return *this;
}

void RenderingPaths::SetBlender(const std::wstring& _name)
{
	mBlender = GResource::Find<GBlender>(_name);

	if ( nullptr == mBlender )
	{
		CRASH_PROG;
		return;
	}
}

void RenderingPaths::SetDepthStencil(const std::wstring& _name)
{
	mDepthStencil = GResource::Find<GDepthStencil>(_name);

	if ( nullptr == mDepthStencil )
	{
		CRASH_PROG;
		return;
	}
}

void RenderingPaths::SetRasterizer(const std::wstring& _name)
{
	mRasterizer = GResource::Find<GRasterizer>(_name);

	if ( nullptr == mRasterizer )
	{
		CRASH_PROG;
		return;
	}
}

void RenderingPaths::SetVtxShader(const std::wstring& _name)
{
	mVtxShader = GResource::Find<VertexGShader>(_name);

	if ( nullptr == mVtxShader )
	{
		CRASH_PROG;
		return;
	}
}

void RenderingPaths::SetPixShader(const std::wstring& _name)
{
	mPixShader = GResource::Find<PixelGShader>(_name);

	if ( nullptr == mPixShader )
	{
		CRASH_PROG;
		return;
	}
}

void RenderingPaths::SetSampler(const std::wstring& _name, const shader_type::TYPE _shaderType, const UINT _index)
{
	SmartPtr<GSampler> foundSampler = GResource::Find<GSampler>(_name);

	if ( nullptr == foundSampler )
	{
		CRASH_PROG;
		return;
	}

	SamplerData data;
	data.sampler = foundSampler;
	data.index = _index;
	data.shaderType = _shaderType;

	mVecOfSamplerData.push_back(data);
}

void RenderingPaths::SetTexture(const std::wstring& _name, const shader_type::TYPE _shaderType, const  UINT _index)
{
	SmartPtr<GTexture> foundTex = GResource::Find<GTexture>(_name);

	if ( nullptr == foundTex )
	{
		CRASH_PROG;
		return;
	}

	TextureData data = TextureData( );
	data.texture = foundTex;
	data.index = _index;
	data.shaderType = _shaderType;

	mVecOfTextureData.push_back(data);
}

void RenderingPaths::Update( )
{
	if ( nullptr != mBlender )
	{
		mBlender->Update( );
	}

	if ( nullptr != mDepthStencil )
	{
		mDepthStencil->Update( );
	}

	if ( nullptr != mRasterizer )
	{
		mRasterizer->Update( );
	}

	if ( nullptr != mVtxShader )
	{
		mVtxShader->Update( );
	}

	if ( nullptr != mPixShader )
	{
		mPixShader->Update( );
	}

	if ( 0 != mVecOfSamplerData.size( ) )
	{
		for ( auto& data : mVecOfSamplerData )
		{
			data.sampler->Update(data.shaderType, data.index);
		}
	}

	if ( 0 != mVecOfTextureData.size( ) )
	{
		for ( auto& data : mVecOfTextureData )
		{
			data.texture->Update(data.shaderType, data.index);
		}
	}

	GEngineDevice::MainDevice( ).GetIContext( ).IASetPrimitiveTopology(mTopologyMode);
}
