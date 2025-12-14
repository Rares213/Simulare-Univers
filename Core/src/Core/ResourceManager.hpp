#pragma once

#include <unordered_map>
#include <string>

#include "Shader.hpp"

namespace core
{
	namespace resname
	{
		constexpr const char* CIRCLE_SHADER_F32 = "circlef32";
		constexpr const char* CIRCLE_SHADER_F64 = "circlef64";
	}
	
	// singleton class that manages resources
	class ResourceManager
	{
	public:
		static std::unordered_map<std::string, Shader> shaders;

		static Shader& loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& name);
		static Shader& loadShader(const std::string& computeShaderPath, const std::string& name);
		static Shader& getShader(const std::string& name);

	private:

		static Shader loadShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath);
		static Shader loadShaderFromFile(const std::string& computePath);

		ResourceManager() {}


	};
}
