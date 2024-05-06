cmake_minimum_required(VERSION 3.20)

project(Servox
    VERSION 1.0.0
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort)

qt_add_executable(app main.cpp controlwidget.cpp motor.cpp)

target_link_libraries(app PRIVATE Qt::Widgets Qt::SerialPort)
