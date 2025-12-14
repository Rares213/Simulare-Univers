#pragma once

#include "SimulationInterface.hpp"

#include "ProgramState.hpp"

#include "PhysicsGL.hpp"
#include "BodiesGL.hpp"

#include "CircleGL.hpp"

#include "TaskManager.hpp"


namespace sim
{
	template<typename T>
	class SimulationGL : public ISimulation
	{
	public:
		SimulationGL(ps::ProgramState& program_state);
		virtual ~SimulationGL();

		// Inherited via ISimulation
		virtual platform::PlatformOption getPlatformType() override;
		virtual platform::PlatformDataType getPlatformDataType() override;

		// Inherited via ILayer
		virtual void event(core::Event& event) override;
		virtual void update() override;
		virtual void draw() override;
		virtual size_t getID() override;

		// Inherited via Observer
		void update(gui::Event& event) override;


	private:

		size_t m_id = 0;
		ps::ProgramState& m_program_state;

		body::glbody::Bodies<T> m_bodies;
		phys::PhysicsGL<T> m_physics;
		core::gl::Circle<T> m_circle;

		task::TaskManager m_task_manager;

		bool m_paused = false;

	private:

		body::init::GenInitialConditions<T> _createInitialConditions();
		std::unique_ptr<forcefinder::ffgl::IFactoryForceFinder<T>> _createFactoryForceFinder();
		std::unique_ptr<integrator::igl::FactoryIntegrator<T>> _createFactoryIntegrator();

		void _handleGuiEvents(gui::Event& event);
		//void _handleQueuedGuiEvents();
		//void _storeEvent(gui::Event& event);

		core::Shader& _getShaderOnType();
		std::array<core::VertexBuffer, 3u>& _getPositionRef();

		platform::PlatformDataType _templateTypeToPlatformType();

	};


}