#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "glm" for configuration "Debug"
set_property(TARGET glm APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(glm PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/glm.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS glm )
list(APPEND _IMPORT_CHECK_FILES_FOR_glm "${_IMPORT_PREFIX}/lib/glm.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
