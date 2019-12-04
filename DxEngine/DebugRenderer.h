#pragma once
#include "GEngineDevice.h"
#include "FWFont.h"
#include "ComCamera.h"

class GMesh;
class GConstBuffer;
class VertexGShader;
class PixelGShader;
class GSampler;
class GSprite;
class GBlender;
class GRasterizer;
class GDepthStencil;

class DebugRenderer
{
private:
	enum DEBUG_DRAW_TYPE : int
	{
		RECT, CIRCLE, MAX,
	};

	struct DebugDrawInfo
	{
		DEBUG_DRAW_TYPE type;
		GMatrix w;
		GMatrix v;
		GMatrix p;
		GVector4D color;
	};

	struct DebugTextInfo
	{
		std::wstring fontName;
		std::wstring text;
		GVector4D color;
	};

public:
	DebugRenderer( ) = default;
	~DebugRenderer( ) = default;

	static void Init( );

	static bool IsOnDebugMode( )
	{
		return mbDebugFlag;
	}

	static void SwitchDebugMode( )
	{
		mbDebugFlag = !mbDebugFlag;
	}

	static void OnDebugMode( )
	{
		mbDebugFlag = true;
	}

	static void OffDebugMode( )
	{
		mbDebugFlag = false;
	}

	static void RenderDebugInfo( );

	static void DrawRect2D(SmartPtr<ComCamera> _cam, const GMatrix& _w, const GVector4D& _color = GVector4D::Green);
	//static void DrawRect2D(SmartPtr<ComCamera> _cam, GVector4D _pos, GVector4D _rot, GVector4D _scale, GVector4D _color);
	//static void DrawRect2D(GVector4D _Pos, GVector4D _Rot, GVector4D _Scale, GVector4D _Color);

	static void DrawCirCle2D(SmartPtr<ComCamera> _cam, const GMatrix& _worldMatrix, const GVector4D& _color = GVector4D::Green);
	//static void DrawCirCle2D(SmartPtr<ComCamera> _cam, GVector4D _pos, GVector4D _Rot, GVector4D _scale, GVector4D _color);
	//static void DrawCirCle2D(GVector4D _pos, GVector4D _rot, GVector4D _scale, GVector4D _color);

	//static void DrawFont(SmartPtr<ComCamera> _cam, GVector4D _pos, GVector4D _rot, GVector4D _scale);
	static void DrawVectorUI(const GVector4D& _vec, const GVector4D& _color = GVector4D::Red);
	static void DrawFontUI(const std::wstring& _text, const GVector4D& _color = GVector4D::Green);

	static void DrawPoint(SmartPtr<ComCamera> _cam, const GMatrix& _w, const GVector4D& _color = GVector4D::White);

private:
	static int mTextCount;
	static GVector4D mStartTextPos;
	static std::vector<DebugTextInfo> mVecOfDebugTextInfo;

	static int mDrawCount;
	static std::vector<DebugDrawInfo> mVecOfDebugDrawInfo;

	static bool mbInitFalg;
	static bool mbDebugFlag;

	static SmartPtr<GMesh> mMesh[DEBUG_DRAW_TYPE::MAX];
	static SmartPtr<GConstBuffer> mConstBuffer;
	static SmartPtr<GConstBuffer> mConstBufferColor;
	static SmartPtr<VertexGShader> mVtxShader;
	static SmartPtr<PixelGShader> mPixShader;
	static SmartPtr<GBlender> mBlender;
	static SmartPtr<GRasterizer> mRasterizer;
	static SmartPtr<GDepthStencil> mDepthStencil;
	static SmartPtr<FWFont> mFont;
};

