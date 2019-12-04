#include "MonsterFSM.h"
#include <..\CommonLibrary\RbrbRoom.h>

MonsterFSM::MonsterFSM( )
	: mDefaultJumpPower(500.f), mDefaultGravityAcceleration(1000.0f)
	, mbOnGroundFlag(false)
{
	EMPTY_STATEMENT;
}

MonsterFSM::~MonsterFSM( )
{
	EMPTY_STATEMENT;
}

void MonsterFSM::Init( )
{
	mActor = &GetActor( );
	meDir = DIRECTION::LEFT;
	meState = ORIGIN_FSM::IDLE;

	mGravityAcceleration = mDefaultGravityAcceleration;
	mJumpPower = 0.0f;
}

void MonsterFSM::Update( )
{
	mPos = mActor->GetPos( );

	CheckDirection( );

	CheckGround( );

	switch ( meState )
	{
	case MonsterFSM::ORIGIN_FSM::IDLE:
		Idle( );
		break;
	case MonsterFSM::ORIGIN_FSM::ATTACK:
		Attack( );
		break;
	case MonsterFSM::ORIGIN_FSM::ATTACKED_BY:
		AttackedBy( );
		break;
	case MonsterFSM::ORIGIN_FSM::JUMP:
		Jump( );
		break;
	case MonsterFSM::ORIGIN_FSM::DIE:
		Die( );
		break;
	case MonsterFSM::ORIGIN_FSM::ORIGIN_FSM_MAX:
	default:
		CRASH_PROG;
		break;
	}

	mActor->SetPos(mPos);
}

void MonsterFSM::Idle( )
{
	EMPTY_STATEMENT;
}

void MonsterFSM::Walk( )
{
	EMPTY_STATEMENT;
}

void MonsterFSM::Attack( )
{
	EMPTY_STATEMENT;
}

void MonsterFSM::AttackedBy( )
{
	EMPTY_STATEMENT;
}

void MonsterFSM::Jump( )
{
	EMPTY_STATEMENT;
}

void MonsterFSM::Die( )
{
	EMPTY_STATEMENT;
}

void MonsterFSM::CheckDirection( )
{
	EMPTY_STATEMENT;
}

bool MonsterFSM::IsInMyRoom( )
{
	RbrbRoom::RoomData curRoom = RbrbRoom::SRoom.GetRoomData(mPos);

	int x = std::lroundf((mPos.m.x - (RbrbRoom::mRoomSizeX / 2 * 40)) / RbrbRoom::mRoomSizeX / 40);
	int y = std::lroundf((mPos.m.y - (RbrbRoom::mRoomSizeY / 2 * 40)) / RbrbRoom::mRoomSizeY / 40);

	if ( mRoomIndexX != curRoom.indexX )
	{
		return false;
	}

	if ( mRoomIndexY != curRoom.indexY )
	{
		return false;
	}

	return true;
}

void MonsterFSM::CheckGround( )
{
	mPos.m.y += mJumpPower * GameTime::DeltaTime( );
	mJumpPower -= mGravityAcceleration * GameTime::DeltaTime( );

	UpdateCheckPos( );

	float Prev = mPos.m.y;
	float Check;

	while ( true )
	{
		COLLISION_TILE_TYPE colTileType = mColTileRenderer->GetTileType(mCheckPosDown);
		Check = CollideInclineTile(colTileType);

		if ( Check == Prev )
		{
			Check += monster_correct_value::downCorrectValue;
			break;
		}

		Prev = Check;
	}

	mPos.m.y = Check;

	UpdateCheckPos( );
	CheckBlock( );
	
	if ( true == DebugRenderer::IsOnDebugMode( ) )
	{
		DebugRenderer::DrawVectorUI(mPos);
	}
}

//void MonsterFSM::UpdateCheckPos( )
//{
//	mCheckPosLeft = mPos + GVector4D::vLeft * monster_correct_value::sideCorrectValue;
//	mCheckPosRight = mPos + GVector4D::vRight * monster_correct_value::sideCorrectValue;
//	mCheckPosTop = mPos + GVector4D::vUp * monster_correct_value::topCorrectValue;
//	mCheckPosDown = mPos + GVector4D::vDown * monster_correct_value::downCorrectValue;
//
//	DrawCheckPosForDebuging( );
//}

void MonsterFSM::DrawCheckPosForDebuging( )
{
	GMatrix worldPosMat;

	worldPosMat.Translate(mCheckPosLeft);
	DebugRenderer::DrawPoint(ComCamera::mMainCam, worldPosMat, GVector4D::Red);
	worldPosMat.Translate(mCheckPosRight);
	DebugRenderer::DrawPoint(ComCamera::mMainCam, worldPosMat, GVector4D::Red);
	worldPosMat.Translate(mCheckPosTop);
	DebugRenderer::DrawPoint(ComCamera::mMainCam, worldPosMat, GVector4D::Red);
	worldPosMat.Translate(mCheckPosDown);
	DebugRenderer::DrawPoint(ComCamera::mMainCam, worldPosMat, GVector4D::Red);
}

void MonsterFSM::InitializeProperty( )
{
	mGravityAcceleration = mDefaultGravityAcceleration;
	mJumpPower = 0.0f;
	mbOnGroundFlag = true;
}

void MonsterFSM::CheckBlock( )
{
	mColTileRenderer->GetTile(mPos);
	ProcessBlockInvasion(DIRECTION::DOWN, mColTileRenderer->GetTile(mCheckPosDown));
	ProcessBlockInvasion(DIRECTION::LEFT, mColTileRenderer->GetTile(mCheckPosLeft));
	ProcessBlockInvasion(DIRECTION::RIGHT, mColTileRenderer->GetTile(mCheckPosRight));
	ProcessBlockInvasion(DIRECTION::UP, mColTileRenderer->GetTile(mCheckPosTop));
}

void MonsterFSM::ProcessBlockInvasion(const DIRECTION _dir, const GTileRenderer::TileData* _tile)
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
	case MonsterFSM::DIRECTION::LEFT:
		{
			GVector4D checkPos = mPos + (GVector4D::vLeft * monster_correct_value::sideCorrectValue);
			checkPos.m.x = _tile->mPosition.m.x + monster_correct_value::correctionValue;
			checkPos.m.x += 2.0f;
			mPos = checkPos - (GVector4D::vLeft * monster_correct_value::sideCorrectValue);
			_tile = mColTileRenderer->GetTile(checkPos);

			while ( COLLISION_TILE_TYPE::NORMAL == _tile->mSpriteIndex )
			{
				checkPos.m.x = _tile->mPosition.m.x + monster_correct_value::correctionValue;
				checkPos.m.x += 2.0f;
				mPos = checkPos - (GVector4D::vLeft * monster_correct_value::sideCorrectValue);
				_tile = mColTileRenderer->GetTile(checkPos);
			}
		}
		return;
	case MonsterFSM::DIRECTION::RIGHT:
		{
			GVector4D checkPos = mPos + (GVector4D::vRight * monster_correct_value::sideCorrectValue);
			checkPos.m.x = _tile->mPosition.m.x - monster_correct_value::correctionValue;
			checkPos.m.x -= 2.0f;
			mPos = checkPos - (GVector4D::vRight * monster_correct_value::sideCorrectValue);
			_tile = mColTileRenderer->GetTile(checkPos);

			while ( COLLISION_TILE_TYPE::NORMAL == _tile->mSpriteIndex )
			{
				checkPos.m.x = _tile->mPosition.m.x - monster_correct_value::correctionValue;
				checkPos.m.x -= 2.0f;
				mPos = checkPos - (GVector4D::vRight * monster_correct_value::sideCorrectValue);
				_tile = mColTileRenderer->GetTile(checkPos);
			}
		}
		return;
	case MonsterFSM::DIRECTION::UP:
		{
			GVector4D checkPos = mPos + (GVector4D::vUp * monster_correct_value::downCorrectValue);
			checkPos.m.y = _tile->mPosition.m.y - monster_correct_value::correctionValue;
			checkPos.m.y -= 2.0f;
			mPos = checkPos - (GVector4D::vUp * monster_correct_value::downCorrectValue);
			_tile = mColTileRenderer->GetTile(checkPos);

			while ( COLLISION_TILE_TYPE::NORMAL == _tile->mSpriteIndex )
			{
				checkPos.m.y = _tile->mPosition.m.y - monster_correct_value::correctionValue;
				checkPos.m.y -= 2.0f;
				mPos = checkPos - (GVector4D::vUp * monster_correct_value::downCorrectValue);
				_tile = mColTileRenderer->GetTile(checkPos);
			}
		}

		mJumpPower = 0.0f;

		return;
	case MonsterFSM::DIRECTION::DOWN:
		{
			GVector4D checkPos = mPos + (GVector4D::vDown * monster_correct_value::downCorrectValue);
			checkPos.m.y = _tile->mPosition.m.y + monster_correct_value::correctionValue;
			mPos = checkPos - (GVector4D::vDown * monster_correct_value::downCorrectValue);
			_tile = mColTileRenderer->GetTile(checkPos);

			while ( COLLISION_TILE_TYPE::NORMAL == _tile->mSpriteIndex )
			{
				checkPos.m.y = _tile->mPosition.m.y + monster_correct_value::correctionValue;
				mPos = checkPos - (GVector4D::vDown * monster_correct_value::downCorrectValue);
				_tile = mColTileRenderer->GetTile(checkPos);
			}
		}

		InitializeProperty( );

		return;
	case MonsterFSM::DIRECTION::DIRECTION_MAX: FALL_THROUGH;
	default:
		CRASH_PROG;
		return;
	}
}

float MonsterFSM::CollideInclineTile(const COLLISION_TILE_TYPE _tileType)
{
	if ( COLLISION_TILE_TYPE::EMPTY == _tileType || COLLISION_TILE_TYPE::NORMAL == _tileType )
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

void MonsterFSM::CheckMovement( )
{
	if ( false == IsInMyRoom( ) )
	{
		if ( DIRECTION::LEFT == meDir )
		{
			meDir = DIRECTION::RIGHT;
		}
		else if ( DIRECTION::RIGHT == meDir )
		{
			meDir = DIRECTION::LEFT;
		}
	}
}
