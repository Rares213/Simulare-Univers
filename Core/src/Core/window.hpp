#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>


namespace core
{
	struct WindowSpec
	{
		int width = 1280;
		int height = 720;
		std::string window_name = "< blank >";
		bool enable_debug_gl = false;
		bool enable_vsync = false;
	};

	// class that manages window object
	class Window
	{
	public:
		Window(const WindowSpec& window_spec);
		~Window() { glfwDestroyWindow(m_window_handle); }

		GLFWwindow* getWindowHandle() { return m_window_handle; }
		int windowShouldClose() { return glfwWindowShouldClose(m_window_handle); }

		void clearScren() { glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); }
		void updateScreen() { glfwSwapBuffers(m_window_handle); }

	private:
		GLFWwindow* m_window_handle = nullptr;

	private:

		static void _framebufferSizeCallback(GLFWwindow* window, int width, int height);
		static void _messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
		

	};
}