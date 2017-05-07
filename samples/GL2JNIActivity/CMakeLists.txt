set(GL2JNI_ACTIVITY_SOURCES
    GL2JNIActivity.cpp
    GL2JNILib.cpp
    GL2JNIView.cpp
)

set(GL2JNI_ACTIVITY_HEADERS
    GL2JNIActivity.h
    GL2JNILib.h
    GL2JNIView.h
)

set(GL2JNI_ACTIVITY_LIB_DEPS
    android++
)

if (WIN32)
    list(APPEND GL2JNI_ACTIVITY_SOURCES
        DLLMain.cpp
    )
endif ()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${LIBRARY_PRODUCT_DIR}/include/android++"
    "${LIBRARY_PRODUCT_DIR}/include/android++/android"
)

add_library(GL2JNIActivity SHARED ${GL2JNI_ACTIVITY_SOURCES} ${GL2JNI_ACTIVITY_HEADERS})
target_link_libraries(GL2JNIActivity ${GL2JNI_ACTIVITY_LIB_DEPS})
target_compile_definitions(GL2JNIActivity PRIVATE "-DBUILDING_ANDROID_PACKAGE")

set(GL2JNI_ACTIVITY_LAUNCHER_SOURCES
    Main.cpp
)

add_executable(GL2JNIActivityLauncher ${GL2JNI_ACTIVITY_LAUNCHER_SOURCES})
target_link_libraries(GL2JNIActivityLauncher GL2JNIActivity)
