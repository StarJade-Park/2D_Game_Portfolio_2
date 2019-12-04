#pragma once
#include "GResource.h"
class GDepthStencil
	: public GResource
{
public:
	GDepthStencil( );
	~GDepthStencil( );

	bool Create( );
	bool Create(const D3D11_DEPTH_STENCIL_DESC& _desc);
	void Update( );

private:
	D3D11_DEPTH_STENCIL_DESC mDSDesc;
	ID3D11DepthStencilState* mState;
};

