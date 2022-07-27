include(${CMAKE_CURRENT_LIST_DIR}/StringUtil.cmake)

# Check if a variable is Defined, throw error if not
function(check_defined VAR)
    if(NOT DEFINED ${VAR})
        message(FATAL_ERROR "${VAR} not defined")
    endif()
endfunction()


# Check if a file exists, throw error if not
function(check_exists _filename)
    if(NOT EXISTS ${_filename})
        message(FATAL_ERROR "${_filename} doesn't exist")
    endif()
endfunction()


# Marshall filenames with |
function(wrap_filenames OUTPUT)
    string(JOIN "|" str ${ARGN})
    set(${OUTPUT} ${str} PARENT_SCOPE)
endfunction()


# Unmarshall filenames with |
function(unwrap_filenames filenames OUTPUT)
    string(REPLACE "\"" "" filenames ${filenames})
    string_split(list ${filenames} "|")
    set(${OUTPUT} ${list} PARENT_SCOPE)
endfunction()


macro(show_script_info)
    get_filename_component(SCRIPT_NAME ${CMAKE_CURRENT_LIST_FILE} NAME)
    message("Run ${SCRIPT_NAME} Script")
endmacro(show_script_info)
