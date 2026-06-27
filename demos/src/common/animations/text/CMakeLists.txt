set(DEMOS_COMMON_ANIMATIONS_TEXT_SOURCES
        text-demo.cpp
)

add_library(demos_common_animations_text STATIC
        ${DEMOS_COMMON_ANIMATIONS_TEXT_SOURCES}
)

target_link_libraries(demos_common_animations_text PRIVATE
        gfx
)

target_include_directories(demos_common_animations_text PUBLIC
        ${INCLUDE_DIR}
)
