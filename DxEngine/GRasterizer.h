#pragma once
#include "GResource.h"

class GRasterizer
	: public GResource
{
public:
	GRasterizer( );
	~GRasterizer( );

	bool Create(bool cullMode = false); // false : back, true : front
	bool Create(const D3D11_RASTERIZER_DESC& _desc);
	void Update( );

private:
	D3D11_RASTERIZER_DESC mRSDesc;
	ID3D11RasterizerState* mState;
};

