#include "GRasterizer.h"

GRasterizer::GRasterizer( )
	: mRSDesc(D3D11_RASTERIZER_DESC( )), mState(nullptr)
{
	EMPTY_STATEMENT;
}

GRasterizer::~GRasterizer( )
{
	if ( nullptr != mState )
	{
		mState->Release( );
		mState = nullptr;
	}
}

bool GRasterizer::Create(bool cullMode /*= false*/)
{
	mRSDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	if ( false == cullMode )
	{
		mRSDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	}
	else
	{
		mRSDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	}
	mRSDesc.MultisampleEnable = false;
	mRSDesc.AntialiasedLineEnable = false;

	return Create(mRSDesc);
}

bool GRasterizer::Create(const D3D11_RASTERIZER_DESC& _desc)
{
	if ( &mRSDesc != &_desc )
	{
		mRSDesc = _desc;
	}

	if ( FAILED(GEngineDevice::MainDevice( ).GetIDevice( ).CreateRasterizerState(&mRSDesc, &mState)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

void GRasterizer::Update( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).RSSetState(mState);
}
