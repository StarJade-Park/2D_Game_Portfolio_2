#pragma once
#include "GResource.h"
#include "ShaderType.h"

class GSampler
	: public GResource
{
public:
	GSampler( );
	~GSampler( );
	bool Create(void);
	bool Create(const D3D11_SAMPLER_DESC& _decs);
	void Update(shader_type::TYPE _shaderType, UINT _startSlot);

private:
	D3D11_SAMPLER_DESC mSamplerDesc;
	ID3D11SamplerState* miSamplerState;
};

