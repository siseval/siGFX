set(GFX_UTILS_SOURCES
    log.cpp
)

add_library(gfx_utils STATIC ${GFX_UTILS_SOURCES})

target_include_directories(gfx_utils PUBLIC
        ${INCLUDE_DIR}
)

