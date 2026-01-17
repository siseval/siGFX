set(GFX_CORE_SOURCES
        primitive-2D.cpp
        primitive-3D.cpp
        render-2D.cpp
        render-3D.cpp
        render-engine.cpp
        render-surface.cpp
        scene-graph-2D.cpp
        scene-graph-3D.cpp
        camera.cpp
)

add_library(gfx_core STATIC ${GFX_CORE_SOURCES})

add_subdirectory(types)

target_link_libraries(gfx_core PUBLIC
        gfx_core_types
)

target_include_directories(gfx_core PUBLIC
        ${INCLUDE_DIR}
)

