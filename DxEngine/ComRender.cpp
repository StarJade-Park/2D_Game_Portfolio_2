#include "ComRender.h"
#include "GActor.h"
#include "GScene.h"

ComRender::ComRender( )
	: mOrder(0)
{
	EMPTY_STATEMENT;
}

ComRender::~ComRender( )
{
	EMPTY_STATEMENT;
}

void ComRender::Init( )
{
	if ( nullptr == &GetActor( ).GetTransform( ) )
	{
		CRASH_PROG;
		return;
	}

	SetParentTransform(&GetActor( ).GetTransform( ));
}

void ComRender::StartSceneChange( )
{
	GetScene( ).mRendererMGR.PushRenderToMap(this);
}

void ComRender::Render(const SmartPtr<ComCamera> _cam)
{
	EMPTY_STATEMENT;
}

void ComRender::CreateData(const int _order)
{
	mOrder = _order;
	GetScene( ).mRendererMGR.PushRenderToMap(this);
}
