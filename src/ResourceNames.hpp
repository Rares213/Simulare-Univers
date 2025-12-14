#pragma once

namespace nbody
{
	// compute shaders name
	namespace csname
	{
		constexpr const char* euler_implicit_f32 = "eicf32";
		constexpr const char* euler_implicit_f64 = "eicf64";

		constexpr const char* acc_f32 = "accf32";
		constexpr const char* acc_f64 = "accf64";

		constexpr const char* leapfrog_first_stage_f32 = "lffsf32";
		constexpr const char* leapfrog_first_stage_f64 = "lffsf64";

		constexpr const char* leapfrog_second_stage_f32 = "lfssf32";
		constexpr const char* leapfrog_second_stage_f64 = "lfssf64";

		constexpr const char* forcefinder_f32 = "fff32";
		constexpr const char* forcefinder_f64 = "fff64";

	}
}