#pragma once

#include <glad/glad.h>

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace core
{
	class Shader
	{
	public:
		Shader() {}
		Shader(const char* vertexCode, const char* fragmentCode) noexcept;
		Shader(const char* computeCode) noexcept;
		Shader(Shader&& other) noexcept;
		Shader(Shader& other) = delete;
		~Shader();

		void operator=(Shader&& other) noexcept;

		void use() const noexcept;
		void unuse() const noexcept;

		void setf32(const std::string& name, float value) const noexcept;
		void setf64(const std::string& name, double value) const noexcept;
		void setf32vec4(const std::string& name, const glm::vec4& value) const noexcept;
		void setf64vec4(const std::string& name, const glm::dvec4& value) const noexcept;
		void setf32Matrix4(const std::string& name, const glm::mat4& matrix) const noexcept;
		void setf64Matrix4(const std::string& name, const glm::dmat4& matrix) const noexcept;

		GLuint getID() noexcept;

	private:
		GLuint m_id = 0;

		void m_compileShader(const char* vertexCode, const char* fragmentCode) noexcept;
		void m_compileComputeShader(const char* computeCode) noexcept;

		void m_checkCompileErrors(unsigned int shader, std::string type) noexcept;
	};
}
