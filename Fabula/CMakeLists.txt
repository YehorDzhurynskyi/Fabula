cmake_minimum_required(VERSION 3.1)

project(Fabula VERSION 1.0.0)

fbl_include_script(PrecompiledHeader.cmake)

set(Fabula_SRCS
    pch.h
    pch.cpp
    main.cpp
    IApplication.h
    IApplication.cpp
    common.h
    config.h
    types.h

    Graphics/types.h
    Graphics/Buffer/Buffer.h
    Graphics/Buffer/IndexBuffer.h
    Graphics/Buffer/VertexBuffer.h
    Graphics/API/opengl.cpp
    Graphics/API/opengl.h
    Graphics/Shader/ShaderProgram.cpp
    Graphics/Shader/ShaderProgram.h

    Graphics/Camera/Camera.cpp
    Graphics/Camera/Camera.h
    Graphics/Camera/CameraOrtho.cpp
    Graphics/Camera/CameraOrtho.h
    Graphics/Camera/CameraPerspective.cpp
    Graphics/Camera/CameraPerspective.h

    Library/Pool.h
    Library/RotatingBuffer.h
    Library/Singleton.h
)

add_library(Fabula ${Fabula_SRCS})
add_precompiled_header(Fabula pch.h SOURCE_CXX pch.cpp)

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
        ${GFX_LIBRARY}
        SDL2
        SDL2image
        glad
        glm
#        freetype
)

set_target_properties(Fabula
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_definitions(Fabula
    PRIVATE
        _USE_MATH_DEFINES
)

target_include_directories(Fabula
    PUBLIC
        ${FBL_DIR}

    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
