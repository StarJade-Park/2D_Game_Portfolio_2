#pragma once
#include <..\psklib\GameTime.h>
#include <..\DxEngine\GActor.h>
#include <..\DxEngine\ComCustom.h>
#include <..\DxEngine\GSound.h>
#include <..\CommonLibrary\GTileRenderer.h>

namespace monster_correct_value
{
constexpr float sideCorrectValue = 12.5f;
constexpr float topCorrectValue = 25.0f;
constexpr float downCorrectValue = 30.0f;

constexpr float correctionValue = gconst::ftol + 20.0f;
}

// enum FSM 정의와 update override로 사용할 것
class MonsterFSM
	: public ComCustom
{
	friend class Monster;
protected:
	enum DIRECTION : int
	{
		LEFT, RIGHT, UP, DOWN, DIRECTION_MAX,
	};

private:
	enum ORIGIN_FSM : int
	{
		IDLE, ATTACK, ATTACKED_BY, JUMP, DIE, ORIGIN_FSM_MAX
	};

public:
	MonsterFSM( );
	virtual ~MonsterFSM( );

	void SetColTiles(GTileRenderer* tile)
	{
		if ( nullptr == tile )
		{
			CRASH_PROG;
			return;
		}

		mColTileRenderer = tile;
	}

	void SetRoomIndex(const UINT _x, const UINT _y)
	{
		mRoomIndexX = _x;
		mRoomIndexY = _y;
	}

protected:
	virtual void CheckDirection( );
	virtual void UpdateCheckPos( ) = 0;
	void DrawCheckPosForDebuging( );

	virtual void Idle( );
	virtual void Walk( );
	virtual void Attack( );
	virtual void AttackedBy( );
	virtual void Jump( );
	virtual void Die( );

	void Update( ) override;
	
	bool IsInMyRoom( );
	void CheckGround( );
	void InitializeProperty( );

	void CheckBlock( );
	void ProcessBlockInvasion(const DIRECTION dir, const GTileRenderer::TileData* _tile);
	float CollideInclineTile(const COLLISION_TILE_TYPE _tileType);

	void CheckMovement( );

protected:
	void Init( ) override;

protected:
	bool mbOnGroundFlag;
	DIRECTION meDir;

	GActor* mActor;
	GTileRenderer* mColTileRenderer;

	float mGravityAcceleration;
	float mJumpPower;

	int mHP;
	int mAttackPower;

	UINT mRoomIndexX;
	UINT mRoomIndexY;

	GVector4D mPos;
	GVector4D mCheckPosLeft;
	GVector4D mCheckPosRight;
	GVector4D mCheckPosTop;
	GVector4D mCheckPosDown;

	const float mDefaultJumpPower;
	const float mDefaultGravityAcceleration;

private:
	ORIGIN_FSM meState;
};

