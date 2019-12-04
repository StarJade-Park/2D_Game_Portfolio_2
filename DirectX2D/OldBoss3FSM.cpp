#include <..\psklib\Well512Random.h>

#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\Com2dAnimation.h>

#include <..\CommonLibrary\GameOption.h>

#include "OldBoss3FSM.h"

#include "RbrbRoom.h"
#include "UIDamage.h"

OldBoss3FSM::OldBoss3FSM( )
	: MonsterFSM( ), meState(OldBoss3FSM::OLD_BOSS_FSM::MAX)
	, mResetTime(0.5f), mDefualtResetTime(1.0f), mDeathTime(1.0f)
	, mbJumpFlag(false), mSpeed(GameOption::instance.GetWidthAspectRatio( ) * 10.0f)
	, mbDeathFlag(false), mJumpPwrAttackedBy(100.f), mAttackedTime(0.0f)
{
	EMPTY_STATEMENT;
}

OldBoss3FSM::~OldBoss3FSM( )
{
	EMPTY_STATEMENT;
}

void OldBoss3FSM::Init( )
{
	MonsterFSM::Init( );
	mHP = 25000;

	mSpriteRender = mActor->GetComponentOrNull<ComRender2DSprite>( );
	mAnimation = mActor->GetComponentOrNull<Com2dAnimation>( );
	mAnimation->ChangeAnimation(L"IDLE");

	meState = OldBoss3FSM::OLD_BOSS_FSM::IDLE;
}

void OldBoss3FSM::UpdateCheckPos( )
{
	mCheckPosLeft = mPos + GVector4D::vLeft * oldboss3fsm_correct_value::leftCorrectValue;
	mCheckPosRight = mPos + GVector4D::vRight * oldboss3fsm_correct_value::rightCorrectValue;
	mCheckPosTop = mPos + GVector4D::vUp * oldboss3fsm_correct_value::topCorrectValue;
	mCheckPosDown = mPos + GVector4D::vDown * oldboss3fsm_correct_value::downCorrectValue;

	DrawCheckPosForDebuging( );
}

void OldBoss3FSM::Idle( )
{
	mAnimation->ChangeAnimation(L"IDLE");
	mResetTime -= GameTime::DeltaTime( );

	if ( 0 >= mResetTime )
	{
		mResetTime = mDefualtResetTime;
		meDir = (DIRECTION)Well512Random::Instance( ).GetValue(DIRECTION::LEFT, DIRECTION::RIGHT + 1);
		meState = (OLD_BOSS_FSM)Well512Random::Instance( ).GetValue(OLD_BOSS_FSM::IDLE, OLD_BOSS_FSM::DIE);
		//meState = OLD_BOSS_FSM::WALK;
	}
}


void OldBoss3FSM::Walk( )
{
	mAnimation->ChangeAnimation(L"WALK");

	if ( DIRECTION::LEFT == meDir )
	{
		mPos.m.x -= mSpeed * GameTime::DeltaTime( );
	}
	else if ( DIRECTION::RIGHT == meDir )
	{
		mPos.m.x += mSpeed * GameTime::DeltaTime( );
	}

	mResetTime -= GameTime::DeltaTime( );
	if ( 0.0f >= mResetTime )
	{
		meState = OldBoss3FSM::OLD_BOSS_FSM::IDLE;
		mResetTime = mDefualtResetTime;
	}
}

void OldBoss3FSM::Attack( )
{
	mAnimation->ChangeAnimation(L"ATTACK_01");

	// todo 총알 생성하기

	if ( true == mAnimation->IsAnimationEnded( ) )
	{
		meState = OldBoss3FSM::OLD_BOSS_FSM::IDLE;
		mResetTime = mDefualtResetTime;
	}
}

void OldBoss3FSM::AttackedBy( )
{
	mAnimation->ChangeAnimation(L"ATTACKED");

	mAttackedTime += GameTime::DeltaTime( );
	if ( 0.15f >= mAttackedTime )
	{
		GSound::Play(L"HIT11.WAV");
		auto damage = mActor->AddComponent<UIDamage>( );
		UINT attack = Well512Random::Instance( ).GetValue(100, 9999);
		damage->AddDamage(attack);
		mHP -= attack;

		mJumpPower = mJumpPwrAttackedBy;

		if ( 0 >= mHP )
		{
			mbDeathFlag = true;
			meState = OldBoss3FSM::OLD_BOSS_FSM::DIE;
			mJumpPower = 200.0f;
			mResetTime = mDeathTime;
			mAttackedTime = 0.0f;
			return;
		}

		meState = OldBoss3FSM::OLD_BOSS_FSM::IDLE;
		mResetTime = mDefualtResetTime;
		mAttackedTime = 0.0f;
	}
}

void OldBoss3FSM::Jump( )
{
	if ( false == mbJumpFlag )
	{
		mAnimation->ChangeAnimation(L"JUMP_UP");
		mJumpPower = mDefaultJumpPower;
		mbJumpFlag = true;
		mbOnGroundFlag = false;
	}

	if ( DIRECTION::LEFT == meDir )
	{
		mPos.m.x -= mSpeed * GameTime::DeltaTime( ) * 0.5f;
	}
	else if ( DIRECTION::RIGHT == meDir )
	{
		mPos.m.x += mSpeed * GameTime::DeltaTime( ) * 0.5f;
	}

	if ( 0.0f >= mJumpPower )
	{
		mAnimation->ChangeAnimation(L"JUMP_DOWN");
	}
}

void OldBoss3FSM::Die( )
{
	mSpriteRender->SetSpriteIndex(5);

	mResetTime -= GameTime::DeltaTime( );
	if ( 0.0f >= mResetTime )
	{
		mActor->Death( );
		mResetTime = mDefualtResetTime;
	}
}

void OldBoss3FSM::Update( )
{
	mPos = mActor->GetPos( );

	CheckMovement( );

	if ( DIRECTION::LEFT == meDir )
	{
		mSpriteRender->CullFront( );
	}
	else if ( DIRECTION::RIGHT == meDir )
	{
		mSpriteRender->CullBack( );
	}

	if ( false == mbDeathFlag )
	{
		CheckGround( );

		if ( true == mbOnGroundFlag )
		{
			if ( true == mbJumpFlag )
			{
				meState = OldBoss3FSM::OLD_BOSS_FSM::IDLE;
				mbJumpFlag = false;
			}
		}
	}
	else
	{
		if ( DIRECTION::LEFT == meDir )
		{
			mPos.m.x += mSpeed * GameTime::DeltaTime( );
		}
		else if ( DIRECTION::RIGHT == meDir )
		{
			mPos.m.x -= mSpeed * GameTime::DeltaTime( );
		}

		mPos.m.y += mJumpPower * GameTime::DeltaTime( );
		mJumpPower -= mGravityAcceleration * GameTime::DeltaTime( );
	}
	
	//if ( true == DebugRenderer::IsOnDebugMode( ) )
	//{
	//	auto room = RbrbRoom::SRoom.GetRoomData(mPos);
	//	DebugRenderer::DrawFontUI(RbrbRoom::SRoom.IsCurRoomByPos(mPos) ? L"Room true" : L"Room false");
	//	std::wstring str = L"Room Index X : ";
	//	DebugRenderer::DrawFontUI(str.append(std::to_wstring(room.indexX)).append(L", Y : ").append(std::to_wstring(room.indexY)));
	//	DebugRenderer::DrawVectorUI(room.centerPos);
	//}

	switch ( meState )
	{
	case OldBoss3FSM::IDLE:
		Idle( );
		break;
	case OldBoss3FSM::WALK:
		Walk( );
		break;
	case OldBoss3FSM::ATTCK:
		Attack( );
		break;
	case OldBoss3FSM::ATTACKED_BY:
		AttackedBy( );
		break;
	case OldBoss3FSM::JUMP:
		Jump( );
		break;
	case OldBoss3FSM::DIE:
		Die( );
		break;
	case OldBoss3FSM::MAX:
	default:
		CRASH_PROG;
		break;
	}

	mActor->SetPos(mPos);
}
