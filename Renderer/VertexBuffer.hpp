#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace core
{
	class VertexBuffer
	{
	public:
		VertexBuffer() noexcept;
		VertexBuffer(GLsizeiptr size, const void* data) noexcept;
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer(VertexBuffer& other) = delete;
		~VertexBuffer();

		void operator=(VertexBuffer&& other) noexcept;

		GLuint getID() noexcept;

		void allocVB(GLsizeiptr size, const void* data) noexcept;

		void updateVB(GLsizeiptr size, const void* data) noexcept;
		void updateVB(GLintptr  offset, GLsizeiptr size, const void* data) noexcept;

		void reallocVB(GLsizeiptr size) noexcept;

		void bindBufferBase(GLuint index) noexcept;

		void* mapBuffer(GLenum access) noexcept;
		GLboolean unmapBuffer() noexcept;

		void copyBuffer(const VertexBuffer& vb) noexcept;


	private:

		GLuint m_id = 0;
	};
}

