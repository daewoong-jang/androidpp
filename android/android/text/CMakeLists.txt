set(TEXT_SOURCES
    Editable.cpp
    Selection.cpp
    SpannableStringBuilder.cpp
)

set(TEXT_HEADERS
    Editable.h
    Selection.h
    Spannable.h
    SpannableStringBuilder.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
    "${LIBRARY_PRODUCT_DIR}/include"
)

add_library(android.text OBJECT ${TEXT_HEADERS} ${TEXT_SOURCES})
