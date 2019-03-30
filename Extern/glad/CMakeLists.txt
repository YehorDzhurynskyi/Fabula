cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(glad)

set(GLAD_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
if(ANDROID)
    set(GLAD_SOURCE_DIR "${GLAD_SOURCE_DIR}/gles/")
else()
    set(GLAD_SOURCE_DIR "${GLAD_SOURCE_DIR}/gl/")
endif()

add_library(glad STATIC)

target_sources(glad
    PRIVATE ${GLAD_SOURCE_DIR}glad.c
    PRIVATE ${GLAD_SOURCE_DIR}glad.h
    PRIVATE ${GLAD_SOURCE_DIR}khrplatform.h
)

target_include_directories(glad
    INTERFACE ${GLAD_SOURCE_DIR}
)
