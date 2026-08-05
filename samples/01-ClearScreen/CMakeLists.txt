cmake_minimum_required(VERSION 3.15...4.3)

add_executable(01-ClearScreen main.cpp ../../.clang-format)

target_link_libraries(01-ClearScreen 
        PRIVATE rasterizer::graphics
)