#pragma once
#include <..\psklib\GameTime.h>
#include <..\psklib\Well512Random.h>

#include <..\DxEngine\GActor.h>
#include <..\DxEngine\ComCustom.h>

class ComRender2DSprite;

class BaseBullet
	: public ComCustom
{
public:
	BaseBullet(GScene& _scene, const std::wstring& _imgName, const float imgCutSize);
	~BaseBullet( );

	void Init( ) override;

	static bool initFlag;
	GActor* mActor;
	ComRender2DSprite* mSprite;
};