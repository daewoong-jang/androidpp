set(MEDIA_SOURCES
    AudioTrack.cpp
    MediaFormat.cpp
    MediaPlayer.cpp
)

set(MEDIA_HEADERS
    AudioFormat.h
    AudioTrack.h
    MediaFormat.h
    MediaPlayer.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(android.media OBJECT ${MEDIA_HEADERS} ${MEDIA_SOURCES})
