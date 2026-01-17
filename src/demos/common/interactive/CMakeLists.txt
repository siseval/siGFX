set(DEMOS_COMMON_INTERACTIVE_SOURCES
        interactive-demo.cpp
)

add_library(demos_common_interactive STATIC ${DEMOS_COMMON_INTERACTIVE_SOURCES})

target_link_libraries(demos_common_interactive PRIVATE
        gfx
)

target_include_directories(demos_common_interactive PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/demos
)
