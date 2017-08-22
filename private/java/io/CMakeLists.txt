set(IO_SOURCES
    ByteReader.cpp
    ByteWriter.cpp
)

set(IO_HEADERS
    ByteBufferProvider.h
    ByteReader.h
    ByteWriter.h
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/android"
    "${CMAKE_SOURCE_DIR}/private"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}"
)

add_library(private.java.io OBJECT ${IO_HEADERS} ${IO_SOURCES})
