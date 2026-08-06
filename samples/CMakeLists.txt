cmake_minimum_required(VERSION 3.15...4.3)
macro (add_sample DIR_NAME)
    add_subdirectory(${DIR_NAME})
    set_target_properties(${DIR_NAME} 
        PROPERTIES 
            FOLDER samples
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${DIR_NAME}
    )
endmacro()

add_sample(01-ClearScreen)
add_sample(02-ImGui)