#pragma once

#include "Core.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "DispatcherGui.hpp"

#include "OverlayMenuSimComp.hpp"

#include "SimulationState.hpp"

namespace gui
{
	
	class GUIMenu : public core::ILayer
	{
	public:
		GUIMenu(Dispatcher& dispatcher, GLFWwindow* window);
		virtual ~GUIMenu();

		virtual void event(core::Event& event) override final;
		virtual void update() override final;
		virtual void draw() override final;
		virtual size_t getID() override final;

		void setMenuSimComp(AOverlayMenuSimComp* sim_comp);
		void setSimulationState(sim::SimState sim_state);

		void submenuPlatformOption();
		void submenuPlatformDataTypeOption();
		void submenuCycleUp();
		void submenuCycleDown();
		void submenuCreateContextButton();
		void submenuDeleteContextButton();

	private:

		size_t m_id = 0u;
		Dispatcher& m_dispatcher;
		AOverlayMenuSimComp* m_sim_comp = nullptr;

		int platform_option = static_cast<int>(platform::PlatformOption::HOST);
		int platform_data_type = static_cast<int>(platform::PlatformDataType::FLOAT);

		sim::SimState m_sim_state;

	private:

		const char* _platfromOptionToString(platform::PlatformOption option);
		const char* _PlatformDataTypeToString(platform::PlatformDataType data_type);

	};

	
}