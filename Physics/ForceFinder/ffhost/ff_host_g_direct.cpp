#include "ff_host_g_direct.hpp"

#include <cmath>
#include "Timer.hpp"

#include "../ffutils/ff_utils.hpp"

namespace forcefinder
{
	namespace ffh
	{
		template<typename T>
		ForceFinderGravityDirect<T>::ForceFinderGravityDirect(GravitySpec<T>& init_stuct) : AForceFinderGravity<T>(init_stuct)
		{
			std::vector<WorkerData> workers_data = commfn::getWorkersData(this->m_count, this->m_workers_count);

			for (size_t i = 0; i < workers_data.size(); i++)
			{
				std::function<void(void)> worker_fun = std::bind(&ForceFinderGravityDirect<T>::_gravity, this, workers_data[i].start, workers_data[i].end);
				this->m_task_manager.add_task(i, std::move(worker_fun), true);
			}
		}
	
		template<typename T>
		void ForceFinderGravityDirect<T>::findForce()
		{
			timer::Timer timer;
			
			if (this->m_workers_count == 0)
				_gravity(0, this->m_count);
			else
			{
				for (size_t i = 0; i < this->m_workers_count; i++)
					this->m_task_manager.launch_task(i);

				this->m_task_manager.wait_all_finished();
			}

			this->m_duration = timer.getDuration();
		}

		template<typename T>
		void ForceFinderGravityDirect<T>::_gravity(size_t start, size_t end)
		{
			_gravityExplicit(start, end, 0, this->m_count);
		}

		template<typename T>
		void ForceFinderGravityDirect<T>::_gravityExplicit(size_t start, size_t end, size_t interacting_start, size_t interacting_end)
		{
			using std::vector;

			const vector<T>& x = this->m_gravity.position->operator[](0);
			const vector<T>& y = this->m_gravity.position->operator[](1);
			const vector<T>& z = this->m_gravity.position->operator[](2);

			const vector<T>& mass = *this->m_gravity.mass;

			vector<T>& fx = this->m_gravity.force->operator[](0);
			vector<T>& fy = this->m_gravity.force->operator[](1);
			vector<T>& fz = this->m_gravity.force->operator[](2);

			const T G = this->m_gravity.G;
			const T soft = this->m_gravity.soft;

			T distance = 0, prod = 0, accum_fx, accum_fy, accum_fz;

			for (size_t i = start; i < end; i++)
			{
				accum_fx = accum_fy = accum_fz = 0.0;

				for (size_t j = interacting_start; j < interacting_end; j++)
				{
					distance = ffmath::square(x[i] - x[j]) + ffmath::square(y[i] - y[j]) + ffmath::square(z[i] - z[j]) + soft;
					distance = ffmath::power_3over2(distance);

					prod = -G * mass[i] * mass[j] / distance;

					accum_fx += prod * (x[i] - x[j]);
					accum_fy += prod * (y[i] - y[j]);
					accum_fz += prod * (z[i] - z[j]);
				}

				fx[i] += accum_fx;
				fy[i] += accum_fy;
				fz[i] += accum_fz;
			}
		}

	}

}

template class forcefinder::ffh::ForceFinderGravityDirect<float>;
template class forcefinder::ffh::ForceFinderGravityDirect<double>;