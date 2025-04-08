cmake_minimum_required(VERSION 3.10)

project(RTSPRecorder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenCV REQUIRED)


set(SOURCES
    src/main.cpp
    src/CameraRecorder.cpp
    src/RTSPRecorderApp.cpp
    src/utils.cpp
)



add_executable(RTSPRecorder ${SOURCES})

target_link_libraries(RTSPRecorder ${OpenCV_LIBS})
target_include_directories(RTSPRecorder PRIVATE ${OpenCV_INCLUDE_DIRS})
