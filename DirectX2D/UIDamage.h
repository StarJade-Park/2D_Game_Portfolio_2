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

// RGB�� �� ���۰��� 0.0f �Ǵ� 1.0f
// 
// ó�� ���� ������ ũ��� �ſ� ŭ(���ũ��)
// �ױ� ������ ������ ũ�� ����
// ���� �� ũ�� �پ��鼭 �����(���� ũ�� �����϶� die)
//

// ������ �Է����ָ� �˾Ƽ� �ǰ��� ������Ʈ�� ���� ������ �ǵ���
// ���� �ð� ���� ���� �̵��ϴٰ� �˾Ƽ� ����
// 
// UIDamage.Add(damage);

class ComRender2DSprite;

// ����������....
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