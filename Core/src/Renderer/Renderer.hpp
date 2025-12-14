#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Drawable.hpp"
#include "Layer.hpp"

#include "Camera.hpp"

#include <vector>

namespace core
{

	class Renderer : public core::ILayer
	{
	public:
		Renderer(GLFWwindow* window_handler);
		virtual ~Renderer() {}

		// Inherited via ILayer
		virtual void event(core::Event& event) override;
		virtual void update() override;
		virtual void draw() override;
		virtual size_t getID() override;

		void submitDrawable(Drawable& drawable_object);
		void removeDrawable(Drawable& drawable_object);


	private:

		size_t m_id = 0;
		
		GLFWwindow* m_window_handler = nullptr;
		Camera m_camera;

		std::vector<Drawable*> m_drawable_objects;

	private:

		CameraMovement _mapKeyToCameraMovement(int input);

		template<typename T>
		void _typeDraw(Drawable& drawable_object);

	};


	template<> void Renderer::_typeDraw<float>(Drawable& drawable);
	template<> void Renderer::_typeDraw<double>(Drawable& drawable);
}
