set(OS_SOURCES
    Binder.cpp
    BinderProvider.cpp
    BundlePrivate.cpp
    HandlerProvider.cpp
    MessageTarget.cpp
    ParcelPrivate.cpp
    ParcelablePrivate.cpp
)

set(OS_HEADERS
    Binder.h
    BinderProvider.h
    BundlePrivate.h
    HandlerProvider.h
    MessageTarget.h
    ParcelPrivate.h
    ParcelablePrivate.h
    PlatformFileDescriptor.h
)

if (WIN32)
    list(APPEND OS_SOURCES
        win/BinderWin.cpp
        win/LooperWin.cpp
        win/PlatformFileDescriptorWin.cpp
    )

    list(APPEND OS_HEADERS
        win/BinderWin.h
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

add_library(private.android.os OBJECT ${OS_HEADERS} ${OS_SOURCES})
