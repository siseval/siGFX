set(DEMOS_COMMON_ANIMATIONS_SHADER_SOURCES
        shader-demo.cpp
)

add_library(demos_common_animations_shader STATIC
        ${DEMOS_COMMON_ANIMATIONS_SHADER_SOURCES}
)

target_link_libraries(demos_common_animations_shader PRIVATE
        gfx
)

target_include_directories(demos_common_animations_shader PUBLIC
        ${INCLUDE_DIR}
)
