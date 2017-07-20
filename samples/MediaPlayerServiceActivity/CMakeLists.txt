set(MEDIA_PLAYER_SERVICE_ACTIVITY_SOURCES
    MediaPlayerHost.cpp
    MediaPlayerService.cpp
    MediaPlayerServiceActivity.cpp
    MediaRenderer.cpp
    MediaSurface.cpp
    MediaView.cpp
)

set(MEDIA_PLAYER_SERVICE_ACTIVITY_HEADERS
    MediaPlayerHost.h
    MediaPlayerService.h
    MediaPlayerServiceActivity.h
    MediaRenderer.h
    MediaSurface.h
    MediaView.h
)

set(MEDIA_PLAYER_SERVICE_ACTIVITY_LIB_DEPS
    android++
)

if (WIN32)
    list(APPEND MEDIA_PLAYER_SERVICE_ACTIVITY_SOURCES
        DLLMain.cpp
    )
endif ()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${LIBRARY_PRODUCT_DIR}/include/android++"
    "${LIBRARY_PRODUCT_DIR}/include/android++/android"
)

add_library(MediaPlayerServiceActivity SHARED ${MEDIA_PLAYER_SERVICE_ACTIVITY_SOURCES} ${MEDIA_PLAYER_SERVICE_ACTIVITY_HEADERS})
target_link_libraries(MediaPlayerServiceActivity ${MEDIA_PLAYER_SERVICE_ACTIVITY_LIB_DEPS})
target_compile_definitions(MediaPlayerServiceActivity PRIVATE "-DBUILDING_ANDROID_PACKAGE")

set(MEDIA_PLAYER_SERVICE_ACTIVITY_LAUNCHER_SOURCES
    Main.cpp
)

add_executable(MediaPlayerServiceActivityLauncher ${MEDIA_PLAYER_SERVICE_ACTIVITY_LAUNCHER_SOURCES})
target_link_libraries(MediaPlayerServiceActivityLauncher MediaPlayerServiceActivity)
