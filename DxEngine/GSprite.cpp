#include "GSprite.h"

GSprite::GSprite( )
	: mTexture(nullptr), mVecOfCutSize(std::vector<GVector4D>( ))
	, mSpriteWidth(gconst::inf<float>), mSpriteHeight(gconst::inf<float>)
{
	EMPTY_STATEMENT;
}

GSprite::~GSprite( )
{
	EMPTY_STATEMENT;
}

GVector4D GSprite::GetCuttedData(const UINT _index)
{
	if ( _index >= (UINT)mVecOfCutSize.size( ) )
	{
		CRASH_PROG;
		return { 0.0f };
	}

	return mVecOfCutSize[_index];
}

bool GSprite::Create(const wchar_t* _texName, UINT _x, UINT _y, UINT _start, UINT _end)
{
	return Create(std::wstring(_texName), _x, _y, _start, _end);
}

bool GSprite::Create(const std::wstring& _texName, float _sizeX, float _sizeY)
{
	mTexture = GResource::Find<GTexture>(_texName);
	if ( nullptr == mTexture )
	{
		CRASH_PROG;
		return false;
	}

	UINT x = (UINT)(mTexture->GetWitdh( ) / _sizeX);
	UINT y = (UINT)(mTexture->GetHeight( ) / _sizeY);

	return Create(_texName, x, y, 0, (x * y));
}

bool GSprite::Create(const std::wstring& _texName, UINT _x, UINT _y, UINT _start, UINT _end)
{
	if ( _start > _end )
	{
		CRASH_PROG;
		return false;
	}

	mTexture = GResource::Find<GTexture>(_texName);
	if ( nullptr == mTexture )
	{
		CRASH_PROG;
		return false;
	}

	mSpriteWidth = mTexture->GetWitdh( ) / (float)_x;
	mSpriteHeight = mTexture->GetHeight( ) / (float)_y;

	if ( _end >= (UINT)mVecOfCutSize.size( ) )
	{
		mVecOfCutSize.resize(_end + 1);
	}

	for ( UINT idx = _start; idx < _end; ++idx )
	{
		PushToVecCutSize(_x, _y, idx);
	}

	return true;
}

void GSprite::PushToVecCutSize(UINT _x, UINT _y, UINT _index)
{
	UINT x = _index % _x; // 전체 이미지에서 자르고자하는 크기로 했을 때 x 갯수
	UINT y = _index / _x; // 전체 이미지에서 자르고자하는 크기로 했을 때 y 갯수

	GVector4D cutSize = GVector4D( );
	cutSize.m.z = (1.0f / (float)_x); // size x
	cutSize.m.w = (1.0f / (float)_y); // size y

	// 1개 sprite의 크기
	cutSize.m.x = cutSize.m.z * x;
	cutSize.m.y = cutSize.m.w * y;

	PushToVecCutSize(cutSize, _index);
}

void GSprite::PushToVecCutSize(GVector4D _cutSize, UINT _index)
{
	//if ( _index >= (UINT)mVecOfCutSize.size( ) ) // note 남아있을 필요?
	//{
	//	mVecOfCutSize.resize(_index);
	//}

	mVecOfCutSize[_index] = _cutSize;
}

void GSprite::Update(shader_type::TYPE _shaderType, UINT _startSlot)
{
	if ( nullptr == mTexture )
	{
		CRASH_PROG;
		return;
	}

	mTexture->Update(_shaderType, _startSlot);
}
