set(GFX_CORE_MATERIAL_SOURCES
        texture.cpp
        material.cpp
        vertex-shader.cpp
        fragment-shader.cpp
)

add_library(gfx_core_material STATIC ${GFX_CORE_MATERIAL_SOURCES})

target_include_directories(gfx_core_material PUBLIC
        ${INCLUDE_DIR}
)

