#pragma once
#include "MonsterFSM.h"

namespace sandbag_correct_value
{
constexpr float leftCorrectValue = 11.5f;
constexpr float rightCorrectValue = 14.5f;
constexpr float topCorrectValue = 25.0f;
constexpr float downCorrectValue = 30.0f;

constexpr float correctionValue = gconst::ftol + 20.0f;
}

class ComRender2DSprite;

class SandBagFSM
	: public MonsterFSM
{
	friend class SandBag;
private:
	enum SAND_BAG_FSM : int
	{
		IDLE, ATTACKED_BY, MAX
	};

public:
	SandBagFSM( );
	~SandBagFSM( );

protected:
	void Init( ) override;
	void UpdateCheckPos( ) override;

	void Idle( ) override;
	void AttackedBy( ) override;

	void Update( ) override;

private:
	SAND_BAG_FSM meState;
	ComRender2DSprite* mSprite;

	float mAttackedTime;
};