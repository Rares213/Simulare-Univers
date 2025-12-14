#include "CircleGL.hpp"

namespace core
{
	namespace gl
	{
		template<typename T>
		Circle<T>::Circle(CircleSpec spec)
			: m_spec(spec)
		{
			_constructCircle();
		}

		template<typename T>
		Circle<T>::Circle(CircleSpec& spec)
			: m_spec(spec)
		{
			_constructCircle();
		}

		template<typename T>
		DrawInfo Circle<T>::prepareDraw()
		{
			m_spec.vbo_position[0].bindBufferBase(1u);
			m_spec.vbo_position[1].bindBufferBase(2u);
			m_spec.vbo_position[2].bindBufferBase(3u);

			DrawInfo draw_info{ m_vao_vertices, m_spec.shader };
			draw_info.instancecount = m_spec.count;

			return draw_info;
		}

		template<typename T>
		void Circle<T>::setRadius(float radius)
		{
			m_spec.radius = radius;
			m_spec.shader.setf32("radius", m_spec.radius);
		}

		template<typename T>
		void Circle<T>::setColor(const glm::vec4& color)
		{
			m_spec.color = color;
			m_spec.shader.setf32vec4("color", m_spec.color);
		}

		template<typename T>
		void Circle<T>::resize(size_t count)
		{
			m_spec.count = count;
		}

		template<typename T>
		int Circle<T>::getType()
		{
			return _getGLType();
		}

		template<typename T>
		void Circle<T>::_constructCircle()
		{
			const std::array<T, 9> vertices = { 0, 2, 0, 2, -2, 0, -2, -2, 0 };

			m_vbo_vertices = VertexBuffer(vertices.size() * sizeof(T), vertices.data());

			VertexBufferSpec vertices_buffer_spec;
			vertices_buffer_spec.binding_index = 0;
			vertices_buffer_spec.buffer = m_vbo_vertices.getID();
			vertices_buffer_spec.offset = 0;
			vertices_buffer_spec.stride = 3 * sizeof(T);

			AttribFormatSpec attrib_ptr_vertices_spec;
			attrib_ptr_vertices_spec.normalized = GL_FALSE;
			attrib_ptr_vertices_spec.relative_offset = 0;
			attrib_ptr_vertices_spec.type = _getGLType();
			attrib_ptr_vertices_spec.size = 3;

			AttribSpec attrib_spec;
			attrib_spec.vb_spec = vertices_buffer_spec;
			attrib_spec.attrib_format_spec.push_back(attrib_ptr_vertices_spec);

			m_vao_vertices.setAttribute(attrib_spec);

			m_spec.shader.setf32("radius", m_spec.radius);
			m_spec.shader.setf32vec4("color", m_spec.color);
		}

		template<>
		int Circle<float>::_getGLType()
		{
			return GL_FLOAT;
		}

		template<>
		int Circle<double>::_getGLType()
		{
			return GL_DOUBLE;
		}

		template class Circle<float>;
		template class Circle<double>;
	}
}