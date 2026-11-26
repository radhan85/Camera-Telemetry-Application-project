cmake_minimum_required(VERSION 3.16)
project(CameraTelemetryApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Qml QuickControls2)
find_package(OpenCV REQUIRED COMPONENTS core imgproc videoio)

add_executable(CameraTelemetryApp
    src/main.cpp
    src/CameraController.cpp
    src/TelemetryGenerator.cpp
    src/VideoItem.cpp
    include/CameraController.hpp
    include/TelemetryGenerator.hpp
    include/VideoItem.hpp
    resources.qrc
)

target_include_directories(CameraTelemetryApp PRIVATE include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(CameraTelemetryApp PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::QuickControls2
    ${OpenCV_LIBS}
)

set_target_properties(CameraTelemetryApp PROPERTIES
    WIN32_EXECUTABLE ON
)
