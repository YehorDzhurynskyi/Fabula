cmake_minimum_required(VERSION 3.1)

project(Fabula VERSION 1.0.0)

fbl_status_message("Configuring Fabula")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# === Extern
# glad (opengl glew lib)
fbl_status_message("Configuring Extern/glad")
add_subdirectory(Extern/glad)

# zlib
fbl_status_message("Configuring Extern/zlib")
add_subdirectory(Extern/zlib)

# libpng
fbl_status_message("Configuring Extern/png")
add_subdirectory(Extern/png)

# freetype
#fbl_status_message("Configuring Extern/freetype")
#add_subdirectory(Extern/freetype)

# SDL2
fbl_status_message("Configuring Extern/SDL2")
add_subdirectory(Extern/SDL2)
fbl_status_message("Configuring Extern/SDL2image")
add_subdirectory(Extern/SDL2image)

fbl_include_script(PrecompiledHeader.cmake)

set(Fabula_SRCS
    Fabula/pch.h
    Fabula/pch.cpp
    Fabula/main.cpp
    Fabula/IApplication.cpp
    Fabula/IApplication.h
    Fabula/common.h
    Fabula/config.h
    Fabula/types.h

    Fabula/Graphics/types.h
    Fabula/Graphics/VertexArray.cpp
    Fabula/Graphics/VertexArray.h
    Fabula/Graphics/Buffer/Buffer.h
    Fabula/Graphics/Buffer/VertexBuffer.h
    Fabula/Graphics/API/opengl.cpp
    Fabula/Graphics/API/opengl.h
    Fabula/Graphics/Shader/ShaderProgram.cpp
    Fabula/Graphics/Shader/ShaderProgram.h

    Fabula/Graphics/Camera/Camera.cpp
    Fabula/Graphics/Camera/Camera.h
    Fabula/Graphics/Camera/CameraOrtho.cpp
    Fabula/Graphics/Camera/CameraOrtho.h
    Fabula/Graphics/Camera/CameraPerspective.cpp
    Fabula/Graphics/Camera/CameraPerspective.h

    Fabula/Library/Pool.h
    Fabula/Library/RotatingBuffer.h
    Fabula/Library/Singleton.h
)

add_library(Fabula ${Fabula_SRCS})
add_precompiled_header(Fabula Fabula/pch.h SOURCE_CXX Fabula/pch.cpp)

fbl_pack_sources(${Fabula_SRCS})

if(ANDROID) # it is just mock
    find_package(OpenGLES2 REQUIRED)
    set(GFX_LIBRARY ${OPENGLES2_LIBRARY})
else()
    find_package(OpenGL REQUIRED)
    set(GFX_LIBRARY ${OPENGL_LIBRARY})
endif()

if(NOT DEFINED GFX_LIBRARY)
    fbl_fatal_message("GFX_LIBRARY is not found")
else()
    fbl_status_message("GFX_LIBRARY is set to: ${GFX_LIBRARY}")
endif()

target_link_libraries(Fabula
    PUBLIC
        EpiLib
        ${GFX_LIBRARY}
        SDL2
        SDL2image
        glad
#        freetype
)

fbl_set_default_target_properties(Fabula)

target_compile_definitions(Fabula
    PRIVATE
        _USE_MATH_DEFINES
)

target_include_directories(Fabula
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
