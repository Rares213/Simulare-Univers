#pragma once

#include "SimulationInterface.hpp"

#include "ProgramState.hpp"

#include "BodiesHost.hpp"
#include "Physics.hpp"
#include "TaskManager.hpp"

#include "Circle.hpp"

#include <memory>
#include <queue>

namespace sim
{
	template<typename T>
	class SimulationHost : public ISimulation
	{
	public:
		SimulationHost(ps::ProgramState& program_state);
		virtual ~SimulationHost();

		// Inherited via ISimulation
		virtual platform::PlatformOption getPlatformType() override;
		virtual platform::PlatformDataType getPlatformDataType() override;

		// Inherited via ILayer
		virtual void event(core::Event& event) override;
		virtual void update() override;
		virtual void draw() override;
		virtual size_t getID() override;

		// Inherited via Observer
		virtual void update(gui::Event& event) override;


	private:

		size_t m_id = 0;
		ps::ProgramState& m_program_state;

		body::hbody::Bodies<T> m_bodies;
		phys::Physics<T> m_physics;
		core::Circle<T> m_circle;

		task::TaskManager m_task_manager;

		std::queue<std::unique_ptr<gui::Event>> m_gui_events;

		bool m_paused = false;

	private:
		
		body::init::GenInitialConditions<T> _createInitialConditions();
		std::unique_ptr<forcefinder::ffh::IFactoryForceFinder<T>> _createFactoryForceFinder();
		std::unique_ptr<integrator::ih::FactoryIntegrator<T>> _createFactoryIntegrator();


		void _handleGuiEvents(gui::Event& event);
		void _handleQueuedGuiEvents();
		void _storeEvent(gui::Event& event);

		platform::PlatformDataType _templateTypeToPlatformType();

	};
}