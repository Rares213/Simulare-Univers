#include "Renderer.hpp"

#include "application.hpp"

namespace core
{
	bool toggle_cursor_movement = false;
	
	Renderer::Renderer(GLFWwindow* window_handler) 
		: 
		m_id(core::genID()),
		m_window_handler(window_handler)
	{

	}

	void Renderer::event(core::Event& event)
	{
		switch (event.getType())
		{
		case core::EventType::KeyPressed:
		{
			core::EventKeyPressed& event_key_pressed = static_cast<core::EventKeyPressed&>(event);
			
			CameraMovement camera_movement = _mapKeyToCameraMovement(event_key_pressed.getKey());
			if (camera_movement != CameraMovement::NONE)
				m_camera.ProcessKeyboard(camera_movement, core::getPrevFrameDelta());

			/*
			if (event_key_pressed.getKey() == GLFW_KEY_I)
			{
				if (toggle_cursor_movement)
					toggle_cursor_movement = false;
				else
					toggle_cursor_movement = true;
			}
			*/

			break;
		}
		case core::EventType::KeyRepeat:
		{
			core::EventKeyRepeat& event_key_repeat = static_cast<core::EventKeyRepeat&>(event);

			CameraMovement camera_movement = _mapKeyToCameraMovement(event_key_repeat.getKey());
			if (camera_movement != CameraMovement::NONE)
				m_camera.ProcessKeyboard(camera_movement, core::getPrevFrameDelta());
			
			break;
		}
		case core::EventType::CursorPosition:
		{
			if(toggle_cursor_movement)
			{
				core::EventCursorPos& event_cursor_pos = static_cast<core::EventCursorPos&>(event);

				m_camera.ProcessMouseMovement(event_cursor_pos.getXpos(), event_cursor_pos.getYpos());
				break;
			}
		}
		}
	}

	void Renderer::update()
	{

	}

	void Renderer::draw()
	{
		for (Drawable* drawable_object : m_drawable_objects)
		{
			if(drawable_object->getType() == GL_FLOAT)
				_typeDraw<float>(*drawable_object);
			else
				_typeDraw<double>(*drawable_object);
		}

		m_drawable_objects.clear();
	}

	size_t Renderer::getID()
	{
		return m_id;
	}

	void Renderer::submitDrawable(Drawable& drawable_object)
	{
		m_drawable_objects.push_back(&drawable_object);
	}

	void Renderer::removeDrawable(Drawable& drawable_object)
	{
		for (auto iter = m_drawable_objects.begin(); iter != m_drawable_objects.end(); ++iter)
			if (*iter == &drawable_object)
				m_drawable_objects.erase(iter);
	}

	template<>
	void Renderer::_typeDraw<float>(Drawable& drawable_object)
	{

		static glm::mat4 projection;
		static glm::mat4 view;

		static int width = 0;
		static int height = 0;

		glfwGetFramebufferSize(m_window_handler, &width, &height);

		projection = glm::perspective(glm::radians(m_camera.Zoom), (float)(width) / (float)(height), 0.1f, 1000.0f);
		view = m_camera.GetViewMatrix();

		const DrawInfo draw_info = drawable_object.prepareDraw();

		draw_info.vao.bind();
		draw_info.shader.use();

		draw_info.shader.setf32Matrix4("projection", projection);
		draw_info.shader.setf32Matrix4("view", view);

		glDrawArraysInstanced(draw_info.mode, draw_info.first, draw_info.count, draw_info.instancecount);

		draw_info.shader.unuse();
		draw_info.vao.unbind();
		
	}

	template<>
	void Renderer::_typeDraw<double>(Drawable& drawable_object)
	{

		static glm::dmat4 projection;
		static glm::dmat4 view;

		static int width = 0;
		static int height = 0;

		glfwGetFramebufferSize(m_window_handler, &width, &height);

		projection = glm::perspective(glm::radians(m_camera.Zoom), (float)(width) / (float)(height), 0.1f, 1000.0f);
		view = m_camera.GetViewMatrix();

		const DrawInfo draw_info = drawable_object.prepareDraw();

		draw_info.vao.bind();
		draw_info.shader.use();

		draw_info.shader.setf32Matrix4("projection", projection);
		draw_info.shader.setf32Matrix4("view", view);

		glDrawArraysInstanced(draw_info.mode, draw_info.first, draw_info.count, draw_info.instancecount);

		draw_info.shader.unuse();
		draw_info.vao.unbind();

	}

	CameraMovement Renderer::_mapKeyToCameraMovement(int input)
	{
		switch (input)
		{
		case GLFW_KEY_W: 
			return CameraMovement::FORWARD;

		case GLFW_KEY_S: 
			return CameraMovement::BACKWARD;

		case GLFW_KEY_A: 
			return CameraMovement::LEFT;

		case GLFW_KEY_D: 
			return CameraMovement::RIGHT;

		default:
			return CameraMovement::NONE;
		}
	}



}
