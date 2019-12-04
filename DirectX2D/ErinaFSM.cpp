#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>

#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\GActor.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\Com2DColliderOBB.h>

#include "ErinaFSM.h"

ErinaFSM::ErinaFSM( )
	: mErinaActor(nullptr), mErinaPos(GVector4D( ))
	, meErinaState(_ERINA_STATE::ERINA_MAX)
	, meErinaDirection(_ERINA_DIRECTION::ERINA_DIR_MAX)
	, mListOfAfterImage(std::list<AfterImage>( ))
	, mHammerComboCounter(0)
	, mbAttackFlag(false)
	, mPlayerSprite(nullptr), mCurErinaAni(nullptr)
	, mDefaultJumpPower(500.f), mDefaultGravityAcceleration(1000.0f)
	, mGravityAcceleration(0.0f), mJumpPower(0.0f), mGap(0.0f)
	, mbOnGroundFlag(false)
	, mSpeed(GameOption::instance.GetAspectRatio( ) * 3.0f)
	, mbAirDownJumpFlag(false), mbSlideFlag(false)
	, mPrevPosX(gconst::inf<float>), mCurPosX(gconst::inf<float>)
	, mbSkillFlag(false), mbAirHammerDrillFlag(false)
	, mColTileRenderer(nullptr)
	, mTimeCounter(0.1f), mVecOfSkillColliders( )
{
	EMPTY_STATEMENT;
}

ErinaFSM::~ErinaFSM( )
{
	EMPTY_STATEMENT;
}

void ErinaFSM::Init( )
{
	mErinaActor = &GetActor( );
	meErinaDirection = _ERINA_DIRECTION::ERINA_LEFT;
	meErinaState = _ERINA_STATE::ERINA_IDLE;
	//meErinaAirState = _ERINA_AIR_STATE::ERINA_ON_GROUND;

	mPlayerSprite = mErinaActor->GetComponentOrNull<ComRender2DSprite>( );
	//mPlayerSpriteRenderer->SetLocalPos(GVector4D::vUp * 0.3f);
	mCurErinaAni = mErinaActor->GetComponentOrNull<Com2dAnimation>( );

	mGravityAcceleration = mDefaultGravityAcceleration;
	mJumpPower = 0.0f;

	for ( int i = 0; i < 14; i++ )
	{
		mVecOfSkillColliders.push_back(mErinaActor->AddComponent<Com2DColliderOBB>(COLLIDER_GROUP::COL_PLAYER_ATTCK));
	}

	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_01_L]->SetLocalPos(GVector4D::vLeft * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_01_L]->SetLocalScale({ 0.4f, 0.7f, 1.0f });
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_01_R]->SetLocalPos(GVector4D::vRight * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_01_R]->SetLocalScale({ 0.4f, 0.7f, 1.0f });

	//mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_02_L]->SetLocalPos(GVector4D::vDown * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_02_L]->SetLocalScale({ 1.0f, 0.5f, 1.0f });
	//mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_02_R]->SetLocalPos(GVector4D::vDown * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_02_R]->SetLocalScale({ 1.0f, 0.5f, 1.0f });

	//mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_03_L]->SetLocalPos(GVector4D::vDown * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_03_L]->SetLocalScale({ 0.7f, 0.5f, 1.0f });
	//mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_03_R]->SetLocalPos(GVector4D::vDown * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_03_R]->SetLocalScale({ 0.7f, 0.5f, 1.0f });

	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_04_L]->SetLocalPos(GVector4D::vLeft * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_04_L]->SetLocalScale({ 0.4f, 0.7f, 1.0f });
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_04_R]->SetLocalPos(GVector4D::vRight * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_04_R]->SetLocalScale({ 0.4f, 0.7f, 1.0f });

	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_L]->SetLocalPos(GVector4D::vLeft * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_L]->SetLocalScale({ 1.0f, 0.3f, 1.0f });
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_R]->SetLocalPos(GVector4D::vRight * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_R]->SetLocalScale({ 1.0f, 0.3f, 1.0f });

	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_DOWN_L]->SetLocalPos(GVector4D::vDown * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_DOWN_L]->SetLocalScale({ 0.5f, 0.7f, 1.0f });
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_DOWN_R]->SetLocalPos(GVector4D::vDown * 0.25f);
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_DOWN_R]->SetLocalScale({ 0.5f, 0.7f, 1.0f });

	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_UP_L]->SetLocalPos((GVector4D::vLeft * 0.25f) + (GVector4D::vUp * 0.25f));
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_UP_L]->SetLocalScale({ 0.4f, 0.7f, 1.0f });
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_UP_R]->SetLocalPos((GVector4D::vRight * 0.25f) + (GVector4D::vUp * 0.25f));
	mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_UP_R]->SetLocalScale({ 0.4f, 0.7f, 1.0f });

	for ( auto& col : mVecOfSkillColliders )
	{
		col->OffUpdate( );
	}
}

void ErinaFSM::ChangeState( )
{
	mErinaPos = mErinaActor->GetPos( );
	UpdateCheckPos( );

	CheckDirection( );

	switch ( meErinaState )
	{
	case ErinaFSM::_ERINA_STATE::ERINA_IDLE:
		mCurErinaAni->ChangeAnimation(L"Idle");
		mCurHammerAni->ChangeAnimation(L"Idle");
		Idle( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_WALK:
		mCurErinaAni->ChangeAnimation(L"Walk");
		Walk( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_WALK_AFTER_SLIDE:
		mCurErinaAni->ChangeAnimation(L"WalkAfterSlide");
		WalkAfterSlide( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_JUMP:
		Jump( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_AIR_DOUBLEJUMP:
		mCurErinaAni->ChangeAnimation(L"Bunny_Spin");
		AirDoubleJump( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_AIR_DOWN_JUMP:
		AirDownJump( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_SLIDE:
		Slide( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_WALL_JUMP:
		WallJump( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_CARROT_BOMB:
		CarrotBomb( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_SUPPER_CARROT:
		SupperCarrot( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_BUNNY_AMULET:
		BunnyAmulet( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_HAMMER_ATTCK:
		HammerAttck( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_HAMMER_ATTCK_WHILE_WALK:
		HammerAttckWhileWalk( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_HAMMER_COMMBO_02:
		HammerCommbo2( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_HAMMER_COMMBO_03:
		HammerCommbo3( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_HAMMER_COMMBO_04:
		HammerCommbo4( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_HAMMER_COMMBO_05:
		HammerCommbo5( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_SLIDE_BUNNY_STRIKE:
		SlideHammerStrike( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_HAMMER_DRILL:
		HammerDrill( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_AIR_HAMMER_DRILL:
		AirHammerDrill( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_AIR_AIR_DASH:
		AirAirDash( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_AIR_BUNNY_SPIN:
		AirBunnySpin( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_AIR_HAMMER_ROLL:
		AirHammerRoll( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_USE_ITEM:
		UseItem( );
		break;
	case ErinaFSM::_ERINA_STATE::ERINA_MAX: FALL_THROUGH;
	default:
		CRASH_PROG;
		break;
	}

	if ( nullptr != mColTileRenderer )
	{
		CheckGround( );
	}

	GenerateAfterImage( );
	if ( true == DebugRenderer::IsOnDebugMode( ) )
	{
		DebugRenderer::DrawFontUI(std::to_wstring(mErinaPos.m.x).append(L", ").append(std::to_wstring(mErinaPos.m.y)));
	}

	mErinaActor->SetPos(mErinaPos);
}

void ErinaFSM::Update( )
{
	if ( nullptr != mHammerActor && nullptr == mCurHammerAni)
	{
		mHammerSprite = mHammerActor->GetComponentOrNull<ComRender2DSprite>( );
		mCurHammerAni = mHammerActor->GetComponentOrNull<Com2dAnimation>( );
	}

	ChangeState( );

	DebugRenderer::DrawPoint(ComCamera::mMainCam, mErinaActor->GetTransform( ).GetWorldMatrix( ));
}
