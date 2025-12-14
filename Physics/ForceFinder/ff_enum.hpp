#pragma once

namespace forcefinder 
{
	enum class ForceFinderOption : unsigned char
	{
		DIRECT,
		DIRECT_SIMD,
		DIRECT_GL,
		DIRECT_OPENCL,
		BARNES_HUT
	};

}