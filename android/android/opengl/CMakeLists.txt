set(OPENGL_SOURCES
    GLSurfaceView.cpp
)

set(OPENGL_HEADERS
    GLSurfaceView.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(android.opengl OBJECT ${OPENGL_HEADERS} ${OPENGL_SOURCES})
