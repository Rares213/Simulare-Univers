#include "OverlayMenuSimCompGL.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

namespace gui
{
	namespace gl
	{
		OverlayMenuSimComp::OverlayMenuSimComp(ps::ProgramState& program_state, Dispatcher& dispathcer)
			:
			AOverlayMenuSimComp(program_state, dispathcer)
		{
			if (!_isForcefinderValid())
			{
				forcefinder_option = static_cast<int>(forcefinder::ForceFinderOption::DIRECT_GL);
				m_program_state.forcefinder_option = forcefinder::ForceFinderOption::DIRECT_GL;
			}

			if (!_isIntegratorValid())
			{
				integrator_option = static_cast<int>(integrator::IntegratorOption::EULER_IMPLICIT_GL);
				m_program_state.integrator_option = integrator::IntegratorOption::EULER_IMPLICIT_GL;
			}

			if (!_isNumberBodiesValid())
			{
				number_bodies = (number_bodies / 64) * 64;
				m_program_state.number_bodies = number_bodies;
			}
		}

		OverlayMenuSimComp::~OverlayMenuSimComp() {}

		void OverlayMenuSimComp::topHeaderInfo()
		{
			ImGui::SeparatorText("Program statistics");
			ImGui::Text("Forcefinder delta %.3lf ms", m_program_state.focefinder_delta);
			ImGui::Text("Integrator delta %.3lf ms", m_program_state.integrator_delta);
		}

		void OverlayMenuSimComp::submenuForcefinderOption()
		{
			using forcefinder::ForceFinderOption;

			ImGui::SeparatorText("Forcefinder");

			if(ImGui::RadioButton("Direct", &forcefinder_option, static_cast<int>(forcefinder::ForceFinderOption::DIRECT_GL)))
			{
				m_program_state.forcefinder_option = ForceFinderOption::DIRECT_GL;

				EventForcefinder forcefinder_event;
				m_dispatcher.notifyObservers(forcefinder_event);
			}

		}

		void OverlayMenuSimComp::submenuIntegratorOption()
		{
			using integrator::IntegratorOption;

			ImGui::SeparatorText("Integrator");

			if (ImGui::RadioButton("Euler Implicit", &integrator_option, static_cast<int>(IntegratorOption::EULER_IMPLICIT_GL)))
			{
				m_program_state.integrator_option = IntegratorOption::EULER_IMPLICIT_GL;

				EventIntegrator integrator_event;
				m_dispatcher.notifyObservers(integrator_event);
			}

			if (ImGui::RadioButton("Leapfrog", &integrator_option, static_cast<int>(IntegratorOption::LEAPFROG_GL)))
			{
				m_program_state.integrator_option = IntegratorOption::LEAPFROG_GL;

				EventIntegrator integrator_event;
				m_dispatcher.notifyObservers(integrator_event);
			}

		}

		void OverlayMenuSimComp::submenuG()
		{
			if (ImGui::DragFloat("G", &G, 0.01f, 0.01f, 10.0f, "%.3f", ImGuiSliderFlags_ClampOnInput))
			{
				m_program_state.G = G;

				EventGravity gravity_event;
				m_dispatcher.notifyObservers(gravity_event);
			}
		}

		void OverlayMenuSimComp::submenuSoft()
		{
			if (ImGui::DragFloat("Soft parameter", &soft, 0.001f, 0.001f, 1.0f, "%.3f", ImGuiSliderFlags_ClampOnInput))
			{
				m_program_state.soft = soft;

				EventSoft soft_event;
				m_dispatcher.notifyObservers(soft_event);
			}
		}

		void OverlayMenuSimComp::submenuStepsize()
		{
			if (ImGui::DragFloat("Step size", &stepsize, 0.0001f, 0.0001f, 1.0f, "%.4f", ImGuiSliderFlags_ClampOnInput))
			{
				m_program_state.stepsize = stepsize;

				EventStepsize stepsize_event;
				m_dispatcher.notifyObservers(stepsize_event);
			}
		}

		void OverlayMenuSimComp::submenuBodiesCount()
		{
			ImGui::DragInt("Number bodies", &number_bodies, 1.0f, 1, std::numeric_limits<int>::max());

			if ((_isNumberBodiesHovered == true) && (ImGui::IsItemActive() == false))
			{
				number_bodies = (number_bodies / 64) * 64;

				m_program_state.number_bodies = number_bodies;

				EventNumberBodies number_bodies_event;
				m_dispatcher.notifyObservers(number_bodies_event);
			}

			_isNumberBodiesHovered = ImGui::IsItemActive();
		}

		void OverlayMenuSimComp::submenuResetBodies()
		{
			if (ImGui::Button("Reset bodies"))
			{
				EventResetBodies reset_event;
				m_dispatcher.notifyObservers(reset_event);
			}

		}

		void OverlayMenuSimComp::submenuShapeSize()
		{
			if (ImGui::DragFloat("Size shape", &shape_size, 0.01f, 0.005f, 2.0f))
			{
				EventShapeSize shape_size_event(shape_size);
				m_dispatcher.notifyObservers(shape_size_event);
			}
		}

		void OverlayMenuSimComp::submenuMiscellaneous()
		{

		}

		bool OverlayMenuSimComp::_isForcefinderValid()
		{
			if (forcefinder_option != static_cast<int>(forcefinder::ForceFinderOption::DIRECT_GL))
			{
				return false;
			}

			return true;
		}

		bool OverlayMenuSimComp::_isIntegratorValid()
		{
			if (integrator_option != static_cast<int>(integrator::IntegratorOption::EULER_IMPLICIT_GL) &&
				integrator_option != static_cast<int>(integrator::IntegratorOption::LEAPFROG_GL))
			{
				return false;
			}

			return true;
		}

		bool OverlayMenuSimComp::_isNumberBodiesValid()
		{
			if (number_bodies % 64 != 0)
				return false;
			
			return true;
		}
	}
}