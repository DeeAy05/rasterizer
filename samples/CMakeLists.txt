cmake_minimum_required(VERSION 3.15...4.3)

add_subdirectory(01-ClearScreen)

set_target_properties(01-ClearScreen 
        PROPERTIES 
            FOLDER samples
            DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
) 