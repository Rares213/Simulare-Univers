#include "OverlayMenu.hpp"

#include "PlatformEnum.hpp"


namespace gui
{

	GUIMenu::GUIMenu(Dispatcher& dispatcher, GLFWwindow* window)
		:
		m_dispatcher(dispatcher)
	{
		m_id = core::genID();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	GUIMenu::~GUIMenu()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUIMenu::event(core::Event& event)
	{

	}

	void GUIMenu::update()
	{

	}

	void GUIMenu::draw()
	{
		static ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();



		ImGui::SetNextWindowPos(ImVec2{ 0.0f, 0.0f }, 0, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("Simulation settings");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("Current context info");
		ImGui::Text("ID: %zu", m_sim_state.context_id);
		ImGui::Text("Platform: %s", _platfromOptionToString(m_sim_state.platform));
		ImGui::Text("Platform type: %s", _PlatformDataTypeToString(m_sim_state.data_type));

		submenuCycleUp();
		ImGui::SameLine();
		ImGui::Text(" cycle ");
		ImGui::SameLine();
		submenuCycleDown();

		submenuPlatformOption();
		submenuPlatformDataTypeOption();

		submenuCreateContextButton(); ImGui::SameLine();
		submenuDeleteContextButton();


		ImGui::End();

		if (m_sim_comp)
		{
			GLFWwindow* window = core::Application::getApp()->getWindow();
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			ImGui::SetNextWindowSize(ImVec2{ 500.0f, 500.0f });
			ImGui::SetNextWindowPos(ImVec2{ static_cast<float>(width - 500), 0.0f });
			ImGui::Begin("Simulation parameters");

			m_sim_comp->topHeaderInfo();

			if (ImGui::TreeNode("Shape"))
			{
				ImGui::Dummy(ImVec2(0.0f, 1.0f));
				m_sim_comp->submenuShapeSize();

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Bodies"))
			{
				ImGui::Dummy(ImVec2(0.0f, 1.0f));
				m_sim_comp->submenuResetBodies();

				m_sim_comp->submenuBodiesCount();
				ImGui::Dummy(ImVec2(0.0f, 1.0f));

				ImGui::TreePop();
			}

			if(ImGui::TreeNode("Physics"))
			{
				m_sim_comp->submenuForcefinderOption();
				m_sim_comp->submenuIntegratorOption();

				ImGui::Dummy(ImVec2(0.0f, 3.0f));

				m_sim_comp->submenuG();
				ImGui::Dummy(ImVec2(0.0f, 1.0f));

				m_sim_comp->submenuSoft();
				ImGui::Dummy(ImVec2(0.0f, 1.0f));

				m_sim_comp->submenuStepsize();
				ImGui::Dummy(ImVec2(0.0f, 1.0f));

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Miscellaneous"))
			{
				m_sim_comp->submenuMiscellaneous();

				ImGui::TreePop();
			}

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	size_t GUIMenu::getID()
	{
		return m_id;
	}

	void GUIMenu::setMenuSimComp(AOverlayMenuSimComp* sim_comp)
	{
		m_sim_comp = sim_comp;
	}

	void GUIMenu::setSimulationState(sim::SimState sim_state)
	{
		m_sim_state = sim_state;
	}

	void GUIMenu::submenuPlatformOption()
	{
		using platform::PlatformOption;

		ImGui::SeparatorText("Platform");

		ImGui::RadioButton("Host", &platform_option, static_cast<int>(PlatformOption::HOST));

		ImGui::RadioButton("OpenGL", &platform_option, static_cast<int>(PlatformOption::OPENGL));

		ImGui::RadioButton("OpenCL", &platform_option, static_cast<int>(PlatformOption::OPENCL));
	}

	void GUIMenu::submenuPlatformDataTypeOption()
	{
		using platform::PlatformDataType;

		ImGui::SeparatorText("Data Type");

		ImGui::RadioButton("Float", &platform_data_type, static_cast<int>(PlatformDataType::FLOAT));

		ImGui::RadioButton("Double", &platform_data_type, static_cast<int>(PlatformDataType::DOUBLE));

	}

	void GUIMenu::submenuCycleUp()
	{
		if (ImGui::ArrowButton("##left", ImGuiDir_Left))
		{
			EventCycleDown cycle_down_event;
			m_dispatcher.notifyObservers(cycle_down_event);
		}
	}

	void GUIMenu::submenuCycleDown()
	{
		if (ImGui::ArrowButton("##right", ImGuiDir_Right))
		{
			EventCycleUp cycle_up_event;
			m_dispatcher.notifyObservers(cycle_up_event);
		}
	}

	void GUIMenu::submenuCreateContextButton()
	{
		if (ImGui::Button("Create"))
		{
			EventCreateContext create_event(static_cast<platform::PlatformOption>(platform_option), static_cast<platform::PlatformDataType>(platform_data_type));
			m_dispatcher.notifyObservers(create_event);
		}
	}

	void GUIMenu::submenuDeleteContextButton()
	{
		if (ImGui::Button("Delete"))
		{
			EventDeleteContext delete_event;
			m_dispatcher.notifyObservers(delete_event);
		}
	}

	const char* GUIMenu::_platfromOptionToString(platform::PlatformOption option)
	{
		switch (option)
		{
		case platform::PlatformOption::HOST:
			return "HOST";

		case platform::PlatformOption::OPENGL:
			return "OPENGL";

		default:
			return "N/A";
		}
	}

	const char* GUIMenu::_PlatformDataTypeToString(platform::PlatformDataType data_type)
	{
		switch (data_type)
		{
		case platform::PlatformDataType::FLOAT:
			return "FLOAT";

		case platform::PlatformDataType::DOUBLE:
			return "DOUBLE";

		default:
			return "N/A";
		}
	}

		
	



}

