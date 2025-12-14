#include "Circle.hpp"


#include "ResourceManager.hpp"

#include <algorithm>


namespace core
{

	template<typename T>
	Circle<T>::Circle(CircleSpec<T> spec) 
		: 
		m_spec(spec),
		m_shader(_getShader())
	{
		const size_t count = m_spec.position[0].size();

		const std::array<float, 9> vertices = { 0, 2, 0, 2, -2, 0, -2, -2, 0 };

		m_vbos[0] = core::VertexBuffer(vertices.size() * sizeof(float), vertices.data());

		m_vbos[1] = core::VertexBuffer(count * sizeof(T), m_spec.position[0].data());
		m_vbos[2] = core::VertexBuffer(count * sizeof(T), m_spec.position[1].data());
		m_vbos[3] = core::VertexBuffer(count * sizeof(T), m_spec.position[2].data());

		core::VertexBufferSpec vertices_buffer_spec;
		vertices_buffer_spec.binding_index = 0;
		vertices_buffer_spec.buffer = m_vbos[0].getID();
		vertices_buffer_spec.offset = 0;
		vertices_buffer_spec.stride = 3 * sizeof(float);

		core::AttribFormatSpec attrib_ptr_vertices_spec;
		attrib_ptr_vertices_spec.normalized = GL_FALSE;
		attrib_ptr_vertices_spec.relative_offset = 0;
		attrib_ptr_vertices_spec.type = GL_FLOAT;
		attrib_ptr_vertices_spec.size = 3;

		core::AttribSpec attrib_spec;
		attrib_spec.vb_spec = vertices_buffer_spec;
		attrib_spec.attrib_format_spec.push_back(attrib_ptr_vertices_spec);

		m_vao.setAttribute(attrib_spec);

		m_shader.setf32("radius", m_spec.radius);
		m_shader.setf32vec4("color", m_spec.color);

	}

	template<typename T>
	Circle<T>::~Circle()
	{

	}

	template<typename T>
	DrawInfo Circle<T>::prepareDraw()
	{
		m_vbos[1].bindBufferBase(1);
		m_vbos[2].bindBufferBase(2);
		m_vbos[3].bindBufferBase(3);

		const size_t count = m_spec.position[0].size();

		m_vbos[1].updateVB(count * sizeof(T), m_spec.position[0].data());
		m_vbos[2].updateVB(count * sizeof(T), m_spec.position[1].data());
		m_vbos[3].updateVB(count * sizeof(T), m_spec.position[2].data());

		DrawInfo draw_info{ m_vao, m_shader };
		draw_info.instancecount = count;

		return draw_info;
	}

	template<typename T>
	int Circle<T>::getType()
	{
		return _getGLType();
	}

	template<typename T>
	void Circle<T>::setRadius(float radius)
	{
		m_spec.radius = radius;
		m_shader.setf32("radius", m_spec.radius);
	}

	template<typename T>
	void Circle<T>::setColor(const glm::vec4& color)
	{
		m_spec.color = color;
		m_shader.setf32vec4("color", m_spec.color);
	}

	template<typename T>
	void Circle<T>::resize(size_t count)
	{
		m_vbos[1].reallocVB(count * sizeof(T));
		m_vbos[2].reallocVB(count * sizeof(T));
		m_vbos[3].reallocVB(count * sizeof(T));
	}

	template<>
	Shader& Circle<float>::_getShader()
	{
		return core::ResourceManager::getShader(core::resname::CIRCLE_SHADER_F32);
	}

	template<>
	Shader& Circle<double>::_getShader()
	{
		return core::ResourceManager::getShader(core::resname::CIRCLE_SHADER_F64);
	}

	template<>
	int Circle<float>::_getGLType() { return GL_FLOAT; }

	template<>
	int Circle<double>::_getGLType() { return GL_DOUBLE; }

	template class Circle<float>;
	template class Circle<double>;
}
