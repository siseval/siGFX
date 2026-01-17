find_package(glfw3 REQUIRED)

set(GFX_SURFACES_GLFW_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/glad.c
        glfw-render-surface.cpp
)

add_library(gfx_surfaces_glfw STATIC ${GFX_SURFACES_GLFW_SOURCES})

target_link_libraries(gfx_surfaces_glfw PUBLIC
        gfx_core
        glfw
        "-framework OpenGL"
)

target_include_directories(gfx_surfaces_glfw PUBLIC
        ${INCLUDE_DIR}
)



