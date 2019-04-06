cmake_minimum_required(VERSION 3.1)

# Config
include("${CMAKE_CURRENT_SOURCE_DIR}/Build/config.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/Build/scripts/FabulaHelpers.cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# === Extern
# glad (opengl glew lib)
add_subdirectory(Extern/glad)

# glm
add_subdirectory(Extern/glm)

# zlib
add_subdirectory(Extern/zlib)

# libpng
add_subdirectory(Extern/png)
target_include_directories(png
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Extern/zlib"
        "${CMAKE_BINARY_DIR}/Extern/zlib"
)

# freetype
add_subdirectory(Extern/freetype)

# SDL2
add_subdirectory(Extern/SDL2)
add_subdirectory(Extern/SDL2image)
target_include_directories(SDL2image
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Extern/png"
        "${CMAKE_BINARY_DIR}/Extern/png"
)
target_compile_definitions(SDL2image
    PRIVATE
        LOAD_PNG
)

# General
set_target_properties(
    SDL2
    SDL2main
    SDL2image
    glad
    glm
    zlib
    png
    pnggenfiles
    freetype

    PROPERTIES
        FOLDER Extern
)

# === Custom
# Fabula
add_subdirectory(Fabula)