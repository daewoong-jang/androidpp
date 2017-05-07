set(INPUTMETHOD_SOURCES
    BaseInputConnection.cpp
    CursorAnchorInfo.cpp
    InputConnection.cpp
    InputMethodManager.cpp
)

set(INPUTMETHOD_HEADERS
    BaseInputConnection.h
    CursorAnchorInfo.h
    EditorInfo.h
    InputConnection.h
    InputMethodManager.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(android.view.inputmethod OBJECT ${INPUTMETHOD_HEADERS} ${INPUTMETHOD_SOURCES})
