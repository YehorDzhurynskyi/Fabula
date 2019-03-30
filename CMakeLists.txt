cmake_minimum_required(VERSION 3.1)

project(Tundra VERSION 1.0.0)

include(Build/config.cmake)

add_executable(Tundra
    Source/test.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(ANDROID) # it is just mock
    find_package(OpenGLES2 REQUIRED)

    target_link_libraries(Tundra
        ${OPENGLES2_LIBRARY}
    )
else()
    find_package(OpenGL REQUIRED)

    target_link_libraries(Tundra
        ${OPENGL_LIBRARY}
    )
endif()

add_subdirectory(Extern/SDL2)
add_subdirectory(Extern/glad)
add_subdirectory(Extern/glm)
add_subdirectory(Extern/zlib)
add_subdirectory(Extern/png)

target_link_libraries(Tundra
    SDL2
    glad
    glm
    zlib
    png
)
