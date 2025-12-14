#pragma once

#include "SimulationState.hpp"

#include "ProgramState.hpp"
#include "OverlayMenuSimComp.hpp"
#include "SimulationInterface.hpp"

namespace sim
{
	class SimulationContext
	{
	public:
		SimulationContext(std::shared_ptr<ps::ProgramState> program_state, std::shared_ptr<gui::AOverlayMenuSimComp> menu, std::shared_ptr<sim::ISimulation> simulation);
		SimulationContext(SimulationContext& other);
		SimulationContext(SimulationContext&& other) noexcept;

		void operator=(SimulationContext& other);
		void operator=(SimulationContext&& other) noexcept;

		SimState m_state;

		std::shared_ptr<ps::ProgramState> m_program_state;
		std::shared_ptr<gui::AOverlayMenuSimComp> m_menu;
		std::shared_ptr<sim::ISimulation> m_simulation;
		
	};
}