set(GRAPHICS_SOURCES
    Bitmap.cpp
)

set(GRAPHICS_HEADERS
    Bitmap.h
    Point.h
    PointF.h
    Rect.h
    RectF.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(android.graphics OBJECT ${GRAPHICS_HEADERS} ${GRAPHICS_SOURCES})
