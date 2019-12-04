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
	// - float 오차 허용치(Tolerance)
	constexpr float ftol = 0.0001f;
	constexpr float PI = 3.14159265359f;
	constexpr float RadToDeg = 180.f / PI;
	constexpr float DegToRad = PI / 180.f;

	// - TYPE별 무한으로 설정
	template<typename TYPE>
	constexpr auto inf = std::numeric_limits<TYPE>::infinity( );

	// - TYPE별 quiet NaN으로 설정
	template<typename TYPE>
	constexpr auto NaNquiet = std::numeric_limits<TYPE>::quiet_NaN( );

	// - TYPE별 signaling NaN으로 설정
	template<typename TYPE>
	constexpr auto NaNsignaling = std::numeric_limits<TYPE>::signaling_NaN( );

	// - TYPE별 무한으로 설정
	template<typename TYPE>
	constexpr auto max = std::numeric_limits<TYPE>::max( );

	// - TYPE별 최소값으로 설정
	template<typename TYPE>
	constexpr auto min = std::numeric_limits<TYPE>::min( );
}