#pragma once

#include "ForceFinder/ff_enum.hpp"
#include "Integrator/integrator_enum.hpp"
#include "InitialConditions.hpp"
#include "PlatformEnum.hpp"

namespace ps
{	

	struct ProgramState
	{
		platform::PlatformDataType platform_type = platform::PlatformDataType::FLOAT;
		platform::PlatformOption platform_option = platform::PlatformOption::HOST;
		
		forcefinder::ForceFinderOption forcefinder_option = forcefinder::ForceFinderOption::BARNES_HUT;
		float G = 1.0f;
		float soft = 0.1f;
		float theta = 1.0f;
		size_t workers_count = 0u;
		
		integrator::IntegratorOption integrator_option = integrator::IntegratorOption::EULER_IMPLICIT_GL;
		float stepsize = 0.01f;

		body::init::InitialConditions init_cond_option = body::init::InitialConditions::SPIRAL;
		size_t number_bodies = 50000;

		double focefinder_delta = 0;
		double integrator_delta = 0;

	};
}