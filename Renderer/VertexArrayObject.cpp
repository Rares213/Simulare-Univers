#include "VertexArrayObject.hpp"

#include <iostream>

namespace core
{
	VertexArrayObject::VertexArrayObject() noexcept
	{
		glCreateVertexArrays(1, &m_id);
	}

	VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
	{
		if (&other == this)
			return;

		this->~VertexArrayObject();
		m_id = other.m_id;
		other.m_id = 0;
	}

	VertexArrayObject::~VertexArrayObject()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArrayObject::operator=(VertexArrayObject&& other) noexcept
	{
		if (&other == this)
			return;

		this->~VertexArrayObject();
		m_id = other.m_id;
		other.m_id = 0;
	}

	void VertexArrayObject::bind() const noexcept
	{
		glBindVertexArray(m_id);
	}

	void VertexArrayObject::unbind() const noexcept
	{
		glBindVertexArray(0);
	}

	void VertexArrayObject::setAttribute(const AttribSpec& attrib_spec) noexcept
	{
		glVertexArrayVertexBuffer(m_id, attrib_spec.vb_spec.binding_index, attrib_spec.vb_spec.buffer, attrib_spec.vb_spec.offset, attrib_spec.vb_spec.stride);

		for (size_t i = 0; i < attrib_spec.attrib_format_spec.size(); i++)
		{
			glEnableVertexArrayAttrib(m_id, i);

			glVertexArrayAttribFormat(
				m_id, 
				i, 
				attrib_spec.attrib_format_spec[i].size, 
				attrib_spec.attrib_format_spec[i].type, 
				attrib_spec.attrib_format_spec[i].normalized,
				attrib_spec.attrib_format_spec[i].relative_offset);

			glVertexArrayAttribBinding(m_id, i, attrib_spec.vb_spec.binding_index);
		}
	}

}
