cmake_minimum_required(VERSION 3.1)

project(Fabula VERSION 1.0.0)

fbl_include_script(PrecompiledHeader.cmake)

set(Fabula_SRCS
    pch.h
    pch.cpp
    main.cpp
    application.h
    application.cpp
    common.h
    common.cpp
    environment.h
    types.h

    Assets/SpriteAtlas.cpp
    Assets/SpriteAtlas.h

    CCC/Camera.h
    CCC/Camera.cpp

    Game/Level.cpp
    Game/Level.h
    Game/MotionBlurRenderPass.cpp
    Game/MotionBlurRenderPass.h
    Game/Player.cpp
    Game/Player.h
    Game/StaticRenderPass.cpp
    Game/StaticRenderPass.h

    Graphics/DynamicBuffer.h
    Graphics/DynamicIndexBuffer.h
    Graphics/DynamicVertexBuffer.h
    Graphics/Renderer.cpp
    Graphics/Renderer.h
    Graphics/API/opengl.cpp
    Graphics/API/opengl.h
    Graphics/Shader/ShaderProgram.cpp
    Graphics/Shader/ShaderProgram.h
    Graphics/Text/TextRenderer.cpp
    Graphics/Text/TextRenderer.h

    Layer/ApplicationLayer.cpp
    Layer/ApplicationLayer.h
    Layer/GameLayer.cpp
    Layer/GameLayer.h
    Layer/HUDLayer.cpp
    Layer/HUDLayer.h
    Layer/Layer.cpp
    Layer/Layer.h
    Layer/LayerStack.cpp
    Layer/LayerStack.h
    Layer/Node.h
    Layer/Event/Event.cpp
    Layer/Event/Event.h
    Layer/Event/EventBus.cpp
    Layer/Event/EventBus.h

    Library/Pool.h
    Library/RotatingBuffer.h
    Library/Singleton.h

    Physics/CircleCollider.cpp
    Physics/CircleCollider.h
)

add_executable(Fabula ${Fabula_SRCS})
add_precompiled_header(Fabula pch.h SOURCE_CXX pch.cpp)

fbl_pack_sources(${Fabula_SRCS})

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
    SDL2image
    glad
    glm
    freetype
)

set_target_properties(Fabula
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_definitions(Fabula
    PRIVATE _USE_MATH_DEFINES
)

target_include_directories(Fabula
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

if(EXISTS "${CMAKE_SOURCE_DIR}/Data")
    add_custom_command(TARGET Fabula POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Data"
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )
    fbl_status_message("[CUSTOM POST BUILD STEP] Move project `Data` into Fabula binary directory")
else()
    fbl_fatal_message("Project `Data` should be located here: ${CMAKE_SOURCE_DIR}/Data")
endif()
