#pragma once

#include "SimulationState.hpp"
#include "SimulationContext.hpp"

#include "OverlayMenu.hpp"

#include <vector>

namespace sim
{
	class SimulationManager : public gui::Observer
	{
	public:
		SimulationManager(GLFWwindow* window);
		~SimulationManager() {}

		// Inherited via gui::Observer
		virtual void update(gui::Event& event) override;

		void addContext(SimulationContext&& sim_context);
		void bindContext(size_t context_handle);
		void bindContext(SimulationContext& context);
		void unbindContext(size_t context_handle);
		
		

	private:
		std::vector<SimulationContext> m_contexts;

		SimState m_state;
		gui::Dispatcher m_gui_dispatcher;
		std::shared_ptr<gui::GUIMenu> m_menu;

	private:

		void _createSimContext(platform::PlatformOption option, platform::PlatformDataType data_type);
		void _deleteSimContext();
		void _cycleUp();
		void _cycleDown();

		SimulationContext* _findSimContext(size_t context_id);
		size_t _findSimContextIndex(size_t context_id);
		std::vector<sim::SimulationContext>::iterator _findSimContextIter(size_t context_id);

		std::unique_ptr<ps::ProgramState> _makeProgramState(platform::PlatformOption option);
		std::unique_ptr<gui::AOverlayMenuSimComp> _makeMenuComp(platform::PlatformOption option, ps::ProgramState& program_state);
		std::unique_ptr<sim::ISimulation> _makeSimulation(platform::PlatformOption option, platform::PlatformDataType data_type, ps::ProgramState& program_state);
	};
}