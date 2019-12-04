#include "BaseBullet.h"
#include <..\DxEngine\GSprite.h>
#include <..\DxEngine\GScene.h>
#include <..\DxEngine\ComRender2DSprite.h>

bool BaseBullet::initFlag = false;

BaseBullet::BaseBullet(GScene& _scene, const std::wstring& _imgName, const float imgCutSize)
{
	if ( false == BaseBullet::initFlag )
	{
		GResource::Create<GSprite>(_imgName, _imgName, imgCutSize, imgCutSize);
		BaseBullet::initFlag = true;
	}

	mActor = _scene.CreateActor(_imgName);
	mActor->SetLocalScale({ 50.f, 50.f, 1.0f });

	mSprite = mActor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_BULLET);
	mSprite->SetSprite(_imgName);
	mSprite->SetSampler(GResource::Find<GSampler>(L"Sampler_Linear"));
}

BaseBullet::~BaseBullet( )
{
}

void BaseBullet::Init( )
{
	BaseBullet::initFlag = true;
}
