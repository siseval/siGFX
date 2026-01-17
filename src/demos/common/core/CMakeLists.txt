set(DEMOS_COMMON_CORE_SOURCES
        demo-player.cpp
)

add_library(demos_common_core STATIC
        ${DEMOS_COMMON_CORE_SOURCES}
)

target_link_libraries(demos_common_core PRIVATE
        demos_common_animations
        gfx
)

target_include_directories(demos_common_core PUBLIC
        ${INCLUDE_DIR}
)
