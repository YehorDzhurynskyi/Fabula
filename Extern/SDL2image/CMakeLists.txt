cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(SDL2image)

add_library(SDL2image)

target_sources(SDL2image
    PRIVATE IMG.c
    PRIVATE IMG_bmp.c
    PRIVATE IMG_gif.c
    PRIVATE IMG_jpg.c
    PRIVATE IMG_lbm.c
    PRIVATE IMG_pcx.c
    PRIVATE IMG_png.c
    PRIVATE IMG_pnm.c
    PRIVATE IMG_svg.c
    PRIVATE IMG_tga.c
    PRIVATE IMG_tif.c
    PRIVATE IMG_webp.c
    PRIVATE IMG_WIC.c
    PRIVATE IMG_xcf.c
    PRIVATE IMG_xpm.c
    PRIVATE IMG_xv.c
    PRIVATE IMG_xxx.c

    PRIVATE miniz.h
    PRIVATE nanosvg.h
    PRIVATE nanosvgrast.h
    PRIVATE SDL_image.h
)

target_include_directories(SDL2image
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SDL2image
    png
    SDL2
)
