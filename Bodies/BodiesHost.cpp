#include "BodiesHost.hpp"

#include "logger.hpp"

#include <algorithm>
#include <utility>

namespace body
{
	namespace hbody
	{
		template<typename T>
		Bodies<T>::Bodies(init::GenInitialConditions<T>& init_cond)
		{
			logger::Logger::info("initializing host bodies\ninitial conditions type\n");

			_init(init_cond);
		}

		template<typename T>
		Bodies<T>::Bodies(init::GenInitialConditions<T>&& init_cond)
		{
			init::GenInitialConditions<T> init = std::move(init_cond);
			_init(init);
		}

		template<typename T>
		void Bodies<T>::resize(init::GenInitialConditions<T>& initial_condition)
		{
			size_t number_bodies = initial_condition.get_count();

			m_properties.position[0].resize(number_bodies);
			m_properties.position[1].resize(number_bodies);
			m_properties.position[2].resize(number_bodies);

			m_properties.velocity[0].resize(number_bodies);
			m_properties.velocity[1].resize(number_bodies);
			m_properties.velocity[2].resize(number_bodies);

			m_properties.mass[0].resize(number_bodies);

			m_properties.acceleration[0].resize(number_bodies);
			m_properties.acceleration[1].resize(number_bodies);
			m_properties.acceleration[2].resize(number_bodies);

			m_properties.force[0].resize(number_bodies);
			m_properties.force[1].resize(number_bodies);
			m_properties.force[2].resize(number_bodies);

			if (m_count < number_bodies)
			{
				size_t diff_count = number_bodies - m_count;

				init::InitResult<T> init_result = initial_condition.get();

				std::copy(init_result.position[0].begin() + m_count, init_result.position[0].end(), m_properties.position[0].begin() + m_count);
				std::copy(init_result.position[2].begin() + m_count, init_result.position[2].end(), m_properties.position[2].begin() + m_count);
				std::copy(init_result.position[1].begin() + m_count, init_result.position[1].end(), m_properties.position[1].begin() + m_count);

				std::copy(init_result.velocity[0].begin() + m_count, init_result.velocity[0].end(), m_properties.velocity[0].begin() + m_count);
				std::copy(init_result.velocity[1].begin() + m_count, init_result.velocity[1].end(), m_properties.velocity[1].begin() + m_count);
				std::copy(init_result.velocity[2].begin() + m_count, init_result.velocity[2].end(), m_properties.velocity[2].begin() + m_count);

				std::copy(init_result.mass[0].begin() + m_count, init_result.mass[0].end(), m_properties.mass[0].begin() + m_count);
			}

			m_count = number_bodies;
		}

		template<typename T>
		void Bodies<T>::reset(init::GenInitialConditions<T>& initial_condition)
		{
			init::InitResult<T> init_result = initial_condition.get();

			m_properties.position = init_result.position;
			m_properties.velocity = init_result.velocity;
			m_properties.acceleration = init_result.acceleration;
			m_properties.force = init_result.force;
			m_properties.mass = init_result.mass;
		}

		template<typename T>
		CompressedGet Bodies<T>::get_properties()
		{
			CompressedGet prop_get(m_properties);

			return prop_get;
		}

		template<typename T>
		void Bodies<T>::reset_force()
		{
			std::fill(m_properties.force[0].begin(), m_properties.force[0].end(), 0);
			std::fill(m_properties.force[1].begin(), m_properties.force[1].end(), 0);
			std::fill(m_properties.force[2].begin(), m_properties.force[2].end(), 0);
		}

		template<typename T>
		PropertiesHost<T>& Bodies<T>::get_prop()
		{
			return m_properties;
		}

		template<typename T>
		void Bodies<T>::printDebug()
		{
			for (size_t i = 0; i < m_properties.position[0].size(); i++)
				std::cout << "p[" << i << "] = " << m_properties.position[0][i] << " " << m_properties.position[1][i] << "\n";
		}

		template<typename T>
		void Bodies<T>::_init(init::GenInitialConditions<T>& init_cond)
		{
			init::InitResult<T> init_result = init_cond.get();

			m_properties.position = init_result.position;
			m_properties.velocity = init_result.velocity;
			m_properties.acceleration = init_result.acceleration;
			m_properties.force = init_result.force;
			m_properties.mass = init_result.mass;

			m_count = init_result.count;
		}


		template class Bodies<float>;
		template class Bodies<double>;

	}

}
