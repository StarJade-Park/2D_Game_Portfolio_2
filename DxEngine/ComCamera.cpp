#include "ComCamera.h"
#include "GActor.h"
#include "GScene.h"
#include "ComTransform.h"
#include "RendererManager.h"
#include "GameWindow.h"

ComCamera* ComCamera::mMainCam = nullptr;

ComCamera::ComCamera( )
	: mMatViewport(GMatrix( )), mMatProjection(GMatrix( )), mMatVP(GMatrix( ))
	, mOrder(RENDER_GROUP::GROUP_NONE), mbAllRenderFlag(false), mSetOfOrderGroup(std::set<int>( ))
	, meProjMode(PROJECTION_MODE::PERSPECTIVE)
	, mFov(gconst::inf<float>) // note 시험용으로 사용했음
	, mWidth(gconst::inf<float>), mHeight(gconst::inf<float>)
	, mNear(1.0f), mFar(1000.0f)
{
	EMPTY_STATEMENT;
}

ComCamera::~ComCamera( )
{
	EMPTY_STATEMENT;
}

void ComCamera::Init( )
{
	if ( nullptr == &GetActor( ).GetTransform( ) )
	{
		CRASH_PROG;
	}

	if ( nullptr == mMainCam )
	{
		mMainCam = this;
	}

	GetScene( ).mRendererMGR.PushCamToMap(this);

	mWidth = GetActor( ).GetScene( ).GetWnd( ).GetWndSize( ).m.x;
	mHeight = GetActor( ).GetScene( ).GetWnd( ).GetWndSize( ).m.y;
}

GVector2D ComCamera::GetMousePosIn2D( )
{
	GVector2D mousePos = GameWindow::MainGameWnd( )->GetMousePos( );
	mousePos.m.y *= -1.0f;
	mousePos.m.x -= GameWindow::MainGameWnd( )->GetWndSize( ).m.x * 0.5f;
	mousePos.m.y += GameWindow::MainGameWnd( )->GetWndSize( ).m.y * 0.5f;

	float xRatio = mWidth / GameWindow::MainGameWnd( )->GetWndSize( ).m.x;
	float yRatio = mHeight / GameWindow::MainGameWnd( )->GetWndSize( ).m.y;

	mousePos.m.x *= xRatio;
	mousePos.m.y *= yRatio;

	mousePos += { GetTransform( ).GetWorldPos( ).m.x, GetTransform( ).GetWorldPos( ).m.y };

	return mousePos;
}

void ComCamera::SetOrder(const int _order)
{
	mOrder = _order;
	GetScene( ).mRendererMGR.CheckOrderOfCamera( );
}

int ComCamera::GetOrder( )
{
	return mOrder;
}

void ComCamera::RenderPreprocess( )
{
	ComTransform& trans = GetActor( ).GetTransform( );

	mMatViewport.MakeViewMatrix(
		trans.GetWorldPos( ),
		trans.GetForwardVectorLH( ),
		trans.GetUpVectorLH( )
	);
	
	switch ( meProjMode )
	{
	case PROJECTION_MODE::PERSPECTIVE:
		mMatProjection.ToPerspectiveLH(mWidth, mHeight, mNear, mFar);
		break;
	case PROJECTION_MODE::ORTHOGONAL:
		mMatProjection.ToOrthographicLH(mWidth, mHeight, mNear, mFar);
		break;
	case PROJECTION_MODE::ERROR_MODE: FALL_THROUGH;
	default:
		CRASH_PROG;
		break;
	}

	mMatVP = mMatViewport * mMatProjection;
}
