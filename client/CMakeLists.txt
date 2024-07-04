cmake_minimum_required(VERSION 3.16)

# reference: https://doc.qt.io/qt-6/cmake-get-started.html

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

qt_add_executable(minioicq
    src/main.cpp
    src/button.cpp
    src/button.h
    src/calculator.cpp
    src/calculator.h
)

# target_include_directories(minioicq PRIVATE include)

target_link_libraries(minioicq PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(minioicq PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
