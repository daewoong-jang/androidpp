set(ANDROIDPP_SOURCES
    Assertions.cpp
    LogHelper.cpp
)

set(ANDROIDPP_HEADERS
    Assertions.h
    CompilerMacros.h
    ExportMacros.h
    Functional.h
    IdentifierErasure.h
    KeywordMacros.h
    LogHelper.h
    Noncopyable.h
    PointerTypes.h
    StringConversion.h
    TemporaryChange.h
)

if (WIN32)
    list(APPEND ANDROIDPP_HEADERS
        win/COMPtr.h
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

add_library(android++.c++ OBJECT ${ANDROIDPP_HEADERS} ${ANDROIDPP_SOURCES})
