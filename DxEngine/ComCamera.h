#pragma once
#include <set>

#include <..\psklib\GMatrix.h>

#include "BaseGComponent.h"

enum PROJECTION_MODE
{
	ERROR_MODE,
	PERSPECTIVE, // - 원근
	ORTHOGONAL, // - 직교
};

class ComCamera final
	: public BaseGComponent
{
public:
	ComCamera( );
	~ComCamera( );
	void Init( ) override;

	GMatrix GetViewport( )
	{
		return mMatViewport;
	}

	GMatrix GetProjection( )
	{
		return mMatProjection;
	}

	GMatrix GetVP( )
	{
		return mMatVP;
	}

	GVector2D GetMousePosIn2D( );

	void SetOrder(const int _order);
	int GetOrder( );
	
	void SwitchAllRender( )
	{
		mbAllRenderFlag = !mbAllRenderFlag;
	}

	bool IsAllRender( ) const
	{
		return mbAllRenderFlag;
	}

	bool IsCanRender(const int _order)
	{
		return mSetOfOrderGroup.find(_order) != mSetOfOrderGroup.end( );
	}

	void RenderPreprocess( ) override;

	void SetWidthAndHeight(const float _width, const float _height)
	{
		SetWidth(_width);
		SetHeight(_height);
	}

	void SetWidth(const float _width)
	{
		mWidth = _width;
	}

	void SetHeight(const float _height)
	{
		mHeight = _height;
	}

	void SetFov(const float _fov)
	{
		mFov = _fov;
	}

	void SetProjMode(const PROJECTION_MODE _mode)
	{
		meProjMode = _mode;
	}

	template<class... TYPES>
	void CreateData(TYPES&&... _args)
	{
		PushToSetOfRender(_args...);
		return;
	}

	// 인풋매니저 식.
	template<class... TYPES>
	void PushToSetOfRender(const int _order, TYPES&&... _args)
	{
		if ( mSetOfOrderGroup.find(_order) != mSetOfOrderGroup.end( ) )
		{
			CRASH_PROG;
			return;
		}

		mSetOfOrderGroup.insert(_order);

		PushToSetOfRender(_args...);
	}

	// todo 만들어야 함.
	void PushToSetOfRender( )
	{
		EMPTY_STATEMENT;
	}

public:
	static ComCamera* mMainCam;

private:
	GMatrix mMatViewport;
	GMatrix mMatProjection;
	GMatrix mMatVP; // = Viewport * ToPerspectiveLH

	int mOrder;
	bool mbAllRenderFlag;
	std::set<int> mSetOfOrderGroup;
	
	PROJECTION_MODE meProjMode;
	float mFov;
	float mWidth;
	float mHeight;
	float mNear;
	float mFar;
};

