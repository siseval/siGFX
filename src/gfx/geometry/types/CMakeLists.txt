set(GFX_GEOMETRY_TYPES_SOURCES
        barycentric-triangle.cpp
        polygon-mesh.cpp
)

add_library(gfx_geometry_types STATIC ${GFX_GEOMETRY_TYPES_SOURCES})

target_link_libraries(gfx_geometry_types PUBLIC
        gfx_core
)

target_include_directories(gfx_geometry_types PUBLIC
        ${INCLUDE_DIR}
)

