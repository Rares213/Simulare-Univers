#pragma once

namespace integrator
{
	enum class IntegratorOption
	{
		EULER_EXPLICIT,
		EULER_IMPLICIT,
		RK2,
		LEAPFROG,
		EULER_IMPLICIT_GL,
		LEAPFROG_GL
	};
}
