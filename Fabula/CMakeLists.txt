cmake_minimum_required(VERSION 3.1)

project(Fabula VERSION 1.0.0)

include("${CMAKE_SOURCE_DIR}/Build/scripts/PrecompiledHeader.cmake")

add_executable(Fabula
    pch.h
    pch.cpp
    test.cpp
)

add_precompiled_header(Fabula pch.h SOURCE_CXX pch.cpp FORCEINCLUDE)

if(ANDROID) # it is just mock
    find_package(OpenGLES2 REQUIRED)

    target_link_libraries(Fabula
        ${OPENGLES2_LIBRARY}
    )
else()
    find_package(OpenGL REQUIRED)

    target_link_libraries(Fabula
        ${OPENGL_LIBRARY}
    )
endif()

target_link_libraries(Fabula
    SDL2
    glad
    glm
    zlib
    png
    freetype
)
