#pragma once
#include "MonsterFSM.h"

class Com2dAnimation;
class ComRender2DSprite;

namespace oldboss3fsm_correct_value
{
constexpr float leftCorrectValue = 12.5f;
constexpr float rightCorrectValue = 12.5f;
constexpr float topCorrectValue = 25.0f;
constexpr float downCorrectValue = 30.0f;

constexpr float correctionValue = gconst::ftol + 20.0f;
}

class OldBoss3FSM
	: public MonsterFSM
{
	friend class OldBoss3;
private:
	enum OLD_BOSS_FSM : int
	{
		IDLE, WALK, ATTCK, JUMP, DIE, ATTACKED_BY, MAX
	};

public:
	OldBoss3FSM( );
	~OldBoss3FSM( );

protected:
	void Init( ) override;
	void UpdateCheckPos( ) override;

	void Idle( ) override;
	void Walk( ) override;
	void Attack( ) override;
	void AttackedBy( ) override;
	void Jump( ) override;
	void Die( ) override;

	void Update( ) override;

private:
	bool mbJumpFlag;
	bool mbDeathFlag;
	OLD_BOSS_FSM meState;

	ComRender2DSprite* mSpriteRender;
	Com2dAnimation* mAnimation;

	float mResetTime;
	const float mDefualtResetTime;
	const float mDeathTime;
	const float mJumpPwrAttackedBy;

	float mSpeed;
	float mWalkTime;
	float mAttackedTime;
};