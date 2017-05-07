set(APP_SOURCES
    Activity.cpp
    AlertDialog.cpp
    Service.cpp
)

set(APP_HEADERS
    Activity.h
    AlertDialog.h
    Service.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(android.app OBJECT ${APP_HEADERS} ${APP_SOURCES})
