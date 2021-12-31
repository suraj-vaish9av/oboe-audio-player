# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("oboeaudioplayer")

include_directories(utils/)
include_directories(audio/)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp

            #utilities
            utils/logging.h
            utils/UtilityFunctions.h

        audio/AudioProperties.h
        audio/AAssetDataSource.h
        audio/NDKExtractor.h
        audio/NDKExtractor.cpp
        audio/AAssetDataSource.cpp

        audio/Player.h
        audio/Player.cpp
        audio/PlayerController.h
        audio/PlayerController.cpp
        )

set (TARGET_LIBS log android)
target_link_libraries( native-lib ${TARGET_LIBS} )

if(${USE_FFMPEG})

    MESSAGE(STATUS "Using FFmpeg extractor")

    add_definitions(-DUSE_FFMPEG=1)
    target_sources( native-lib PRIVATE audio/FFMpegExtractor.cpp )

    # Add the local path to FFmpeg, you can use the ${ANDROID_ABI} variable to specify the ABI name
    # e.g. /Users/donturner/Code/ffmpeg/build/${ANDROID_ABI}
    set(FFMPEG_DIR "D:/ffmpeg")

    #include_directories(native-lib ${FFMPEG_DIR}/include)
    include_directories(native-lib ${FFMPEG_DIR})

    add_library( avformat SHARED IMPORTED)
    set_target_properties(avformat PROPERTIES IMPORTED_LOCATION
            ${FFMPEG_DIR}/libavformat)
    add_library( avutil SHARED IMPORTED)
    set_target_properties(avutil PROPERTIES IMPORTED_LOCATION
            ${FFMPEG_DIR}/libavutil)
    add_library( avcodec SHARED IMPORTED)
    set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION
            ${FFMPEG_DIR}/libavcodec)
    add_library( swresample SHARED IMPORTED)
    set_target_properties(swresample PROPERTIES IMPORTED_LOCATION
            ${FFMPEG_DIR}/libswresample)
    set (TARGET_LIBS ${TARGET_LIBS} avformat avutil avcodec swresample)
    add_library(native-lib
            SHARED
            audio/FFMpegExtractor.h
            audio/FFMpegExtractor.cpp)
else()
    MESSAGE(STATUS "Using NDK media extractor")
    add_definitions(-DUSE_FFMPEG=0)
    target_sources( native-lib PRIVATE audio/NDKExtractor.cpp )
    set (TARGET_LIBS ${TARGET_LIBS} mediandk)
endif()

target_link_libraries( native-lib ${TARGET_LIBS} )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# Find the Oboe package
find_package (oboe REQUIRED CONFIG)

find_library( # Sets the name of the path variable.
        android-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        android )
#find_library(android-lib android)

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                        oboe::oboe
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                        ${android-lib}
        )