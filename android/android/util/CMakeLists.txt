set(UTIL_SOURCES
    DisplayMetrics.cpp
    Log.cpp
)

set(UTIL_HEADERS
    Base64.h
    DisplayMetrics.h
    Log.h
)

if (WIN32)
    list(APPEND UTIL_SOURCES
        win/Base64Win.cpp
    )
endif ()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
    "${LIBRARY_PRODUCT_DIR}/include/ndk"
    "${LIBRARY_PRODUCT_DIR}/include"
)

add_library(android.util OBJECT ${UTIL_HEADERS} ${UTIL_SOURCES})
