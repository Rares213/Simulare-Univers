#include "ResourceManager.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace core
{
	std::unordered_map<std::string, Shader> ResourceManager::shaders;


	Shader& ResourceManager::loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& name)
	{
		shaders[name] = loadShaderFromFile(vertexShaderPath, fragmentShaderPath);
		return shaders[name];
	}

	Shader& ResourceManager::loadShader(const std::string& computeShaderPath, const std::string& name)
	{
		shaders[name] = loadShaderFromFile(computeShaderPath);
		return shaders[name];
	}

	Shader& ResourceManager::getShader(const std::string& name)
	{
		return shaders.at(name);
	}

	Shader ResourceManager::loadShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);

			std::stringstream vertexShaderStream, fragmentShaderStream;

			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			vertexShaderFile.close();
			fragmentShaderFile.close();

			vertexCode = vertexShaderStream.str();
			fragmentCode = fragmentShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR: SHADER FILE NOT SUCCESSFULLY READ: " << e.what() << std::endl;
		}

		Shader temp_shader(vertexCode.c_str(), fragmentCode.c_str());
		return temp_shader;
	}

	Shader ResourceManager::loadShaderFromFile(const std::string& computePath)
	{
		std::string computeCode;
		std::ifstream computeShaderFile;

		computeShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			computeShaderFile.open(computePath);

			std::stringstream computeShaderStream;

			computeShaderStream << computeShaderFile.rdbuf();

			computeShaderFile.close();

			computeCode = computeShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR: SHADER FILE NOT SUCCESSFULLY READ: " << e.what() << std::endl;
		}

		Shader temp_compute_shader(computeCode.c_str());
		return temp_compute_shader;
	}
}
