#pragma once

#include "Drawable.hpp"

#include <vector>
#include <array>

namespace core
{
	template<typename T>
	struct CircleSpec
	{
		std::vector<std::vector<T>>& position;

		float radius = 0.005f;
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
	
	template<typename T>
	class Circle : public Drawable
	{
	public:
		Circle(CircleSpec<T> spec);
		virtual ~Circle();

		virtual DrawInfo prepareDraw() override;
		virtual int getType() override;

		void setRadius(float radius);
		void setColor(const glm::vec4& color);

		void resize(size_t count);

	private:

		CircleSpec<T> m_spec;
		Shader& m_shader;

		std::array<VertexBuffer, 4u> m_vbos;
		VertexArrayObject m_vao;

	private:

		int _getGLType();
		Shader& _getShader();
	};

	template<> int Circle<float>::_getGLType();
	template<> int Circle<double>::_getGLType();
	

}