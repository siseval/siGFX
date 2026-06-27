set(GFX_SHADERS_SOURCES
)

add_library(gfx_shaders INTERFACE ${GFX_SHADERS_SOURCES})

target_include_directories(gfx_shaders INTERFACE
        ${INCLUDE_DIR}
)

