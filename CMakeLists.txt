cmake_minimum_required(VERSION 3.10)

# Название проекта
project(RTSPRecorder)

# Указываем минимальную версию C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Находим пакет OpenCV
find_package(OpenCV REQUIRED)

# Указываем исходные файлы проекта
set(SOURCES
    main.cpp
)

# Добавляем исполняемый файл
add_executable(RTSPRecorder ${SOURCES})

# Связываем OpenCV с проектом
target_link_libraries(RTSPRecorder ${OpenCV_LIBS})

# Указываем включаемые директории OpenCV
target_include_directories(RTSPRecorder PRIVATE ${OpenCV_INCLUDE_DIRS})
