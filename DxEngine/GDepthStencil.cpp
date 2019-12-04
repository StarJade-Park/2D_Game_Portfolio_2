#include "GDepthStencil.h"

GDepthStencil::GDepthStencil( )
	: mDSDesc(D3D11_DEPTH_STENCIL_DESC( ))
	, mState(nullptr)
{
	EMPTY_STATEMENT;
}

GDepthStencil::~GDepthStencil( )
{
	if ( nullptr != mState )
	{
		mState->Release( );
		mState = nullptr;
	}
}

bool GDepthStencil::Create( )
{
	mDSDesc.DepthEnable = true;
	mDSDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	mDSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	mDSDesc.StencilEnable = false;

	return Create(mDSDesc);
}

bool GDepthStencil::Create(const D3D11_DEPTH_STENCIL_DESC& _desc)
{
	if ( &mDSDesc != &_desc )
	{
		mDSDesc = _desc;
	}

	if ( FAILED(GEngineDevice::MainDevice( ).GetIDevice( ).CreateDepthStencilState(&mDSDesc, &mState)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

void GDepthStencil::Update( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).OMSetDepthStencilState(mState, 0);
}
