#pragma once
#include "GResource.h"
#include "GEngineDevice.h"
#include "ShaderType.h"


class GShader
	: public GResource
{
public:
	static UINT GetFormatByteSize(DXGI_FORMAT _fmt);
	virtual void Update( ) = 0;

protected:
	GShader(shader_type::TYPE _shaderType = shader_type::TYPE::NONE)
		: miBlob(nullptr), miErrBlob(nullptr)
		, mVerLigh(0), mVerLow(0), meShaderType(_shaderType)
	{
		EMPTY_STATEMENT;
	}

	~GShader( )
	{
		if ( nullptr != miBlob )
		{
			miBlob->Release( );
			miBlob = nullptr;
		}

		if ( nullptr != miErrBlob )
		{
			miErrBlob->Release( );
			miErrBlob = nullptr;
		}
	}

protected:
	static char mShaderVerStr[256];
	ID3DBlob* miBlob;
	ID3DBlob* miErrBlob;
	UINT mVerLigh;
	UINT mVerLow;
	shader_type::TYPE meShaderType;
};

