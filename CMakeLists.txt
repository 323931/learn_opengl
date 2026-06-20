cmake_minimum_required(VERSION 3.16)
project(opengl_triangle LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL)
find_package(glm REQUIRED)

add_executable(opengl_triangle
    main.cpp
    glwindow/glwindow.h
    glwindow/glwindow.cpp
    scenewindow/scenewindow.h
    scenewindow/scenewindow.cpp
    shape/src/shapeGenerator.cpp
    shaders.qrc
    camera/src/camera.cpp
)

target_include_directories(opengl_triangle PRIVATE
    glwindow
    scenewindow
    shape/include
    camera/include
    tools/
)


target_link_libraries(opengl_triangle PRIVATE Qt5::Widgets Qt5::OpenGL glm::glm)
