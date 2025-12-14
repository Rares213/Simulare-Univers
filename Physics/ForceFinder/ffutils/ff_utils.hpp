#pragma once

#include <string>
#include <cmath>

namespace forcefinder
{
	namespace ffmath
	{
		template<typename T>
		T square(T number) { return (number * number); }

		template<typename T>
		T power_3over2(T number) { return (number * std::sqrt(number)); }
	}
}
