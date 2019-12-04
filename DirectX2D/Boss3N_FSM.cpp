#include <..\psklib\Well512Random.h>

#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\Com2dAnimation.h>

#include <..\CommonLibrary\GameOption.h>

#include "Boss3N_FSM.h"
#include "RbrbRoom.h"
#include "BaseBullet.h"
#include "UIDamage.h"

Boss3NFSM::Boss3NFSM( )
	: MonsterFSM( ), meState(Boss3NFSM::BOSS_3N_FSM::MAX)
	, mResetTime(0.5f), mDefualtResetTime(1.0f), mDeathTime(1.0f)
	, mbJumpFlag(false)
	, mSpeed(GameOption::instance.GetWidthAspectRatio( ) * 10.0f)
	, mFlySpeed(GameOption::instance.GetWidthAspectRatio( ) * 20.0f)
	, mbDeathFlag(false), mJumpPwrAttackedBy(100.f)
	, mbDisableGravity(false)
	, mFlyJumpTime(0.0f)
	, mFlyLimitTime(5.0f), mFlyTime(0.0f)
	, mAttackedTime(0.0f)
{
	EMPTY_STATEMENT;
}

Boss3NFSM::~Boss3NFSM( )
{
	//for ( auto& bullet : mListOfBullet )
	//{
	//	if ( nullptr != bullet )
	//	{
	//		delete bullet;
	//		bullet = nullptr;
	//	}
	//}

	//mListOfBullet.clear( );
}

void Boss3NFSM::Init( )
{
	MonsterFSM::Init( );
	mHP = 100000;

	mSpriteRender = mActor->GetComponentOrNull<ComRender2DSprite>( );
	mAnimation = mActor->GetComponentOrNull<Com2dAnimation>( );
	mAnimation->ChangeAnimation(L"IDLE");

	meState = Boss3NFSM::BOSS_3N_FSM::IDLE;
}

void Boss3NFSM::UpdateCheckPos( )
{
	mCheckPosLeft = mPos + GVector4D::vLeft * boss3nfsm_correct_value::leftCorrectValue;
	mCheckPosRight = mPos + GVector4D::vRight * boss3nfsm_correct_value::rightCorrectValue;
	mCheckPosTop = mPos + GVector4D::vUp * boss3nfsm_correct_value::topCorrectValue;
	mCheckPosDown = mPos + GVector4D::vDown * boss3nfsm_correct_value::downCorrectValue;

	DrawCheckPosForDebuging( );
}

void Boss3NFSM::Idle( )
{
	mAnimation->ChangeAnimation(L"IDLE");
	mResetTime -= GameTime::DeltaTime( );

	if ( 0 >= mResetTime )
	{
		mResetTime = mDefualtResetTime;
		meDir = (DIRECTION)Well512Random::Instance( ).GetValue(DIRECTION::LEFT, DIRECTION::RIGHT + 1);
		meState = (BOSS_3N_FSM)Well512Random::Instance( ).GetValue(BOSS_3N_FSM::IDLE, BOSS_3N_FSM::DIE);
		//meState = BOSS_3N_FSM::ATTACK_01;
	}
}


void Boss3NFSM::Walk( )
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
		meState = Boss3NFSM::BOSS_3N_FSM::IDLE;
		mResetTime = mDefualtResetTime;
	}
}

void Boss3NFSM::AttackFly( )
{
	if ( false == mbJumpFlag )
	{
		mAnimation->ChangeAnimation(L"ATTACK_FLY");

		if ( false == mbDisableGravity )
		{
			mFlyJumpTime = Well512Random::Instance( ).GetFloatValue(0.1f, 0.2f);
		}

		mJumpPower = mDefaultJumpPower;
		mbJumpFlag = true;
		mbOnGroundFlag = false;
		mbDisableGravity = true;

		return;
	}
	else
	{
		if ( DIRECTION::LEFT == meDir )
		{
			mPos.m.x -= mFlySpeed * GameTime::DeltaTime( );
			mFlyTime += GameTime::DeltaTime( );
		}
		else if ( DIRECTION::RIGHT == meDir )
		{
			mPos.m.x += mFlySpeed * GameTime::DeltaTime( );
			mFlyTime += GameTime::DeltaTime( );
		}

		if ( mFlyLimitTime <= mFlyTime )
		{
			if ( true == mbDisableGravity )
			{
				mJumpPower = 0.0f;
			}

			mbDisableGravity = false;
			mFlyTime = 0.0f;
		}
	}

	if ( 0.0f >= mJumpPower )
	{
		mAnimation->ChangeAnimation(L"JUMP_DOWN");
	}

	if (true == mbOnGroundFlag )
	{
		meState = Boss3NFSM::BOSS_3N_FSM::IDLE;
		mResetTime = mDefualtResetTime;
	}
}

void Boss3NFSM::Attack01( )
{
	mAnimation->ChangeAnimation(L"ATTACK_01");

	if ( true == mAnimation->IsAnimationEnded( ) )
	{
		//BaseBullet* newBullet = new BaseBullet(GetScene( ), L"MAHO2_A.PNG", 140.f);
		//newBullet->mActor->SetPos({ mPos.m.x, mPos.m.y, mPos.m.z - 10.f, mPos.m.w});
		//mListOfBullet.push_back(newBullet);
	}

	if ( true == mAnimation->IsAnimationEnded( ) )
	{
		meState = Boss3NFSM::BOSS_3N_FSM::IDLE;
		mResetTime = mDefualtResetTime;
	}
}

void Boss3NFSM::Attack02( )
{
	mAnimation->ChangeAnimation(L"ATTACK_02");

	if ( true == mAnimation->IsAnimationEnded( ) )
	{
		// todo ÃÑ¾Ë »ý¼º
	}

	if ( 0.0f >= mResetTime )
	{
		meState = Boss3NFSM::BOSS_3N_FSM::IDLE;
		mResetTime = mDefualtResetTime;
	}
}

void Boss3NFSM::AttackedBy( )
{
	mAttackedTime += GameTime::DeltaTime( );
	if ( 0.15f <= mAttackedTime )
	{
		if ( 0 >= mHP )
		{
			mbDeathFlag = true;
			meState = Boss3NFSM::BOSS_3N_FSM::DIE;
			mResetTime = mDeathTime;
			mJumpPower = 100.f;
			mAttackedTime = 0.0f;
			return;
		}

		GSound::Play(L"HIT11.WAV");
		mAnimation->ChangeAnimation(L"ATTACKED");

		auto damage = mActor->AddComponent<UIDamage>( );
		UINT attack = Well512Random::Instance( ).GetValue(100, 5001);
		damage->AddDamage(attack);
		mHP -= attack;

		meState = Boss3NFSM::BOSS_3N_FSM::IDLE;
		mResetTime = mDefualtResetTime;
		mJumpPower = 0.0f;
		mAttackedTime = 0.0f;
		mbDisableGravity = false;
		mbJumpFlag = false;
	}
}

void Boss3NFSM::Jump( )
{
	if ( false == mbJumpFlag )
	{
		mAnimation->ChangeAnimation(L"JUMP_UP");
		mJumpPower = mDefaultJumpPower;
		mbJumpFlag = true;
		mbOnGroundFlag = false;
	}

	if ( 0.0f >= mJumpPower )
	{
		mAnimation->ChangeAnimation(L"JUMP_DOWN");
	}
}

void Boss3NFSM::Die( )
{
	mSpriteRender->SetSpriteIdx(35);
	//mAnimation->ChangeAnimation(L"ATTACKED");

	mResetTime -= GameTime::DeltaTime( );
	if ( 0.0f >= mResetTime )
	{
		mActor->Death( );
		mResetTime = mDefualtResetTime;
	}
}

void Boss3NFSM::Update( )
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
		if ( false == mbDisableGravity )
		{
			CheckGround( );
		}
		else
		{
			mFlyJumpTime -= GameTime::DeltaTime( );
			
			if ( 0.0f <= mFlyJumpTime )
			{
				mPos.m.y += mJumpPower * GameTime::DeltaTime( );
			}

		}

		if ( true == mbOnGroundFlag )
		{
			if ( true == mbJumpFlag )
			{
				meState = Boss3NFSM::BOSS_3N_FSM::IDLE;
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

	switch ( meState )
	{
	case Boss3NFSM::IDLE:
		Idle( );
		break;
	case Boss3NFSM::WALK:
		Walk( );
		break;
	case Boss3NFSM::ATTACK_01:
	case Boss3NFSM::ATTACK_02:
	case Boss3NFSM::ATTCK_FLY:
		AttackFly( );
		break;
	case Boss3NFSM::JUMP:
		Jump( );
		break;
	case Boss3NFSM::DIE:
		Die( );
		break;
	case Boss3NFSM::ATTACKED_BY:
		AttackedBy( );
		break;
	case Boss3NFSM::MAX:
	default:
		CRASH_PROG;
		break;
	}
	
	mActor->SetPos(mPos);
}
