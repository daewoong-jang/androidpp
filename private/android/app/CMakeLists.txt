set(APP_SOURCES
    ActivityHostWindow.cpp
    ActivityPrivate.cpp
    AlertDialogPrivate.cpp
    ApplicationLoader.cpp
    ApplicationProcess.cpp
    ServiceHostWindow.cpp
    ServicePrivate.cpp
    WindowProvider.cpp
)

set(APP_HEADERS
    ActivityHostWindow.h
    ActivityPrivate.h
    AlertDialogPrivate.h
    ApplicationLoader.h
    ApplicationProcess.h
    HostWindow.h
    MessageClientBase.h
    ServiceHostWindow.h
    ServicePrivate.h
    WindowHandle.h
    WindowProvider.h
)

if (WIN32)
    list(APPEND APP_SOURCES
        win/AlertDialogPrivateWin.cpp
        win/ApplicationProcessWin.cpp
        win/WindowProviderWin.cpp
    )

    list(APPEND APP_HEADERS
        win/WindowProviderWin.h
    )
endif ()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
    "${LIBRARY_PRODUCT_DIR}/include/wtl"
)

add_library(private.android.app OBJECT ${APP_HEADERS} ${APP_SOURCES})
