#pragma once

#include "GVector.h"
#include "GMatrix.h"

namespace my_math
{
	inline constexpr UINT gcd(UINT n1, UINT n2)
	{
		UINT tmp = 0;
		while ( n2 != 0 )
		{
			tmp = n1;
			n1 = n2;
			n2 = tmp % n2;
		}
		return n1;
	}

	inline constexpr UINT lcm(UINT n1, UINT n2)
	{
		return (n1 * n2) / gcd(n1, n2);
	}
}