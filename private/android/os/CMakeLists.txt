set(OS_SOURCES
    Binder.cpp
    BinderProvider.cpp
    BundlePrivate.cpp
    HandlerProvider.cpp
    MemoryFilePrivate.cpp
    MessageTarget.cpp
    ParcelPrivate.cpp
    ParcelablePrivate.cpp
    PlatformEvent.cpp
    PlatformHandle.cpp
    PlatformMutex.cpp
    ServiceObject.cpp
)

set(OS_HEADERS
    Binder.h
    BinderProvider.h
    BundlePrivate.h
    HandlerProvider.h
    MemoryFilePrivate.h
    MessageTarget.h
    ParcelPrivate.h
    ParcelablePrivate.h
    PlatformEvent.h
    PlatformEventPrivate.h
    PlatformFileDescriptor.h
    PlatformHandle.h
    PlatformMutex.h
    ServiceChannel.h
    ServiceMessageClient.h
    ServiceMessageHost.h
    ServiceObject.h
    ServiceObjectRef.h
)

if (WIN32)
    list(APPEND OS_SOURCES
        win/BinderProviderWin.cpp
        win/LooperWin.cpp
        win/MemoryFilePrivateWin.cpp
        win/PlatformEventWin.cpp
        win/PlatformMutexWin.cpp
        win/PlatformFileDescriptorWin.cpp
        win/PlatformHandleWin.cpp
    )

    list(APPEND OS_HEADERS
        win/BinderProviderWin.h
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
