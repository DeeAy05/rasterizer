cmake_minimum_required(VERSION 3.15...4.3)

set(INC_FILES 
    inc/graphics/Window.hpp
)

set (SRC_FILES 
    src/Window.cpp
)
set (IMGUI_INC_FILES 
    ../externals/imgui/imconfig.h    
    ../externals/imgui/imgui.h
    ../externals/imgui/imgui_internal.h
    ../externals/imgui/imstb_rectpack.h
    ../externals/imgui/imstb_textedit.h
    ../externals/imgui/imstb_truetype.h
)
set (IMGUI_SRC_FILES 
    ../externals/imgui/imgui.cpp
    ../externals/imgui/imgui_demo.cpp
    ../externals/imgui/imgui_draw.cpp
    ../externals/imgui/imgui_tables.cpp
    ../externals/imgui/imgui_widgets.cpp
)
set (IMGUI_BACKEND_FILES 
    ../externals/imgui/backends/imgui_impl_sdl3.h
    ../externals/imgui/backends/imgui_impl_sdl3.cpp
    ../externals/imgui/backends/imgui_impl_sdlrenderer3.h
    ../externals/imgui/backends/imgui_impl_sdlrenderer3.cpp
)
set (IMGUI_FILES 
    ${IMGUI_INC_FILES}
    ${IMGUI_SRC_FILES}
    ${IMGUI_BACKEND_FILES}
)
set (ALL_FILES 
    ${INC_FILES}
    ${SRC_FILES}
    ${IMGUI_FILES}
    ../.clang-format
)

source_group(imgui FILES ${IMGUI_INC_FILES} ${IMGUI_SRC_FILES})
source_group(imgui/backends FILES ${IMGUI_BACKEND_FILES})

add_library(graphics STATIC ${ALL_FILES})
add_library(rasterizer::graphics ALIAS graphics)
target_compile_features(graphics PUBLIC cxx_std_23)
target_include_directories(graphics 
    PUBLIC inc
    PUBLIC ../externals/imgui
    PRIVATE ../externals/imgui/backends
        )
target_link_libraries(graphics PUBLIC SDL3::SDL3)

target_compile_options(graphics 
    PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wpedantic -Werror>
)
