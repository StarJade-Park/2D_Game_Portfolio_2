#include "ComRenderColorRect.h"
#include "GMesh.h"
#include "GConstBuffer.h"
#include "GConstBuffer.h"
#include "VertexGShader.h"
#include "PixelGShader.h"
#include "GSampler.h"
#include "GBlender.h"
#include "GActor.h"
#include "ComTransform.h"
#include "ComCamera.h"
#include "GRasterizer.h"
#include "GDepthStencil.h"

ComRenderColorRect::ComRenderColorRect( )
	: mMesh(nullptr)
	, mConstBuffer(nullptr), mConstBufferColor(nullptr)
	, mVtxShader(nullptr), mPixShader(nullptr)
	, mSampler(nullptr), mBlender(nullptr)
	, mColor(GVector4D( ))
{
	EMPTY_STATEMENT;
}

ComRenderColorRect::~ComRenderColorRect( )
{
	EMPTY_STATEMENT;
}

void ComRenderColorRect::Init( )
{
	ComRender::Init( );

	mColor = 1.0f;

	mMesh = GResource::Find<GMesh>(L"TextureMesh");
	mConstBuffer = GResource::Find<GConstBuffer>(L"ConstBuffer");
	mConstBufferColor = GResource::Find<GConstBuffer>(L"RECTCOLOR");
	mVtxShader = GResource::Find<VertexGShader>(L"ColorRect.hlsl");
	mPixShader = GResource::Find<PixelGShader>(L"ColorRect.hlsl");
	mSampler = GResource::Find<GSampler>(L"Sampler");
	mBlender = GResource::Find<GBlender>(L"Blender");
	mRasterizerBack = GResource::Find<GRasterizer>(L"RS_State_Back");
	mDepthStencil = GResource::Find<GDepthStencil>(L"DS_State");
}

void ComRenderColorRect::Render(const SmartPtr<ComCamera> _cam)
{
	UpdateTransform( );

	//GMatrix worldMatrix = GetWorldMatrix( );
	GMatrix wvp = GetWorldMatrix( ) * (_cam->GetVP( ));
	wvp.Transpose( );

	GEngineDevice::MainDevice( ).GetIContext( ).IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mConstBufferColor->SetData(&mColor, sizeof(GVector4D));
	mConstBufferColor->Update( );

	mConstBuffer->SetData(&wvp, sizeof(GMatrix));
	mConstBuffer->Update( );

	mVtxShader->Update( );
	mPixShader->Update( );

	mSampler->Update(shader_type::TYPE::PS, 0);
	mBlender->Update( );

	mRasterizerBack->Update( );
	mDepthStencil->Update( );

	mMesh->Update(0, 0);
	mMesh->Render(0);
}
