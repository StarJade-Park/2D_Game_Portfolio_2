#pragma once
#include "GShader.h"

class VertexGShader
	: public GShader
{
private:
	class InputLayout
		: public ControlBlock
	{
	public:
		InputLayout( )
			: mOffset(0), mVecOfInputData(std::vector<D3D11_INPUT_ELEMENT_DESC>( ))
		{
			EMPTY_STATEMENT;
		}

		~InputLayout( )
		{
			EMPTY_STATEMENT;
		}

	public:
		UINT mOffset;
		std::vector<D3D11_INPUT_ELEMENT_DESC> mVecOfInputData;
	};

public:
	VertexGShader( )
		: GShader(shader_type::TYPE::VS)
		, mVtxShader(nullptr), miInputLayout(nullptr)
		, mVecOfInLayout(std::vector<SmartPtr<InputLayout>>( ))
	{
	}

	~VertexGShader( )
	{
		if ( nullptr != miInputLayout )
		{
			miInputLayout->Release( );
			miInputLayout = nullptr;
		}

		if ( nullptr != mVtxShader )
		{
			mVtxShader->Release( );
			mVtxShader = nullptr;
		}
	}

	bool Load(const UINT _verHigh, const UINT _verLow, const char* _hlslFunction);
	void Update( ) override;

	void AddLayOut(
		const char* _smName,
		UINT _index,
		DXGI_FORMAT _fmt,
		UINT _inputSlot,
		UINT _IDSR, /*InstanceDataStepRate*/
		D3D11_INPUT_CLASSIFICATION _input = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA
	);

	void EndLayOut(
		const char* _smName,
		UINT _index,
		DXGI_FORMAT _fmt,
		UINT _inputSlot,
		UINT _IDSR,
		D3D11_INPUT_CLASSIFICATION _input = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA
	);

private:
	ID3D11VertexShader* mVtxShader;
	ID3D11InputLayout* miInputLayout;
	std::vector<SmartPtr<InputLayout>> mVecOfInLayout;
};

