#pragma once

#include "ProgramState.hpp"

#include "DispatcherGui.hpp"

namespace gui
{
	class AOverlayMenuSimComp
	{
	public:
		AOverlayMenuSimComp(ps::ProgramState& program_state, Dispatcher& dispathcer);
		virtual ~AOverlayMenuSimComp() {}

		virtual void topHeaderInfo() = 0;
		virtual void submenuForcefinderOption() = 0;
		virtual void submenuIntegratorOption() = 0;
		virtual void submenuG() = 0;
		virtual void submenuSoft() = 0;
		virtual void submenuStepsize() = 0;
		virtual void submenuBodiesCount() = 0;
		virtual void submenuResetBodies() = 0;
		virtual void submenuShapeSize() = 0;
		virtual void submenuMiscellaneous() = 0;

	protected:

		ps::ProgramState& m_program_state;
		Dispatcher& m_dispatcher;

		int forcefinder_option = 0;
		int integrator_option = 0;
		float G = 0;
		float soft = 0;
		float stepsize = 0;
		int number_bodies = 0;
		float shape_size = 0.005f;

	private:
		void _initMembers();
	};
}