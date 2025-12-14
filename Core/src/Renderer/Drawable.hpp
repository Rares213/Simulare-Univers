#pragma once

#include "VertexBuffer.hpp"
#include "VertexArrayObject.hpp"
#include "Shader.hpp"

namespace core
{

	struct DrawInfo
	{
		const VertexArrayObject& vao;
		const Shader& shader;

		GLenum mode = GL_TRIANGLES;
		GLint first = 0;
		GLsizei count = 3;
		GLsizei instancecount = 0;
	};
	
	class Drawable
	{
	public:

		virtual ~Drawable() {}

		virtual DrawInfo prepareDraw() = 0;
		virtual int getType() = 0;
	};
}