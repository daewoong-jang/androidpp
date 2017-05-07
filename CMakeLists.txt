cmake_minimum_required(VERSION 3.4)

project(android++)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
define_property(TARGET PROPERTY FOLDER INHERITED BRIEF_DOCS "folder" FULL_DOCS "IDE folder name")

set(LIBRARY_PRODUCT_DIR "${CMAKE_BINARY_DIR}" CACHE PATH "Path to the directory where products will be copied to.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_PRODUCT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_PRODUCT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_PRODUCT_DIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(HelperMacros)
include(Options${CMAKE_SYSTEM_NAME})

include_directories(
    "${LIBRARY_PRODUCT_DIR}/include"
    "${LIBRARY_PRODUCT_DIR}/include/ndk"
)

add_definitions(-DBUILDING_ANDROID)

if (WIN32)
    add_definitions(-DWIN32_POSIX -D_WINSOCKAPI_ -D__STDC_FORMAT_MACROS -DHAVE_SYS_TYPES_H -DHAVE_SIGNAL_H -DHAVE_STRUCT_TIMESPEC)
    add_definitions(-DGL_GLEXT_PROTOTYPES -DCAIRO_WIN32_STATIC_BUILD -DCURL_STATICLIB -DLIBXML_STATIC -DLIBXSLT_STATIC -DXMD_H)

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB")
    set(WIN32_SYSTEM_LIBRARIES comsupp concrt libcmt libcpmt msvcrt OLDNAMES Version vcruntime ucrt atls Shlwapi winmm Crypt32)

    include_directories("${CMAKE_SOURCE_DIR}/android++/win")

    link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    if (${MSVC_CXX_ARCHITECTURE_ID} STREQUAL "X86")
        link_directories("${LIBRARY_PRODUCT_DIR}/lib32")
    else ()
        link_directories("${LIBRARY_PRODUCT_DIR}/lib64")
    endif ()

    macro(ADD_EXECUTABLE _name)
        _add_executable(${ARGV})
        if (TARGET ${_name})
            target_link_libraries(${_name} android++ ${WIN32_SYSTEM_LIBRARIES})
        endif()
    endmacro()
endif ()

add_subdirectory(android)
add_subdirectory(android++)
add_subdirectory(private)
add_subdirectory(samples)
