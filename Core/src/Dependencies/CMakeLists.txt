add_library(glfw3lib INTERFACE)

target_include_directories(glfw3lib
    INTERFACE
    Include
)

target_link_directories(glfw3lib
    INTERFACE
    Libs
)

target_link_libraries(glfw3lib
    INTERFACE
        glfw3    
)