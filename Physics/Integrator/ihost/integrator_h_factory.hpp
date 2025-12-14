#pragma once

#include "integrator_h.hpp"

namespace integrator
{	
	namespace ih
	{
		template<typename T>
		struct FactoryIntegratorData
		{
			const IntegratorOption& integrator_option;
			float& stepsize = 0;

			std::vector<std::vector<T>>& pos;
			std::vector<std::vector<T>>& vel;
			std::vector<std::vector<T>>& acc;
			std::vector<std::vector<T>>& force;
			std::vector<T>& mass;

		};
		
		template<typename T>
		class FactoryIntegrator
		{
		public:
			FactoryIntegrator(FactoryIntegratorData<T>& data) : m_data(data) {}
			virtual ~FactoryIntegrator() {}

			std::unique_ptr<IIntegrator<T>> createIntegrator()
			{
				IntegratorData<T> data{ m_data.pos, m_data.vel, m_data.acc, m_data.force, m_data.mass, m_data.stepsize };

				switch (m_data.integrator_option)
				{
				case IntegratorOption::EULER_EXPLICIT:
					return std::make_unique<EulerExplicit<T>>(data);

				case IntegratorOption::EULER_IMPLICIT:
					return std::make_unique<EulerImplicit<T>>(data);

				case IntegratorOption::RK2:
					return std::make_unique<RungeKutta2<T>>(data);
					
				case IntegratorOption::LEAPFROG:
					return std::make_unique<LeapFrog<T>>(data);

				default:
					return nullptr;
				}
			}

		private:

			FactoryIntegratorData<T> m_data;


		};

	}
}