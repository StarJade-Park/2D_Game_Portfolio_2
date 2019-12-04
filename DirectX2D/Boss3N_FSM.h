#pragma once
#include <list>
#include "MonsterFSM.h"

class Com2dAnimation;
class ComRender2DSprite;
//class Bullet_Normal;

namespace boss3nfsm_correct_value
{
constexpr float leftCorrectValue = 12.5f;
constexpr float rightCorrectValue = 12.5f;
constexpr float topCorrectValue = 25.0f;
constexpr float downCorrectValue = 30.0f;

constexpr float correctionValue = gconst::ftol + 20.0f;
}

class Boss3NFSM
	: public MonsterFSM
{
	friend class Boss3N;
private:
	enum BOSS_3N_FSM : int
	{
		IDLE, WALK, ATTCK_FLY, ATTACK_01, ATTACK_02, JUMP, DIE, ATTACKED_BY, MAX
	};

public:
	Boss3NFSM( );
	~Boss3NFSM( );

protected:
	void Init( ) override;
	void UpdateCheckPos( ) override;

	void Idle( ) override;
	void Walk( ) override;
	
	void AttackFly( );
	void Attack01( );
	void Attack02( );

	void AttackedBy( ) override;
	void Jump( ) override;
	void Die( ) override;

	void Update( ) override;

private:
	bool mbJumpFlag;
	bool mbDisableGravity;
	bool mbDeathFlag;
	BOSS_3N_FSM meState;

	ComRender2DSprite* mSpriteRender;
	Com2dAnimation* mAnimation;

	float mResetTime;
	const float mDefualtResetTime;
	const float mDeathTime;
	const float mJumpPwrAttackedBy;

	float mSpeed;
	float mFlySpeed;
	float mWalkTime;

	float mFlyJumpTime;
	float mFlyTime;
	const float mFlyLimitTime;
	//std::list<Bullet_Normal*> mListOfBullet;

	float mAttackedTime;
};