set(CONTENT_SOURCES
    ApplicationLauncher.cpp
    ContextPrivate.cpp
    IntentPrivate.cpp
)

set(CONTENT_HEADERS
    ApplicationLauncher.h
    ContextPrivate.h
    IntentPrivate.h
)

if (WIN32)
    list(APPEND CONTENT_SOURCES
        win/ApplicationLauncherWin.cpp
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

add_library(private.android.content OBJECT ${CONTENT_HEADERS} ${CONTENT_SOURCES})
