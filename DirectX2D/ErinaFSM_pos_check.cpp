#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>

#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\GActor.h>
#include <..\DxEngine\GResource.h>

#include <..\CommonLibrary\GTileRenderer.h>

#include "ErinaFSM.h"

namespace player_correct_value
{

constexpr float sideCorrectValue = 12.5f;
constexpr float topCorrectValue = 25.0f;
constexpr float downCorrectValue = 29.0f;

constexpr float correctionValue = gconst::ftol + 20.0f;

}

void ErinaFSM::CheckGround( )
{
	mErinaPos.m.y += mJumpPower * GameTime::DeltaTime( );
	mJumpPower -= mGravityAcceleration * GameTime::DeltaTime( );
	
	if ( true == DebugRenderer::IsOnDebugMode( ) )
	{
		DebugRenderer::DrawFontUI(std::wstring(L"Jump Pwr : ").append(std::to_wstring(mJumpPower)));
		DebugRenderer::DrawFontUI(std::wstring(L"Gravity acc : ").append(std::to_wstring(mGravityAcceleration)));
	}

	float Prev = mErinaPos.m.y;
	float Check;
	mCheckPosDown = mErinaPos + GVector4D::vDown * player_correct_value::downCorrectValue;

	while ( true )
	{
		COLLISION_TILE_TYPE colTileType = mColTileRenderer->GetTileType(mCheckPosDown);
		Check = CollideInclineTile(colTileType);

		if ( Check == Prev )
		{
			Check += player_correct_value::downCorrectValue;
			break;
		}

		Prev = Check;
	}

	mErinaPos.m.y = Check;

	CheckBlock( );
}

void ErinaFSM::UpdateCheckPos( )
{
	mCheckPosLeft = mErinaPos + GVector4D::vLeft * player_correct_value::sideCorrectValue;
	mCheckPosRight = mErinaPos + GVector4D::vRight * player_correct_value::sideCorrectValue;
	mCheckPosTop = mErinaPos + GVector4D::vUp * player_correct_value::topCorrectValue;
	mCheckPosDown = mErinaPos + GVector4D::vDown * player_correct_value::downCorrectValue;
}

void ErinaFSM::CheckBlock( )
{
	GMatrix worldPosMat;

	UpdateCheckPos( );

	if ( true == DebugRenderer::IsOnDebugMode( ) ) 
	{
		worldPosMat.Translate(mCheckPosLeft);
		DebugRenderer::DrawPoint(ComCamera::mMainCam, worldPosMat, GVector4D::Red);
		worldPosMat.Translate(mCheckPosRight);
		DebugRenderer::DrawPoint(ComCamera::mMainCam, worldPosMat, GVector4D::Red);
		worldPosMat.Translate(mCheckPosTop);
		DebugRenderer::DrawPoint(ComCamera::mMainCam, worldPosMat, GVector4D::Red);
		worldPosMat.Translate(mCheckPosDown);
		DebugRenderer::DrawPoint(ComCamera::mMainCam, worldPosMat, GVector4D::Red);

		DebugRenderer::DrawVectorUI(mCheckPosLeft);
		DebugRenderer::DrawVectorUI(mCheckPosRight);
		DebugRenderer::DrawVectorUI(mCheckPosTop);
		DebugRenderer::DrawVectorUI(mCheckPosDown);
		DebugRenderer::DrawVectorUI(mErinaPos);
	}

	ProcessBlockInvasion(_ERINA_DIRECTION::ERINA_DOWN, mColTileRenderer->GetTile(mCheckPosDown));
	ProcessBlockInvasion(_ERINA_DIRECTION::ERINA_LEFT, mColTileRenderer->GetTile(mCheckPosLeft));
	ProcessBlockInvasion(_ERINA_DIRECTION::ERINA_RIGHT, mColTileRenderer->GetTile(mCheckPosRight));
	ProcessBlockInvasion(_ERINA_DIRECTION::ERINA_UP, mColTileRenderer->GetTile(mCheckPosTop));

	if ( true == DebugRenderer::IsOnDebugMode( ) ) 
	{
		std::wstring debug_col_type =
			std::to_wstring(mColTileRenderer->GetTileType(mCheckPosLeft)).append(L"L. ").
			append(std::to_wstring(mColTileRenderer->GetTileType(mCheckPosRight))).append(L"R. ").
			append(std::to_wstring(mColTileRenderer->GetTileType(mCheckPosTop))).append(L"T. ").
			append(std::to_wstring(mColTileRenderer->GetTileType(mCheckPosDown))).append(L"D");

		DebugRenderer::DrawFontUI(debug_col_type);
		DebugRenderer::DrawVectorUI(mErinaPos, GVector4D::Green);
	}
}

void ErinaFSM::ProcessBlockInvasion(const _ERINA_DIRECTION _dir, const GTileRenderer::TileData* _tile)
{
	if ( nullptr == _tile )
	{
		return;
	}

	if ( COLLISION_TILE_TYPE::NORMAL != (COLLISION_TILE_TYPE)_tile->mSpriteIndex )
	{
		return;
	}

	switch ( _dir )
	{
	case ErinaFSM::_ERINA_DIRECTION::ERINA_LEFT:
		{
			GVector4D checkPos = mErinaPos + (GVector4D::vLeft * player_correct_value::sideCorrectValue);
			checkPos.m.x = _tile->mPosition.m.x + player_correct_value::correctionValue;
			checkPos.m.x += 2.0f;
			mErinaPos = checkPos - (GVector4D::vLeft * player_correct_value::sideCorrectValue);
			_tile = mColTileRenderer->GetTile(checkPos);

			while ( COLLISION_TILE_TYPE::NORMAL == _tile->mSpriteIndex )
			{
				checkPos.m.x = _tile->mPosition.m.x + player_correct_value::correctionValue;
				checkPos.m.x += 2.0f;
				mErinaPos = checkPos - (GVector4D::vLeft * player_correct_value::sideCorrectValue);
				_tile = mColTileRenderer->GetTile(checkPos);
			}
		}
		return;
	case ErinaFSM::_ERINA_DIRECTION::ERINA_RIGHT:
		{
			GVector4D checkPos = mErinaPos + (GVector4D::vRight * player_correct_value::sideCorrectValue);
			checkPos.m.x = _tile->mPosition.m.x - player_correct_value::correctionValue;
			checkPos.m.x -= 2.0f;
			mErinaPos = checkPos - (GVector4D::vRight * player_correct_value::sideCorrectValue);
			_tile = mColTileRenderer->GetTile(checkPos);

			while ( COLLISION_TILE_TYPE::NORMAL == _tile->mSpriteIndex )
			{
				checkPos.m.x = _tile->mPosition.m.x - player_correct_value::correctionValue;
				checkPos.m.x -= 2.0f;
				mErinaPos = checkPos - (GVector4D::vRight * player_correct_value::sideCorrectValue);
				_tile = mColTileRenderer->GetTile(checkPos);
			}
		}
		return;
	case ErinaFSM::_ERINA_DIRECTION::ERINA_UP:
		{
			GVector4D checkPos = mErinaPos + (GVector4D::vUp * player_correct_value::downCorrectValue);
			checkPos.m.y = _tile->mPosition.m.y - player_correct_value::correctionValue;
			checkPos.m.y -= 2.0f;
			mErinaPos = checkPos - (GVector4D::vUp * player_correct_value::downCorrectValue);
			_tile = mColTileRenderer->GetTile(checkPos);

			while ( COLLISION_TILE_TYPE::NORMAL == _tile->mSpriteIndex )
			{
				checkPos.m.y = _tile->mPosition.m.y - player_correct_value::correctionValue;
				checkPos.m.y -= 2.0f;
				mErinaPos = checkPos - (GVector4D::vUp * player_correct_value::downCorrectValue);
				_tile = mColTileRenderer->GetTile(checkPos);
			}
		}

		mCurErinaAni->ChangeAnimation(L"Jump_Down");
		meErinaState = _ERINA_STATE::ERINA_JUMP;
		mJumpPower = 0.0f;

		return;
	case ErinaFSM::_ERINA_DIRECTION::ERINA_DOWN:
		{
			GVector4D checkPos = mErinaPos + (GVector4D::vDown * player_correct_value::downCorrectValue);
			checkPos.m.y = _tile->mPosition.m.y + player_correct_value::correctionValue;
			mErinaPos = checkPos - (GVector4D::vDown * player_correct_value::downCorrectValue);
			_tile = mColTileRenderer->GetTile(checkPos);

			while ( COLLISION_TILE_TYPE::NORMAL == _tile->mSpriteIndex )
			{
				checkPos.m.y = _tile->mPosition.m.y + player_correct_value::correctionValue;
				mErinaPos = checkPos - (GVector4D::vDown * player_correct_value::downCorrectValue);
				_tile = mColTileRenderer->GetTile(checkPos);
			}
		}

		InitializeProperty( );

		return;
	case ErinaFSM::_ERINA_DIRECTION::ERINA_DIR_MAX: FALL_THROUGH;
	default:
		CRASH_PROG;
		return;
	}
}

float ErinaFSM::CollideInclineTile(const COLLISION_TILE_TYPE _tileType)
{
	if( COLLISION_TILE_TYPE::EMPTY == _tileType || COLLISION_TILE_TYPE::NORMAL == _tileType )
	{
		return mCheckPosDown.m.y;
	}

	// 여기부터 경사 타일 확인한다.
	auto result = [&](float _modY, float _newYPos)->float
	{
		return ((_modY + _newYPos) * 40.0f);
	};

	auto returnVal = [&](float _yPosInTile, float _modY, float _newYPos)->float
	{
		if ( 0.5f == _yPosInTile )
		{
			InitializeProperty( );

			return result(_modY, _newYPos);
		}
		else if ( _yPosInTile >= _newYPos )
		{
			return mCheckPosDown.m.y;
		}
		else
		{
			InitializeProperty( );

			return result(_modY, _newYPos);
		}
	};

	float modX = std::roundf(mCheckPosDown.m.x / 40.0f);
	float modY = std::roundf(mCheckPosDown.m.y / 40.0f);
	float xPosInTile = (std::remainderf(mCheckPosDown.m.x, 40.0f) / 40.0f) + 0.5f;
	float yPosInTile = (std::remainderf(mCheckPosDown.m.y, 40.0f) / 40.0f);
	float newYpos = 0.0f;

	switch ( _tileType )
	{
	case COLLISION_TILE_TYPE::LEFT_50:
		newYpos = xPosInTile - (1.0f) + 0.5f;
		
		return returnVal(yPosInTile, modY, newYpos);
	case COLLISION_TILE_TYPE::RIGHT_50:
		newYpos = (1.0f) - xPosInTile - 0.5f;
		
		return returnVal(yPosInTile, modY, newYpos);
	case COLLISION_TILE_TYPE::RIGHT_25_1:
		newYpos = (1.0f - xPosInTile) * 0.5f - 0.5f;
		
		return returnVal(yPosInTile, modY, newYpos);
	case COLLISION_TILE_TYPE::RIGHT_25_2:
		newYpos = ((1.0f - xPosInTile) * 0.5f);
		
		return returnVal(yPosInTile, modY, newYpos);
	case COLLISION_TILE_TYPE::LEFT_25_1:
		newYpos = (xPosInTile - 1.0f) * 0.5f;
		
		return returnVal(yPosInTile, modY, newYpos);
	case COLLISION_TILE_TYPE::LEFT_25_2:
		newYpos = ((xPosInTile - 1.0f) * 0.5f) + 0.5f;
		
		return returnVal(yPosInTile, modY, newYpos);
	case COLLISION_TILE_TYPE::PLATFORM:
		newYpos = yPosInTile + 0.5f;
		
		if ( 0.8f <= newYpos && 0 >= mJumpPower )
		{
			float newYPos_index = std::roundf(mCheckPosDown.m.y / 40.0f) + 0.01f;
			float newYPos_remainder = (std::remainderf(mCheckPosDown.m.y, 40.0f) / 40.0f);
			
			InitializeProperty( );
			
			return std::roundf(result(newYPos_index, newYPos_remainder));
		}
		else
		{
			return mCheckPosDown.m.y;
		}
	default:
		CRASH_PROG;
		break;
	}
	
	CRASH_PROG;
	return gconst::inf<float>;
}
