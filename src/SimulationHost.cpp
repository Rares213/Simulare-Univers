#include "SimulationHost.hpp"

#include "ResourceManager.hpp"

#include <iostream>

namespace sim
{
	namespace ids
	{
		constexpr size_t ID_PHYSICS = 0u;
	}
	
	template<typename T>
	SimulationHost<T>::SimulationHost(ps::ProgramState& program_state)
		: 
		m_id(core::genID()),
		m_program_state(program_state),
		m_bodies(std::move(_createInitialConditions())),
		m_physics(_createFactoryForceFinder(), _createFactoryIntegrator()),
		m_circle(core::CircleSpec<T>{m_bodies.get_prop().position})
	{
		m_task_manager.add_task(ids::ID_PHYSICS, std::bind(&phys::Physics<T>::runPhysics, &m_physics), true);
	}

	template<typename T>
	SimulationHost<T>::~SimulationHost()
	{
		m_task_manager.wait_all_finished();
	}

	template<typename T>
	platform::PlatformOption SimulationHost<T>::getPlatformType()
	{
		return platform::PlatformOption::HOST;
	}

	template<typename T>
	platform::PlatformDataType SimulationHost<T>::getPlatformDataType()
	{
		return _templateTypeToPlatformType();
	}

	template<typename T>
	void SimulationHost<T>::event(core::Event& event)
	{
		switch (event.getType())
		{
		case core::EventType::KeyPressed:
			core::EventKeyPressed& key_pressed = static_cast<core::EventKeyPressed&>(event);
			if (key_pressed.getKey() == GLFW_KEY_P)
			{
				if (m_paused == false)
					m_paused = true;
				else
					m_paused = false;
			}

		}

	}

	template<typename T>
	void SimulationHost<T>::update()
	{
		if(m_task_manager.check_task_finished(ids::ID_PHYSICS) && m_paused == true)
		{
			m_bodies.reset_force();
			_handleQueuedGuiEvents();

			m_program_state.focefinder_delta = m_physics.getForcefinderDelta();
			m_program_state.integrator_delta = m_physics.getIntegratorDelta();
			
			m_task_manager.launch_task(0);

		}
	}

	template<typename T>
	void SimulationHost<T>::draw()
	{
		core::submitDrawable(m_circle);
	}

	template<typename T>
	size_t SimulationHost<T>::getID()
	{
		return m_id;
	}

	template<typename T>
	void SimulationHost<T>::update(gui::Event& event)
	{
		if (m_task_manager.check_task_finished(ids::ID_PHYSICS))
			_handleGuiEvents(event);
		else
			_storeEvent(event);
	}


	template<typename T>
	body::init::GenInitialConditions<T> SimulationHost<T>::_createInitialConditions()
	{
		return body::init::GenInitialConditions<T>(m_program_state.init_cond_option, m_program_state.number_bodies);
	}

	template<typename T>
	std::unique_ptr<forcefinder::ffh::IFactoryForceFinder<T>> SimulationHost<T>::_createFactoryForceFinder()
	{
		body::PropertiesHost<T>& prop = m_bodies.get_prop();
		
		forcefinder::ffh::FactoryGravityData<T> factory_gravity_spec
		{
			m_program_state.G,
			m_program_state.soft,
			m_program_state.workers_count,
			prop.position,
			prop.force,
			prop.mass[0]
		};

		switch (m_program_state.forcefinder_option)
		{
		case forcefinder::ForceFinderOption::DIRECT:
			return std::make_unique<forcefinder::ffh::FactoryForceFinderGravityDirect<T>>(factory_gravity_spec);

		case forcefinder::ForceFinderOption::DIRECT_SIMD:
			return std::make_unique<forcefinder::ffh::FactoryForceFindertGravityDirectSIMD<T>>(factory_gravity_spec);

		case forcefinder::ForceFinderOption::BARNES_HUT:
		{
			forcefinder::ffh::FactoryBarnesHutGravityData<T> factory_bh{ factory_gravity_spec, m_program_state.theta};
			
			return std::make_unique<forcefinder::ffh::FactoryForceFinderGravityBH<T>>(factory_bh);
		}

		default:
			return nullptr;
		}
	}

	template<typename T>
	std::unique_ptr<integrator::ih::FactoryIntegrator<T>> SimulationHost<T>::_createFactoryIntegrator()
	{
		body::PropertiesHost<T>& prop = m_bodies.get_prop();

		integrator::ih::FactoryIntegratorData<T> factory_data
		{
			m_program_state.integrator_option,
			m_program_state.stepsize,
			prop.position,
			prop.velocity,
			prop.acceleration,
			prop.force,
			prop.mass[0]
		};

		return std::make_unique<integrator::ih::FactoryIntegrator<T>>(factory_data);
	}

	template<typename T>
	void SimulationHost<T>::_handleGuiEvents(gui::Event& event)
	{
		switch (event.getType())
		{
		case gui::EventType::EventGravity:
		case gui::EventType::EventSoft:
		case gui::EventType::EventWorkersCount:
			m_physics.rebuildForceFinder();
			break;

		case gui::EventType::EventForcefinder:
			m_physics.setForceFinderFactory(_createFactoryForceFinder());
			break;

		case gui::EventType::EventStepsize:
		case gui::EventType::EventIntegrator:
			m_physics.rebuildIntegrator();
			break;

		case gui::EventType::EventNumberBodies:
		{
			body::init::GenInitialConditions<T> init_cond = _createInitialConditions();
			m_bodies.resize(init_cond);
			m_circle.resize(m_program_state.number_bodies);
			m_physics.rebuildForceFinder();
			m_physics.rebuildIntegrator();
			break;
		}

		case gui::EventType::EventResetBodies:
		{
			body::init::GenInitialConditions<T> init_cond = _createInitialConditions();
			m_bodies.reset(init_cond);
			break;
		}

		case gui::EventType::EventShapeSize:
		{
			gui::EventShapeSize& shape_size_event = dynamic_cast<gui::EventShapeSize&>(event);

			m_circle.setRadius(shape_size_event.getShapeSize());
			break;
		}
		}


	}

	template<typename T>
	void SimulationHost<T>::_handleQueuedGuiEvents()
	{
		while (!m_gui_events.empty())
		{
			_handleGuiEvents(*m_gui_events.front().get());
			m_gui_events.pop();
		}
	}

	template<typename T>
	void SimulationHost<T>::_storeEvent(gui::Event& event)
	{
		try
		{
			switch (event.getType())
			{
			case gui::EventType::EventGravity:
			{
				gui::EventGravity& derived_event = dynamic_cast<gui::EventGravity&>(event);

				m_gui_events.push(std::make_unique<gui::EventGravity>(derived_event));
				break;
			}
			case gui::EventType::EventSoft:
			{
				gui::EventSoft& derived_event = dynamic_cast<gui::EventSoft&>(event);

				m_gui_events.push(std::make_unique<gui::EventSoft>(derived_event));
				break;
			}
			case gui::EventType::EventWorkersCount:
			{
				gui::EventWorkersCount& derived_event = dynamic_cast<gui::EventWorkersCount&>(event);

				m_gui_events.push(std::make_unique<gui::EventWorkersCount>(derived_event));
				break;
			}
			case gui::EventType::EventForcefinder:
			{
				gui::EventForcefinder& derived_event = dynamic_cast<gui::EventForcefinder&>(event);

				m_gui_events.push(std::make_unique<gui::EventForcefinder>(derived_event));
				break;
			}
			case gui::EventType::EventStepsize:
			{
				gui::EventStepsize& derived_event = dynamic_cast<gui::EventStepsize&>(event);

				m_gui_events.push(std::make_unique<gui::EventStepsize>(derived_event));
				break;
			}
			case gui::EventType::EventIntegrator:
			{
				gui::EventIntegrator& derived_event = dynamic_cast<gui::EventIntegrator&>(event);

				m_gui_events.push(std::make_unique<gui::EventIntegrator>(derived_event));
				break;
			}
			case gui::EventType::EventNumberBodies:
			{
				gui::EventNumberBodies& derived_event = dynamic_cast<gui::EventNumberBodies&>(event);

				m_gui_events.push(std::make_unique<gui::EventNumberBodies>(derived_event));
				break;
			}
			case gui::EventType::EventResetBodies:
			{
				gui::EventResetBodies& derived_event = dynamic_cast<gui::EventResetBodies&>(event);

				m_gui_events.push(std::make_unique<gui::EventResetBodies>(derived_event));
				break;
			}

			case gui::EventType::EventShapeSize:
			{
				gui::EventShapeSize& derived_event = dynamic_cast<gui::EventShapeSize&>(event);

				m_gui_events.push(std::make_unique<gui::EventShapeSize>(derived_event));
				break;
			}


			}

		}
		catch (const std::bad_cast& error)
		{
			std::cout << "_storeEvent: " << error.what() << "\n";
		}

	}

	template<>
	platform::PlatformDataType SimulationHost<float>::_templateTypeToPlatformType()
	{
		return platform::PlatformDataType::FLOAT;
	}

	template<>
	platform::PlatformDataType SimulationHost<double>::_templateTypeToPlatformType()
	{
		return platform::PlatformDataType::DOUBLE;
	}

	template class SimulationHost<float>;
	template class SimulationHost<double>;
}


