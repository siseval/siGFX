add_subdirectory(gfx)

if (BUILD_DEMOS)
    add_subdirectory(demos)
endif ()

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
