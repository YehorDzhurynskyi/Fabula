function(fbl_pack_sources)
    if(NOT MSVC OR NOT WIN32)
        message(FATAL_ERROR "This function doesn't support selected platform, it's necessary replace slashes by back-slashes only on WIN platform")
    endif()
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction()

function(fbl_include_script)
    foreach(_script IN ITEMS ${ARGN})
        include("${FBL_DIR}/Build/scripts/${_script}")
    endforeach()
endfunction()

function(fbl_status_message _message)
    message(STATUS "**********************[FABULA][STATUS] ${_message}")
endfunction()

function(fbl_fatal_message _message)
    message(FATAL_ERROR "**********************[FABULA][FATAL ERROR] ${_message}")
endfunction()

function(fbl_set_default_target_properties _target)
    set_target_properties(${_target}
        PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )
endfunction()
