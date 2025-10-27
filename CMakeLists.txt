cmake_minimum_required(VERSION 3.16)

project(chat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Protobuf_USE_STATIC_LIBS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Protobuf 6.32.1 QUIET)
if(NOT Protobuf_FOUND)
    message(STATUS "Protobuf library not found. Starting download and build of Protobuf 6.32.1...")
    include(FetchContent)
    FetchContent_Declare(
        protobuf
        URL "https://github.com/protocolbuffers/protobuf/releases/download/v32.1/protobuf-32.1.tar.gz"
        URL_HASH SHA256=3feeabd077a112b56af52519bc4ece90e28b4583f4fc2549c95d765985e0fd3c
    )
    FetchContent_MakeAvailable(protobuf)
    message(STATUS "Protobuf library downloaded and built successfully.")
endif()

message(STATUS "Protobuf library version: ${Protobuf_VERSION}")

file(GLOB GUI_SOURCES
        "src/gui/*.cpp"
        "src/gui/*.ui"
        "src/gui/*.h"
)

file(GLOB CORE_SOURCES
        "src/core/*.cpp"
        "src/core/*.h"
)

file(GLOB NETWORK_SOURCES
        "src/network/*.cpp"
        "src/network/*.h"
)

set(PROJECT_SOURCES
        ${GUI_SOURCES}
        ${CORE_SOURCES}
        ${NETWORK_SOURCES}
)

set(CLIENT_SOURCE
        ${PROJECT_SOURCES}
        "src/client.cpp"
)

set(SERVER_SOURCE
        ${PROJECT_SOURCES}
        "src/server.cpp"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(chat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET chat APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(chat SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(chat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(chat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.chat)
endif()
set_target_properties(chat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS chat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chat)
endif()
