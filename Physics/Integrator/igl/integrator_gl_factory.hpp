#pragma once

#include "integrator_gl.hpp"
#include "../integrator_enum.hpp"

#include <memory>

namespace integrator
{
	namespace igl
	{
		template<typename T>
		struct FactoryIntegratorData
		{
			IntegratorOption& integrator_option;
			float& step_size;
			size_t& count;

			std::array<core::VertexBuffer, 3u>& vbo_position;
			std::array<core::VertexBuffer, 3u>& vbo_velocity;
			std::array<core::VertexBuffer, 3u>& vbo_acceleration;
			std::array<core::VertexBuffer, 3u>& vbo_force;
			core::VertexBuffer& vbo_mass;

		};
		
		template<typename T>
		class FactoryIntegrator
		{
		public:
			FactoryIntegrator(FactoryIntegratorData<T> data) : m_data(data) {}
			virtual ~FactoryIntegrator() {}
			
			std::unique_ptr<IIntegrator> createIntegrator()
			{
				IntegratorData<T> data
				{
					m_data.count,
					m_data.step_size,
					m_data.vbo_position,
					m_data.vbo_velocity,
					m_data.vbo_acceleration,
					m_data.vbo_force,
					m_data.vbo_mass
				};
				
				switch (m_data.integrator_option)
				{
				case IntegratorOption::EULER_IMPLICIT_GL:
					return std::make_unique<EulerImplicit<T>>(data);

				case IntegratorOption::LEAPFROG_GL:
					return std::make_unique<LeapFrog<T>>(data);
					
				default:
					return nullptr;
				}
			}

		private:
			FactoryIntegratorData<T> m_data;

		};
		
		template class FactoryIntegrator<float>;
		template class FactoryIntegrator<double>;
	}
}
