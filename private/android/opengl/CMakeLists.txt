set(OPENGL_SOURCES
    GLExtensions.cpp
    GLTexture.cpp
    GLTextureHandle.cpp

    GLES2/esShader.cpp
    GLES2/esShapes.cpp
    GLES2/esTransform.cpp
    GLES2/esUtil.cpp
)

set(OPENGL_HEADERS
    GLExtensions.h
    GLTexture.h
    GLTextureHandle.h

    GLES2/esUtil.h
)

if (WIN32)
    list(APPEND OPENGL_SOURCES
        GLES2/Win32/esUtil_TGA.cpp
        GLES2/Win32/esUtil_win32.cpp
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

add_library(private.android.opengl OBJECT ${OPENGL_HEADERS} ${OPENGL_SOURCES})
