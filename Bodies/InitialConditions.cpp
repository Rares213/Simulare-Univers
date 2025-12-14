#include "InitialConditions.hpp"

#include <random>
#include <cmath>

#include "glad/glad.h"

namespace body
{
	namespace init
	{
		template<typename T>
		GenInitialConditions<T>::GenInitialConditions(InitialConditions init_type, size_t count) : m_initial_cond(init_type, count)
		{
			_init();
		}

		template<typename T>
		init::GenInitialConditions<T>::GenInitialConditions(InitResult<T> custom_init) : m_initial_cond(custom_init) {}
	
		template<typename T>
		void GenInitialConditions<T>::_init()
		{
			switch (m_initial_cond.init_type)
			{
			case InitialConditions::STATIC:
				_static();
				break;

			case InitialConditions::SPIRAL:
				_spiral();
				break;

			case InitialConditions::KEPLER:
				_velocity_spiral();
				break;

			case InitialConditions::TEST_BH:
				_test_bh();
			}
		}

		template<typename T>
		void GenInitialConditions<T>::_static()
		{
			_static_explicit(3u);
		}

		template<typename T>
		void GenInitialConditions<T>::_spiral()
		{
			_static_explicit(2u);

			m_initial_cond.mass[0][0] = 100000;
			m_initial_cond.position[0][0] = 0;
			m_initial_cond.position[1][0] = 0;
			m_initial_cond.position[2][0] = 0;



			_velocity_spiral();
		}

		template<typename T>
		void GenInitialConditions<T>::_test_bh()
		{
			m_initial_cond.position[0][0] = 0;
			m_initial_cond.position[1][0] = 0;
			m_initial_cond.position[2][0] = 0;

			m_initial_cond.position[0][1] = 2;
			m_initial_cond.position[1][1] = 6;
			m_initial_cond.position[2][1] = 2;

			m_initial_cond.position[0][2] = 5;
			m_initial_cond.position[1][2] = 5;
			m_initial_cond.position[2][2] = 5;

			m_initial_cond.position[0][3] = 6;
			m_initial_cond.position[1][3] = 2;
			m_initial_cond.position[2][3] = 3;
		}

		template<typename T>
		void GenInitialConditions<T>::_static_explicit(size_t number_dimensions)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<T> dis(0, 1);

			for (size_t i = 0; i < number_dimensions; i++)
				for (size_t j = 0; j < m_initial_cond.count; j++)
					m_initial_cond.position[i][j] = dis(gen) * 50 - 25;

			_init_mass();
		}

		template<typename T>
		void GenInitialConditions<T>::_velocity_spiral()
		{
			for (size_t i = 1; i < m_initial_cond.count; i++)
			{
				m_initial_cond.velocity[0][i] = -std::sqrt(m_initial_cond.mass[0][0] / std::hypot(m_initial_cond.position[0][i], m_initial_cond.position[1][i]))
					* std::sin(std::atan2(m_initial_cond.position[1][i], m_initial_cond.position[0][i]));

				m_initial_cond.velocity[1][i] =  std::sqrt(m_initial_cond.mass[0][0] / std::hypot(m_initial_cond.position[0][i], m_initial_cond.position[1][i]))
					* std::cos(std::atan2(m_initial_cond.position[1][i], m_initial_cond.position[0][i]));
			}
		}

		template<typename T>
		void GenInitialConditions<T>::_init_mass()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<T> dis(0, 1);

			for (size_t i = 0; i < m_initial_cond.count; i++)
				m_initial_cond.mass[0][i] = dis(gen) * 10 + 1;
		}

		template<typename T>
		InitResult<T>::InitResult(InitialConditions init_type, size_t count)
		{
			position     = std::vector<std::vector<T>>(3, std::vector<T>(count, 0));
			velocity     = std::vector<std::vector<T>>(3, std::vector<T>(count, 0));
			acceleration = std::vector<std::vector<T>>(3, std::vector<T>(count, 0));
			force        = std::vector<std::vector<T>>(3, std::vector<T>(count, 0));
			mass         = std::vector<std::vector<T>>(1, std::vector<T>(count, 0));

			this->count = count;
			this->init_type = init_type;
		}

		template class GenInitialConditions<float>;
		template class GenInitialConditions<double>;

		template class GenInitialConditions<GLfloat>;
		template class GenInitialConditions<GLdouble>;
	}
}
