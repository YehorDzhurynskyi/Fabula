cmake_minimum_required(VERSION 3.1)

# Config
include(Build/config.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# === Extern
# SDL2
add_subdirectory(Extern/SDL2)

# glad (opengl glew lib)
add_subdirectory(Extern/glad)

# glm
add_subdirectory(Extern/glm)

# zlib
add_subdirectory(Extern/zlib)

# libpng
add_subdirectory(Extern/png)
target_include_directories(png
    PRIVATE "${CMAKE_SOURCE_DIR}/Extern/zlib"
    PRIVATE "${CMAKE_BINARY_DIR}/Extern/zlib"
)

# General
set_target_properties(SDL2 SDL2main glad glm zlib png pnggenfiles
    PROPERTIES FOLDER Extern
)

# === Custom
# Fabula
add_subdirectory(Fabula)
