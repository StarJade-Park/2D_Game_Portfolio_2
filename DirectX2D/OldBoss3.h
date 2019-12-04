#pragma once
#include "Monster.h"

class OldBoss3FSM;

class OldBoss3
	: public Monster
{
public:
	OldBoss3(GScene& _scene, const std::wstring& _imgName);
	~OldBoss3( );

protected:
	void CreateAnimation( ) override;
	void CreateCollider( ) override;
	void CreateFSM( ) override;

	void Enter(ComCollider* own, ComCollider* other) override;
	void Stay(ComCollider* own, ComCollider* other) override;
	void Exit(ComCollider* own, ComCollider* other) override;

private:
	OldBoss3FSM* mFSM;
	Com2DColliderOBB* mCollider;

	float mAttackedTime;
	const float mAttackedResetTime;
};