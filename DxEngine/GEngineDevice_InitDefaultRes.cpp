#include "GEngineDevice.h"
#include "GResource.h"
#include "GSampler.h"
#include "GMesh.h"
//#include "GTexture.h"
#include "GSprite.h"
#include "VertexGShader.h"
#include "PixelGShader.h"
#include "GConstBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "InstanceBuffer.h"
#include "GBlender.h"
#include "GRasterizer.h"
#include "GDepthStencil.h"
#include "DebugRenderer.h"
#include "FWFont.h"

namespace // 익명 namespace
{

void InitSampler( )
{
	GResource::Create<GSampler>(L"Sampler");

	// note create : linear sampler
	{
		D3D11_SAMPLER_DESC samplerDesc = D3D11_SAMPLER_DESC( );
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		// uvw mapping 방식 지정
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

		// note border(~= 마진) color 지정
		memcpy_s(samplerDesc.BorderColor, sizeof(GVector4D), &GVector4D::Red, sizeof(GVector4D));

		// Comparison(압축) 방법 지정
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;

		samplerDesc.MipLODBias = 1.0F;

		GResource::Create<GSampler>(L"Sampler_Linear", samplerDesc);
	}

	// note create : linear sampler
	{
		D3D11_SAMPLER_DESC samplerDesc = D3D11_SAMPLER_DESC( );
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;

		// uvw mapping 방식 지정
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

		// note border(~= 마진) color 지정
		memcpy_s(samplerDesc.BorderColor, sizeof(GVector4D), &GVector4D::Red, sizeof(GVector4D));

		// Comparison(압축) 방법 지정
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;

		samplerDesc.MipLODBias = 1.0F;

		GResource::Create<GSampler>(L"Sampler_Tile", samplerDesc);
	}
}

void InitBlender( )
{
	GResource::Create<GBlender>(L"Blender");

	{
		D3D11_BLEND_DESC mBlendDesc = D3D11_BLEND_DESC( );
		mBlendDesc.AlphaToCoverageEnable = false;
		mBlendDesc.IndependentBlendEnable = false;

		mBlendDesc.RenderTarget[0].BlendEnable = true;
		mBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		mBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		mBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_MIN;

		mBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		mBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		mBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		mBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		GResource::Create<GBlender>(L"Tile_Blender", mBlendDesc);
	}

	// note create : debug blender
	{
		D3D11_BLEND_DESC debugBlendDesc = D3D11_BLEND_DESC( );
		debugBlendDesc.AlphaToCoverageEnable = false;
		debugBlendDesc.IndependentBlendEnable = false;
		debugBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		debugBlendDesc.RenderTarget[0].BlendEnable = false;
		debugBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		debugBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		debugBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		debugBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		debugBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		debugBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		GResource::Create<GBlender>(L"DebugBlender", debugBlendDesc);
	}
}

void InitBuffer( )
{
	/*********** const buffer ***********/
	GResource::Create<GConstBuffer>(L"ConstBuffer", shader_type::TYPE::VS, sizeof(GMatrix), 0);
	GResource::Create<GConstBuffer>(L"ConstBufferUV", shader_type::TYPE::VS, sizeof(GVector4D), 1);
	GResource::Create<GConstBuffer>(L"ConstBufferColor", shader_type::TYPE::PS, sizeof(GVector4D), 2);

	GResource::Create<GConstBuffer>(L"ConstBufDebugColor", shader_type::TYPE::PS, sizeof(GVector4D), 1);

	GResource::Create<GConstBuffer>(L"ConstBufferCutData", shader_type::TYPE::PS, sizeof(GVector4D), 3);

	GResource::Create<GConstBuffer>(L"RECTCOLOR", shader_type::TYPE::PS, sizeof(GVector4D), 1);
}

void InitMesh( )
{
	// note create : texture mesh
	{
		std::vector<MeshData::Idx32> ArrIdx;
		ArrIdx.push_back({ 0, 3, 2 });
		ArrIdx.push_back({ 0, 1, 3 });

		GResource::Create<IndexBuffer>(L"TextureIndexBuffer", ArrIdx, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

		std::vector<MeshData::Vtx2D> ArrVtx;
		ArrVtx.resize(4);

		ArrVtx[0].mPos = GVector4D(-0.5f, 0.5f, 0.5f, 1.0f);
		ArrVtx[1].mPos = GVector4D(0.5f, 0.5f, 0.5f, 1.0f);
		ArrVtx[2].mPos = GVector4D(-0.5f, -0.5f, 0.5f, 1.0f);
		ArrVtx[3].mPos = GVector4D(0.5f, -0.5f, 0.5f, 1.0f);

		ArrVtx[0].mUv = GVector2D(0.0f, 0.0f);
		ArrVtx[1].mUv = GVector2D(1.0f, 0.0f);
		ArrVtx[2].mUv = GVector2D(0.0f, 1.0f);
		ArrVtx[3].mUv = GVector2D(1.0f, 1.0f);

		ArrVtx[0].mColor = GVector4D(1.0f, 0.0f, 0.0f, 1.0f);
		ArrVtx[1].mColor = GVector4D(0.0f, 1.0f, 0.0f, 1.0f);
		ArrVtx[2].mColor = GVector4D(0.0f, 0.0f, 1.0f, 1.0f);
		ArrVtx[3].mColor = GVector4D(1.0f, 1.0f, 0.0f, 1.0f);

		GResource::Create<VertexBuffer>(L"TextureVertexBuffer", ArrVtx, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

		auto texMesh = GResource::Create<GMesh>(L"TextureMesh");
		texMesh->AddIndexBuffer(L"TextureIndexBuffer");
		texMesh->AddVertexBuffer(L"TextureVertexBuffer");
	}

	// note create : debug mesh
	{
		std::vector<DWORD> ArrIdx;
		ArrIdx.push_back(0);
		ArrIdx.push_back(1);
		ArrIdx.push_back(2);
		ArrIdx.push_back(3);
		ArrIdx.push_back(0);

		GResource::Create<IndexBuffer>(L"DebugIndexBuffer", ArrIdx, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

		std::vector<MeshData::VtxDEBUG> ArrVtx;
		ArrVtx.resize(4);

		ArrVtx[0].mPos = GVector4D(-0.5f, 0.5f, 0.0f, 1.0f);
		ArrVtx[1].mPos = GVector4D(0.5f, 0.5f, 0.0f, 1.0f);
		ArrVtx[2].mPos = GVector4D(0.5f, -0.5f, 0.0f, 1.0f);
		ArrVtx[3].mPos = GVector4D(-0.5f, -0.5f, 0.0f, 1.0f);

		GResource::Create<VertexBuffer>(L"DebugVertexBuffer", ArrVtx, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

		auto debugMesh = GResource::Create<GMesh>(L"DebugMesh");
		debugMesh->AddIndexBuffer(L"DebugIndexBuffer");
		debugMesh->AddVertexBuffer(L"DebugVertexBuffer");
	}

	{
		std::vector<DWORD> debugCircleIdxArr;
		std::vector<MeshData::Vtx2D> debugCircleVtxArr;
		debugCircleVtxArr.resize(360);

		for ( int angle = 0; angle < 360; ++angle )
		{
			debugCircleIdxArr.push_back(angle);

			float x = (cosf((float)angle * gconst::DegToRad) * 0.5f);
			float y = (sinf((float)angle * gconst::DegToRad) * 0.5f);

			GVector4D calPos = { x,y, 0.0f };

			debugCircleVtxArr[angle].mPos = calPos;
		}
		debugCircleIdxArr.push_back(0);

		GResource::Create<IndexBuffer>(L"DebugCircleIdx", debugCircleIdxArr, D3D11_USAGE::D3D11_USAGE_DYNAMIC);
		GResource::Create<VertexBuffer>(L"DebugCircleVtx", debugCircleVtxArr, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

		auto debugSpherMesh = GResource::Create<GMesh>(L"DebugSphereMesh");
		debugSpherMesh->AddIndexBuffer(L"DebugCircleIdx");
		debugSpherMesh->AddVertexBuffer(L"DebugCircleVtx");
	}
}

void InitRasterizer( )
{
	GResource::Create<GRasterizer>(L"RS_State_Back");
	GResource::Create<GRasterizer>(L"RS_State_Front", true);

	{
		D3D11_RASTERIZER_DESC debugRsDesc = D3D11_RASTERIZER_DESC( );
		debugRsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		debugRsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		debugRsDesc.MultisampleEnable = FALSE;
		debugRsDesc.AntialiasedLineEnable = FALSE;
		GResource::Create<GRasterizer>(L"Debug_RS_State", debugRsDesc);
	}

	{
		D3D11_RASTERIZER_DESC rsDesc = D3D11_RASTERIZER_DESC( );
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rsDesc.MultisampleEnable = false;
		rsDesc.AntialiasedLineEnable = false;

		GResource::Create<GRasterizer>(L"RS_State_None", rsDesc);
	}
}

void InitSahder( )
{
	// note color rect shader
	{
		SmartPtr<VertexGShader> vtx =
			GResource::Load<VertexGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"ColorRect.hlsl"),
			5,
			0,
			"VS_ColorRect"
			);

		vtx->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->EndLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		GResource::Load<PixelGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"ColorRect.hlsl"),
			5,
			0,
			"PS_ColorRect"
			);
	}

	// note texture shader
	{
		SmartPtr<VertexGShader> vtx = GResource::Load<VertexGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"TextureShader.hlsl"),
			5,
			0,
			"VS_TEX"
			);

		vtx->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->EndLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		GResource::Load<PixelGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"TextureShader.hlsl"),
			5,
			0,
			"PS_TEX"
			);
	}

	// note afterimage shader
	{
		SmartPtr<VertexGShader> vtx =
			GResource::Load<VertexGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"Afterimage.hlsl"),
			5,
			0,
			"VS_AFTERIMAGE"
			);

		vtx->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->EndLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		GResource::Load<PixelGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"Afterimage.hlsl"),
			5,
			0,
			"PS_AFTERIMAGE"
			);
	}

	// note loading bar shader
	{
		SmartPtr<VertexGShader> vtx =
			GResource::Load<VertexGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"LoadingBar.hlsl"),
			5,
			0,
			"VS_LOADBAR"
			);

		vtx->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->EndLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		GResource::Load<PixelGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"LoadingBar.hlsl"),
			5,
			0,
			"PS_LOADBAR"
			);
	}

	// note DebugMesh shader 
	{
		SmartPtr<VertexGShader> vtx =
			GResource::Load<VertexGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"DebugMesh.hlsl"),
			5,
			0,
			"VS_DEBUGMESH"
			);

		vtx->EndLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		GResource::Load<PixelGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"DebugMesh.hlsl"),
			5,
			0,
			"PS_DEBUGMESH"
			);
	}
}

} // END : 익명 namespace

void GEngineDevice::InitDefaultResource( )
{
	InitSampler( );
	InitBlender( );
	InitBuffer( );
	InitMesh( );
	InitRasterizer( );
	InitSahder( );

	{
		D3D11_DEPTH_STENCIL_DESC debugDsDesc = D3D11_DEPTH_STENCIL_DESC( );
		debugDsDesc.DepthEnable = FALSE;
		debugDsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		debugDsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		debugDsDesc.StencilEnable = FALSE;

		GResource::Create<GDepthStencil>(L"Debug_DS_State", debugDsDesc);
	}

	GResource::Create<GDepthStencil>(L"DS_State");

	GResource::Create<FWFont>(L"D2Coding", L"D2Coding");

	DebugRenderer::Init( );
	DebugRenderer::OffDebugMode( );
#ifdef _DEBUG
	//DebugRenderer::OnDebugMode( );
#endif // _DEBUG

	mbDefualtInitFlag = true;
}