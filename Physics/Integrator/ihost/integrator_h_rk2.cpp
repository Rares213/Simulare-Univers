#include "integrator_h.hpp"
#include <algorithm>

#include "Timer.hpp"

namespace integrator
{
	namespace ih
	{
		template<typename T>
		RungeKutta2<T>::RungeKutta2(IntegratorData<T>& data) : m_data(data)
		{
			size_t count = m_data.mass.size();

			k1_p = std::vector<std::vector<T>>(3u, std::vector<T>(count));
			k1_v = std::vector<std::vector<T>>(3u, std::vector<T>(count));
			k2_p = std::vector<std::vector<T>>(3u, std::vector<T>(count));
			k2_v = std::vector<std::vector<T>>(3u, std::vector<T>(count));
			pos_for_k2v = std::vector<std::vector<T>>(3u, std::vector<T>(count));
		}

		template<typename T>
		void RungeKutta2<T>::updatePosition(forcefinder::ffh::IForceFinder<T>& force_finder)
		{
			firstStage(force_finder);
			secondStage(force_finder);
		}

		template<typename T>
		void RungeKutta2<T>::firstStage(forcefinder::ffh::IForceFinder<T>& force_finder)
		{
			const std::vector<std::vector<T>>& vel = m_data.vel;
			const std::vector<std::vector<T>>& force = m_data.force;
			const std::vector<T>& mass = m_data.mass;

			const size_t count = m_data.mass.size();
			const T step_size = m_data.stepsize;

			k1_p = vel;

			force_finder.findForce();

			timer::Timer timer;

			for (size_t i = 0; i < count; i++)
			{
				k1_v[0][i] = force[0][i] / mass[i];
				k1_v[1][i] = force[1][i] / mass[i];
				k1_v[2][i] = force[2][i] / mass[i];
			}

			m_duration = timer.getDuration();
		}

		template<typename T>
		void RungeKutta2<T>::secondStage(forcefinder::ffh::IForceFinder<T>& force_finder)
		{
			std::vector<std::vector<T>>& pos   = m_data.pos;
			std::vector<std::vector<T>>& vel   = m_data.vel;
			std::vector<std::vector<T>>& force = m_data.force;
			const std::vector<T>& mass         = m_data.mass;

			const size_t count = mass.size();
			const T step_size = m_data.stepsize;
			const T c = static_cast<T>(0.5);

			{
				timer::Timer timer;

				for (size_t i = 0; i < count; i++)
				{
					k2_p[0][i] = vel[0][i] + step_size * k1_v[0][i];
					k2_p[1][i] = vel[1][i] + step_size * k1_v[1][i];
					k2_p[2][i] = vel[2][i] + step_size * k1_v[2][i];
				}

				for (size_t i = 0; i < count; i++)
				{
					pos_for_k2v[0][i] = pos[0][i] + step_size * k1_p[0][i];
					pos_for_k2v[1][i] = pos[1][i] + step_size * k1_p[1][i];
					pos_for_k2v[2][i] = pos[2][i] + step_size * k1_p[2][i];
				}

				std::fill(force[0].begin(), force[0].end(), 0);
				std::fill(force[1].begin(), force[1].end(), 0);
				std::fill(force[2].begin(), force[2].end(), 0);

				m_duration += timer.getDuration();
			}

			forcefinder::ffh::ForceFinderData<T> data;
			data.position = &pos_for_k2v;
			force_finder.setBodiesProperties(data);
			force_finder.findForce();
			data.position = &m_data.pos;
			force_finder.setBodiesProperties(data);

			timer::Timer timer;

			for (size_t i = 0; i < count; i++)
			{
				k2_v[0][i] = force[0][i] / mass[i];
				k2_v[1][i] = force[1][i] / mass[i];
				k2_v[2][i] = force[2][i] / mass[i];
			}

			for (size_t i = 0; i < count; i++)
			{
				vel[0][i] = vel[0][i] + step_size * (c * k1_v[0][i] + c * k2_v[0][i]);
				vel[1][i] = vel[1][i] + step_size * (c * k1_v[1][i] + c * k2_v[1][i]);
				vel[2][i] = vel[2][i] + step_size * (c * k1_v[2][i] + c * k2_v[2][i]);
			}

			for (size_t i = 0; i < count; i++)
			{
				pos[0][i] = pos[0][i] + step_size * (c * k1_p[0][i] + c * k2_p[0][i]);
				pos[1][i] = pos[1][i] + step_size * (c * k1_p[1][i] + c * k2_p[1][i]);
				pos[2][i] = pos[2][i] + step_size * (c * k1_p[2][i] + c * k2_p[2][i]);
			}

			m_duration += timer.getDuration();

		}


	}
}

template class integrator::ih::RungeKutta2<float>;
template class integrator::ih::RungeKutta2<double>;
