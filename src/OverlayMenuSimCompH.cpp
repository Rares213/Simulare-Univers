#include "OverlayMenuSimCompH.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <thread>

namespace gui
{
	namespace host
	{
		OverlayMenuSimComp::OverlayMenuSimComp(ps::ProgramState& program_state, Dispatcher& dispatcher)
			:
			AOverlayMenuSimComp(program_state, dispatcher)
		{
			if (!_isForcefinderValid())
			{
				forcefinder_option = static_cast<int>(forcefinder::ForceFinderOption::DIRECT);
				m_program_state.forcefinder_option = forcefinder::ForceFinderOption::DIRECT;
			}

			if (!_isIntegratorValid())
			{
				integrator_option = static_cast<int>(integrator::IntegratorOption::EULER_IMPLICIT);
				m_program_state.integrator_option = integrator::IntegratorOption::EULER_IMPLICIT;
			}

			workers_count = m_program_state.workers_count;
		}

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

			if (ImGui::RadioButton("Direct", &forcefinder_option, static_cast<int>(ForceFinderOption::DIRECT)))
			{
				m_program_state.forcefinder_option = ForceFinderOption::DIRECT;
				EventForcefinder forcefinder_event;
				m_dispatcher.notifyObservers(forcefinder_event);
			}

			if (ImGui::RadioButton("Direct SIMD", &forcefinder_option, static_cast<int>(ForceFinderOption::DIRECT_SIMD)))
			{
				m_program_state.forcefinder_option = ForceFinderOption::DIRECT_SIMD;
				EventForcefinder forcefinder_event;
				m_dispatcher.notifyObservers(forcefinder_event);
			}

			if (ImGui::RadioButton("Barnes Hut", &forcefinder_option, static_cast<int>(ForceFinderOption::BARNES_HUT)))
			{
				m_program_state.forcefinder_option = ForceFinderOption::BARNES_HUT;
				EventForcefinder forcefinder_event;
				m_dispatcher.notifyObservers(forcefinder_event);
			}

		}

		void OverlayMenuSimComp::submenuIntegratorOption()
		{
			using integrator::IntegratorOption;
			
			ImGui::SeparatorText("Integrator");

			if (ImGui::RadioButton("Euler Explicit", &integrator_option, static_cast<int>(IntegratorOption::EULER_EXPLICIT)))
			{
				m_program_state.integrator_option = IntegratorOption::EULER_EXPLICIT;

				EventIntegrator integrator_event;
				m_dispatcher.notifyObservers(integrator_event);
			}

			if (ImGui::RadioButton("Euler Implicit", &integrator_option, static_cast<int>(IntegratorOption::EULER_IMPLICIT)))
			{
				m_program_state.integrator_option = IntegratorOption::EULER_IMPLICIT;

				EventIntegrator integrator_event;
				m_dispatcher.notifyObservers(integrator_event);
			}

			if (ImGui::RadioButton("RungeKutta 2", &integrator_option, static_cast<int>(IntegratorOption::RK2)))
			{
				m_program_state.integrator_option = IntegratorOption::RK2;

				EventIntegrator integrator_event;
				m_dispatcher.notifyObservers(integrator_event);
			}

			if (ImGui::RadioButton("Leapfrog", &integrator_option, static_cast<int>(IntegratorOption::LEAPFROG)))
			{
				m_program_state.integrator_option = IntegratorOption::LEAPFROG;

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
			switch (m_program_state.forcefinder_option)
			{
			case forcefinder::ForceFinderOption::DIRECT:
			case forcefinder::ForceFinderOption::BARNES_HUT:
				if (ImGui::DragInt("Number bodies", &number_bodies, 1.0f, 1, std::numeric_limits<int>::max()))
				{
					m_program_state.number_bodies = number_bodies;

					EventNumberBodies number_bodies_event;
					m_dispatcher.notifyObservers(number_bodies_event);
				}
				break;

			case forcefinder::ForceFinderOption::DIRECT_SIMD:
				if (ImGui::DragInt("Number bodies", &number_bodies, 1.0f, 1, std::numeric_limits<int>::max()))
				{
					m_program_state.number_bodies = number_bodies;

					EventNumberBodies number_bodies_event;
					m_dispatcher.notifyObservers(number_bodies_event);
				}

				if (ImGui::Button("Round"))
				{
					size_t vectorizable_size;
					if (m_program_state.platform_type == platform::PlatformDataType::FLOAT)
						vectorizable_size = 8u;
					else
						vectorizable_size = 4u;

					size_t round_down_number_bodies = m_program_state.number_bodies / vectorizable_size;
					m_program_state.number_bodies = round_down_number_bodies * vectorizable_size;
					number_bodies = m_program_state.number_bodies;

					EventNumberBodies number_bodies_event;
					m_dispatcher.notifyObservers(number_bodies_event);
				}
				break;
			}
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

			if (ImGui::DragInt("Workers count", &workers_count, 0.05f, 0, std::thread::hardware_concurrency()))
			{
				m_program_state.workers_count = static_cast<size_t>(workers_count);
				
				EventWorkersCount workers_count_event;
				m_dispatcher.notifyObservers(workers_count_event);
			}
		}

		bool OverlayMenuSimComp::_isForcefinderValid()
		{
			if (
				forcefinder_option != static_cast<int>(forcefinder::ForceFinderOption::DIRECT) &&
				forcefinder_option != static_cast<int>(forcefinder::ForceFinderOption::DIRECT_SIMD) &&
				forcefinder_option != static_cast<int>(forcefinder::ForceFinderOption::BARNES_HUT)
				)
			{
				return false;
			}

			return true;
		}

		bool OverlayMenuSimComp::_isIntegratorValid()
		{
			if (
				integrator_option != static_cast<int>(integrator::IntegratorOption::EULER_EXPLICIT) &&
				integrator_option != static_cast<int>(integrator::IntegratorOption::EULER_IMPLICIT) &&
				integrator_option != static_cast<int>(integrator::IntegratorOption::RK2) && 
				integrator_option != static_cast<int>(integrator::IntegratorOption::LEAPFROG)
				)
			{
				return false;
			}

			return true;
		}


	}
}