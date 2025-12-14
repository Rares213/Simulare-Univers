#include "SimulationGL.hpp"

#include "ResourceManager.hpp"

#include <iostream>

namespace sim
{
	template<typename T>
	sim::SimulationGL<T>::SimulationGL(ps::ProgramState& program_state)
		:
		m_id(core::genID()),
		m_program_state(program_state),
		m_bodies(_createInitialConditions()),
		m_physics(_createFactoryForceFinder(), _createFactoryIntegrator()),
		m_circle(core::gl::CircleSpec{program_state.number_bodies, _getShaderOnType(), _getPositionRef()})
	{
		
	}

	template<typename T>
	SimulationGL<T>::~SimulationGL()
	{
		
	}

	template<typename T>
	platform::PlatformOption SimulationGL<T>::getPlatformType()
	{
		return platform::PlatformOption::OPENGL;
	}

	template<typename T>
	platform::PlatformDataType SimulationGL<T>::getPlatformDataType()
	{
		return _templateTypeToPlatformType();
	}

	template<typename T>
	void SimulationGL<T>::event(core::Event& event)
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
	void SimulationGL<T>::update()
	{
		if (m_paused == true)
		{
			m_bodies.reset_force();
			m_physics.runPhysics();
			m_program_state.focefinder_delta = m_physics.getForcefinderDelta();
			m_program_state.integrator_delta = m_physics.getIntegratorDelta();
		}
	}

	template<typename T>
	void SimulationGL<T>::draw()
	{
		core::submitDrawable(m_circle);
	}

	template<typename T>
	size_t SimulationGL<T>::getID()
	{
		return m_id;
	}

	template<typename T>
	void SimulationGL<T>::update(gui::Event& event)
	{
		_handleGuiEvents(event);
	}

	template<typename T>
	void SimulationGL<T>::_handleGuiEvents(gui::Event& event)
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
	body::init::GenInitialConditions<T> sim::SimulationGL<T>::_createInitialConditions()
	{
		return body::init::GenInitialConditions<T>(m_program_state.init_cond_option, m_program_state.number_bodies);
	}

	template<typename T>
	std::unique_ptr<forcefinder::ffgl::IFactoryForceFinder<T>> SimulationGL<T>::_createFactoryForceFinder()
	{
		body::PropertiesGL& prop = m_bodies.get_prop();

		forcefinder::ffgl::FactoryGravityData<T> factory_gravity_spec
		{
			m_program_state.G,
			m_program_state.soft,
			m_program_state.number_bodies,
			prop.position,
			prop.force,
			prop.mass[0]
		};

		switch (m_program_state.forcefinder_option)
		{
		case forcefinder::ForceFinderOption::DIRECT_GL:
			return std::make_unique<forcefinder::ffgl::FactoryForceFinderGravityDirect<T>>(factory_gravity_spec);

		default:
			return nullptr;
		}
	}

	template<typename T>
	std::unique_ptr<integrator::igl::FactoryIntegrator<T>> SimulationGL<T>::_createFactoryIntegrator()
	{
		body::PropertiesGL& prop = m_bodies.get_prop();

		integrator::igl::FactoryIntegratorData<T> factory_data
		{
			m_program_state.integrator_option,
			m_program_state.stepsize,
			m_program_state.number_bodies,
			prop.position,
			prop.velocity,
			prop.acceleration,
			prop.force,
			prop.mass[0]
		};

		return std::make_unique<integrator::igl::FactoryIntegrator<T>>(factory_data);
	}

	template<>
	core::Shader& sim::SimulationGL<GLfloat>::_getShaderOnType()
	{
		return core::ResourceManager::getShader(core::resname::CIRCLE_SHADER_F32);
	}

	template<>
	core::Shader& sim::SimulationGL<GLdouble>::_getShaderOnType()
	{
		return core::ResourceManager::getShader(core::resname::CIRCLE_SHADER_F64);
	}

	template<typename T>
	std::array<core::VertexBuffer, 3u>& SimulationGL<T>::_getPositionRef()
	{
		return m_bodies.get_prop().position;
	}

	template<>
	platform::PlatformDataType SimulationGL<float>::_templateTypeToPlatformType()
	{
		return platform::PlatformDataType::FLOAT;
	}

	template<>
	platform::PlatformDataType SimulationGL<double>::_templateTypeToPlatformType()
	{
		return platform::PlatformDataType::DOUBLE;
	}

	template class SimulationGL<float>;
	template class SimulationGL<double>;
}

