set(WIDGET_SOURCES
    PopupMenu.cpp
)

set(WIDGET_HEADERS
    PopupMenu.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(android.widget OBJECT ${WIDGET_HEADERS} ${WIDGET_SOURCES})
