# CMakeList.txt : CMake project for LicentaFizica, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("NBodySim")

add_executable (NBodySim)
target_sources(NBodySim
	PRIVATE
		imgui/imgui.cpp
		imgui/imgui_demo.cpp
		imgui/imgui_draw.cpp
		imgui/imgui_impl_glfw.cpp
		imgui/imgui_impl_opengl3.cpp
		imgui/imgui_tables.cpp
		imgui/imgui_widgets.cpp

	PRIVATE
		NBodySim.cpp
		Bodies/Bodies.cpp
		Bodies/BodiesHost.cpp
		Bodies/BodiesGL.cpp
		Bodies/InitialConditions.cpp
		src/OverlayMenu.cpp
		src/SimulationHost.cpp
		src/SimulationGL.cpp
		src/TaskManager.cpp
		src/Timer.cpp
		src/DispatcherGui.cpp
		src/OverlayMenuSimComp.cpp
		src/OverlayMenuSimCompH.cpp
		src/OverlayMenuSimCompGL.cpp
		src/SimulationContext.cpp
		src/SimulationContextManager.cpp

	PRIVATE
		Physics/ForceFinder/ffhost/ff_host_g_abstract.cpp
		Physics/ForceFinder/ffhost/ff_host_g_direct.cpp
		Physics/ForceFinder/ffhost/ff_host_g_direct_simd.cpp
		Physics/ForceFinder/ffgl/ff_gl_g_abstract.cpp
		Physics/ForceFinder/ffgl/ff_gl_g_direct.cpp
		Physics/ForceFinder/ffhost/ff_host_g_bh.cpp
		Physics/Integrator/ihost/integrator_h_euler_explicit.cpp
		Physics/Integrator/ihost/integrator_h_euler_implicit.cpp
		Physics/Integrator/ihost/integrator_h_rk2.cpp
		Physics/Integrator/ihost/integrator_h_leapfrog.cpp
		Physics/Integrator/igl/integrator_gl_euler_implicit.cpp
		Physics/Integrator/igl/integrator_gl_leapfrog.cpp
		Physics/ForceFinder/ffgl/ff_gl_factory.cpp


	PUBLIC
	FILE_SET imguiHeaders
	BASE_DIRS
		imgui
	TYPE HEADERS
	FILES
		imgui/imconfig.h
		imgui/imgui.h
		imgui/imgui_impl_glfw.h
		imgui/imgui_impl_opengl3.h
		imgui/imgui_impl_opengl3_loader.h
		imgui/imgui_internal.h
		imgui/imstb_rectpack.h
		imgui/imstb_textedit.h
		imgui/imstb_truetype.h

	PUBLIC
	FILE_SET srcHeaders
	BASE_DIRS
		src
	TYPE HEADERS
	FILES
		src/SimulationInterface.hpp
		src/SimulationHost.hpp
		src/SimulationGL.hpp
		src/TaskManager.hpp
		src/Timer.hpp
		src/ProgramState.hpp
		src/logger.hpp
		src/SimulationState.hpp
		src/DispatcherGui.hpp
		src/OverlayMenu.hpp
		src/OverlayMenuSimComp.hpp
		src/OverlayMenuSimCompH.hpp
		src/OverlayMenuSimCompGL.hpp
		src/SimulationContext.hpp
		src/SimulationContextManager.hpp

	PUBLIC
	FILE_SET bodiesHeaders
	BASE_DIRS
		Bodies
	TYPE HEADERS
	FILES
		Bodies/Bodies.hpp
		Bodies/BodiesHost.hpp
		Bodies/BodiesGL.hpp
		Bodies/BodiesEnums.hpp
		Bodies/InitialConditions.hpp

	PUBLIC
	FILE_SET physicsHeaders
	BASE_DIRS
		Physics
	TYPE HEADERS
	FILES
		Physics/ForceFinder/ff_enum.hpp
		Physics/ForceFinder/ffhost/ff_host.hpp
		Physics/ForceFinder/ffhost/ff_host_factory.hpp
		Physics/ForceFinder/ffhost/ff_host_g_abstract.hpp 
		Physics/ForceFinder/ffhost/ff_host_g_direct.hpp
		Physics/ForceFinder/ffhost/ff_host_g_bh.hpp
		Physics/ForceFinder/ffgl/ff_gl.hpp
		Physics/ForceFinder/ffgl/ff_gl_factory.hpp
		Physics/ForceFinder/ffgl/ff_gl_g_abstract.hpp
		Physics/ForceFinder/ffgl/ff_gl_g_direct.hpp
		Physics/Integrator/integrator_enum.hpp
		Physics/Integrator/ihost/integrator_h.hpp
		Physics/Integrator/ihost/integrator_h_factory.hpp
		Physics/Integrator/igl/integrator_gl.hpp
		Physics/Integrator/igl/integrator_gl_factory.hpp
		Physics/Physics.hpp
		Physics/PhysicsGL.hpp


	INTERFACE
	FILE_SET interfaceHeaders
	BASE_DIRS
		src
	TYPE HEADERS
	FILES
		src/ObserverGui.hpp
		src/ResouceNames.hpp
		src/AppResources.hpp

)

target_compile_features(NBodySim PRIVATE cxx_std_20)
if(
  (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR
  (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
	target_compile_options(NBodySim PRIVATE /W3)
	
elseif(
  (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
  (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
	target_compile_options(NBodySim PRIVATE -Wall)
endif()

add_subdirectory(Core)

target_link_libraries(NBodySim
	PUBLIC
	Corelib
	glfw3lib
)



# TODO: Add tests and install targets if needed.
