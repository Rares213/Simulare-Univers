#include "SimulationContext.hpp"

namespace sim
{
	SimulationContext::SimulationContext(std::shared_ptr<ps::ProgramState> program_state, std::shared_ptr<gui::AOverlayMenuSimComp> menu, std::shared_ptr<sim::ISimulation> simulation)
		:
		m_program_state(std::move(program_state)),
		m_menu(std::move(menu)),
		m_simulation(std::move(simulation))
	{
		m_state.context_id = core::genID();
		m_state.platform = m_simulation->getPlatformType();
		m_state.data_type = m_simulation->getPlatformDataType();
	}

	SimulationContext::SimulationContext(SimulationContext&& other) noexcept
		: 
		m_program_state(std::move(other.m_program_state)),
		m_menu(std::move(other.m_menu)),
		m_simulation(std::move(other.m_simulation))
	{
		m_state = other.m_state;

		other.m_state.context_id = 0u;
		other.m_state.platform = platform::PlatformOption::NO_STATE;
		other.m_state.data_type = platform::PlatformDataType::NO_STATE;
	}

	void SimulationContext::operator=(SimulationContext& other)
	{
		m_program_state = other.m_program_state;
		m_menu = other.m_menu;
		m_simulation = other.m_simulation;

		m_state = other.m_state;
	}

	SimulationContext::SimulationContext(SimulationContext& other)
	{
		m_program_state = other.m_program_state;
		m_menu = other.m_menu;
		m_simulation = other.m_simulation;

		m_state = other.m_state;
	}

	void SimulationContext::operator=(SimulationContext&& other) noexcept
	{
		m_program_state = std::move(other.m_program_state);
		m_menu = std::move(other.m_menu);
		m_simulation = std::move(other.m_simulation);

		m_state = other.m_state;

		other.m_state.context_id = 0u;
		other.m_state.platform = platform::PlatformOption::NO_STATE;
		other.m_state.data_type = platform::PlatformDataType::NO_STATE;
	}


}
