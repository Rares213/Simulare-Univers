#pragma once

#include "../../ForceFinder/ffhost/ff_host.hpp"

#include <vector>

namespace integrator
{
	namespace ih
	{
		template<typename T>
		struct IntegratorData
		{
			std::vector<std::vector<T>>& pos;
			std::vector<std::vector<T>>& vel;
			std::vector<std::vector<T>>& acc;
			std::vector<std::vector<T>>& force;
			std::vector<T>& mass;

			T stepsize = 0;
		};
		
		template<typename T>
		class IIntegrator
		{
		public:

			virtual ~IIntegrator() {}
			
			virtual void updatePosition(forcefinder::ffh::IForceFinder<T>& force_finder) = 0;
			virtual double getDuration() = 0;
		};
		
		template<typename T>
		class EulerExplicit : public IIntegrator<T>
		{
		public:

			EulerExplicit(IntegratorData<T>& data) : m_data(data) {}
			virtual ~EulerExplicit() {}

			virtual void updatePosition(forcefinder::ffh::IForceFinder<T>& force_finder) override;
			virtual double getDuration() override { return m_duration; }

		private:

			double m_duration = 0;
			IntegratorData<T> m_data;
		};

		template<typename T>
		class EulerImplicit : public IIntegrator<T>
		{
		public:

			EulerImplicit(IntegratorData<T>& data) : m_data(data) {}
			virtual ~EulerImplicit() {}

			virtual void updatePosition(forcefinder::ffh::IForceFinder<T>& force_finder) override;
			virtual double getDuration() override { return m_duration; }

		private:

			double m_duration = 0;
			IntegratorData<T> m_data;
		};

		/* Heun's method */
		template<typename T>
		class RungeKutta2 : public IIntegrator<T>
		{
		public:

			RungeKutta2(IntegratorData<T>& data);
			virtual ~RungeKutta2() {}

			virtual void updatePosition(forcefinder::ffh::IForceFinder<T>& force_finder) override;
			virtual double getDuration() override { return m_duration; }

		private:

			double m_duration = 0;

			void firstStage(forcefinder::ffh::IForceFinder<T>& force_finder);
			void secondStage(forcefinder::ffh::IForceFinder<T>& force_finder);

			IntegratorData<T> m_data;

			std::vector<std::vector<T>> k1_p;
			std::vector<std::vector<T>> k1_v;

			std::vector<std::vector<T>> k2_p;
			std::vector<std::vector<T>> k2_v;

			std::vector<std::vector<T>> pos_for_k2v;
		};

		template<typename T>
		class LeapFrog : public IIntegrator<T>
		{
		public:
			LeapFrog(IntegratorData<T>& data);
			virtual ~LeapFrog() {}

			virtual void updatePosition(forcefinder::ffh::IForceFinder<T>& force_finder) override;
			virtual double getDuration() override { return m_duration; }

		private:

			double m_duration = 0;
			IntegratorData<T> m_data;
			std::vector<std::vector<T>> prev_acc;

			void _findAcc();


		};

	}

}
