find_package(glfw3 REQUIRED)

set(GFX_SURFACES_GLFW_SOURCES
        glad.c
        glfw-render-surface.cpp
)

add_library(gfx_surfaces_glfw STATIC ${GFX_SURFACES_GLFW_SOURCES})

target_link_libraries(gfx_surfaces_glfw PUBLIC
        gfx
        glfw
        "-framework OpenGL"
)

target_include_directories(gfx_surfaces_glfw PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)



