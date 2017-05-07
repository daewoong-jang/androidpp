set(CONTENT_SOURCES
    ComponentName.cpp
    Context.cpp
    ContextThemeWrapper.cpp
    ContextWrapper.cpp
    Intent.cpp
)

set(CONTENT_HEADERS
    ComponentName.h
    Context.h
    ContextThemeWrapper.h
    ContextWrapper.h
    DialogInterface.h
    Intent.h
    ServiceConnection.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_subdirectory(res)

add_library(android.content OBJECT ${CONTENT_HEADERS} ${CONTENT_SOURCES})
