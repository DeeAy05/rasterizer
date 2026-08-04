cmake_minimum_required(VERSION 3.15...4.3)

set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build the SDL test library")
set(SDL_DISABLE_UNINSTALL OFF CACHE BOOL "Disable uninstallation of SDL3")
set(AUDIO_BUILD_SAMPLES OFF CACHE BOOL "Build the SDL audio samples")

add_subdirectory(Audio)
add_subdirectory(glm)
add_subdirectory(SDL)
add_subdirectory(SDL_TTF)
add_subdirectory(input)

set(SDL_TARGETS 
    SDL_uclibc SDL3-static SDL3-shared SDL3_ttf SDL3_ttf-static
    freetype harfbuzz plutosvg plutovg
)

set(GLM_TARGETS glm glm_shared)

macro( set_targets_folder TARGETS FOLDER) 
    foreach(target ${TARGETS})
        if (TARGET ${target})
            set_target_properties(${target} PROPERTIES FOLDER ${FOLDER})
        endif()
    endforeach()
endmacro()

set_targets_folder("${SDL_TARGETS}" "externals/SDL")
set_targets_folder("${GLM_TARGETS}" "externals/glm") 
set_targets_folder("Audio;input_SDL3" "externals")