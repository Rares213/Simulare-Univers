#pragma once

#include "ForceFinder/ffgl/ff_gl_factory.hpp"
#include "Integrator/igl/integrator_gl_factory.hpp"

namespace phys
{
	template<typename T>
	class PhysicsGL
	{
	public:
		PhysicsGL(std::unique_ptr<forcefinder::ffgl::IFactoryForceFinder<T>> forcefinder_factory, std::unique_ptr<integrator::igl::FactoryIntegrator<T>> integrator_factory)
		{
			m_forcefinder_factory = std::move(forcefinder_factory);
			m_integrator_factory = std::move(integrator_factory);

			m_forcefinder = m_forcefinder_factory->createForceFinder();
			m_integrator = m_integrator_factory->createIntegrator();
		}

		void setForceFinderFactory(std::unique_ptr<forcefinder::ffgl::IFactoryForceFinder<T>> forcefinder_factory)
		{
			m_forcefinder_factory = std::move(forcefinder_factory);
			m_forcefinder = m_forcefinder_factory->createForceFinder();
		}

		void rebuildForceFinder() { m_forcefinder = m_forcefinder_factory->createForceFinder(); }
		void rebuildIntegrator() { m_integrator = m_integrator_factory->createIntegrator(); }

		void runPhysics() { m_integrator->updatePosition(*m_forcefinder.get()); }

		double getForcefinderDelta() { return m_forcefinder->getDuration(); }
		double getIntegratorDelta() { return m_integrator->getDuration(); }

	private:

		std::unique_ptr<forcefinder::ffgl::IFactoryForceFinder<T>> m_forcefinder_factory;
		std::unique_ptr<integrator::igl::FactoryIntegrator<T>> m_integrator_factory;

		std::unique_ptr<forcefinder::ffgl::IForceFinder> m_forcefinder;
		std::unique_ptr<integrator::igl::IIntegrator> m_integrator;

	};

	template class PhysicsGL<GLfloat>;
}