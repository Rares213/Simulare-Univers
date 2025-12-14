#pragma once

#include <chrono>

namespace timer
{
	class Timer
	{
	public:
		Timer();
		double getDuration();

	private:
		std::chrono::time_point<std::chrono::steady_clock> start;
	};
}
