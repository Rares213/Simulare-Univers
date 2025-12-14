#include "integrator_h.hpp"

#include <algorithm>
#include "Timer.hpp"

namespace integrator
{
	namespace ih
	{
		template<typename T>
		LeapFrog<T>::LeapFrog(IntegratorData<T>& data)
			: m_data(data)
		{
			prev_acc = std::vector<std::vector<T>>(3, std::vector<T>(m_data.mass.size()));
		}

		template<typename T>
		void LeapFrog<T>::updatePosition(forcefinder::ffh::IForceFinder<T>& force_finder)
		{
			// algorithm used
			// x[i + 1] = x[i] + v[i] * stepSize + 0.5 * stepSize^2 * a[i]
			// v[i + 1] = v[i] + 0.5 * stepSize * (a[i] + a[i + 1])
			
			std::vector<std::vector<T>>& pos = m_data.pos;
			std::vector<std::vector<T>>& vel = m_data.vel;
			std::vector<std::vector<T>>& acc = m_data.acc;

			const std::vector<std::vector<T>>& force = m_data.force;
			const std::vector<T>& mass = m_data.mass;
			
			const size_t size = m_data.mass.size();
			const T step_size = m_data.stepsize;
			const T half_step_size_squared = 0.5 * step_size * step_size;
			const T half_step_size = step_size * 0.5;

			// doing x[i + 1] = x[i] + v[i] * stepSize + 0.5 * stepSize^2 * a[i]
			{
				timer::Timer timer;
				
				for (size_t i = 0; i < size; ++i)
					pos[0][i] = pos[0][i] + vel[0][i] * step_size + acc[0][i] * half_step_size_squared;

				for (size_t i = 0; i < size; ++i)
					pos[1][i] = pos[1][i] + vel[1][i] * step_size + acc[1][i] * half_step_size_squared;

				for (size_t i = 0; i < size; ++i)
					pos[2][i] = pos[2][i] + vel[2][i] * step_size + acc[2][i] * half_step_size_squared;

				// save current acc
				prev_acc = acc;

				m_duration = timer.getDuration();
			}
			force_finder.findForce();
			{
				timer::Timer timer;
				_findAcc();

				// doing v[i + 1] = v[i] + 0.5 * stepSize * (a[i] + a[i + 1])
				for (size_t i = 0; i < size; ++i)
					vel[0][i] = vel[0][i] + half_step_size * (prev_acc[0][i] + acc[0][i]);

				for (size_t i = 0; i < size; ++i)
					vel[1][i] = vel[1][i] + half_step_size * (prev_acc[1][i] + acc[1][i]);

				for (size_t i = 0; i < size; ++i)
					vel[2][i] = vel[2][i] + half_step_size * (prev_acc[2][i] + acc[2][i]);

				m_duration += timer.getDuration();
			}
			
		}

		template<typename T>
		void LeapFrog<T>::_findAcc()
		{
			std::vector<std::vector<T>>& acc = m_data.acc;

			const std::vector<std::vector<T>>& force = m_data.force;
			const std::vector<T>& mass = m_data.mass;

			const size_t size = m_data.mass.size();

			for (size_t i = 0; i < size; i++)
				acc[0][i] = force[0][i] / mass[i];

			for (size_t i = 0; i < size; i++)
				acc[1][i] = force[1][i] / mass[i];

			for (size_t i = 0; i < size; i++)
				acc[2][i] = force[2][i] / mass[i];
		}

		template class LeapFrog<float>;
		template class LeapFrog<double>;
	}
}