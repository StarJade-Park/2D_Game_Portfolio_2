#pragma once
#include <list>

#include <..\DxEngine\ComCustom.h>
#include <..\DxEngine\DebugRenderer.h>

#include <..\CommonLibrary\GameOption.h>
#include <..\CommonLibrary\GTileRenderer.h>
#include <..\DxEngine\GSound.h>

class ComRender2DSprite;
class Com2dAnimation;
class GActor;
class Com2DColliderOBB;

enum COLLISION_TILE_TYPE : UINT;

class ErinaFSM
	: public ComCustom
{
private:
	enum class _ERINA_STATE : int
	{
		ERINA_IDLE, ERINA_WALK, ERINA_WALK_AFTER_SLIDE, ERINA_JUMP, ERINA_AIR_DOUBLEJUMP, ERINA_SLIDE, ERINA_WALL_JUMP, ERINA_AIR_DOWN_JUMP,
		ERINA_CARROT_BOMB, ERINA_SUPPER_CARROT, ERINA_BUNNY_AMULET,
		ERINA_HAMMER_ATTCK, ERINA_HAMMER_ATTCK_WHILE_WALK,
		ERINA_HAMMER_COMMBO_02, ERINA_HAMMER_COMMBO_03, ERINA_HAMMER_COMMBO_04, ERINA_HAMMER_COMMBO_05,
		ERINA_SLIDE_BUNNY_STRIKE, ERINA_HAMMER_DRILL,
		ERINA_AIR_HAMMER_DRILL, ERINA_AIR_AIR_DASH, ERINA_AIR_BUNNY_SPIN, ERINA_AIR_HAMMER_ROLL,
		ERINA_USE_ITEM, ERINA_MAX
	};

	enum class _ERINA_DIRECTION : int
	{
		ERINA_LEFT, ERINA_RIGHT, ERINA_UP, ERINA_DOWN, ERINA_DIR_MAX,
	};

	//enum class _ERINA_AIR_STATE: int
	//{
	//	ERINA_IN_AIR, ERINA_ON_GROUND, ERINA_AIR_MAX,
	//};

	enum _ERINA_ATTACK_ : int
	{
		ATT_HAMMER_01_L, ATT_HAMMER_01_R,
		ATT_HAMMER_02_L, ATT_HAMMER_02_R,
		ATT_HAMMER_03_L, ATT_HAMMER_03_R,
		ATT_HAMMER_04_L, ATT_HAMMER_04_R,
		ATT_HAMMER_05_L, ATT_HAMMER_05_R,
		ATT_HAMMER_UP_L, ATT_HAMMER_UP_R,
		ATT_HAMMER_DOWN_L, ATT_HAMMER_DOWN_R,
		ATT_MAX
	};

public:
	struct AfterImage
	{
		ComRender2DSprite* render;
		GVector4D pos;
		_ERINA_DIRECTION dir;
		float alpha;
	};

public:
	ErinaFSM( );
	~ErinaFSM( );

	void SetColTiles(GTileRenderer* tile)
	{
		if ( nullptr == tile )
		{
			CRASH_PROG;
			return;
		}

		mColTileRenderer = tile;
	}

	void SetHammerActor(GActor* _actor)
	{
		mHammerActor = _actor;
	}

private:
	void Init( ) override;
	void Update( ) override;

	void ChangeState( );
	void Idle( );
	void Walk( );
	void Jump( );
	void AirDoubleJump( );
	void AirDownJump( );
	void Slide( );
	void WallJump( );
	void CarrotBomb( );
	void SupperCarrot( );
	void BunnyAmulet( );
	void HammerAttck( );
	void HammerAttckWhileWalk( );
	void HammerCommbo2( );
	void HammerCommbo3( );
	void HammerCommbo4( );
	void HammerCommbo5( );
	void SlideHammerStrike( );
	void HammerDrill( );
	void AirHammerDrill( );
	void AirAirDash( );
	void AirBunnySpin( );
	void AirHammerRoll( );
	void UseItem( );

	void CheckDirection( );
	void CheckGround( );
	void UpdateCheckPos( );
	void InitializeProperty( );

	bool CheckJump( );

	bool CheckSlide( );
	void WalkAfterSlide( );
	
	bool FallDown( );

	void EndAttackState( );

	bool CheckHammerDrillUp( );

	void TranslateHorizonByRatio(float _ratio);
	
	void GenerateAfterImage( );

	void CheckBlock( );
	void ProcessBlockInvasion(const _ERINA_DIRECTION dir, const GTileRenderer::TileData* _tile);
	float CollideInclineTile(const COLLISION_TILE_TYPE _tileType);

	void OffAllAttCols( );

private:
	GActor* mErinaActor;
	GActor* mHammerActor;

	std::vector<Com2DColliderOBB*> mVecOfSkillColliders;

	ComRender2DSprite* mPlayerSprite;
	ComRender2DSprite* mHammerSprite;

	Com2dAnimation* mCurErinaAni;
	Com2dAnimation* mCurHammerAni;


	GVector4D mErinaPos;
	GVector4D mCheckPosLeft;
	GVector4D mCheckPosRight;
	GVector4D mCheckPosTop;
	GVector4D mCheckPosDown;

	float mSpeed;

	_ERINA_STATE meErinaState;
	_ERINA_DIRECTION meErinaDirection;
	//_ERINA_AIR_STATE meErinaAirState;

	UINT mHammerComboCounter;
	bool mbAttackFlag;

	std::list<AfterImage> mListOfAfterImage;

	const float mDefaultJumpPower;
	const float mDefaultGravityAcceleration;
	float mGravityAcceleration;
	float mJumpPower;
	bool mbOnGroundFlag;

	bool mbSkillFlag;
	bool mbAirHammerDrillFlag;

	bool mbSlideFlag;
	bool mbAirDownJumpFlag;

	float mPrevPosX;
	float mCurPosX;

	float mTimeCounter;
	float mGap;

	float mHammerSpeed;

	GTileRenderer* mColTileRenderer;
};

