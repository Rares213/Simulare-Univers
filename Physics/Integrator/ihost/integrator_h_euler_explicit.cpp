#include "integrator_h.hpp"

#include "Timer.hpp"

namespace integrator
{
	namespace ih
	{
		template<typename T>
		void EulerExplicit<T>::updatePosition(forcefinder::ffh::IForceFinder<T>& force_finder)
		{
			force_finder.findForce();
			
			std::vector<std::vector<T>>& m_pos   = m_data.pos;
			std::vector<std::vector<T>>& m_vel   = m_data.vel;
			std::vector<std::vector<T>>& m_acc   = m_data.acc;
			std::vector<std::vector<T>>& m_force = m_data.force;
			std::vector<T>& m_mass  = m_data.mass;

			const size_t count = m_data.mass.size();
			const T step_size = m_data.stepsize;

			timer::Timer timer;

			for (size_t i = 0; i < count; i++)
			{
				m_acc[0][i] = m_force[0][i] / m_mass[i];
				m_acc[1][i] = m_force[1][i] / m_mass[i];
				m_acc[2][i] = m_force[2][i] / m_mass[i];
			}

			for (size_t i = 0; i < count; i++)
			{
				m_pos[0][i] += step_size * m_vel[0][i];
				m_pos[1][i] += step_size * m_vel[1][i];
				m_pos[2][i] += step_size * m_vel[2][i];
			}

			for (size_t i = 0; i < count; i++)
			{
				m_vel[0][i] += step_size * m_acc[0][i];
				m_vel[1][i] += step_size * m_acc[1][i];
				m_vel[2][i] += step_size * m_acc[2][i];
			}

			m_duration = timer.getDuration();
		}

	}
}

template class integrator::ih::EulerExplicit<float>;
template class integrator::ih::EulerExplicit<double>;