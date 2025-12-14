#pragma once

#include "ResourceNames.hpp"
#include "ResourceManager.hpp"

#include <filesystem>
#include <iostream>

class LoadResources
{
public:

	static void loadComputeShaders()
	{
		std::filesystem::path path("./Physics/ForceFinder/cshaders/force_finder_f32.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Forcefinder direct f32 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::forcefinder_f32);

		path = std::filesystem::path("./Physics/ForceFinder/cshaders/force_finder_f64.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Forcefinder direct f64 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::forcefinder_f64);

		path = std::filesystem::path("./Physics/Integrator/cshaders/acc/acc_f32.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Acceleration f32 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::acc_f32);

		path = std::filesystem::path("./Physics/Integrator/cshaders/acc/acc_f64.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Acceleration f64 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::acc_f64);

		path = std::filesystem::path("./Physics/Integrator/cshaders/EulerImplicit/euler_implicit_f32.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Implicit Euler f32 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::euler_implicit_f32);

		path = std::filesystem::path("./Physics/Integrator/cshaders/EulerImplicit/euler_implicit_f64.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Implicit Euler f64 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::euler_implicit_f64);

		path = std::filesystem::path("./Physics/Integrator/cshaders/Leapfrog/leapfrog_first_stage_f32.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Leapfrog first stage f32 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::leapfrog_first_stage_f32);

		path = std::filesystem::path("./Physics/Integrator/cshaders/Leapfrog/leapfrog_first_stage_f64.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Leapfrog first stage f64 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::leapfrog_first_stage_f64);

		path = std::filesystem::path("./Physics/Integrator/cshaders/Leapfrog/leapfrog_second_stage_f32.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Leapfrog second stage f32 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::leapfrog_second_stage_f32);

		path = std::filesystem::path("./Physics/Integrator/cshaders/Leapfrog/leapfrog_second_stage_f64.comp");
		path = std::filesystem::absolute(path);
		std::cout << "Leapfrog second stage f64 path: " << path << "\n";
		core::ResourceManager::loadShader(path.string(), nbody::csname::leapfrog_second_stage_f64);
	}

private:
	LoadResources() {}
};