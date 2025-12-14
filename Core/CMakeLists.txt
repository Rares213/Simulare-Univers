add_library(Corelib)

target_sources(Corelib
    PRIVATE
        src/Core/application.cpp
        src/Core/window.cpp
        src/Core/ResourceManager.cpp
        src/Renderer/VertexBuffer.cpp
        src/Renderer/VertexArrayObject.cpp
        src/Renderer/Shader.cpp
        src/Dependencies/include/glad/glad.c
        src/Renderer/Circle.cpp
        src/Renderer/CircleGL.cpp
        src/Renderer/Renderer.cpp
        src/Renderer/Camera.cpp

    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS 
        include;
        src/Dependencies/include;
        src/Renderer;
        src/Core;
    FILES
        src/Dependencies/include/glad/glad.h
        src/Core/application.hpp
        src/Core/Layer.hpp
        src/Core/window.hpp
        src/Core/ResourceManager.hpp
        src/Core/Event.hpp
        src/Dependencies/include/KHR/khrplatform.h
        src/Dependencies/include/glm/glm.hpp
        src/Dependencies/include/glm/gtc/type_ptr.hpp
        src/Renderer/Shader.hpp
        src/Renderer/VertexArrayObject.hpp
        src/Renderer/VertexBuffer.hpp
        src/Renderer/Drawable.hpp
        src/Renderer/Circle.hpp
        src/Renderer/CircleGL.hpp
        src/Renderer/Renderer.hpp
        src/Renderer/Camera.hpp
       
)

target_compile_features(Corelib PRIVATE cxx_std_20)

add_subdirectory(src/Dependencies)
