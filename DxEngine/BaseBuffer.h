#pragma once
#include "GResource.h"

class BaseBuffer
	: public GResource
{
public:
	virtual ~BaseBuffer( )
	{
		if ( nullptr != mBuffer )
		{
			mBuffer->Release( );
			mBuffer = nullptr;
		}
	}

	ID3D11Buffer& GetBuffer( )
	{
		return *mBuffer;
	}

protected:
	BaseBuffer( )
		: mBufferDescription(D3D11_BUFFER_DESC( )), mBuffer(nullptr)
	{
		EMPTY_STATEMENT;
	}

protected:
	D3D11_BUFFER_DESC mBufferDescription;
	ID3D11Buffer* mBuffer;
};