#pragma once
#include <vector>
#include <..\psklib\Well512Random.h>
#include <..\DxEngine\ComCustom.h>

namespace
{
constexpr int render_max_count = 100;
constexpr float color_max = 1.0f;
constexpr float color_min = 0.0f;
}

// RGB의 각 시작값은 0.0f 또는 1.0f
// 
// 처음 때린 데미지 크기는 매우 큼(상대크기)
// 죽기 전까지 데미지 크기 보통
// 죽을 때 크기 줄어들면서 사라짐(일정 크기 이하일때 die)
//

// 데미지 입력해주면 알아서 피격한 오브젝트의 위에 랜더링 되도록
// 일정 시간 동안 위로 이동하다가 알아서 죽음
// 
// UIDamage.Add(damage);

class ComRender2DSprite;

// 맞을때마다....
// actor->addcom<UIDamage>(_damage);
class UIDamage
	: public ComCustom
{
public:
	UIDamage( );
	~UIDamage( );

	void AddDamage(const UINT _damage); // void SCORE::Init(int _Point, float _OffsetX, float _OffsetY, int _Order)

private:
	void Init( ) override;
	void Update( ) override;
	void FindingDigits(const UINT _damage);

private:
	UINT mDamage;

	float mDamageSizeRatio;
	float mNormalSizeTime;

	bool mbR2G;
	bool mbG2B;
	bool mbB2R;

	bool mbScaleStart;

	UINT mOffsetX;
	UINT mOffsetY;

	GVector4D mColor;

	std::vector<UINT> mVecOfDamage;
	std::vector<GActor*> mVecOfDamageActors;
};