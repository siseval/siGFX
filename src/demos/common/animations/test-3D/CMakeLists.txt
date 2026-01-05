set(DEMOS_COMMON_ANIMATIONS_TEST_3D_SOURCES
    test-3D-demo.cpp
)

add_library(demos_common_animations_test_3D STATIC
    ${DEMOS_COMMON_ANIMATIONS_TEST_3D_SOURCES}
)

target_link_libraries(demos_common_animations_test_3D PRIVATE
    gfx
)

target_include_directories(demos_common_animations_test_3D PUBLIC
    ${INCLUDE_DIR}
)
