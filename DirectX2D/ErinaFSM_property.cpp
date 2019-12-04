#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>

#include <..\DxEngine\Com2DColliderOBB.h>
#include <..\DxEngine\Com2DColliderSphere.h>
#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\Com2dAnimation.h>
#include <..\DxEngine\GActor.h>
#include <..\DxEngine\GResource.h>

#include <..\CommonLibrary\GTileRenderer.h>

#include "ErinaFSM.h"

bool ErinaFSM::FallDown( )
{
	if ( -100.0f >= mJumpPower )
	{
		meErinaState = _ERINA_STATE::ERINA_JUMP;
		mCurErinaAni->ChangeAnimation(L"Jump_Down");
		return true;
	}

	return false;
}

void ErinaFSM::CheckDirection( )
{
	if ( true == GameKeyInput::Press(L"Right") )
	{
		meErinaDirection = _ERINA_DIRECTION::ERINA_RIGHT;
		mPlayerSprite->CullBack( );
		mHammerSprite->CullBack( );
	}
	else if ( true == GameKeyInput::Press(L"Left") )
	{
		meErinaDirection = _ERINA_DIRECTION::ERINA_LEFT;
		mPlayerSprite->CullFront( );
		mHammerSprite->CullFront( );
	}
}

bool ErinaFSM::CheckJump( )
{
	if ( true == GameKeyInput::Down(L"Jump") )
	{
		GSound::Play(L"JUMP0.WAV");
		mCurErinaAni->ChangeAnimation(L"Jump_Up");
		meErinaState = _ERINA_STATE::ERINA_JUMP;
		mJumpPower = mDefaultJumpPower;

		return true;
	}

	return false;
}

void ErinaFSM::EndAttackState( )
{
	if ( true == mCurErinaAni->IsAnimationEnded( ) )
	{
		mHammerComboCounter = 0;
		meErinaState = _ERINA_STATE::ERINA_IDLE;

		mHammerSpeed = 0.0f;

		mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_L]->SetLocalPos(GVector4D::vLeft * 0.25f);
		mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_L]->SetLocalScale({ 1.0f, 0.3f, 1.0f });
		mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_R]->SetLocalPos(GVector4D::vRight * 0.25f);
		mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_05_R]->SetLocalScale({ 1.0f, 0.3f, 1.0f });

		return;
	}
}

bool ErinaFSM::CheckHammerDrillUp( )
{
	if ( true == GameKeyInput::Press(L"Up") && true == GameKeyInput::Down(L"Attack") )
	{
		GSound::Play(L"UPPER.WAV");

		mCurErinaAni->ChangeAnimation(L"Hammer_Drill_Up");
		mCurHammerAni->ChangeAnimation(L"Hammer_Drill_Up");
		meErinaState = _ERINA_STATE::ERINA_HAMMER_DRILL;
		mJumpPower = 350.0f;
		mGravityAcceleration = mDefaultGravityAcceleration;

		OffAllAttCols( );
		if ( _ERINA_DIRECTION::ERINA_LEFT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_UP_L]->OnUpdate( );
		}
		else if ( _ERINA_DIRECTION::ERINA_RIGHT == meErinaDirection )
		{
			mVecOfSkillColliders[_ERINA_ATTACK_::ATT_HAMMER_UP_R]->OnUpdate( );
		}

		return true;
	}

	return false;
}

void ErinaFSM::TranslateHorizonByRatio(float _ratio)
{
	// todo 스킬별 이동 시간을 두는 형태로 변경
	if ( meErinaDirection == _ERINA_DIRECTION::ERINA_RIGHT )
	{
		mErinaPos.m.x += mSpeed * _ratio * GameTime::DeltaTime( );
	}
	else if ( meErinaDirection == _ERINA_DIRECTION::ERINA_LEFT )
	{
		mErinaPos.m.x -= mSpeed * _ratio * GameTime::DeltaTime( );
	}
}

bool ErinaFSM::CheckSlide( )
{
	if ( true == GameKeyInput::Press(L"Down") && true == GameKeyInput::Down(L"Jump") )
	{
		GSound::Play(L"SLIDE.WAV");

		mCurErinaAni->ChangeAnimation(L"Slide");
		mPrevPosX = mErinaPos.m.x;
		meErinaState = _ERINA_STATE::ERINA_SLIDE;

		return true;
	}

	return false;
}

void ErinaFSM::GenerateAfterImage( )
{
	if ( false == mbSkillFlag )
	{
		for ( auto& cur : mListOfAfterImage )
		{
			cur.render->Death( );
		}

		mListOfAfterImage.clear( );
		return;
	}

	mTimeCounter -= GameTime::DeltaTime( );
	if ( mTimeCounter <= 0.0f )
	{
		ComRender2DSprite* sprite = mErinaActor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_PLAYER_FX);
		sprite->SetSprite(L"PLAYER_A.PNG");
		sprite->SetSpriteIdx(mPlayerSprite->GetSpriteIdx( ));
		sprite->SetVtxShader(GResource::Find<VertexGShader>(L"Afterimage.hlsl"));
		sprite->SetPixShader(GResource::Find<PixelGShader>(L"Afterimage.hlsl"));
		mPlayerSprite->GetWorldPos( );
		mListOfAfterImage.push_back({ sprite , mPlayerSprite->GetWorldPos( ), meErinaDirection, 0.9f });
		mTimeCounter = 0.07f;
		if ( mListOfAfterImage.size( ) >= 20 )
		{
			(*mListOfAfterImage.begin( )).render->Death( );
			mListOfAfterImage.pop_front( );
		}
	}

	float z = 20.0f;
	for ( auto& cur : mListOfAfterImage )
	{
		cur.alpha -= 3.5f * GameTime::DeltaTime( );
		if ( cur.alpha <= 0.2f )
		{
			cur.render->OffUpdate( );
			cur.render->SetWorldPos({ cur.pos.m.x, cur.pos.m.y, 100.0f, cur.pos.m.w });
			continue;
		}
		cur.render->SetColor({ 0.0f, 1.0f, 1.0f, cur.alpha });

		z -= 1.0f;
		cur.render->SetWorldPos({ cur.pos.m.x, cur.pos.m.y, cur.pos.m.z + z, cur.pos.m.w });

		if ( _ERINA_DIRECTION::ERINA_RIGHT == cur.dir )
		{
			cur.render->CullBack( );
		}
		else if ( _ERINA_DIRECTION::ERINA_LEFT == cur.dir )
		{
			cur.render->CullFront( );
		}
	}
}

void ErinaFSM::InitializeProperty( )
{
	mGravityAcceleration = mDefaultGravityAcceleration;
	mJumpPower = 0.0f;

	if ( false == mbOnGroundFlag )
	{
		mbOnGroundFlag = true;
		GSound::Play(L"LAND.WAV");
		meErinaState = _ERINA_STATE::ERINA_IDLE;
	}

	if ( true == mbAirHammerDrillFlag )
	{
		mbAirHammerDrillFlag = false;
		mbSkillFlag = false;
	}

	if ( true == mbAirDownJumpFlag )
	{
		TranslateHorizonByRatio(3.0f);

		mCurPosX = mErinaPos.m.x;
		if ( std::abs(mCurPosX - mPrevPosX) >= 30.f )
		{
			mbSkillFlag = false;
			mbAirDownJumpFlag = false;
			mPrevPosX = gconst::max<float>;
			mCurPosX = gconst::max<float>;
		}
	}

	if ( true == mbSlideFlag )
	{
		TranslateHorizonByRatio(1.75f);

		mCurPosX = mErinaPos.m.x;
		if ( std::abs(mCurPosX - mPrevPosX) >= 100.f )
		{
			meErinaState = _ERINA_STATE::ERINA_WALK_AFTER_SLIDE;
			mbSlideFlag = false;
			mPrevPosX = gconst::max<float>;
			mCurPosX = gconst::max<float>;
		}
	}
}

void ErinaFSM::OffAllAttCols( )
{
	for ( auto& col : mVecOfSkillColliders )
	{
		col->OffUpdate( );
	}

	mHammerSprite->SetLocalPos(0.0f);
}
