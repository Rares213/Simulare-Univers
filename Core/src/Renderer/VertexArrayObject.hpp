#pragma once

#include "VertexBuffer.hpp"

#include <vector>
#include <string>

namespace core
{
	struct VertexBufferSpec
	{
		GLuint binding_index = 0;
		GLuint buffer = 0;
		GLintptr offset = 0;
		GLsizei stride = 0;
	};
	
	struct AttribFormatSpec
	{
		GLint size = 0;
		GLenum type = 0;
		GLboolean normalized = 0;
		GLuint relative_offset = 0;
	};

	struct AttribSpec
	{
		VertexBufferSpec vb_spec;
		std::vector<AttribFormatSpec> attrib_format_spec;
	};
	
	class VertexArrayObject
	{
	public:
		VertexArrayObject() noexcept;
		VertexArrayObject(VertexArrayObject&& other) noexcept;
		VertexArrayObject(VertexArrayObject& other) = delete;
		~VertexArrayObject();

		void operator=(VertexArrayObject&& other) noexcept;

		void bind() const noexcept;
		void unbind() const noexcept;

		void setAttribute(const AttribSpec& attrib_spec) noexcept;

	private:
		GLuint m_id = 0;
	};
}
