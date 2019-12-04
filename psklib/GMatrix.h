#pragma once
#include "MathConstDef.h"
#include "GVector.h"

class GMatrix
{
public:
	GMatrix( )
		: dxMatrix(DirectX::XMMatrixIdentity( ))
	{
		EMPTY_STATEMENT;
	}

	GMatrix(const GMatrix& _other)
		: dxMatrix(_other.dxMatrix)
	{
		EMPTY_STATEMENT;
	}

	GMatrix(const DirectX::XMMATRIX& _dxMatrix)
		: dxMatrix(_dxMatrix)
	{
		EMPTY_STATEMENT;
	}

	GMatrix(DirectX::XMMATRIX&& _dxMatrix)
		: dxMatrix(std::move(_dxMatrix))
	{
		EMPTY_STATEMENT;
	}

	GMatrix(GMatrix&&) = default;
	~GMatrix( ) = default;

	void Identity( )
	{
		dxMatrix = DirectX::XMMatrixIdentity( );
	}

	// - 이동
	void Translate(const GVector4D& newPos)
	{
		dxMatrix = DirectX::XMMatrixTranslation(newPos.m.x, newPos.m.y, newPos.m.z);
	}

	void Scale(const GVector4D& _scale)
	{
		dxMatrix = DirectX::XMMatrixScaling(_scale.m.x, _scale.m.y, _scale.m.z);
	}

	void Transpose( )
	{
		dxMatrix = DirectX::XMMatrixTranspose(dxMatrix);
	}

	void MakeViewMatrix(GVector4D _eye, GVector4D _at, GVector4D _up)
	{
		Identity( );

		_at.Normalize( );
		_up.Normalize( );
		VecArr[2] = _at;
		VecArr[1] = _up;
		VecArr[0] = GVector4D::Cross(VecArr[1], VecArr[2]);

		Transpose( );

		VecArr[3] = -_eye;

		VecArr[3] = GVector4D
		{
			// ArrVec[0~2]의 크기가 1이므로 정사영을 내린것이다.
			// 내적의 정사영을 이해못하면 뷰행렬도 이해할수 없다.
			-GVector4D::DotProduct(VecArr[0], _eye), // 
			-GVector4D::DotProduct(VecArr[1], _eye), // 0.0F;
			-GVector4D::DotProduct(VecArr[2], _eye), // 
			1.0f,
		};
	}

	void ToOrthographicLH(float _width, float _height, float _near, float _far)
	{
		dxMatrix = DirectX::XMMatrixOrthographicLH(_width, _height, _near, _far);
	}

	void ToPerspectiveLH(float _width, float _height, float _near, float _far)
	{
		dxMatrix = DirectX::XMMatrixPerspectiveLH(_width, _height, _near, _far);
	}

	void Viewport(float _StartX, float _StartY, float _Width, float _Height, float _MinZ, float _MaxZ)
	{
		Identity( );
		fArr[0][0] = _Width / 2;
		fArr[1][1] = -(_Height / 2); // 이게 Y축을 반전시킨다.
		fArr[2][2] = _MaxZ - _MinZ;

		fArr[3][0] = _StartX + fArr[0][0];
		fArr[3][1] = _StartY + (_Height / 2);
		fArr[3][2] = _MinZ;
	}

#pragma region rotate

	void RotXByDeg(const float _deg)
	{
		return RotXByRad(_deg * gconst::DegToRad);
	}

	void RotXByRad(const float _rad)
	{
		dxMatrix = DirectX::XMMatrixRotationX(_rad);
	}

	void RotYByDeg(const float _deg)
	{
		return RotYByRad(_deg * gconst::DegToRad);
	}

	void RotYByRad(const float _rad)
	{
		dxMatrix = DirectX::XMMatrixRotationY(_rad);
	}

	void RotZByDeg(const float _deg)
	{
		return RotZByRad(_deg * gconst::DegToRad);
	}

	void RotZByRad(const float _rad)
	{
		dxMatrix = DirectX::XMMatrixRotationZ(_rad);
	}

	void RotByDeg(const GVector4D& _deg)
	{
		RotByRad(_deg * gconst::DegToRad);
	}

	void RotByRad(const GVector4D& _rad)
	{
		Identity( );
		*this = DirectX::XMMatrixRotationRollPitchYaw(_rad.m.x, _rad.m.y, _rad.m.z);
	}

	void RotQuaternionByDeg(const GVector4D& _deg)
	{
		RotQuaternionByRad(_deg * gconst::DegToRad);
	}

	void RotQuaternionByRad(const GVector4D& _rad)
	{
		dxMatrix = DirectX::XMMatrixRotationQuaternion(
			DirectX::XMQuaternionRotationRollPitchYaw(_rad.m.x, _rad.m.y, _rad.m.z)
		);
	}

#pragma endregion

#pragma region operator=
	GMatrix& operator=(const float _value)
	{
		for ( size_t y = 0; y < 4; ++y )
		{
			for ( size_t x = 0; x < 4; ++x )
			{
				fArr[y][x] = _value;
			}
		}

		return *this;
	}

	GMatrix& operator=(const GMatrix& _other)
	{
		memcpy_s(this, sizeof(GMatrix), &_other, sizeof(GMatrix));
		return *this;
	}

	GMatrix& operator=(GMatrix&& _other) = default;

	GMatrix& operator=(DirectX::XMMATRIX&& _dxMatrix)
	{
		dxMatrix = std::move(_dxMatrix);
		return *this;
	}
#pragma endregion

#pragma region operator*
	// - Matrix(4x4) * Matrix(4x4)
	GMatrix operator*(const GMatrix& _other)
	{
		return DirectX::XMMatrixMultiply(dxMatrix, _other.dxMatrix);
	}

	// - Vector(=Matrix(1x4)) * Matrix(4x4)
	GVector4D operator*(const GVector4D& _vector)
	{
		GVector4D newVec = GVector4D( );
		for ( size_t x = 0; x < 4; ++x )
		{
			float sum = 0.0f;
			for ( size_t i = 0; i < 4; ++i )
			{
				sum += _vector.Arr[i] * fArr[i][x];
			}

			newVec.Arr[x] = sum;
		}

		return newVec;
	}
#pragma endregion

	GVector4D& FowordVector( )
	{
		return VecArr[2];
	}

	GVector4D& UpVector( )
	{
		return VecArr[1];
	}

	GVector4D& RightVector( )
	{
		return VecArr[0];
	}

	GVector4D& operator[](const size_t _index)
	{
		return VecArr[_index];
	}

	GVector4D& operator[](const int _index)
	{
		return VecArr[_index];
	}

	float operator()(const size_t _indexX, const size_t _indexY)
	{
		return VecArr[_indexY](_indexX);
	}

	float operator()(const int _indexX, const int _indexY)
	{
		return VecArr[_indexY](_indexX);
	}

public:
	union
	{
		float fArr[4][4];
		struct e
		{
			float _01, _02, _03, _04;
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
		} e;
		DirectX::XMMATRIX dxMatrix;
		GVector4D VecArr[4];
	};
};

