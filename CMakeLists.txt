cmake_minimum_required(VERSION 3.1)

set(FBL_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "")

# Config
include("${CMAKE_CURRENT_SOURCE_DIR}/Build/config.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/Build/scripts/FabulaHelpers.cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# === Extern
# glad (opengl glew lib)
fbl_status_message("Configuring Extern/glad")
add_subdirectory(Extern/glad)

# glm
fbl_status_message("Configuring Extern/glm")
add_subdirectory(Extern/glm)

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
#    freetype

    PROPERTIES
        FOLDER Extern
)

# === Custom
# Fabula
fbl_status_message("Configuring Fabula")
add_subdirectory(Fabula)
