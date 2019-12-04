#pragma once
#include "ComRender.h"
class GMesh;
class GConstBuffer;
class VertexGShader;
class PixelGShader;
class GSampler;
class GSprite;
class GBlender;
class GRasterizer;
class GDepthStencil;

class ComRenderColorRect
	: public ComRender
{
public:
	ComRenderColorRect( );
	~ComRenderColorRect( );

	void Init( ) override;
	void Render(const SmartPtr<ComCamera> _cam) override;

#pragma region Get Set
	GVector4D& GetColor( )
	{
		return mColor;
	}

	void SetColor(const GVector4D _texUvw)
	{
		mColor = _texUvw;
	}

	GMesh& GetMesh( )
	{
		return *mMesh;
	}

	void SetMesh(const SmartPtr<GMesh> _mesh)
	{
		mMesh = _mesh;
	}

	GConstBuffer& GetConstBuffer( )
	{
		return *mConstBuffer;
	}

	void SetConstBuffer(const SmartPtr<GConstBuffer> _constBuff)
	{
		mConstBuffer = _constBuff;
	}

	GConstBuffer& GetConstBufferColor( )
	{
		return *mConstBufferColor;
	}

	void SetConstBufferColor(const SmartPtr<GConstBuffer> _cBuffUvw)
	{
		mConstBufferColor = _cBuffUvw;
	}

	VertexGShader& GetVtxShader( )
	{
		return *mVtxShader;
	}

	void SetVtxShader(const SmartPtr<VertexGShader> _vtxS)
	{
		mVtxShader = _vtxS;
	}

	PixelGShader& GetPixShader( )
	{
		return *mPixShader;
	}

	void SetPixShader(const SmartPtr<PixelGShader> _pixS)
	{
		mPixShader = _pixS;
	}

	GSampler& GetSampler( )
	{
		return *mSampler;
	}

	void SetSampler(const SmartPtr<GSampler> _smp)
	{
		mSampler = _smp;
	}
#pragma endregion

private:
	SmartPtr<GMesh> mMesh;
	SmartPtr<GConstBuffer> mConstBuffer;
	SmartPtr<GConstBuffer> mConstBufferColor;
	SmartPtr<VertexGShader> mVtxShader;
	SmartPtr<PixelGShader> mPixShader;
	SmartPtr<GSampler> mSampler;
	SmartPtr<GBlender> mBlender;
	SmartPtr<GRasterizer> mRasterizerBack;
	SmartPtr<GDepthStencil> mDepthStencil;

	GVector4D mColor;
};

