set(GFX_GEOMETRY_SOURCES
    flatten.cpp
    rasterize.cpp
    triangulate.cpp
    transform.cpp
)

add_library(gfx_geometry STATIC ${GFX_GEOMETRY_SOURCES})

target_link_libraries(gfx_geometry PUBLIC
    gfx_core
    gfx_utils
)

target_include_directories(gfx_geometry PUBLIC
        ${INCLUDE_DIR}
)

