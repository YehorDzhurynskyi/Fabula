if(MSVC) # or other generator platform which support multi-configuration build
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Multi-configuration types" FORCE)
endif()

set(GLM_TEST_ENABLE OFF CACHE BOOL "Generate Unit tests for 'glm' lib")
set(PNG_BUILD_ZLIB ON CACHE BOOL "Custom zlib Location, else find_package is used")
set(PNG_TESTS OFF CACHE BOOL "Generate Unit tests for 'png' lib")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
