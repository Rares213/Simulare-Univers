#pragma once

#include "ff_host.hpp"
#include "TaskManager.hpp"

namespace forcefinder
{
	namespace ffh
	{
		struct WorkerData
		{
			size_t start = 0;
			size_t end = 0;
		};

		template<typename T>
		struct GravitySpec
		{
			T G = 0;
			T soft = 0;
			size_t workers = 0;

			std::vector<std::vector<T>>* position = nullptr;
			std::vector<std::vector<T>>* force = nullptr;
			std::vector<T>* mass = nullptr;
		};
		
		namespace commfn
		{
			std::vector<WorkerData> getWorkersData(size_t count, size_t workers_count);
		}
		
		//abstract class for host gravity
		template<typename T>
		class AForceFinderGravity : public IForceFinder<T>
		{
		public:
			AForceFinderGravity(GravitySpec<T>& init_struct);
			virtual ~AForceFinderGravity() {}

			virtual void setBodiesProperties(ForceFinderData<T>& properties) override;

		protected:

			const size_t m_count;
			const size_t m_workers_count;

			double m_duration = 0;

			GravitySpec<T> m_gravity;
			task::TaskManager m_task_manager;

		};
	}
	
}
