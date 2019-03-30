cmake_minimum_required(VERSION 3.1)

project(Tundra VERSION 1.0.0)

include(Build/config.cmake)

add_executable(Tundra
    Source/test.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
    find_package(OpenGL REQUIRED)

    target_compile_definitions(Tundra
        PRIVATE GLEW_STATIC
    )
    
    target_link_libraries(Tundra
        ${OPENGL_LIBRARY}
    )
elseif(ANDROID)
    find_package(OpenGLES2 REQUIRED)
endif()

add_subdirectory(Extern/SDL2)
add_subdirectory(Extern/glad)
add_subdirectory(Extern/glm)

target_link_libraries(Tundra
    SDL2
    glad
    glm
)
