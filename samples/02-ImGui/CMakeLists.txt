cmake_minimum_required(VERSION 3.15...4.3)

add_executable(02-ImGui main.cpp ../../.clang-format)

target_link_libraries(02-ImGui 
        PRIVATE rasterizer::graphics
)