#pragma once
#include "GEngineDevice.h"
#include "GShader.h"

class GConstBuffer
	: public GResource
{
public:
	GConstBuffer( )
		: meShaderType(shader_type::TYPE::NONE)
		, mBuffer(nullptr), mBufDesc(D3D11_BUFFER_DESC( ))
		, mReg(0), mBufferSize(0)
		, mSubResData(D3D11_SUBRESOURCE_DATA( ))
		, mMappedSubRes(D3D11_MAPPED_SUBRESOURCE( ))
		, mData(nullptr)
		, ShUpdateFunction(nullptr)
	{
		EMPTY_STATEMENT;
	}

	~GConstBuffer( )
	{
		if ( nullptr != mData )
		{
			delete[ ](char*)mData;
			mData = nullptr;
		}

		if ( nullptr != mBuffer )
		{
			mBuffer->Release( );
			mBuffer = nullptr;
		}
		
	}

	bool Create(shader_type::TYPE _shaderType, size_t _size, UINT _registerNumber);
	void SetData(void* _data, UINT _size);
	void Update( );

private:
	void VSUpdate( );
	void HSUpdate( );
	void DSUpdate( );
	void GSUpdate( );
	void PSUpdate( );

private:
	shader_type::TYPE meShaderType;
	ID3D11Buffer* mBuffer;
	D3D11_BUFFER_DESC mBufDesc;
	UINT mReg; // register 몇 번인가?
	UINT mBufferSize;
	D3D11_SUBRESOURCE_DATA mSubResData;
	D3D11_MAPPED_SUBRESOURCE mMappedSubRes;
	void* mData;
	void(GConstBuffer::*ShUpdateFunction)(); // member function ptr
};

