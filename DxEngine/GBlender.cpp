#include "GBlender.h"
#include "GEngineDevice.h"

GBlender::GBlender( )
	: mBlendDesc(D3D11_BLEND_DESC( )), mState(nullptr), mColor(GVector4D( ))
{
	EMPTY_STATEMENT;
}

GBlender::~GBlender( )
{
	if ( nullptr != mState )
	{
		mState->Release( );
		mState = nullptr;
	}
}

bool GBlender::Create( )
{
	mBlendDesc.AlphaToCoverageEnable = false;
	mBlendDesc.IndependentBlendEnable = false;

	mBlendDesc.RenderTarget[0].BlendEnable = true;
	mBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	mBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	mBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	
	mBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	mBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	mBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	mBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	return Create(mBlendDesc);
}

bool GBlender::Create(const D3D11_BLEND_DESC& _desc)
{
	if ( &mBlendDesc != &_desc )
	{
		mBlendDesc = _desc;
	}

	if ( FAILED(GEngineDevice::MainDevice( ).GetIDevice( ).CreateBlendState(&mBlendDesc, &mState)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

void GBlender::Update( )
{
	GEngineDevice::MainDevice( ).GetIContext( ).OMSetBlendState(
		mState, mColor.Arr, 0xffffffff /* = default value */
	);
}


