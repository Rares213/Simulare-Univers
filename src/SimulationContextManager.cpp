#include "SimulationContextManager.hpp"

#include "OverlayMenuSimCompH.hpp"
#include "OverlayMenuSimCompGL.hpp"

#include "SimulationHost.hpp"
#include "SimulationGL.hpp"

#include <iostream>

namespace sim
{
	SimulationManager::SimulationManager(GLFWwindow* window)

	{
		m_menu = std::make_shared<gui::GUIMenu>(m_gui_dispatcher, window);
		m_gui_dispatcher.addObserverGui(*this);
		
		core::Application* core_app = core::Application::getApp();
		core_app->addLayer(m_menu);
	}

	void SimulationManager::update(gui::Event& event)
	{
		try
		{
			switch (event.getType())
			{
			case gui::EventType::EventCreateContext:
			{
				gui::EventCreateContext& create_event = dynamic_cast<gui::EventCreateContext&>(event);
				_createSimContext(create_event.getPlatformOption(), create_event.getPlatformDataType());
				break;
			}

			case gui::EventType::EventDeleteContext:
				_deleteSimContext();
				break;

			case gui::EventType::EventCycleUp:
				_cycleUp();
				break;

			case gui::EventType::EventCycleDown:
				_cycleDown();
				break;
			}

		}
		catch (const std::bad_cast& error)
		{
			std::cout << error.what() << "\n";
		}
	}

	void SimulationManager::addContext(SimulationContext&& sim_context)
	{
		m_contexts.push_back(std::move(sim_context));
	}

	void SimulationManager::bindContext(size_t context_handle)
	{
       	if (context_handle == 0u)
			return;
		
		core::Application* core_app = core::Application::getApp();

		for (const SimulationContext& context : m_contexts)
		{
			if (context.m_state.context_id == context_handle)
			{
				core_app->addLayer(context.m_simulation);
				m_menu->setMenuSimComp(context.m_menu.get());
				m_menu->setSimulationState(context.m_state);
				m_state = context.m_state;
				break;
			}
		}
	}

	void SimulationManager::bindContext(SimulationContext& context)
	{
		core::Application* core_app = core::Application::getApp();

		core_app->addLayer(context.m_simulation);
		m_menu->setMenuSimComp(context.m_menu.get());
		m_menu->setSimulationState(context.m_state);
		m_state = context.m_state;

	}

	void SimulationManager::unbindContext(size_t context_handle)
	{
		if (context_handle == 0u)
			return;
		
		m_state = SimState{};
		m_menu->setMenuSimComp(nullptr);

		core::Application* core_app = core::Application::getApp();
		for (const SimulationContext& context : m_contexts)
			if (context.m_state.context_id == context_handle)
			{
				core_app->removeLayer(context.m_simulation->getID());
				break;
			}
		
	}

	void SimulationManager::_createSimContext(platform::PlatformOption option, platform::PlatformDataType data_type)
	{
		unbindContext(m_state.context_id);

		std::shared_ptr<ps::ProgramState> ps = _makeProgramState(option);
		std::shared_ptr<gui::AOverlayMenuSimComp> menu_comp = _makeMenuComp(option, *ps.get());
		std::shared_ptr<sim::ISimulation> simulation = _makeSimulation(option, data_type, *ps.get());

		m_gui_dispatcher.addObserverGui(*simulation.get());

		SimulationContext sim_context(std::move(ps), std::move(menu_comp), std::move(simulation));
		bindContext(sim_context);

		addContext(std::move(sim_context));
	}

	void SimulationManager::_deleteSimContext()
	{
		if (m_contexts.empty())
			return;
		
		auto iter_context = _findSimContextIter(m_state.context_id);

		unbindContext(m_state.context_id);

		m_gui_dispatcher.removeObserverGui(*iter_context->m_simulation);

		m_contexts.erase(iter_context);
		if (m_contexts.empty())
			m_menu->setSimulationState(m_state);
		else
			bindContext(m_contexts[0].m_state.context_id);
	}

	void SimulationManager::_cycleUp()
	{
		if (m_contexts.empty())
			return;

		if (m_contexts.size() == 1u)
			return;

		size_t current_context_index = _findSimContextIndex(m_state.context_id);

		unbindContext(m_state.context_id);

		if (current_context_index == m_contexts.size() - 1u)
			bindContext(m_contexts[0]);
		else
			bindContext(m_contexts[++current_context_index]);
	}

	void SimulationManager::_cycleDown()
	{
		if (m_contexts.empty())
			return;

		if (m_contexts.size() == 1u)
			return;

		size_t current_context_index = _findSimContextIndex(m_state.context_id);

		unbindContext(m_state.context_id);

		if (current_context_index == 0u)
			bindContext(m_contexts[m_contexts.size() - 1u]);
		else
			bindContext(m_contexts[--current_context_index]);

	}

	SimulationContext* SimulationManager::_findSimContext(size_t context_id)
	{
		for (SimulationContext& context : m_contexts)
			if (context.m_state.context_id == context_id)
				return &context;

		return nullptr;
	}

	size_t SimulationManager::_findSimContextIndex(size_t context_id)
	{
		for (size_t i = 0; i < m_contexts.size(); i++)
			if (m_contexts[i].m_state.context_id == context_id)
				return i;

		return 0;
	}

	std::vector<sim::SimulationContext>::iterator SimulationManager::_findSimContextIter(size_t context_id)
	{
		for (auto iter = m_contexts.begin(); iter != m_contexts.end(); ++iter)
			if (iter->m_state.context_id == context_id)
				return iter;

		return m_contexts.end();
	}

	std::unique_ptr<ps::ProgramState> SimulationManager::_makeProgramState(platform::PlatformOption option)
	{
		using platform::PlatformOption, 
			forcefinder::ForceFinderOption, 
			integrator::IntegratorOption;

		std::unique_ptr<ps::ProgramState> ps = std::make_unique<ps::ProgramState>();

		return ps;
	}

	std::unique_ptr<gui::AOverlayMenuSimComp> SimulationManager::_makeMenuComp(platform::PlatformOption option, ps::ProgramState& program_state)
	{
		using platform::PlatformOption;
		
		switch (option)
		{
		case PlatformOption::HOST:
		{
			return std::make_unique<gui::host::OverlayMenuSimComp>(program_state, m_gui_dispatcher);
		}

		case PlatformOption::OPENGL:
		{
			return std::make_unique<gui::gl::OverlayMenuSimComp>(program_state, m_gui_dispatcher);
		}
		default:
			return nullptr;
		}

	}

	std::unique_ptr<sim::ISimulation> SimulationManager::_makeSimulation(platform::PlatformOption option, platform::PlatformDataType data_type, ps::ProgramState& program_state)
	{
		using platform::PlatformOption, platform::PlatformDataType;

		switch (option)
		{
		case PlatformOption::HOST:
			if(data_type == PlatformDataType::FLOAT)
				return std::make_unique<sim::SimulationHost<float>>(program_state);
			else
				return std::make_unique<sim::SimulationHost<double>>(program_state);

		case PlatformOption::OPENGL:
			if (data_type == PlatformDataType::FLOAT)
				return std::make_unique<sim::SimulationGL<float>>(program_state);
			else
				return std::make_unique<sim::SimulationGL<double>>(program_state);

		default:
			return nullptr;
		}
	}

}
