#pragma once
#include "GShader.h"

class PixelGShader
	: public GShader
{
public:
	PixelGShader( )
		: GShader(shader_type::TYPE::PS), mPixShader(nullptr)
	{
		EMPTY_STATEMENT;
	}

	~PixelGShader( )
	{
		if ( nullptr != mPixShader )
		{
			mPixShader->Release( );
			mPixShader = nullptr;
		}
	}

	bool Load(UINT _verHigh, UINT _verLow, const char* _hlslFunction);
	void Update( ) override;

private:
	ID3D11PixelShader* mPixShader;
};

