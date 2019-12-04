#pragma once
#include "Monster.h"

class Boss3NFSM;

class Boss3N
	: public Monster
{
public:
	Boss3N(GScene& _scene, const std::wstring& _imgName);
	~Boss3N( );

protected:
	void CreateAnimation( ) override;
	void CreateCollider( ) override;
	void CreateFSM( ) override;

	void Enter(ComCollider* own, ComCollider* other) override;
	void Stay(ComCollider* own, ComCollider* other) override;
	void Exit(ComCollider* own, ComCollider* other) override;

private:
	Boss3NFSM* mFSM;
	Com2DColliderOBB* mCollider;

	float mAttackedTime;
	const float mAttackedResetTime;
};
