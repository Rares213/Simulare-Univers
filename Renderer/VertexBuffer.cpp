#include "VertexBuffer.hpp"

#include <algorithm>

namespace core
{
	VertexBuffer::VertexBuffer() noexcept
	{
		glCreateBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(GLsizeiptr size, const void* data) noexcept
	{
		glCreateBuffers(1, &m_id);
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	{
		if (&other == this)
			return;

		this->~VertexBuffer();
		m_id = other.m_id;

		other.m_id = 0;
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		if (&other == this)
			return;

		this->~VertexBuffer();
		m_id = other.m_id;

		other.m_id = 0;
	}

	GLuint VertexBuffer::getID() noexcept
	{
		return m_id;
	}

	void VertexBuffer::allocVB(GLsizeiptr size, const void* data) noexcept
	{
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::updateVB(GLsizeiptr size, const void* data) noexcept
	{
		glNamedBufferSubData(m_id, 0, size, data);
	}

	void VertexBuffer::updateVB(GLintptr offset, GLsizeiptr size, const void* data) noexcept
	{
		glNamedBufferSubData(m_id, offset, size, data);
	}

	void VertexBuffer::reallocVB(GLsizeiptr size) noexcept
	{
		GLint64 size_current_buffer;
		glGetNamedBufferParameteri64v(m_id, GL_BUFFER_SIZE, &size_current_buffer);

		VertexBuffer temp_new_size_buffer(size, nullptr);

		const GLsizeiptr min_size = std::min(size, size_current_buffer);

		glCopyNamedBufferSubData(m_id, temp_new_size_buffer.m_id, 0, 0, min_size);

		*this = std::move(temp_new_size_buffer);
	}

	void VertexBuffer::bindBufferBase(GLuint index) noexcept
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_id);
	}

	void* VertexBuffer::mapBuffer(GLenum access) noexcept
	{
		return glMapNamedBuffer(m_id, access);
	}

	GLboolean VertexBuffer::unmapBuffer() noexcept
	{
		return glUnmapNamedBuffer(m_id);
	}

	void VertexBuffer::copyBuffer(const VertexBuffer& vb) noexcept
	{
		GLint64 this_size;
		glGetNamedBufferParameteri64v(m_id, GL_BUFFER_SIZE, &this_size);

		GLint64 vb_size;
		glGetNamedBufferParameteri64v(vb.m_id, GL_BUFFER_SIZE, &vb_size);

		if(this_size != vb_size)
			glNamedBufferData(m_id, vb_size, NULL, GL_STATIC_DRAW);

		glCopyNamedBufferSubData(vb.m_id, m_id, 0, 0, vb_size);
	}

}

