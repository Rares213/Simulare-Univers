#include "ff_host_g_abstract.hpp"


namespace forcefinder
{
	namespace ffh
	{
		namespace commfn
		{
			std::vector<WorkerData> getWorkersData(size_t count, size_t workers_count)
			{
				std::vector<WorkerData> workers_data(workers_count);

				if(workers_count > 0)
				{
					size_t count_per_thread = count / workers_count;
					size_t extra_count = count % workers_count;

					for (size_t i = 0; i < workers_count; i++)
					{
						workers_data[i].start = count_per_thread * i;
						workers_data[i].end = count_per_thread * (i + 1);
					}

					if (extra_count != 0)
						workers_data[workers_count - 1].end += extra_count;
				}

				return workers_data;
			}

		}
		
		template<typename T>
		AForceFinderGravity<T>::AForceFinderGravity(GravitySpec<T>& init_struct) 
			:
			m_gravity(init_struct),
			m_count(init_struct.mass->size()),
			m_workers_count(init_struct.workers)
		{

		}

		template<typename T>
		void AForceFinderGravity<T>::setBodiesProperties(ForceFinderData<T>& properties)
		{
			m_gravity.position = properties.position;
		}

		template class AForceFinderGravity<float>;
		template class AForceFinderGravity<double>;
	}
}


