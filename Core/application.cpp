#include "application.hpp"

#include "ResourceManager.hpp"

#include <iostream>

namespace core
{
	Application* g_app = nullptr;
	Renderer* g_renderer = nullptr;
	double g_last_frame_delta = 0;
	size_t id_count = 0;
	
	void submitDrawable(Drawable& drawable_object)
	{
		g_renderer->submitDrawable(drawable_object);
	}

	void removeDrawable(Drawable& drawable_object)
	{
		g_renderer->removeDrawable(drawable_object);
	}

	double getTime()
	{
		return glfwGetTime();
	}

	double getPrevFrameDelta()
	{
		return g_last_frame_delta;
	}

	size_t genID()
	{
		return ++id_count;
	}

	Application::Application(const AppSpec& app_spec)
		: m_app_spec(app_spec)
	{
		glfwSetErrorCallback(Application::_glfwErrorCallback);

		glfwInit();

		m_window = std::make_unique<Window>(m_app_spec.win_spec);

		std::shared_ptr<Renderer> renderer = std::make_unique<Renderer>(m_window->getWindowHandle());
		g_renderer = renderer.get();
		addLayer(renderer);

		g_app = this;

		glfwSetKeyCallback(m_window->getWindowHandle(), _keyCallback);
		glfwSetCursorPosCallback(m_window->getWindowHandle(), _cursorPosCallback);

		_loadResources();
	}

	Application::~Application()
	{
		m_layers.clear();
		m_window.reset();
		glfwTerminate();

		g_app = nullptr;
		g_renderer = nullptr;
	}

	void Application::addLayer(std::shared_ptr<ILayer> Layer)
	{
		m_layers.push_back(Layer); 
	}

	void Application::removeLayer(size_t id)
	{
		for (auto iter = m_layers.begin(); iter != m_layers.end(); ++iter)
			if (iter->get()->getID() == id)
			{
				m_layers.erase(iter);
				break;
			}
	}

	void Application::clearLayers()
	{
		m_layers.clear(); 
	}

	Application* Application::getApp()
	{
		return g_app;
	}

	GLFWwindow* Application::getWindow()
	{
		return m_window->getWindowHandle(); 
	}

	void Application::run()
	{
		static double prev_time = glfwGetTime();

		while (!m_window->windowShouldClose())
		{
			m_window->clearScren();
			
			glfwPollEvents();

			for (size_t i = 0; i < m_layers.size(); ++i)
				m_layers[i]->update();

			for (size_t i = 0; i < m_layers.size(); ++i)
				m_layers[i]->draw();

			m_window->updateScreen();

			double current_time = glfwGetTime();
			g_last_frame_delta = current_time - prev_time;
			prev_time = current_time;
		}


	}

	void Application::_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(g_app->getWindow(), 1);
			return;
		}

		if (action == GLFW_PRESS)
		{
			EventKeyPressed pressed(key);
			for (std::shared_ptr<ILayer>& layer : g_app->m_layers)
				layer->event(pressed);

			std::cout << pressed.getDescription() << "\n";
		}
		else if (action == GLFW_RELEASE)
		{
			EventKeyReleased released(key);
			for (std::shared_ptr<ILayer>& layer : g_app->m_layers)
				layer->event(released);

			std::cout << released.getDescription() << "\n";
		}
		else if (action == GLFW_REPEAT)
		{
			EventKeyRepeat repeat(key);
			for (std::shared_ptr<ILayer>& layer : g_app->m_layers)
				layer->event(repeat);

			std::cout << repeat.getDescription() << "\n";
		}


	}

	void Application::_cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		EventCursorPos event_cursor_pos(xpos, ypos);
		for (std::shared_ptr<ILayer>& layer : g_app->m_layers)
			layer->event(event_cursor_pos);
	}

	void Application::_glfwErrorCallback(int error, const char* description)
	{
		std::cerr << "\nGLFW error\n" << "code: " << error  << "\ndescription: " << description << std::endl;
	}

	void Application::_loadResources()
	{
		std::filesystem::path core_path_resources("./Core/src/Resources");
		core_path_resources = std::filesystem::absolute(core_path_resources);
		std::cout << "Core resources path: " << core_path_resources << "\n";

		std::filesystem::path shaders_path(core_path_resources.append("Shaders"));

		
		_loadShaders(shaders_path);
	}

	void Application::_loadShaders(std::filesystem::path shader_dir_path)
	{
		std::filesystem::path circle_vertex(shader_dir_path);
		std::filesystem::path circle_fragment(shader_dir_path);
		circle_vertex.append("./circle_f32.vert");
		circle_fragment.append("./circle_f32.frag");

		circle_vertex = std::filesystem::absolute(circle_vertex);
		circle_fragment = std::filesystem::absolute(circle_fragment);

		std::cout << "Circle f32 vertex path: " << circle_vertex << "\nCircle f32 fragment path: " << circle_fragment << "\n";

		ResourceManager::loadShader(circle_vertex.string(), circle_fragment.string(), resname::CIRCLE_SHADER_F32);

		circle_vertex = std::filesystem::path(shader_dir_path);
		circle_fragment = std::filesystem::path(shader_dir_path);
		circle_vertex.append("./circle_f64.vert");
		circle_fragment.append("./circle_f64.frag");

		circle_vertex = std::filesystem::absolute(circle_vertex);
		circle_fragment = std::filesystem::absolute(circle_fragment);

		std::cout << "Circle f64 vertex path: " << circle_vertex << "\nCircle f64 fragment path: " << circle_fragment << "\n";

		ResourceManager::loadShader(circle_vertex.string(), circle_fragment.string(), resname::CIRCLE_SHADER_F64);

	}
}



