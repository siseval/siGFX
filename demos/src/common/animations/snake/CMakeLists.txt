set(DEMOS_COMMON_ANIMATIONS_SNAKE_SOURCES
        snake-demo.cpp
)

add_library(demos_common_animations_snake STATIC
        ${DEMOS_COMMON_ANIMATIONS_SNAKE_SOURCES}
)

target_link_libraries(demos_common_animations_snake PRIVATE
        gfx
)

target_include_directories(demos_common_animations_snake PUBLIC
        ${INCLUDE_DIR}
)
