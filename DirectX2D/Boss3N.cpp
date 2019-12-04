#include "Boss3N.h"
#include "Boss3N_FSM.h"

Boss3N::Boss3N(GScene& _scene, const std::wstring& _imgName)
	: Monster(_scene, _imgName, 64.f), mFSM(nullptr), mCollider(nullptr)
	, mAttackedTime(0.0f), mAttackedResetTime(0.35f)
{
	mActor->SetLocalScale({ 100.f, 100.f, 1.0f });

	CreateAnimation( );
	CreateCollider( );
	CreateFSM( );

	mAttackedTime = mAttackedResetTime;
}

Boss3N::~Boss3N( )
{
	EMPTY_STATEMENT;
}

void Boss3N::CreateAnimation( )
{
	mAnimation = mActor->AddComponent<Com2dAnimation>( );

	mAnimation->CreateAnimation(mImgName, L"IDLE", 0, 3, mDefaultAniSpeed, true);
	mAnimation->CreateAnimation(mImgName, L"WALK", 6, 10, mDefaultAniSpeed, true);
	mAnimation->CreateAnimation(mImgName, L"JUMP_UP", 12, 15, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"JUMP_DOWN", 24, 27, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"ATTACK_FLY", 30, 33, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"FLY", 30, 33, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"ATTACKED", 34, 35, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"ATTACK_01", 36, 40, mDefaultAniSpeed, false);
	mAnimation->CreateAnimation(mImgName, L"ATTACK_02", 42, 46, mDefaultAniSpeed, false);
}

void Boss3N::CreateCollider( )
{
	mCollider = mActor->AddComponent<Com2DColliderOBB>(COLLIDER_GROUP::COL_MONSTER);
	GVector4D colscale = mCollider->GetLocalScale( );
	mCollider->SetLocalScale({ colscale.m.x * 0.4f, colscale.m.y * 0.8f, colscale.m.z });

	mCollider->RegistEnterFunction(&Boss3N::Enter, this);
	mCollider->RegistStayFunction(&Boss3N::Stay, this);
	mCollider->RegistExitFunction(&Boss3N::Exit, this);
}

void Boss3N::CreateFSM( )
{
	mFSM = mActor->AddComponent<Boss3NFSM>( );
}

void Boss3N::Enter(ComCollider* own, ComCollider* other)
{
	mFSM->meState = Boss3NFSM::BOSS_3N_FSM::ATTACKED_BY;
}

void Boss3N::Stay(ComCollider* own, ComCollider* other)
{
	mFSM->meState = Boss3NFSM::BOSS_3N_FSM::ATTACKED_BY;
}

void Boss3N::Exit(ComCollider* own, ComCollider* other)
{
	mFSM->meState = Boss3NFSM::BOSS_3N_FSM::IDLE;
}
