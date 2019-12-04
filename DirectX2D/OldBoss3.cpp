#include <..\psklib\Well512Random.h>

#include "OldBoss3.h"
#include "OldBoss3FSM.h"
#include "UIDamage.h"

OldBoss3::OldBoss3(GScene& _scene, const std::wstring& _imgName)
	: Monster(_scene, _imgName, 48.f), mFSM(nullptr), mAttackedTime(0.0f), mAttackedResetTime(0.35f)
{
	mActor->SetLocalScale({ 75.f, 75.f, 1.0f });

	CreateAnimation( );
	CreateCollider( );
	CreateFSM( );

	mAttackedTime = mAttackedResetTime;
}

OldBoss3::~OldBoss3( )
{
	EMPTY_STATEMENT;
}

void OldBoss3::CreateAnimation( )
{
	mAnimation = mActor->AddComponent<Com2dAnimation>( );

	mAnimation->CreateAnimation(mImgName, L"IDLE", 0, 4, mDefaultAniSpeed, true);
	mAnimation->CreateAnimation(mImgName, L"ATTACKED", 4, 5, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"WALK", 10, 14, mDefaultAniSpeed, true);
	mAnimation->CreateAnimation(mImgName, L"JUMP_UP", 20, 24, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"JUMP_DOWN", 30, 34, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"ATTACK_01", 40, 45, mDefaultAniSpeed, false);
}

void OldBoss3::CreateCollider( )
{
	mCollider = mActor->AddComponent<Com2DColliderOBB>(COLLIDER_GROUP::COL_MONSTER);
	GVector4D colscale = mCollider->GetLocalScale( );
	mCollider->SetLocalScale({ colscale.m.x * 0.4f, colscale.m.y * 0.8f, colscale.m.z });

	mCollider->RegistEnterFunction(&OldBoss3::Enter, this);
	mCollider->RegistStayFunction(&OldBoss3::Stay, this);
	mCollider->RegistExitFunction(&OldBoss3::Exit, this);
}

void OldBoss3::CreateFSM( )
{
	mFSM = mActor->AddComponent<OldBoss3FSM>( );
}

void OldBoss3::Enter(ComCollider* own, ComCollider* other)
{
	if ( 0 > mFSM->mHP )
	{
		mCollider->OffUpdate( );
	}

	mFSM->meState = OldBoss3FSM::OLD_BOSS_FSM::ATTACKED_BY;
}

void OldBoss3::Stay(ComCollider* own, ComCollider* other)
{
	//DebugConsole::PrintlnConsoleW(std::to_wstring(mAttackedTime));
	mAttackedTime -= GameTime::DeltaTime( );
	
	if ( 0.0f >= mAttackedTime )
	{
		auto uiDamage = mActor->AddComponent<UIDamage>( );
		uiDamage->AddDamage(Well512Random::Instance( ).GetValue(10, 2000));

		mAttackedTime = mAttackedResetTime; // todo 공격 종류에 따라 달라져야 함

		if ( 0 > mFSM->mHP )
		{
			mCollider->OffUpdate( );
		}

		mFSM->meState = OldBoss3FSM::OLD_BOSS_FSM::ATTACKED_BY;
		mFSM->mResetTime = 0.1f;
		mFSM->mJumpPower = mFSM->mJumpPwrAttackedBy;
	}
}

void OldBoss3::Exit(ComCollider* own, ComCollider* other)
{
	// todo
}
