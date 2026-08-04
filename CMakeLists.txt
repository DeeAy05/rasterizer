cmake_minimum_required(VERSION 3.15...4.3)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(RASTERIZER_BUILD_SAMPLES "Build samples for the rasterizer library" ON)

set(RASTERIZER_VERSION_MAJOR 0)
set(RASTERIZER_VERSION_MINOR 0)
set(RASTERIZER_VERSION_PATCH 1)
set(RASTERIZER_VERSION ${RASTERIZER_VERSION_MAJOR}.${RASTERIZER_VERSION_MINOR}.${RASTERIZER_VERSION_PATCH})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(rasterizer VERSION ${RASTERIZER_VERSION} LANGUAGES C CXX)

IF(MSVC)
    add_compile_options(/MP)
endif(MSVC)

add_subdirectory(externals EXCLUDE_FROM_ALL)
# add_subdirectory(math)
# add_subdirectory(graphics)
if (RASTERIZER_BUILD_SAMPLES)
    # add_subdirectory(samples)
endif(RASTERIZER_BUILD_SAMPLES)
