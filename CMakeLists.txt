cmake_minimum_required(VERSION 3.5)

project(TrafficSimulation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp

    src/include/MainWindow.hpp
    src/include/common/Point.hpp
    src/include/controller/SimulationController.hpp
    src/include/interface/LinePainter.hpp
    src/include/interface/PointPainter.hpp
    src/include/model/Driver.hpp
    src/include/model/Junction.hpp
    src/include/model/MovingObject.hpp
    src/include/model/Path.hpp
    src/include/model/Pedestrian.hpp
    src/include/model/Road.hpp
    src/include/model/Simulation.hpp
    src/include/model/Vehicle.hpp
    src/include/view/PointPainters.hpp
    src/include/view/PathPainters.hpp
    src/include/view/dialogs/CreateJunctionDialog.hpp
    src/include/view/dialogs/CreatePavementDialog.hpp
    src/include/view/dialogs/CreateRoadDialog.hpp


    src/MainWindow.cpp
    src/mainwindow.ui
    src/controller/SimulationController.cpp
    src/model/Driver.cpp
    src/model/Junction.cpp
    src/model/MovingObject.cpp
    src/model/Path.cpp
    src/model/Pedestrian.cpp
    src/model/Road.cpp
    src/model/Simulation.cpp
    src/model/Vehicle.cpp
    src/view/PathPainters.cpp
    src/view/PointPainters.cpp
    src/view/dialogs/CreateJunctionDialog.cpp
    src/view/dialogs/CreatePavementDialog.cpp
    src/view/dialogs/CreateRoadDialog.cpp
    src/view/dialogs/createjunctiondialog.ui
    src/view/dialogs/createpavementdialog.ui
    src/view/dialogs/createroaddialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TrafficSimulation
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TrafficSimulation APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TrafficSimulation SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TrafficSimulation
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TrafficSimulation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(TrafficSimulation PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS TrafficSimulation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TrafficSimulation)
endif()
