#pragma once

#include "Drawable.hpp"

#include <array>


namespace core
{
	namespace gl
	{
		struct CircleSpec
		{
			size_t count = 0u;
			Shader& shader;
			std::array<VertexBuffer, 3u>& vbo_position;

			float radius = 0.005f;
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		};

		template<typename T>
		class Circle : public Drawable
		{
		public:
			Circle(CircleSpec spec);
			Circle(CircleSpec& spec);
			virtual ~Circle() {}

			// Inherited via Drawable
			virtual DrawInfo prepareDraw() override;
			virtual int getType() override;

			void setRadius(float radius);
			void setColor(const glm::vec4& color);

			void resize(size_t count);

		private:

			CircleSpec m_spec;

			VertexBuffer m_vbo_vertices;
			VertexArrayObject m_vao_vertices;

		private:

			void _constructCircle();
			int _getGLType();

		};

	}
	
}