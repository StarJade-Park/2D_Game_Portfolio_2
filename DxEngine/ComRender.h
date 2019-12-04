#pragma once
#include <..\psklib\MyMath.h>

#include "ComSubTransform.h"

class ComCamera;

enum RENDER_GROUP : int
{
	GROUP_BACKGROUND = -1000,
	GROUP_ETC_OBJECT = -200,
	GROUP_MONSTER = -100,
	GROUP_NPC = -50,
	GROUP_PLAYER_FX = 10,
	GROUP_PLAYER = -150,
	GROUP_PLAYER_HAMMER = -200,
	GROUP_BULLET = -300,
	GROUP_DAMAGE = -500,
	GROUP_UI = 900,
	GROUP_FADE = 950,
	GROUP_NONE = 1000,
};

class ComRender
	: public ComSubTransform
{
	friend class RendererManager;
public:
	ComRender( );
	~ComRender( );
	void Init( ) override;
	void StartSceneChange( ) override;

	int GetOrder( ) const
	{
		return mOrder;
	}

	void SetOrder(const int _order)
	{
		mOrder = _order;
	}

	void CreateData(const int _order);

private:
	virtual void Render(const SmartPtr<ComCamera> _cam) /*= 0*/; // todo 모든 renderer가 구현하도록

private:
	int mOrder;
};

