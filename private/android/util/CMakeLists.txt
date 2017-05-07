set(UTIL_SOURCES
)

set(UTIL_HEADERS
    PlatformDisplayMetrics.h
)

if (WIN32)
    list(APPEND UTIL_SOURCES
        win/LogWin.cpp
        win/PlatformDisplayMetricsWin.cpp
    )
endif ()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(private.android.util OBJECT ${UTIL_HEADERS} ${UTIL_SOURCES})
