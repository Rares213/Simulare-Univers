#include "OverlayMenuSimComp.hpp"

namespace gui
{
	AOverlayMenuSimComp::AOverlayMenuSimComp(ps::ProgramState& program_state, Dispatcher& dispatcher)
		:
		m_program_state(program_state),
		m_dispatcher(dispatcher)
	{
		_initMembers();
	}

	void AOverlayMenuSimComp::_initMembers()
	{
		forcefinder_option = static_cast<int>(m_program_state.forcefinder_option);
		integrator_option = static_cast<int>(m_program_state.integrator_option);
		G = m_program_state.G;
		soft = m_program_state.soft;
		stepsize = m_program_state.stepsize;
		number_bodies = m_program_state.number_bodies;
	}
}