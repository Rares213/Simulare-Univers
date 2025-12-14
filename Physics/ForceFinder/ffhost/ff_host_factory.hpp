#pragma once

#include "ff_host.hpp"
#include "ff_host_g_direct.hpp"
#include "ff_host_g_bh.hpp"

#include <memory>

namespace forcefinder
{
	namespace ffh
	{
		template<typename T>
		struct FactoryGravityData
		{
			float& G;
			float& soft;
			size_t& workers;

			std::vector<std::vector<T>>& position;
			std::vector<std::vector<T>>& force;
			std::vector<T>& mass;
		};

		template<typename T>
		struct FactoryBarnesHutGravityData
		{
			FactoryGravityData<T> abstract_data;
			float& theta;
		};


		template<typename T>
		class IFactoryForceFinder
		{
		public:
			
			virtual ~IFactoryForceFinder() {}

			virtual std::unique_ptr<IForceFinder<T>> createForceFinder() = 0;
		};

		template<typename T>
		class FactoryForceFinderGravityDirect : public IFactoryForceFinder<T>
		{
		public:

			FactoryForceFinderGravityDirect(FactoryGravityData<T>& data) : m_data(data) {}

			virtual ~FactoryForceFinderGravityDirect() {}
			
			virtual std::unique_ptr<IForceFinder<T>> createForceFinder() override
			{
				GravitySpec<T> data
				{
					m_data.G,
					m_data.soft,
					m_data.workers,
					&m_data.position,
					&m_data.force,
					&m_data.mass
				};

				return std::make_unique<ffh::ForceFinderGravityDirect<T>>(data);
			}

		private:

			FactoryGravityData<T> m_data;
		};

		template<typename T>
		class FactoryForceFindertGravityDirectSIMD : public IFactoryForceFinder<T>
		{
		public:

			FactoryForceFindertGravityDirectSIMD(FactoryGravityData<T>& data) : m_data(data) {}

			virtual ~FactoryForceFindertGravityDirectSIMD() {}

			virtual std::unique_ptr<IForceFinder<T>> createForceFinder() override
			{
				GravitySpec<T> data
				{
					m_data.G,
					m_data.soft,
					m_data.workers,
					&m_data.position,
					&m_data.force,
					&m_data.mass
				};
				
				return std::make_unique<ForceFinderGravityDirectSIMD<T>>(data);
			}

		private:

			FactoryGravityData<T> m_data;
		};

		template<typename T>
		class FactoryForceFinderGravityBH : public IFactoryForceFinder<T>
		{
		public:

			FactoryForceFinderGravityBH(FactoryBarnesHutGravityData<T>& data) : m_data(data) {}
			virtual ~FactoryForceFinderGravityBH() {}

			virtual std::unique_ptr<IForceFinder<T>> createForceFinder() override
			{
				GravitySpec<T> gravity_data
				{
					m_data.abstract_data.G,
					m_data.abstract_data.soft,
					m_data.abstract_data.workers,
					&m_data.abstract_data.position,
					&m_data.abstract_data.force,
					&m_data.abstract_data.mass
				};

				BarnesHutGravSpec<T> bh_data
				{
					gravity_data,
					m_data.theta
				};

				return std::make_unique<ForceFinderGravityBH<T>>(bh_data);
			}

		private:

			FactoryBarnesHutGravityData<T> m_data;
		};
	}
}