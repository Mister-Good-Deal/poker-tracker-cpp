# Get all properties that cmake supports
if (NOT CMAKE_PROPERTY_LIST)
    execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)

    # Convert command output into a CMake list
    string(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
    string(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
endif ()

function(print_properties)
    message("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
endfunction()

function(print_target_properties target)
    if (NOT TARGET ${target})
        message(STATUS "There is no target named '${target}'")
        return()
    endif ()

    message(STATUS "Properties of '${target}':")

    foreach (property ${CMAKE_PROPERTY_LIST})
        string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" property ${property})

        # Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
        if (property STREQUAL "LOCATION" OR property MATCHES "^LOCATION_" OR property MATCHES "_LOCATION$")
            continue()
        endif ()

        get_property(was_set TARGET ${target} PROPERTY ${property} SET)

        if (was_set)
            get_target_property(value ${target} ${property})
            message(STATUS "${target} ${property} = ${value}")
        endif ()
    endforeach ()
endfunction()

macro(PRINT_ALL_VARIABLES)
    message(STATUS "print_all_variables------------------------------------------{")

    get_cmake_property(_variableNames VARIABLES)

    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach ()

    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

function(extract_version)
    file(READ "${CMAKE_SOURCE_DIR}/VERSION.txt" file_contents)

    set(POKER_BOT_VERSION "${file_contents}" PARENT_SCOPE)
endfunction()
