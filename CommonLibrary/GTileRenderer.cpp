#include <..\DxEngine\ComTransform.h>
#include <..\DxEngine\GMesh.h>
#include <..\DxEngine\GConstBuffer.h>
#include <..\DxEngine\VertexGShader.h>
#include <..\DxEngine\PixelGShader.h>
#include <..\DxEngine\ComCamera.h>
#include <..\DxEngine\GTexture.h>
#include <..\DxEngine\GSampler.h>
#include <..\DxEngine\GSprite.h>
#include <..\DxEngine\GScene.h>
#include <..\DxEngine\GBlender.h>
#include <..\DxEngine\GRasterizer.h>
#include <..\DxEngine\GDepthStencil.h>
#include <..\DxEngine\RendererManager.h>

#include "GTileRenderer.h"

constexpr int rangeX = 10;
constexpr int rangeY = 6;

void GTileRenderer::SetTileSprite(const std::wstring& _spriteName)
{
	mTileSprite = GResource::Find<GSprite>(_spriteName);

	mInstanceData->GetRenderPath( ).SetTexture(_spriteName, shader_type::TYPE::PS, 0);
}

void GTileRenderer::AddTile(const std::pair<int, int>& _mapIndex, const UINT _data, const bool _visible, const float _alphaValue)
{
	if ( nullptr == mTileSprite )
	{
		CRASH_PROG;
		return;
	}

	if ( nullptr != mVecOfTiles[_mapIndex.first][_mapIndex.second] )
	{
		//OutputDebugStringW(L"타일 위치 중복입니다.\n");
		return;
	}

	mVecOfTiles[_mapIndex.first][_mapIndex.second] = new TileData( );
	TileData& tileInfo = (*mVecOfTiles[_mapIndex.first][_mapIndex.second]);

	short* dataArr = (short*)&_data; // 4 Byte To 2 Byte
	short spriteIdx = dataArr[0];
	short rotate = dataArr[1];

	tileInfo.mSpriteIndex = spriteIdx;
	if ( spriteIdx != 0 )
	{
		--tileInfo.mSpriteIndex;
	}

	switch ( rotate )
	{
	case -32768: // H
		tileInfo.mDegree = GVector4D(0.0f, 180.0f, 0.0f);
		break;
	case -24576: // H, D
		CRASH_PROG;
		break;
	case -16384: // H, V
		tileInfo.mDegree = GVector4D(0.0f, 0.0f, 180.0f);
		break;
	case -8192: // ???
		CRASH_PROG;
		break;
	case 0: // empty : none
		EMPTY_STATEMENT;
		break;
	case 8192: // D
		CRASH_PROG;
		break;
	case 16384: // V
		tileInfo.mDegree = GVector4D(0.0f, 180.0f, 180.0f);
		break;
	case 24576: // V, D
		CRASH_PROG;
		break;
	default:
		CRASH_PROG;
		break;
	}

	tileInfo.mbVisible = _visible;
	tileInfo.mAlphaValue = _alphaValue;
}

void GTileRenderer::AddColTile(const std::pair<int, int>& _mapIndex, const UINT _data, const bool _visible, const float _alphaValue)
{
	if ( nullptr == mTileSprite )
	{
		CRASH_PROG;
		return;
	}

	if ( nullptr != mVecOfTiles[_mapIndex.first][_mapIndex.second] )
	{
		//OutputDebugStringW(L"타일 위치 중복입니다.\n");
		return;
	}

	mVecOfTiles[_mapIndex.first][_mapIndex.second] = new TileData( );
	TileData& tileInfo = (*mVecOfTiles[_mapIndex.first][_mapIndex.second]);

	tileInfo.mSpriteIndex = _data;
	tileInfo.mbVisible = _visible;
	tileInfo.mAlphaValue = _alphaValue;
	tileInfo.mPosition = GVector4D(_mapIndex.first * 40.0f, _mapIndex.second * 40.0f);
	if ( 0 != tileInfo.mSpriteIndex )
	{
		tileInfo.mbMoveAble = false;
	}
	else
	{
		tileInfo.mbMoveAble = true;
	}
}

void GTileRenderer::AddTile(const GVector4D& _index)
{
	AddTile(std::lroundf(_index.m.x), std::lroundf(_index.m.y));
}

void GTileRenderer::AddTile(const int _x, const int _y)
{
	if ( nullptr == mTileSprite )
	{
		CRASH_PROG;
		return;
	}

	if ( nullptr != mVecOfTiles[_x][_y] )
	{
		//OutputDebugStringW(L"타일 위치 중복입니다.\n");
		return;
	}

	mVecOfTiles[_x][_y] = new TileData( );
}

void GTileRenderer::AddTileByWorldPos(const GVector2D& _pos)
{
	GVector4D idx = GVector4D( );
	// index    mousePos / sprite size
	idx.m.x = (_pos.m.x) / 40.0f;
	idx.m.y = (_pos.m.y) / 40.0f;

	AddTile(std::lroundf(idx.m.x), std::lroundf(idx.m.y));
}

void GTileRenderer::DelTile(const int _x, const int _y)
{
	// CREATE : key
	std::pair<int, int> key = std::pair<int, int>(_x, _y);
	if ( nullptr == mVecOfTiles[_x][_y] )
	{
		return;
	}

	mVecOfTiles[_x][_y] = nullptr;
}

void GTileRenderer::DelTileByWorldPos(const GVector2D& _pos)
{
	GVector4D idx = GVector4D( );
	// index    mousePos / sprite size
	idx.m.x = (_pos.m.x) / 40.0f;
	idx.m.y = (_pos.m.y) / 40.0f;

	DelTile(std::lroundf(idx.m.x), std::lroundf(idx.m.y));
}

int a = 0;

void GTileRenderer::Init( )
{
	// note instance vertex shader 
	if ( nullptr == GResource::Find<VertexGShader>(L"TextureForInstance.hlsl") )
	{
		SmartPtr<VertexGShader> vtx =
			GResource::Load<VertexGShader>(
			GFileSystem::Join(GFileSystem::FindPathOrNull(L"ShaderCode").GetAbsPath( ), L"TextureForInstance.hlsl"),
			5,
			0,
			"VS_TEX"
			);

		vtx->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		vtx->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		// 한 번에 넘길 수 있는 data의 크기가 float4(16byte)이므로 matrix를 넘겨줄 때는 아래와 같이 한다.
		vtx->AddLayOut(
			"WVP", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
			1, 1/*instance data임을 나타냄*/, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA
		);
		vtx->AddLayOut(
			"WVP", 1, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
			1, 1, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA
		);
		vtx->AddLayOut(
			"WVP", 2, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
			1, 1, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA
		);
		vtx->AddLayOut(
			"WVP", 3, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
			1, 1, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA
		);

		vtx->EndLayOut("SPRITE_UV", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
			1, 1, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA
		);
	}

	if ( nullptr == GResource::Find<InstanceBuffer>(L"TileInstaceBuffer") )
	{
		GResource::Create<InstanceBuffer>(L"TileInstaceBuffer", 20000, &typeid(TileInstanceData), sizeof(TileInstanceData));
	}

	if ( nullptr == GResource::Find<GMesh>(L"TileInstanceMesh") )
	{
		auto newMesh = GResource::Create<GMesh>(L"TileInstanceMesh");
		newMesh->AddVertexBuffer(L"TextureVertexBuffer");
		newMesh->AddInstanceBuffer(L"TileInstaceBuffer");
		newMesh->AddIndexBuffer(L"TextureIndexBuffer");
	}

	mInstanceData = RendererManager::CreateInstanceDataOrNull(L"TileInstanceData", L"TileInstanceMesh");
	mInstanceData->GetRenderPath( ).SetBlender(L"Tile_Blender");
	mInstanceData->GetRenderPath( ).SetDepthStencil(L"DS_State");
	mInstanceData->GetRenderPath( ).SetRasterizer(L"RS_State_None");
	mInstanceData->GetRenderPath( ).SetVtxShader(L"TextureForInstance.hlsl");
	mInstanceData->GetRenderPath( ).SetPixShader(L"TextureShader.hlsl");
	mInstanceData->GetRenderPath( ).SetSampler(L"Sampler", shader_type::TYPE::PS, 0);
	mInstanceData->GetRenderPath( ).SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//RenderingPaths renderingPath = RenderingPaths( );
	//renderingPath.SetBlender(L"Blender");
	//renderingPath.SetDepthStencil(L"DS_State");
	//renderingPath.SetRasterizer(L"RS_State_None");
	//renderingPath.SetVtxShader(L"TextureForInstance.hlsl");
	//renderingPath.SetPixShader(L"TextureShader.hlsl");
	//renderingPath.SetSampler(L"Sampler", shader_type::TYPE::PS, 0);
	//renderingPath.SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//instData->SetRenderPath(renderingPath);

	ComRender::Init( );
}

void GTileRenderer::Render(const SmartPtr<ComCamera> _cam)
{
	if ( nullptr == mTileSprite )
	{
		return;
	}

	UpdateTransform( );
	//GMatrix worldMatrix = GetWorldMatrix( );
	GMatrix wvp = GetWorldMatrix( ) * (_cam->GetVP( ));
	GMatrix posMatrix = GMatrix( );
	GMatrix resultMatrix = GMatrix( );
	GMatrix scaleMatrix = GMatrix( );

	scaleMatrix.Scale(mMoveSize + 0.05f);

	TileInstanceData instData = TileInstanceData( );

	int maxX = (int)mCenterTilePos.m.x;
	int maxY = (int)mCenterTilePos.m.y;
	for ( int x = maxX - rangeX; x < maxX + rangeX + 2; ++x )
	{
		if ( 0 > x || mVecOfTiles.size( ) <= x )
		{
			continue;
		}

		for ( int y = maxY - rangeY; y < maxY + rangeY + 1; ++y )
		{
			if ( 0 > y || mVecOfTiles[x].size( ) <= y )
			{
				continue;
			}

			if ( nullptr == mVecOfTiles[x][y] )
			{
				continue;
			}

			if ( false == mVecOfTiles[x][y]->mbVisible )
			{
				continue;
			}

			mTileSprite->Update(shader_type::TYPE::PS, 0);

			mTextureUvw = mTileSprite->GetCuttedData(mVecOfTiles[x][y]->mSpriteIndex);

			GMatrix rotMatrix;
			rotMatrix.RotByDeg(mVecOfTiles[x][y]->mDegree);

			posMatrix.Translate(
				{ (float)(x) * mMoveSize.m.x,
				(float)(y) * mMoveSize.m.y }
			);

			resultMatrix = scaleMatrix * rotMatrix * posMatrix * wvp;

			instData.WVP = resultMatrix;
			instData.spriteUV = mTextureUvw;

			mInstanceData->PushData(instData);
		}
	}
}
