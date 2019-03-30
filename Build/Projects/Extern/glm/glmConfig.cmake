set(GLM_VERSION "0.9.9")
set(GLM_INCLUDE_DIRS "C:/Users/kadzh_000/Source/Repos/Tundra/Extern/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
