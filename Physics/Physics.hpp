#pragma once

#include "ForceFinder/ffhost/ff_host_factory.hpp"
#include "Integrator/ihost/integrator_h_factory.hpp"

#include <memory>

namespace phys
{
	template<typename T>
	class Physics
	{
	public:
		Physics(std::unique_ptr<forcefinder::ffh::IFactoryForceFinder<T>> forcefinder_factory, std::unique_ptr<integrator::ih::FactoryIntegrator<T>> integrator_factory)
		{
			m_forcefinder_factory = std::move(forcefinder_factory);
			m_integrator_factory = std::move(integrator_factory);

			m_forcefinder = m_forcefinder_factory->createForceFinder();
			m_integrator = m_integrator_factory->createIntegrator();
		}

		void setForceFinderFactory(std::unique_ptr<forcefinder::ffh::IFactoryForceFinder<T>> forcefinder_factory) 
		{ 
			m_forcefinder_factory = std::move(forcefinder_factory); 
			m_forcefinder = m_forcefinder_factory->createForceFinder();
		}

		void setIntegratorFactory(std::unique_ptr<integrator::ih::FactoryIntegrator<T>> integrator_factory) 
		{ 
			m_integrator_factory = std::move(integrator_factory); 
			m_integrator = m_integrator_factory->createIntegrator();
		}

		void rebuildForceFinder() { m_forcefinder = m_forcefinder_factory->createForceFinder(); }
		void rebuildIntegrator() { m_integrator = m_integrator_factory->createIntegrator(); }

		void runPhysics() { m_integrator->updatePosition(*m_forcefinder.get()); }

		double getForcefinderDelta() { return m_forcefinder->getDuration(); }
		double getIntegratorDelta() { return m_integrator->getDuration(); }

	private:

		std::unique_ptr<forcefinder::ffh::IFactoryForceFinder<T>> m_forcefinder_factory;
		std::unique_ptr<integrator::ih::FactoryIntegrator<T>> m_integrator_factory;

		std::unique_ptr<forcefinder::ffh::IForceFinder<T>> m_forcefinder;
		std::unique_ptr<integrator::ih::IIntegrator<T>> m_integrator;

	};

	template class Physics<float>;
	template class Physics<double>;
}