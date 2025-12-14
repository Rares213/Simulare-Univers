#include "Shader.hpp"

#include <iostream>

namespace core
{
	Shader::Shader(const char* vertexCode, const char* fragmentCode) noexcept
	{
		m_compileShader(vertexCode, fragmentCode);
	}

	Shader::Shader(const char* computeCode) noexcept
	{
		m_compileComputeShader(computeCode);
	}

	Shader::Shader(Shader&& other) noexcept
	{
		if (&other == this)
			return;

		this->~Shader();
		m_id = other.m_id;
		other.m_id = 0;
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}

	void Shader::operator=(Shader&& other) noexcept
	{
		if (&other == this)
			return;

		this->~Shader();
		m_id = other.m_id;
		other.m_id = 0;
	}

	void Shader::use() const noexcept
	{
		glUseProgram(m_id);
	}

	void Shader::unuse() const noexcept
	{
		glUseProgram(0);
	}

	void Shader::setf32(const std::string& name, float value) const noexcept
	{
		glProgramUniform1f(m_id, glGetUniformLocation(m_id, name.c_str()), value);
	}

	void Shader::setf64(const std::string& name, double value) const noexcept
	{
		glProgramUniform1d(m_id, glGetUniformLocation(m_id, name.c_str()), value);
	}

	void Shader::setf32vec4(const std::string& name, const glm::vec4& value) const noexcept
	{
		glProgramUniform4fv(m_id, glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
	}

	void Shader::setf64vec4(const std::string& name, const glm::dvec4& value) const noexcept
	{
		glProgramUniform4dv(m_id, glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
	}

	void Shader::setf32Matrix4(const std::string& name, const glm::mat4& matrix) const noexcept
	{
		glProgramUniformMatrix4fv(m_id, glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::setf64Matrix4(const std::string& name, const glm::dmat4& matrix) const noexcept
	{
		glProgramUniformMatrix4dv(m_id, glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}

	GLuint Shader::getID() noexcept
	{
		return m_id;
	}

	void Shader::m_compileShader(const char* vertexCode, const char* fragmentCode) noexcept
	{
		GLuint vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexCode, NULL);
		glCompileShader(vertex);
		m_checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentCode, NULL);
		glCompileShader(fragment);
		m_checkCompileErrors(fragment, "FRAGMENT");

		m_id = glCreateProgram();
		glAttachShader(m_id, vertex);
		glAttachShader(m_id, fragment);
		glLinkProgram(m_id);
		m_checkCompileErrors(m_id, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::m_compileComputeShader(const char* computeCode) noexcept
	{
		GLuint compute;

		compute = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(compute, 1, &computeCode, NULL);
		glCompileShader(compute);
		m_checkCompileErrors(compute, "COMPUTE");

		m_id = glCreateProgram();
		glAttachShader(m_id, compute);
		glLinkProgram(m_id);
		m_checkCompileErrors(m_id, "PROGRAM");

		glDeleteShader(compute);
	}

	void Shader::m_checkCompileErrors(unsigned int shader, std::string type) noexcept
	{
		int success;
		char infoLog[2048];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
}


