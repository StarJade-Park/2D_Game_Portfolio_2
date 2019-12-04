#pragma once
#include <map>

#include <..\DxEngine\ComRender.h>
#include <..\DxEngine\GSprite.h>

#include <..\DxEngine\DebugRenderer.h>
#include <..\DxEngine\InstanceData.h>

class GMesh;
class GConstBuffer;
class VertexGShader;
class PixelGShader;
class GSampler;
class GSprite;
class GBlender;
class GRasterizer;
class GDepthStencil;

enum COLLISION_TILE_TYPE : UINT
{
	EMPTY, NORMAL, LEFT_50, RIGHT_50, LEFT_25_1, RIGHT_25_1, LEFT_25_2, RIGHT_25_2,
	PLATFORM = 14, MAX, 
};

enum COLLISION_TILE_DIRECTION : UINT
{
	LEFT, RIGHT, TOP, DOWN
};

struct TileInstanceData
{
	GMatrix WVP;
	GVector4D spriteUV;
};

class GTileRenderer
	: public ComRender
{
public:
	// 32x32 tile
	struct TileData
	{
	public:
		TileData( )
			: mbMoveAble(true), mSpriteIndex(gconst::inf<UINT>), mDegree(0.0f), mPosition(0.0f)
		{
			EMPTY_STATEMENT;
		}

		~TileData( )
		{
			EMPTY_STATEMENT;
		}

	public:
		bool mbMoveAble;
		bool mbVisible;
		float mAlphaValue;
		UINT mSpriteIndex; // - 0 is null tile
		GVector4D mDegree;
		GVector4D mPosition;
	};

public:
	GTileRenderer( )
		: mVecOfTiles(std::vector<std::vector<TileData*>>( ))
		, mMaxSizeX(0), mMaxSizeY(0)
		, mMoveSize(GVector2D(1.0f, 1.0f))
		, mCenterTilePos(GVector2D(0.0f, 0.0f))
		, mTextureUvw(GVector4D( )), mColor(GVector4D(1.0f))
		, mTileSprite(nullptr), mMesh(nullptr)
		, mConstBuffer(nullptr), mConstBufferUV(nullptr), mConstBufferColor(nullptr)
		, mVtxShader(nullptr), mPixShader(nullptr)
		, mSampler(nullptr), mBlender(nullptr)
		, mRasterizerCullNone(nullptr)
		, mDepthStencil(nullptr)
		, mInstanceData(nullptr)
	{
		EMPTY_STATEMENT;
	}

	~GTileRenderer( )
	{
		for ( auto& vecTile : mVecOfTiles )
		{
			for ( auto& tile : vecTile )
			{
				delete tile;
				tile = nullptr;
			}
		}
	}

	void SetMaxSizeOfMap(const UINT _xSize, const UINT _ySize)
	{
		mVecOfTiles.resize(_xSize);

		for ( auto& tile : mVecOfTiles )
		{
			tile.resize(_ySize);
		}
	}

	size_t GetMaxSizeX( ) const
	{
		return mVecOfTiles.size( );
	}

	size_t GetMaxSizeY(int _xIdx) const
	{
		return mVecOfTiles[_xIdx].size( );
	}

	void SetTileSprite(const std::wstring& _spriteName);

	void SetTileSpriteIdx(const std::pair<int, int>& _tileIndex, const int _spriteIndex)
	{
		auto tile = mVecOfTiles[_tileIndex.first][_tileIndex.second];
		tile->mSpriteIndex = _spriteIndex;
	}
	
	void SetMoveSize(const GVector2D& _size)
	{
		mMoveSize = _size;
	}

	void SetCenterPos(const GVector2D& _pos)
	{
		mCenterTilePos = _pos;
	}

	GVector2D GetCenterPos( ) const
	{
		return mCenterTilePos;
	}
	
	void AddTile(const std::pair<int, int>& _mapIndex, const UINT _data, const bool _visible, const float _alphaValue);
	void AddColTile(const std::pair<int, int>& _mapIndex, const UINT _data, const bool _visible, const float _alphaValue);

	// - unused z, w
	void AddTile(const GVector4D& _index);
	void AddTile(const int _x, const int _y);
	void AddTileByWorldPos(const GVector2D& _pos);
	
	void DelTile(const int _x, const int _y);
	void DelTileByWorldPos(const GVector2D& _pos);

	UINT GetTileSprite(const GVector4D& _pos)
	{
		int x = std::lroundf(_pos.m.x / 40.0f);
		int y = std::lroundf(_pos.m.y / 40.0f);

		if ( x < 0 || x > mVecOfTiles.size( ) )
		{
			return gconst::inf<UINT>;
		}

		if ( y < 0 || y > mVecOfTiles[x].size( ) )
		{
			return gconst::inf<UINT>;
		}

		return mVecOfTiles[x][y]->mSpriteIndex;
	}

	COLLISION_TILE_TYPE GetTileType(const GVector4D& _pos)
	{
		int x = std::lroundf(_pos.m.x / 40.0f);
		int y = std::lroundf(_pos.m.y / 40.0f);

		if ( x < 0 || x > mVecOfTiles.size( ) )
		{
			return COLLISION_TILE_TYPE::MAX;
		}

		if ( y < 0 || y > mVecOfTiles[x].size( ) )
		{
			return COLLISION_TILE_TYPE::MAX;
		}

		return (COLLISION_TILE_TYPE)mVecOfTiles[x][y]->mSpriteIndex;
	}

	void ClearTileMap( )
	{
		for ( auto& vecTile : mVecOfTiles )
		{
			for ( auto& tile : vecTile )
			{
				delete tile;
				tile = nullptr;
			}
		}

		mVecOfTiles.clear( );
	}

	TileData& GetTile(const int _x, const int _y)
	{
		return *mVecOfTiles[_x][_y];
	}

	TileData* GetTile(const GVector4D& _pos)
	{
		int x = std::lroundf(_pos.m.x / 40.0f);
		int y = std::lroundf(_pos.m.y / 40.0f);

		if ( x < 0 || x > mVecOfTiles.size( ) )
		{
			return nullptr;
		}

		if ( y < 0 || y > mVecOfTiles[x].size( ) )
		{
			return nullptr;
		}

		return mVecOfTiles[x][y];
	}

private:
	void Init( ) override;
	void Render(const SmartPtr<ComCamera> _cam) override;

private:
	UINT mMaxSizeX;
	UINT mMaxSizeY;

	GVector2D mMoveSize;
	GVector2D mCenterTilePos;

	GVector4D mTextureUvw;
	GVector4D mColor;

	std::vector<std::vector<TileData*>> mVecOfTiles;
	SmartPtr<GSprite> mTileSprite;
	SmartPtr<GMesh> mMesh;
	SmartPtr<InstanceData> mInstanceData;

	SmartPtr<GConstBuffer> mConstBuffer;
	SmartPtr<GConstBuffer> mConstBufferUV;
	SmartPtr<GConstBuffer> mConstBufferColor;

	SmartPtr<VertexGShader> mVtxShader;
	SmartPtr<PixelGShader> mPixShader;

	SmartPtr<GSampler> mSampler;
	SmartPtr<GBlender> mBlender;

	SmartPtr<GRasterizer> mRasterizerCullNone;
	SmartPtr<GDepthStencil> mDepthStencil;
};

