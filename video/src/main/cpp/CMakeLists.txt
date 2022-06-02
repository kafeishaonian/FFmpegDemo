# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
# Declares and names the project.

project("VideoPlayer")

# 配置编译选项


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


include_directories(
        include
        glm
        ${CMAKE_SOURCE_DIR}/player
        ${CMAKE_SOURCE_DIR}/player/decoder
        ${CMAKE_SOURCE_DIR}/util
        ${CMAKE_SOURCE_DIR}/player/render
        ${CMAKE_SOURCE_DIR}/player/render/audio
        ${CMAKE_SOURCE_DIR}/player/render/video
)


#set(PLAYER_FILE
#        MediaVideoPlayer_Jni.cpp
#        /player/decoder/DecoderBase.h)

file(GLOB PLAYER_FILE
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/util/*.cpp
        ${CMAKE_SOURCE_DIR}/player/*.cpp
        ${CMAKE_SOURCE_DIR}/player/decoder/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/audio/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/video/*.cpp)


add_library( # Sets the name of the library.
        VideoPlayer

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${PLAYER_FILE})

add_library(libffmpeg SHARED IMPORTED)

set_target_properties(libffmpeg
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/src/jniLibs/${ANDROID_ABI}/libffmpeg.so)

include_directories(${PROJECT_SOURCE_DIR}/src/main/cpp/include)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

find_library(android android)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        VideoPlayer

        # Links the target library to the log library
        # included in the NDK.
        ${libffmpeg}
        ${log-lib}
        ${android})