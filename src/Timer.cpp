#include "Timer.hpp"

namespace timer
{
	using namespace std::chrono_literals;
	
	Timer::Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	double Timer::getDuration()
	{
		std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;

		double duration_ms = duration.count() * 1000.0;
		return duration_ms;
	}


}