#pragma once

#include "PlatformEnum.hpp"

namespace sim
{
	struct SimState
	{
		size_t context_id = 0u;
		platform::PlatformOption platform = platform::PlatformOption::NO_STATE;
		platform::PlatformDataType data_type = platform::PlatformDataType::NO_STATE;
	};
}