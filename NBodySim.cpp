
#include "Core.hpp"

#include "SimulationContextManager.hpp"

#include "AppResources.hpp"

int main(int argc, char** argv)
{
	std::filesystem::path app_main_dir("../../..");
	app_main_dir = std::filesystem::absolute(app_main_dir);
	std::filesystem::current_path(app_main_dir);

	std::cout << "Directory path: " << std::filesystem::current_path() << "\n";
	
	core::AppSpec spec;
	spec.win_spec.window_name = "NBodySim";
	spec.win_spec.enable_debug_gl = false;
	core::Application core_app(spec);

	LoadResources::loadComputeShaders();
	
	sim::SimulationManager simulation_manager(core_app.getWindow());
	core_app.run();
	
}
