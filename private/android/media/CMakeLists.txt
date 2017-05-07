set(MEDIA_SOURCES
    AudioTrackPrivate.cpp
    MediaPlayerPrivate.cpp
)

set(MEDIA_HEADERS
    AudioTrackPrivate.h
    MediaPlayerPrivate.h
)

if (WIN32)
    list(APPEND MEDIA_SOURCES
        mock/MediaPlayerPrivateMock.cpp

        win/AudioTrackPrivateXAudio2Win.cpp
        win/DataSourceProviderByteStream.cpp
        win/XAudio2AudioDevice.cpp
    )

    list(APPEND MEDIA_HEADERS
        mock/MediaPlayerPrivateMock.h

        win/AudioTrackPrivateXAudio2Win.h
        win/DataSourceProviderByteStream.h
        win/XAudio2AudioDevice.h
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

add_library(private.android.media OBJECT ${MEDIA_HEADERS} ${MEDIA_SOURCES})
