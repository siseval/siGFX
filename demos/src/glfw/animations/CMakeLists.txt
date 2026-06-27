set(GLFW_ANIMATIONS_DEMO_SOURCES
        main.cpp
)

add_executable(glfw-animations-demo
        ${GLFW_ANIMATIONS_DEMO_SOURCES}
)

target_link_libraries(glfw-animations-demo PUBLIC
        demos_common_core
        gfx
)

target_include_directories(glfw-animations-demo PUBLIC
        ${INCLUDE_DIR}
)
