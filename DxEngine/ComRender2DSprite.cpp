#include "ComRender2DSprite.h"
#include "GActor.h"
#include "ComTransform.h"
#include "GMesh.h"
#include "GConstBuffer.h"
#include "VertexGShader.h"
#include "PixelGShader.h"
#include "ComCamera.h"
#include "GTexture.h"
#include "GSampler.h"
#include "GSprite.h"
#include "GScene.h"
#include "GBlender.h"
#include "GRasterizer.h"
#include "GDepthStencil.h"
#include "DebugRenderer.h"

ComRender2DSprite::ComRender2DSprite( )
	: mTextureUvw(GVector4D( )), mColor(1.0f), mInitColor(GVector4D::One), mSpriteIdx(gconst::inf<UINT>)
	, mFillData(1.0f)
	, mMesh(nullptr), mConstBuffer(nullptr), mVtxShader(nullptr), mPixShader(nullptr)
	, mSampler(nullptr), mSprite(nullptr), mBlender(nullptr)
	, mRasterizerBack(nullptr), mRasterizerFront(nullptr), mConstBufferColor(nullptr)
	, mConstBufferFillData(nullptr)
	, mbCullBack(false)
{
	EMPTY_STATEMENT;
}

ComRender2DSprite::~ComRender2DSprite( )
{
	EMPTY_STATEMENT;
}

void ComRender2DSprite::SetSprite(const std::wstring& _spriteName, UINT _spriteIndex)
{
	mSpriteIdx = _spriteIndex;
	mSprite = GResource::Find<GSprite>(_spriteName);
}

void ComRender2DSprite::Init( )
{
	ComRender::Init( );

	mMesh = GResource::Find<GMesh>(L"TextureMesh");
	
	mConstBuffer = GResource::Find<GConstBuffer>(L"ConstBuffer");
	mConstBufferUV = GResource::Find<GConstBuffer>(L"ConstBufferUV");
	mConstBufferColor = GResource::Find<GConstBuffer>(L"ConstBufferColor");
	mConstBufferFillData = GResource::Find<GConstBuffer>(L"ConstBufferCutData");

	mVtxShader = GResource::Find<VertexGShader>(L"TextureShader.hlsl");
	mPixShader = GResource::Find<PixelGShader>(L"TextureShader.hlsl");
	mSampler = GResource::Find<GSampler>(L"Sampler");
	mBlender = GResource::Find<GBlender>(L"Blender");
	mRasterizerBack = GResource::Find<GRasterizer>(L"RS_State_Back");
	mRasterizerFront = GResource::Find<GRasterizer>(L"RS_State_Front");
	mDepthStencil = GResource::Find<GDepthStencil>(L"DS_State");
}

void ComRender2DSprite::Render(const SmartPtr<ComCamera> _cam)
{
	UpdateTransform( );
	//GMatrix worldMatrix = GetWorldMatrix( );
	GMatrix wvp = GetWorldMatrix( ) * (_cam->GetVP( ));
	
	if ( true == DebugRenderer::IsOnDebugMode( ) )
	{
		DebugRenderer::DrawPoint(ComCamera::mMainCam, GetWorldMatrix( ));
	}

	if ( false == mbCullBack )
	{
		mRasterizerBack->Update( );
	}
	else
	{
		wvp.fArr[0][0] = -wvp.fArr[0][0];
		mRasterizerFront->Update( );
	}

	wvp.Transpose( );

	//mTextureUvw = GVector4D(0.0f, 0.0f, 1.0f, 1.0f);
	GEngineDevice::MainDevice( ).GetIContext( ).IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mTextureUvw = mSprite->GetCuttedData(mSpriteIdx);
	mConstBufferUV->SetData(&mTextureUvw, sizeof(GVector4D));
	mConstBufferUV->Update( );

	mConstBuffer->SetData(&wvp, sizeof(GMatrix));
	mConstBuffer->Update( );

	mConstBufferColor->SetData(&mColor, sizeof(GVector4D));
	mConstBufferColor->Update( );

	mConstBufferFillData->SetData(&mFillData, sizeof(GVector4D));
	mConstBufferFillData->Update( );

	mVtxShader->Update( );
	mPixShader->Update( );

	mSampler->Update(shader_type::TYPE::PS, 0);
	mSprite->Update(shader_type::TYPE::PS, 0);

	mBlender->Update( );

	mDepthStencil->Update( );

	mMesh->Update(0, 0);
	mMesh->Render(0);

	mConstBufferColor->SetData(&mInitColor, sizeof(GVector4D));
	mConstBufferColor->Update( );
}
