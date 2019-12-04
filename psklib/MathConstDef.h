#pragma once
#include <limits>
#include <cmath>
//#include <utility>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>

#undef max
#undef min

namespace gconst
{
	// - float ���� ���ġ(Tolerance)
	constexpr float ftol = 0.0001f;
	constexpr float PI = 3.14159265359f;
	constexpr float RadToDeg = 180.f / PI;
	constexpr float DegToRad = PI / 180.f;

	// - TYPE�� �������� ����
	template<typename TYPE>
	constexpr auto inf = std::numeric_limits<TYPE>::infinity( );

	// - TYPE�� quiet NaN���� ����
	template<typename TYPE>
	constexpr auto NaNquiet = std::numeric_limits<TYPE>::quiet_NaN( );

	// - TYPE�� signaling NaN���� ����
	template<typename TYPE>
	constexpr auto NaNsignaling = std::numeric_limits<TYPE>::signaling_NaN( );

	// - TYPE�� �������� ����
	template<typename TYPE>
	constexpr auto max = std::numeric_limits<TYPE>::max( );

	// - TYPE�� �ּҰ����� ����
	template<typename TYPE>
	constexpr auto min = std::numeric_limits<TYPE>::min( );
}