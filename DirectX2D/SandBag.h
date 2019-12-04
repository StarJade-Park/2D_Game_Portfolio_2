#pragma once
#include "Monster.h"

class SandBagFSM;

class SandBag
	: public Monster
{
public:
	SandBag(GScene& _scene, const std::wstring& _imgName);
	~SandBag( );

protected:
	void CreateAnimation( ) override;
	void CreateCollider( ) override;
	void CreateFSM( ) override;

	void Enter(ComCollider* own, ComCollider* other) override;
	void Stay(ComCollider* own, ComCollider* other) override;
	void Exit(ComCollider* own, ComCollider* other) override;

private:
	SandBagFSM* mFSM;
};