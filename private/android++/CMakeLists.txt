if (NOT ANDROID)
    set(ANDROID_SOURCES
        ${CMAKE_SOURCE_DIR}/android/android.cpp
    )

    set(ANDROID_HEADERS
        ${CMAKE_SOURCE_DIR}/android/android.h
    )

    set(ANDROID_LIB_DEPS
        cairo
        jsoncpp
    )

    if (WIN32)
        list(APPEND ANDROID_SOURCES
            win/DLLMain.cpp
        )

        list(APPEND ANDROID_LIB_DEPS
            ndk
            log
            mfuuid
            ${WIN32_SYSTEM_LIBRARIES}
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

    add_library(android++ SHARED
        ${ANDROID_HEADERS}
        ${ANDROID_SOURCES}
        $<TARGET_OBJECTS:android.app>
        $<TARGET_OBJECTS:android.content>
        $<TARGET_OBJECTS:android.content.res>
        $<TARGET_OBJECTS:android.graphics>
        $<TARGET_OBJECTS:android.media>
        $<TARGET_OBJECTS:android.opengl>
        $<TARGET_OBJECTS:android.os>
        $<TARGET_OBJECTS:android.text>
        $<TARGET_OBJECTS:android.util>
        $<TARGET_OBJECTS:android.view>
        $<TARGET_OBJECTS:android.view.inputmethod>
        $<TARGET_OBJECTS:android.widget>
        $<TARGET_OBJECTS:java.lang>
        $<TARGET_OBJECTS:private.android.app>
        $<TARGET_OBJECTS:private.android.content>
        $<TARGET_OBJECTS:private.android.media>
        $<TARGET_OBJECTS:private.android.opengl>
        $<TARGET_OBJECTS:private.android.os>
        $<TARGET_OBJECTS:private.android.util>
        $<TARGET_OBJECTS:private.android.view>
        $<TARGET_OBJECTS:private.android.view.inputmethod>
        $<TARGET_OBJECTS:private.android.widget>
        $<TARGET_OBJECTS:android++.c++>
    )

    target_link_libraries(android++ ${ANDROID_LIB_DEPS})

    add_post_build_command(android++)
    copy_library_headers_directory(android++ "${CMAKE_SOURCE_DIR}/android" include/android++/android)
    copy_library_headers_directory(android++ "${CMAKE_SOURCE_DIR}/android++" include/android++/android++)

    if (WIN32)
        list(APPEND ANDROID_STUB_SOURCES
            win/MainWin.cpp
        )

        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    endif ()

    add_executable(android++.stub ${ANDROID_STUB_SOURCES})
    target_link_libraries(android++.stub android++ ${ANDROID_LIB_DEPS})
endif ()
