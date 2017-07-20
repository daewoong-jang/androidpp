set(GRAPHICS_SOURCES
    SurfaceTexturePrivate.cpp
)

set(GRAPHICS_HEADERS
    SurfaceTexturePrivate.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(private.android.graphics OBJECT ${GRAPHICS_HEADERS} ${GRAPHICS_SOURCES})
