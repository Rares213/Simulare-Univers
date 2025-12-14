#pragma once

#include "Core.hpp"
#include "EventGui.hpp"

#include "PlatformEnum.hpp"

namespace sim
{
	class ISimulation : public core::ILayer, public gui::Observer
	{
	public:
		virtual ~ISimulation() {}

		virtual platform::PlatformOption getPlatformType() = 0;
		virtual platform::PlatformDataType getPlatformDataType() = 0;
	};

}