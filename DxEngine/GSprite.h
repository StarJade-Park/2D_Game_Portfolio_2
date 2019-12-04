#pragma once
#include "GTexture.h"
#include "ShaderType.h"

class GShader;

// - texture를 cut하여 보관할 클래스
class GSprite
	: public GResource
{
public:
	GSprite( );
	~GSprite( );

	UINT GetSpritesCount( )
	{
		return (UINT)mVecOfCutSize.size( );
	}

	GVector4D GetCuttedData(const UINT _index);

	bool Create(const wchar_t* _texName, UINT _x, UINT _y, UINT _start, UINT _end);
	bool Create(const std::wstring& _texName, float _sizeX, float _sizeY);
	bool Create(const std::wstring& _texName, UINT _x, UINT _y, UINT _start, UINT _end);

	void PushToVecCutSize(UINT _x, UINT _y, UINT _index);
	void PushToVecCutSize(GVector4D _cutSize, UINT _index);

	void Update(shader_type::TYPE _shaderType, UINT _startSlot);

	GTexture& GetTexture( )
	{
		return *mTexture;
	}
	
	float GetSpriteWidth( ) const
	{
		return mSpriteWidth;
	}

	float GetSpriteHeight( ) const
	{
		return mSpriteHeight;
	}

private:
	SmartPtr<GTexture> mTexture;
	std::vector<GVector4D> mVecOfCutSize;

	float mSpriteWidth;
	float mSpriteHeight;
};

