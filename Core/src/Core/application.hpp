#pragma once


#include "window.hpp"
#include "Layer.hpp"
#include "Renderer.hpp"

#include <vector>
#include <memory>
#include <filesystem>

namespace core
{
	void submitDrawable(Drawable& drawable_object);
	void removeDrawable(Drawable& drawable_object);
	double getTime();
	double getPrevFrameDelta();
	size_t genID();

	struct AppSpec
	{
		WindowSpec win_spec;
	};

	class Application
	{
	public:
		Application(const AppSpec& app_spec);
		~Application();

		void addLayer(std::shared_ptr<ILayer> Layer);
		void removeLayer(size_t id);
		void clearLayers();
		
		static Application* getApp();
		GLFWwindow* getWindow();

		void run();


	private:

		AppSpec m_app_spec;

		std::unique_ptr<Window> m_window;

		std::vector<std::shared_ptr<ILayer>> m_layers;

	private:

		static void _glfwErrorCallback(int error, const char* description);
		static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void _cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

		void _loadResources();
		void _loadShaders(std::filesystem::path shader_dir_path);

	};

}

