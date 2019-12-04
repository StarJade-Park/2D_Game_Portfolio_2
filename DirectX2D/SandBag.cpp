#include "SandBag.h"
#include "SandBagFSM.h"

SandBag::SandBag(GScene& _scene, const std::wstring& _imgName)
	: Monster(_scene, _imgName, 64.f), mFSM(nullptr)
{
	CreateAnimation( );
	CreateCollider( );
	CreateFSM( );
}

SandBag::~SandBag( )
{
	EMPTY_STATEMENT;
}

void SandBag::CreateAnimation( )
{
	EMPTY_STATEMENT;
}

void SandBag::CreateCollider( )
{
	Com2DColliderOBB* col = mActor->AddComponent<Com2DColliderOBB>(COLLIDER_GROUP::COL_MONSTER);
	col->SetLocalPos(GVector4D::vRight * 0.03f);
	GVector4D colscale = col->GetLocalScale( );
	col->SetLocalScale({ colscale.m.x * 0.3f, colscale.m.y * 0.5f, colscale.m.z });

	col->RegistEnterFunction(&SandBag::Enter, this);
	col->RegistStayFunction(&SandBag::Stay, this);
	col->RegistExitFunction(&SandBag::Exit, this);
}

void SandBag::CreateFSM( )
{
	mFSM = mActor->AddComponent<SandBagFSM>( );
}

void SandBag::Enter(ComCollider* own, ComCollider* other)
{
	mFSM->meState = SandBagFSM::SAND_BAG_FSM::ATTACKED_BY;
}

void SandBag::Stay(ComCollider* own, ComCollider* other)
{
	mFSM->meState = SandBagFSM::SAND_BAG_FSM::ATTACKED_BY;
}

void SandBag::Exit(ComCollider* own, ComCollider* other)
{
	mFSM->meState = SandBagFSM::SAND_BAG_FSM::IDLE;
}
