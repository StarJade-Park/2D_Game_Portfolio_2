#pragma once
#include "GResource.h"

class GBlender
	: public GResource
{
public:
	GBlender( );
	~GBlender( );

	bool Create( );
	bool Create(const D3D11_BLEND_DESC& _Desc);
	void Update( );

private:
	D3D11_BLEND_DESC mBlendDesc;
	ID3D11BlendState* mState;
	GVector4D mColor;
};

