#pragma once
#include "Debug.h"
#include "MathConstDef.h"

struct GVector2D
{
public:
	GVector2D( )
		: m( )
	{
		EMPTY_STATEMENT;
	}

	GVector2D(float _x, float _y)
		: m(_x, _y)
	{
		EMPTY_STATEMENT;
	}

	GVector2D(const GVector2D&) = default;
	GVector2D& operator=(const GVector2D&) = default;

	GVector2D(GVector2D&&) = default;
	GVector2D& operator=(GVector2D&&) = default;

	GVector2D& operator+=(const GVector2D& _other)
	{
		m.x += _other.m.x;
		m.y += _other.m.y;

		return *this;
	}

	GVector2D& operator-=(const GVector2D& _other)
	{
		m.x -= _other.m.x;
		m.y -= _other.m.y;

		return *this;
	}

	// - 스칼라 곱 연산자
	GVector2D& operator*=(const float _scala)
	{
		m.x *= _scala;
		m.y *= _scala;

		return *this;
	}

	GVector2D& operator/=(const float _scala)
	{
		m.x /= _scala;
		m.y /= _scala;

		return *this;
	}

	// - 역벡터 연산자
	GVector2D operator-( )
	{
		return GVector2D(-m.x, -m.y);
	}

	// - 벡터 뺄셈
	GVector2D operator-(const GVector2D& _other)
	{
		return GVector2D(m.x - _other.m.x, m.y - _other.m.y);
	}

	GVector2D operator+(const float _val)
	{
		return GVector2D(m.x + _val, m.y + _val);
	}

	// - 내적 구하기
	float operator*(const GVector2D& _other)
	{
		return ((m.x * _other.m.x) + (m.y * _other.m.y));
	}

	// - 스칼라 곱
	GVector2D operator*(const float _scala)
	{
		return GVector2D(m.x * _scala, m.y * _scala);
	}

public:
	union
	{
		struct member
		{
			member( )
				: x(0.0f), y(0.0f)
			{
				EMPTY_STATEMENT;
			}

			member(float _x, float _y)
				: x(_x), y(_y)
			{
				EMPTY_STATEMENT;
			}

			float x;
			float y;
		} m;
		float Arr[2];
	};
};

struct GVector3D
{
public:
	union
	{
		struct member
		{
			float x;
			float y;
			float z;
		} m;

		struct color
		{
			float r;
			float g;
			float b;
		} c;

		float Arr[3];
		GVector2D Vec2D;
	};
};

struct GVector4D
{
public:
	static const GVector4D Zero;
	static const GVector4D One;

	static const GVector4D Red;
	static const GVector4D Green;
	static const GVector4D Blue;
	static const GVector4D White;
	static const GVector4D Black;
	static const GVector4D Magenta;

	static const GVector4D vUp;
	static const GVector4D vLeft;
	static const GVector4D vRight;
	static const GVector4D vDown;

	static const GVector4D vBack;

	union
	{
		float Arr[4];
		struct member
		{
		public:
			member( )
				: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
			{
				EMPTY_STATEMENT;
			}

			member(float _x, float _y, float _z, float _w = 1.0f)
				: x(_x), y(_y), z(_z), w(_w)
			{
				EMPTY_STATEMENT;
			}
		public:
			float x;
			float y;
			float z;
			float w;
		} m;

		struct color
		{
			float r;
			float g;
			float b;
			float a;
		} c;

		DirectX::XMVECTOR dxVector;
		DirectX::XMFLOAT3 dxFloat3;
		GVector2D Vec2D;
		GVector3D Vec3D;
	};

#pragma region constructor

	GVector4D( )
		: m( )
	{
		EMPTY_STATEMENT;
	}

	GVector4D(const GVector4D& _other)
		: m(_other.m)
	{
		EMPTY_STATEMENT;
	}

	GVector4D(const GVector2D& _vector2D)
		: m(_vector2D.m.x, _vector2D.m.y, 0.0f)
	{
		EMPTY_STATEMENT;
	}

	GVector4D(GVector4D&& _right) = default;

	// - set all member to input value
	GVector4D(const float _value)
		: m(_value, _value, _value)
	{
		EMPTY_STATEMENT;
	}

	GVector4D(const float _x, const float _y)
		: m(_x, _y, 0.0f)
	{
		EMPTY_STATEMENT;
	}

	GVector4D(const float _x, const float _y, const float _z)
		: m(_x, _y, _z)
	{
		EMPTY_STATEMENT;
	}

	GVector4D(const float _x, const float _y, const float _z, const float _w)
		: m(_x, _y, _z, _w)
	{
		EMPTY_STATEMENT;
	}

	GVector4D(DirectX::XMVECTOR&& _right)
		: dxVector(std::move(_right))
	{
		EMPTY_STATEMENT;
	}
#pragma endregion

	void SetAll(const float _value)
	{
		m.x = m.y = m.z = _value;
	}

	GVector4D& operator=(GVector4D&& _right) = default;
	GVector4D& operator=(DirectX::XMVECTOR&& _right)
	{
		dxVector = std::move(_right);
		return *this;
	}

	GVector4D& operator=(const GVector4D& _other)
	{
		memcpy_s(this, sizeof(GVector4D), &_other, sizeof(GVector4D));
		return *this;
	}

#pragma region operator+
	GVector4D operator+(const float _scala)
	{
		return GVector4D(m.x + _scala, m.y + _scala, m.z + _scala);
	}

	GVector4D operator+(const GVector2D& _scale)
	{
		return GVector4D(m.x + _scale.m.x, m.y + _scale.m.y, m.z);
	}

	GVector4D operator+(const GVector3D& _scale)
	{
		return GVector4D(m.x + _scale.m.x, m.y + _scale.m.y, m.z + _scale.m.z);
	}

	GVector4D operator+(const GVector4D& _scale)
	{
		return GVector4D(m.x + _scale.m.x, m.y + _scale.m.y, m.z + _scale.m.z, m.w + _scale.m.w);
	}

	GVector4D operator+=(const float _scala)
	{
		m.x += _scala;
		m.y += _scala;
		m.z += _scala;

		return *this;
	}

	GVector4D operator+=(const GVector2D& _scale)
	{
		m.x += _scale.m.x;
		m.y += _scale.m.y;

		return *this;
	}

	GVector4D operator+=(const GVector3D& _scale)
	{
		m.x += _scale.m.x;
		m.y += _scale.m.y;
		m.z += _scale.m.z;

		return *this;
	}

	GVector4D operator+=(const GVector4D& _scale)
	{
		m.x += _scale.m.x;
		m.y += _scale.m.y;
		m.z += _scale.m.z;
		m.w += _scale.m.w;

		return *this;
	}
#pragma endregion

#pragma region operator-
	GVector4D operator-(const float _scala) const
	{
		return GVector4D(m.x - _scala, m.y - _scala, m.z - _scala);
	}

	GVector4D operator-(const GVector2D& _scale) const
	{
		return GVector4D(m.x - _scale.m.x, m.y - _scale.m.y, m.z);
	}

	GVector4D operator-(const GVector3D& _scale) const
	{
		return GVector4D(m.x - _scale.m.x, m.y - _scale.m.y, m.z - _scale.m.z);
	}

	GVector4D operator-(const GVector4D& _scale) const
	{
		return GVector4D(m.x - _scale.m.x, m.y - _scale.m.y, m.z - _scale.m.z, m.w - _scale.m.w);
	}

	GVector4D operator-( ) const
	{
		return GVector4D(-m.x, -m.y, -m.z, m.w);
	}

	GVector4D operator-=(const float _scala)
	{
		m.x -= _scala;
		m.y -= _scala;
		m.z -= _scala;

		return *this;
	}

	GVector4D operator-=(const GVector2D& _scale)
	{
		m.x -= _scale.m.x;
		m.y -= _scale.m.y;

		return *this;
	}

	GVector4D operator-=(const GVector3D& _scale)
	{
		m.x -= _scale.m.x;
		m.y -= _scale.m.y;
		m.z -= _scale.m.z;

		return *this;
	}

	GVector4D operator-=(const GVector4D& _scale)
	{
		m.x -= _scale.m.x;
		m.y -= _scale.m.y;
		m.z -= _scale.m.z;
		m.w -= _scale.m.w;

		return *this;
	}
#pragma endregion

#pragma region operator*
	GVector4D operator*(const float _scala) const
	{
		return { m.x * _scala, m.y * _scala, m.z * _scala };
	}

	GVector4D operator*(const GVector4D& _other) const
	{
		return GVector4D( m.x * _other.m.x, m.y * _other.m.y, m.z * _other.m.z, m.w * _other.m.w);
	}

	GVector4D& operator*=(const float _scale)
	{
		m.x *= _scale;
		m.y *= _scale;
		m.z *= _scale;
		return *this;
	}
#pragma endregion

#pragma region rotate
	GVector4D& RotateXByDeg(const float _deg)
	{
		return RotateXByRad(_deg * gconst::DegToRad);
	}

	GVector4D& RotateXByRad(const float _rad)
	{
		GVector4D tempAngle = *this;

		// 회전된 각도의 벡터로 변경된다.
		m.y = (tempAngle.m.y * cosf(_rad)) - (tempAngle.m.z * sinf(_rad));
		m.z = (tempAngle.m.y * sinf(_rad)) + (tempAngle.m.z * cosf(_rad));

		return *this;
	}

	GVector4D& RotateYByDeg(const float _deg)
	{
		return RotateYByRad(_deg * gconst::DegToRad);
	}

	GVector4D& RotateYByRad(const float _rad)
	{
		GVector4D tempAngle = *this;

		m.x = (tempAngle.m.x * cosf(_rad)) - (tempAngle.m.z * sinf(_rad));
		m.z = (tempAngle.m.x * sinf(_rad)) + (tempAngle.m.z * cosf(_rad));

		return *this;
	}

	GVector4D& RotateZByDeg(const float _deg)
	{
		return RotateZByRad(_deg * gconst::DegToRad);
	}

	GVector4D& RotateZByRad(const float _rad)
	{
		GVector4D tempAngle = *this;

		m.x = (tempAngle.m.x * cosf(_rad)) - (tempAngle.m.y * sinf(_rad));
		m.y = (tempAngle.m.x * sinf(_rad)) + (tempAngle.m.y * cosf(_rad));

		return *this;
	}

	GVector4D& RotateByDeg(const float _deg)
	{
		return RotateByRad(_deg * gconst::DegToRad);
	}

	GVector4D& RotateByRad(const float _rad)
	{
		RotateXByRad(_rad);
		RotateYByRad(_rad);
		RotateZByRad(_rad);

		return *this;
	}
#pragma endregion

	float operator[](const size_t _index)
	{
		return Arr[_index];
	}

	float operator()(const size_t _index)
	{
		return Arr[_index];
	}

	float operator[](const int _index)
	{
		return Arr[_index];
	}

	float operator()(const int _index)
	{
		return Arr[_index];
	}

	GVector4D& DegToDir(const float _deg)
	{
		return RadToDir(_deg * gconst::DegToRad);
	}

	GVector4D& RadToDir(const float _rad)
	{
		m.x = cosf(_rad);
		m.y = sinf(_rad);

		return *this;
	}

	POINT ToPOINT( )
	{
		POINT ReturnPoint = POINT( );
		ReturnPoint.x = (LONG)m.x;
		ReturnPoint.y = (LONG)m.y;
		return ReturnPoint;
	}

	float Magnitude(void)
	{
		return DirectX::XMVector3Length(dxVector).m128_f32[0];
		//return std::sqrtf((m.x * m.x) + (m.y * m.y) + (m.z * m.z)); // sqrtf
	}

	GVector4D Normalize(void)
	{
		dxVector = DirectX::XMVector4Normalize(dxVector);

		return (GVector4D(*this));
	}

	// - 외적은 서로가 역백터라면 zero vector
	static GVector4D Cross(const GVector4D& _left, const GVector4D& _right)
	{
		return DirectX::XMVector3Cross(_left.dxVector, _right.dxVector);
	}

	// - 내적
	static float DotProduct(const GVector4D& _left, const GVector4D& _right)
	{
		return DirectX::XMVector3Dot(_left.dxVector, _right.dxVector).m128_f32[0];
		//return (_left.m.x * _right.m.x) + (_left.m.y * _right.m.y) + (_left.m.z * _right.m.z);
	}

	void Reverse (void)
	{
		(*this) = -(*this);
	}

	void SetMemberInRange(const float _min = 0.0f, const float _max = 1.0f)
	{
		for ( auto& member : Arr )
		{
			if ( member < _min )
			{
				member = _min;
			}

			if ( member > _max )
			{
				member = _max;
			}
		}
	}

	UINT ToUIntRGBA( ) const
	{
		GVector4D color = GVector4D(*this);
		color.SetMemberInRange( );

		UINT resultColor = NULL;

		// convert processing
		unsigned char* resultColorArr = (unsigned char*)&resultColor;
		resultColorArr[0] = (unsigned char)(color.m.x * 255.0f);
		resultColorArr[1] = (unsigned char)(color.m.y * 255.0f);
		resultColorArr[2] = (unsigned char)(color.m.z * 255.0f);
		resultColorArr[3] = (unsigned char)(color.m.w * 255.0f);

		return resultColor;
	}
};