#include "GSampler.h"
#include "GShader.h"

GSampler::GSampler( )
	: mSamplerDesc(D3D11_SAMPLER_DESC( ))
	, miSamplerState(nullptr)
{

}

GSampler::~GSampler( )
{
	if ( nullptr != miSamplerState )
	{
		miSamplerState->Release( );
		miSamplerState = nullptr;
	}
}

bool GSampler::Create(void)
{
	mSamplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT; // note ��Ʈ ���ӿ��� ���
	//mSamplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR; // note ���ø� �ʿ��� ���ӿ��� ���

	// uvw mapping ��� ����
	mSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	mSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	mSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	// note border(~= ����) color ����
	memcpy_s(mSamplerDesc.BorderColor, sizeof(GVector4D), &GVector4D::Red, sizeof(GVector4D));

	// Comparison(����) ��� ����
	mSamplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	mSamplerDesc.MaxAnisotropy = 0;
	mSamplerDesc.MinLOD = -FLT_MAX;
	mSamplerDesc.MaxLOD = FLT_MAX;

	mSamplerDesc.MipLODBias = 1.0F;

	return Create(mSamplerDesc);
}

bool GSampler::Create(const D3D11_SAMPLER_DESC& _decs)
{
	if ( &mSamplerDesc != &_decs )
	{
		mSamplerDesc = _decs;
	}

	if ( FAILED(GEngineDevice::MainDevice( ).GetIDevice( ).
				CreateSamplerState(&mSamplerDesc, &miSamplerState)
	) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

void GSampler::Update(shader_type::TYPE _shaderType, UINT _startSlot)
{
	if ( nullptr == miSamplerState )
	{
		CRASH_PROG;
		return;
	}

	switch ( _shaderType )
	{
	case shader_type::TYPE::VS:
		GEngineDevice::MainDevice( ).GetIContext( )
			.VSSetSamplers(_startSlot, 1, &miSamplerState);
		break;
	case shader_type::TYPE::HS:
		GEngineDevice::MainDevice( ).GetIContext( )
			.HSSetSamplers(_startSlot, 1, &miSamplerState);
		break;
	case shader_type::TYPE::DS:
		GEngineDevice::MainDevice( ).GetIContext( )
			.DSSetSamplers(_startSlot, 1, &miSamplerState);
		break;
	case shader_type::TYPE::GS:
		GEngineDevice::MainDevice( ).GetIContext( )
			.GSSetSamplers(_startSlot, 1, &miSamplerState);
		break;
	case shader_type::TYPE::PS:
		GEngineDevice::MainDevice( ).GetIContext( )
			.PSSetSamplers(_startSlot, 1, &miSamplerState);
		break;
	case shader_type::TYPE::NONE: FALL_THROUGH;
	default:
		CRASH_PROG;
		break;
	}
}
