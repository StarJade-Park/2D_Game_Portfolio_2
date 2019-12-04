#include "GMesh.h"
#include "GConstBuffer.h"
#include "VertexGShader.h"
#include "PixelGShader.h"
#include "GSampler.h"
#include "GSprite.h"
#include "GBlender.h"
#include "GRasterizer.h"
#include "GDepthStencil.h"

#include "DebugRenderer.h"

constexpr size_t debug_draw_info_size = 1000;
constexpr size_t debug_text_info_size = 100;

int DebugRenderer::mTextCount = 0;
GVector4D DebugRenderer::mStartTextPos = GVector4D::Zero;
std::vector<DebugRenderer::DebugTextInfo> DebugRenderer::mVecOfDebugTextInfo = std::vector<DebugRenderer::DebugTextInfo>( );

int DebugRenderer::mDrawCount = 0;
std::vector<DebugRenderer::DebugDrawInfo> DebugRenderer::mVecOfDebugDrawInfo = std::vector<DebugRenderer::DebugDrawInfo>( );

bool DebugRenderer::mbInitFalg = false;
bool DebugRenderer::mbDebugFlag = false;

SmartPtr<GMesh> DebugRenderer::mMesh[DebugRenderer::DEBUG_DRAW_TYPE::MAX] = { nullptr, nullptr };
SmartPtr<GConstBuffer> DebugRenderer::mConstBuffer = nullptr;
SmartPtr<GConstBuffer> DebugRenderer::mConstBufferColor = nullptr;
SmartPtr<VertexGShader> DebugRenderer::mVtxShader = nullptr;
SmartPtr<PixelGShader> DebugRenderer::mPixShader = nullptr;
SmartPtr<GBlender> DebugRenderer::mBlender = nullptr;
SmartPtr<GRasterizer> DebugRenderer::mRasterizer = nullptr;
SmartPtr<GDepthStencil> DebugRenderer::mDepthStencil = nullptr;
SmartPtr<FWFont> DebugRenderer::mFont = nullptr;

void DebugRenderer::Init( )
{
	if ( true == mbInitFalg )
	{
		return;
	}

	mMesh[DEBUG_DRAW_TYPE::RECT] = GResource::Find<GMesh>(L"DebugMesh");
	mMesh[DEBUG_DRAW_TYPE::CIRCLE] = GResource::Find<GMesh>(L"DebugSphereMesh");

	mConstBuffer = GResource::Find<GConstBuffer>(L"ConstBuffer");
	mConstBufferColor = GResource::Find<GConstBuffer>(L"ConstBufDebugColor");

	mVtxShader = GResource::Find<VertexGShader>(L"DebugMesh.hlsl");
	mPixShader = GResource::Find<PixelGShader>(L"DebugMesh.hlsl");

	mBlender = GResource::Find<GBlender>(L"DebugBlender");

	mRasterizer = GResource::Find<GRasterizer>(L"Debug_RS_State");
	mDepthStencil = GResource::Find<GDepthStencil>(L"Debug_DS_State");

	mVecOfDebugDrawInfo.resize(debug_draw_info_size);
	mVecOfDebugTextInfo.resize(debug_text_info_size);

	mbInitFalg = true;
}

void DebugRenderer::RenderDebugInfo( )
{
	for ( int index = 0; index < mDrawCount; ++index )
	{
		GMatrix WVP = mVecOfDebugDrawInfo[index].w * mVecOfDebugDrawInfo[index].v * mVecOfDebugDrawInfo[index].p;
		WVP.Transpose( );

		// 최대한 덜 호출하는 것이 빠를것이다.
		mConstBufferColor->SetData(&mVecOfDebugDrawInfo[index].color, sizeof(GVector4D));
		mConstBufferColor->Update( );

		mConstBuffer->SetData(&WVP, sizeof(GMatrix));
		mConstBuffer->Update( );

		mBlender->Update( );
		
		GEngineDevice::MainDevice( ).GetIContext( ).IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		
		mDepthStencil->Update( );
		mRasterizer->Update( );

		mVtxShader->Update( );
		mPixShader->Update( );

		mMesh[mVecOfDebugDrawInfo[index].type]->Update(0, 0);
		mMesh[mVecOfDebugDrawInfo[index].type]->Render(0);
	}

	mDrawCount = 0;

	for ( int index = 0; index < mTextCount; ++index )
	{
		FWFont* font = GResource::Find<FWFont>(mVecOfDebugTextInfo[index].fontName);

		if ( nullptr == font )
		{
			CRASH_PROG;
			continue;
		}

		font->DrawToUI(
			mVecOfDebugTextInfo[index].text.c_str( ),
			default_font_size,
			mStartTextPos + (GVector4D::vDown * default_font_size * (float)index),
			mVecOfDebugTextInfo[index].color
		);
	}

	mTextCount = 0;
}

void DebugRenderer::DrawRect2D(SmartPtr<ComCamera> _cam, const GMatrix& _w, const GVector4D& _color/*= GVector4D::Green*/)
{
	mVecOfDebugDrawInfo[mDrawCount].type = DEBUG_DRAW_TYPE::RECT;
	mVecOfDebugDrawInfo[mDrawCount].w = _w;
	mVecOfDebugDrawInfo[mDrawCount].v = _cam->GetViewport( );
	mVecOfDebugDrawInfo[mDrawCount].p = _cam->GetProjection( );
	mVecOfDebugDrawInfo[mDrawCount].color = _color;
	++mDrawCount;
}

//void DebugRenderer::DrawRect2D(SmartPtr<ComCamera> _cam, GVector4D _pos, GVector4D _rot, GVector4D _scale, GVector4D _color)
//{
//	CRASH_PROG;
//	++mDrawCount;
//}
//
//void DebugRenderer::DrawRect2D(GVector4D _Pos, GVector4D _Rot, GVector4D _Scale, GVector4D _Color)
//{
//	CRASH_PROG;
//	++mDrawCount;
//}

void DebugRenderer::DrawCirCle2D(SmartPtr<ComCamera> _cam, const GMatrix& _worldMatrix, const GVector4D& _color)
{
	mVecOfDebugDrawInfo[mDrawCount].type = DEBUG_DRAW_TYPE::CIRCLE;
	mVecOfDebugDrawInfo[mDrawCount].w = _worldMatrix;
	mVecOfDebugDrawInfo[mDrawCount].v = _cam->GetViewport( );
	mVecOfDebugDrawInfo[mDrawCount].p = _cam->GetProjection( );
	mVecOfDebugDrawInfo[mDrawCount].color = _color;
	++mDrawCount;
}

//
//void DebugRenderer::DrawCirCle2D(SmartPtr<ComCamera> _cam, GVector4D _pos, GVector4D _Rot, GVector4D _scale, GVector4D _color)
//{
//	CRASH_PROG;
//	++mDrawCount;
//}

//void DebugRenderer::DrawCirCle2D(GVector4D _pos, GVector4D _rot, GVector4D _scale, GVector4D _color)
//{
//	CRASH_PROG;
//	++mDrawCount;
//}

void DebugRenderer::DrawVectorUI(const GVector4D& _vec, const GVector4D & _color)
{
	mVecOfDebugTextInfo[mTextCount].fontName = L"D2Coding";
	mVecOfDebugTextInfo[mTextCount].text =
		std::wstring(L"x : ").append(std::to_wstring(_vec.m.x)).append(L", ").
		append(L"y : ").append(std::to_wstring(_vec.m.y)).append(L", ").
		append(L"z : ").append(std::to_wstring(_vec.m.z)).append(L", ").
		append(L"w : ").append(std::to_wstring(_vec.m.w));
	mVecOfDebugTextInfo[mTextCount].color = _color;
	++mTextCount;
}

void DebugRenderer::DrawFontUI(const std::wstring& _text, const GVector4D& _color/*= GVector4D::Green*/)
{
	mVecOfDebugTextInfo[mTextCount].fontName = L"D2Coding";
	mVecOfDebugTextInfo[mTextCount].text = _text;
	mVecOfDebugTextInfo[mTextCount].color = _color;
	++mTextCount;
}

void DebugRenderer::DrawPoint(SmartPtr<ComCamera> _cam, const GMatrix& _w, const GVector4D& _color/*= GVector4D::White*/)
{
	mVecOfDebugDrawInfo[mDrawCount].type = DEBUG_DRAW_TYPE::RECT;
	mVecOfDebugDrawInfo[mDrawCount].w = _w;
	
	mVecOfDebugDrawInfo[mDrawCount].w.fArr[0][0] = 2.5f;
	mVecOfDebugDrawInfo[mDrawCount].w.fArr[1][1] = 2.5f;
	mVecOfDebugDrawInfo[mDrawCount].w.fArr[2][2] = 2.5f;

	mVecOfDebugDrawInfo[mDrawCount].v = _cam->GetViewport( );
	mVecOfDebugDrawInfo[mDrawCount].p = _cam->GetProjection( );
	
	mVecOfDebugDrawInfo[mDrawCount].color = _color;
	++mDrawCount;
}
