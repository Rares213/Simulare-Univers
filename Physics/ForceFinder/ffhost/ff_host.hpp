#pragma once

#include <vector>


namespace forcefinder
{
	namespace ffh
	{

		template<typename T>
		struct ForceFinderData
		{
			std::vector<std::vector<T>>* position = nullptr;
			std::vector<std::vector<T>>* velocity = nullptr;
			std::vector<std::vector<T>>* acceleration = nullptr;
			std::vector<std::vector<T>>* force = nullptr;
			std::vector<T>* mass = nullptr;
		};

		//interface for host forces
		template<typename T>
		class IForceFinder
		{
		public:

			virtual ~IForceFinder() {}

			virtual void findForce() = 0;
			virtual void setBodiesProperties(ForceFinderData<T>& properties) = 0;
			virtual double getDuration() = 0;
		};

	}

}