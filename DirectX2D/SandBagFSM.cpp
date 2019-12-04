#include <..\DxEngine\ComRender2DSprite.h>
#include <..\psklib\GameTime.h>
#include "SandBagFSM.h"
#include "UIDamage.h"

SandBagFSM::SandBagFSM( )
	: MonsterFSM( ), meState(SandBagFSM::SAND_BAG_FSM::IDLE)
	, mAttackedTime(0.0f)
{
	EMPTY_STATEMENT;
}

SandBagFSM::~SandBagFSM( )
{
	EMPTY_STATEMENT;
}

void SandBagFSM::Init( )
{
	MonsterFSM::Init( );

	mSprite = mActor->GetComponentOrNull<ComRender2DSprite>( );
	mSprite->SetSpriteIdx(0);
}

void SandBagFSM::UpdateCheckPos( )
{
	mCheckPosLeft = mPos + GVector4D::vLeft * sandbag_correct_value::leftCorrectValue;
	mCheckPosRight = mPos + GVector4D::vRight * sandbag_correct_value::rightCorrectValue;
	mCheckPosTop = mPos + GVector4D::vUp * sandbag_correct_value::topCorrectValue;
	mCheckPosDown = mPos + GVector4D::vDown * sandbag_correct_value::downCorrectValue;

	DrawCheckPosForDebuging( );
}

void SandBagFSM::Idle( )
{
	mSprite->SetSpriteIdx(0);
}

void SandBagFSM::AttackedBy( )
{
	mSprite->SetSpriteIdx(1);

	mAttackedTime += GameTime::DeltaTime( );
	if ( 0.175f <= mAttackedTime )
	{
		GSound::Play(L"HIT11.WAV");
		auto damage = mActor->AddComponent<UIDamage>( );
		damage->AddDamage(Well512Random::Instance( ).GetValue(123, 9999));
		mAttackedTime = 0.0f;
	}
}

void SandBagFSM::Update( )
{
	mPos = mActor->GetPos( );

	CheckGround( );

	switch ( meState )
	{
	case SandBagFSM::IDLE:
		Idle( );
		break;
	case SandBagFSM::ATTACKED_BY:
		AttackedBy( );
		break;
	case SandBagFSM::MAX:
	default:
		CRASH_PROG;
		break;
	}

	mActor->SetPos(mPos);
}
