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

class ComRender2DSprite
	: public ComRender
{
public:
	ComRender2DSprite( );
	~ComRender2DSprite( );

#pragma region Get Set
	void SetColor(const GVector4D& _color)
	{
		mColor = _color;
	}

	GVector4D GetColor( ) const
	{
		return mColor;
	}

	void SetSprite(const std::wstring& _spriteName, UINT _spriteIndex = 0);

	void SetSpriteIndex(UINT _spriteIndex)
	{
		mSpriteIdx = _spriteIndex;
	}

	GVector4D& GetTextureUvw( )
	{
		return mTextureUvw;
	}

	void SetTextureUvw(const GVector4D& _texUvw)
	{
		mTextureUvw = _texUvw;
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

	GConstBuffer& GetConstBufferUVW( )
	{
		return *mConstBufferUV;
	}

	void SetConstBufferUVW(const SmartPtr<GConstBuffer> _cBuffUvw)
	{
		mConstBufferUV = _cBuffUvw;
	}

	GConstBuffer& GetConstBufferFillData( )
	{
		return *mConstBufferFillData;
	}

	void SetConstBufferFillData(const SmartPtr<GConstBuffer> _cBuffFill)
	{
		mConstBufferFillData = _cBuffFill;
	}

	void SetFillData(const GVector4D _fillData)
	{
		mFillData = _fillData;
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

	GSprite& GetSprite( )
	{
		return *mSprite;
	}

	void SetSprite(const SmartPtr<GSprite> _sprite)
	{
		mSprite = _sprite;
	}

	UINT GetSpriteIdx( ) const
	{
		return mSpriteIdx;
	}

	void SetSpriteIdx(const UINT _idx)
	{
		mSpriteIdx = _idx;
	}

	void CullBack( )
	{
		mbCullBack = true;
	}

	void CullFront( )
	{
		mbCullBack = false;
	}

	void SetBlender(const SmartPtr<GBlender> _blender)
	{
		mBlender = _blender;
	}

#pragma endregion

private:
	void Init( ) override;
	void Render(const SmartPtr<ComCamera> _cam) override;

private:
	SmartPtr<GMesh> mMesh;

	SmartPtr<GConstBuffer> mConstBuffer;
	SmartPtr<GConstBuffer> mConstBufferUV;
	SmartPtr<GConstBuffer> mConstBufferColor;
	SmartPtr<GConstBuffer> mConstBufferFillData;

	SmartPtr<VertexGShader> mVtxShader;
	SmartPtr<PixelGShader> mPixShader;
	SmartPtr<GSampler> mSampler;
	SmartPtr<GSprite> mSprite;
	SmartPtr<GBlender> mBlender;
	SmartPtr<GRasterizer> mRasterizerBack;
	SmartPtr<GRasterizer> mRasterizerFront;
	SmartPtr<GDepthStencil> mDepthStencil;

	GVector4D mTextureUvw;
	GVector4D mColor;
	GVector4D mInitColor;
	GVector4D mFillData;
	UINT mSpriteIdx;

	bool mbCullBack;
};

