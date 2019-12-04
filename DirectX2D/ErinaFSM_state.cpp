#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>
#include <..\DxEngine\GScene.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\Com2DColliderOBB.h>
#include <..\DxEngine\Com2DColliderSphere.h>
#include <..\DxEngine\ComRender2DSprite.h>

#include "ErinaFSM.h"

void ErinaFSM::Idle( )
{
	OffAllAttCols( );

	if ( true == FallDown( ) )
	{
		return;
	}

	if ( true == GameKeyInput::Press(L"Left") )
	{
		meErinaState = _ERINA_STATE::ERINA_WALK;
		return;
	}

	if ( true == GameKeyInput::Press(L"Right") )
	{
		meErinaState = _ERINA_STATE::ERINA_WALK;
		return;
	}

	if ( true == GameKeyInput::Press(L"Down") && true == GameKeyInput::Down(L"Attack") )
	{
		mCurErinaAni->ChangeAnimation(L"Carrot_Bomb");
		meErinaState = _ERINA_STATE::ERINA_CARROT_BOMB;
		return;
	}

	if ( true == GameKeyInput::Press(L"Down") && true == GameKeyInput::Down(L"Jump") )
	{
		GSound::Play(L"SLIDE.WAV");
		mCurErinaAni->ChangeAnimation(L"Slide");
		mPrevPosX = mErinaPos.m.x;
		meErinaState = _ERINA_STATE::ERINA_SLIDE;

		return;
	}

	if ( true == CheckHammerDrillUp( ) )
	{
		return;
	}

	if ( true == CheckJump( ) )
	{
		return;
	}

	if ( true == GameKeyInput::Down(L"Attack") )
	{
		mCurErinaAni->ChangeAnimation(L"Hammer_Attack");
		mCurHammerAni->ChangeAnimation(L"Hammer_Attack");
		GSound::Play(L"HAMMER.WAV");

		if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mHammerSprite->SetLocalPos(GVector4D::vRight * 0.25f);
		}
		else if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mHammerSprite->SetLocalPos(GVector4D::vLeft * 0.25f);
		}

		meErinaState = _ERINA_STATE::ERINA_HAMMER_ATTCK;

		if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_01_L]->OnUpdate( );
		}
		else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_01_R]->OnUpdate( );
		}

		++mHammerComboCounter;

		return;
	}

	if ( true == GameKeyInput::Press(L"Space") )
	{
		mCurErinaAni->ChangeAnimation(L"Use_Heal_Item");
		meErinaState = _ERINA_STATE::ERINA_USE_ITEM;

		return;
	}

	_ERINA_STATE::ERINA_BUNNY_AMULET;
}

void ErinaFSM::WalkAfterSlide( )
{
	mGap += GameTime::DeltaTime( );
	if ( mGap >= 0.25f )
	{
		mbSkillFlag = false;
		meErinaState = _ERINA_STATE::ERINA_IDLE;
		mGap = 0.0f;
	}
	TranslateHorizonByRatio(1.0f);
}

void ErinaFSM::Walk( )
{
	if ( true == FallDown( ) )
	{
		return;
	}

	if ( true == GameKeyInput::Press(L"Left") )
	{
		meErinaState = _ERINA_STATE::ERINA_WALK;
		TranslateHorizonByRatio(1.0f);

		if ( true == CheckSlide( ) )
		{
			return;
		}

		if ( true == CheckJump( ) )
		{
			return;
		}

		if ( true == CheckHammerDrillUp( ) )
		{
			return;
		}

		if ( true == GameKeyInput::Down(L"Attack") )
		{
			mCurErinaAni->ChangeAnimation(L"Hammer_Attack");
			mCurHammerAni->ChangeAnimation(L"Hammer_Attack");
			GSound::Play(L"HAMMER.WAV");

			if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
			{
				mHammerSprite->SetLocalPos(GVector4D::vRight * 0.25f);
			}
			else if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
			{
				mHammerSprite->SetLocalPos(GVector4D::vLeft * 0.25f);
			}

			if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
			{
				mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_01_L]->OnUpdate( );
			}

			meErinaState = _ERINA_STATE::ERINA_HAMMER_ATTCK_WHILE_WALK;
			++mHammerComboCounter;
		}

		return;
	}

	if ( true == GameKeyInput::Press(L"Right") )
	{
		meErinaState = _ERINA_STATE::ERINA_WALK;
		TranslateHorizonByRatio(1.0f);

		if ( true == CheckSlide( ) )
		{
			return;
		}

		if ( true == CheckJump( ) )
		{
			return;
		}

		if ( true == CheckHammerDrillUp( ) )
		{
			return;
		}

		if ( true == GameKeyInput::Down(L"Attack") )
		{
			mCurErinaAni->ChangeAnimation(L"Hammer_Attack");
			mCurHammerAni->ChangeAnimation(L"Hammer_Attack");
			GSound::Play(L"HAMMER.WAV");

			if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
			{
				mHammerSprite->SetLocalPos(GVector4D::vRight * 0.25f);
			}
			else if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
			{
				mHammerSprite->SetLocalPos(GVector4D::vLeft * 0.25f);
			}

			if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
			{
				mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_01_R]->OnUpdate( );
			}
			meErinaState = _ERINA_STATE::ERINA_HAMMER_ATTCK_WHILE_WALK;
			++mHammerComboCounter;
		}

		return;
	}

	meErinaState = _ERINA_STATE::ERINA_IDLE;

	_ERINA_STATE::ERINA_SLIDE;
	_ERINA_STATE::ERINA_CARROT_BOMB;
	_ERINA_STATE::ERINA_BUNNY_AMULET;
	_ERINA_STATE::ERINA_USE_ITEM;
	_ERINA_STATE::ERINA_HAMMER_DRILL;
}

void ErinaFSM::Jump( )
{
	mbOnGroundFlag = false;

	if ( true == GameKeyInput::Press(L"Left") || true == GameKeyInput::Press(L"Right") )
	{
		TranslateHorizonByRatio(1.0f);
	}

	if ( true == GameKeyInput::Press(L"Down") && true == GameKeyInput::Press(L"Attack") )
	{
		meErinaState = _ERINA_STATE::ERINA_AIR_HAMMER_DRILL;
		mCurErinaAni->ChangeAnimation(L"Hammer_Drill_Down");
		mCurHammerAni->ChangeAnimation(L"Hammer_Drill_Down");
		mGravityAcceleration = mDefaultGravityAcceleration * 5.0f;
		
		if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_DOWN_L]->OnUpdate( );
		}
		else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_DOWN_R]->OnUpdate( );
		}

		return;
	}

	if ( 0.0f >= mJumpPower )
	{
		mCurErinaAni->ChangeAnimation(L"Jump_Down");
	}

	// _ERINA_STATE::ERINA_AIR_DOWN_JUMP
	if ( true == GameKeyInput::Press(L"Down") && true == GameKeyInput::Down(L"Jump") )
	{
		meErinaState = _ERINA_STATE::ERINA_AIR_DOWN_JUMP;
		mGravityAcceleration = mDefaultGravityAcceleration * 5.0f;
		return;
	}

	if ( true == GameKeyInput::Down(L"Jump") )
	{
		meErinaState = _ERINA_STATE::ERINA_AIR_DOUBLEJUMP;
		GSound::Play(L"JUMP2.WAV");
		mJumpPower = mDefaultJumpPower;
		mGravityAcceleration = mDefaultGravityAcceleration;
		return;
	}

	return;

	_ERINA_STATE::ERINA_AIR_HAMMER_DRILL;
	_ERINA_STATE::ERINA_AIR_AIR_DASH;
	_ERINA_STATE::ERINA_AIR_BUNNY_SPIN;
	_ERINA_STATE::ERINA_AIR_HAMMER_ROLL;
	_ERINA_STATE::ERINA_WALL_JUMP;
	_ERINA_STATE::ERINA_BUNNY_AMULET;
}

void ErinaFSM::AirDoubleJump( )
{
	if ( true == GameKeyInput::Press(L"Left") || true == GameKeyInput::Press(L"Right") )
	{
		TranslateHorizonByRatio(1.0f);
	}

	_ERINA_STATE::ERINA_AIR_BUNNY_SPIN;
	_ERINA_STATE::ERINA_AIR_AIR_DASH;
	_ERINA_STATE::ERINA_AIR_HAMMER_DRILL;
	_ERINA_STATE::ERINA_AIR_HAMMER_ROLL;
}

void ErinaFSM::AirDownJump( )
{
	mbAirDownJumpFlag = true;
	mbSkillFlag = true;
	mPrevPosX = mErinaPos.m.x;

	if ( 0.0f >= mJumpPower )
	{
		mCurErinaAni->ChangeAnimation(L"Jump_Down");
	}
}

void ErinaFSM::Slide( )
{
	mbSlideFlag = true;
	mbSkillFlag = true;
	_ERINA_STATE::ERINA_SLIDE_BUNNY_STRIKE;
}

void ErinaFSM::WallJump( )
{
	_ERINA_STATE::ERINA_WALL_JUMP; // todo 최대 횟수만큼만 동작하도록

	_ERINA_STATE::ERINA_AIR_DOUBLEJUMP;
	_ERINA_STATE::ERINA_AIR_HAMMER_DRILL;
	_ERINA_STATE::ERINA_AIR_AIR_DASH;
	_ERINA_STATE::ERINA_AIR_BUNNY_SPIN;
	_ERINA_STATE::ERINA_AIR_HAMMER_ROLL;
	_ERINA_STATE::ERINA_WALL_JUMP;
	_ERINA_STATE::ERINA_BUNNY_AMULET;
	_ERINA_STATE::ERINA_IDLE;
}

void ErinaFSM::CarrotBomb( )
{
	if ( true == mCurErinaAni->IsAnimationEnded( ) )
	{
		meErinaState = _ERINA_STATE::ERINA_IDLE;
		return;
	}

	_ERINA_STATE::ERINA_SUPPER_CARROT;
}

void ErinaFSM::SupperCarrot( )
{
	// todo
}

void ErinaFSM::BunnyAmulet( )
{
	// todo
}

void ErinaFSM::HammerAttck( )
{
	if ( true == GameKeyInput::Down(L"Attack") )
	{
		mCurErinaAni->ChangeAnimation(L"Hammer_Commbo_2");
		mCurHammerAni->ChangeAnimation(L"Hammer_Commbo_2");
		GSound::Play(L"HAMMER.WAV");
		mHammerSprite->SetLocalPos(0.0f);

		meErinaState = _ERINA_STATE::ERINA_HAMMER_COMMBO_02;

		OffAllAttCols( );

		if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_02_L]->OnUpdate( );
		}
		else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_02_R]->OnUpdate( );
		}

		++mHammerComboCounter;

		return;
	}

	EndAttackState( );
}

void ErinaFSM::HammerAttckWhileWalk( )
{
	TranslateHorizonByRatio(1.5f);

	if ( true == GameKeyInput::Down(L"Attack") )
	{
		mCurErinaAni->ChangeAnimation(L"Hammer_Commbo_2");
		mCurHammerAni->ChangeAnimation(L"Hammer_Commbo_2");
		GSound::Play(L"HAMMER.WAV");

		meErinaState = _ERINA_STATE::ERINA_HAMMER_COMMBO_02;

		OffAllAttCols( );

		if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_02_L]->OnUpdate( );
		}
		else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_02_R]->OnUpdate( );
		}

		++mHammerComboCounter;

		return;
	}

	EndAttackState( );
}

void ErinaFSM::HammerCommbo2( )
{
	if ( true == GameKeyInput::Down(L"Attack") )
	{
		mCurErinaAni->ChangeAnimation(L"Hammer_Commbo_3");
		mCurHammerAni->ChangeAnimation(L"Hammer_Commbo_3");
		GSound::Play(L"HAMMER.WAV");

		meErinaState = _ERINA_STATE::ERINA_HAMMER_COMMBO_03;

		OffAllAttCols( );
		if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_03_L]->OnUpdate( );
		}
		else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_03_R]->OnUpdate( );
		}

		++mHammerComboCounter;

		return;
	}

	EndAttackState( );
}

void ErinaFSM::HammerCommbo3( )
{
	if ( true == GameKeyInput::Down(L"Attack") )
	{
		mCurErinaAni->ChangeAnimation(L"Hammer_Commbo_4");
		mCurHammerAni->ChangeAnimation(L"Hammer_Commbo_4");
		GSound::Play(L"HAMMER.WAV");

		if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mHammerSprite->SetLocalPos(GVector4D::vRight * 0.25f);
		}
		else if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mHammerSprite->SetLocalPos(GVector4D::vLeft * 0.25f);
		}

		meErinaState = _ERINA_STATE::ERINA_HAMMER_COMMBO_04;

		OffAllAttCols( );
		if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_04_L]->OnUpdate( );
		}
		else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_04_R]->OnUpdate( );
		}

		++mHammerComboCounter;

		return;
	}

	EndAttackState( );
}

void ErinaFSM::HammerCommbo4( )
{
	if ( true == GameKeyInput::Down(L"Attack") )
	{
		mCurErinaAni->ChangeAnimation(L"Hammer_Commbo_5");
		mCurHammerAni->ChangeAnimation(L"Hammer_Commbo_5");
		GSound::Play(L"HAMMER.WAV");
		mHammerSprite->SetLocalPos(0.0f);

		meErinaState = _ERINA_STATE::ERINA_HAMMER_COMMBO_05;

		OffAllAttCols( );

		if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_L]->OnUpdate( );
		}
		else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_R]->OnUpdate( );
		}

		++mHammerComboCounter;

		return;
	}

	EndAttackState( );
}

void ErinaFSM::HammerCommbo5( )
{
	if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
	{
		mHammerSpeed += 10.0f * GameTime::DeltaTime( );

		mHammerSprite->SetLocalPos(GVector4D::vLeft * mHammerSpeed * GameTime::DeltaTime( ));
		mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_L]->SetLocalPos(GVector4D::vLeft * mHammerSpeed * GameTime::DeltaTime( ));
		mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_L]->OnUpdate( );
	}
	else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
	{
		mHammerSpeed += 10.0f * GameTime::DeltaTime( );
		
		mHammerSprite->SetLocalPos(GVector4D::vRight * mHammerSpeed * GameTime::DeltaTime( ));
		mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_R]->SetLocalPos(GVector4D::vRight * mHammerSpeed * GameTime::DeltaTime( ));
		mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_R]->OnUpdate( );
	}

	if ( true == mCurErinaAni->IsAnimationEnded( ) && true == GameKeyInput::Press(L"Attack") )
	{
		mCurErinaAni->PlaySameAnimation( );
		GSound::Play(L"BLOCK.WAV");

		if ( true == mCurHammerAni->IsAnimationEnded( ) )
		{
			mCurHammerAni->ChangeAnimation(L"Hammer_Commbo_5_Continue");
		}

		return;
	}

	EndAttackState( );
}

void ErinaFSM::SlideHammerStrike( )
{
	// todo
}

void ErinaFSM::HammerDrill( )
{
	mbOnGroundFlag = false;
	mbSkillFlag = true;

	TranslateHorizonByRatio(0.3f);

	if ( 0.0f >= mJumpPower )
	{
		mCurErinaAni->ChangeAnimation(L"Jump_Down");
		mbSkillFlag = false;
	}
}

void ErinaFSM::AirHammerDrill( )
{
	mbSkillFlag = true;
	mbAirHammerDrillFlag = true;
	// todo
}

void ErinaFSM::AirAirDash( )
{
	mbSkillFlag = true;
	// todo
}

void ErinaFSM::AirBunnySpin( )
{
	// todo
}

void ErinaFSM::AirHammerRoll( )
{
	// todo
}

void ErinaFSM::UseItem( )
{
	if ( true == mCurErinaAni->IsAnimationEnded( ) )
	{
		meErinaState = _ERINA_STATE::ERINA_IDLE;
		return;
	}
}
