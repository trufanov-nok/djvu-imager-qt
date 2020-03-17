cmake_minimum_required(VERSION 3.5)

project(DjVu_Imager-Qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES DjVu_Imager-Qt_ru_RU.ts)

qt5_add_resources(RESOURCES "res/appicon.qrc")

set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    qmytablewidget.cpp
    optionsdialog.cpp
    optionsdialog.h
    optionsdialog.ui
    progressdialog.cpp
    progressdialog.h
    progressdialog.ui
    qconvertthread.cpp
    qconvertthread.h
    pageparam.h
    qviewthread.cpp
    qviewthread.h
    qinsertthread.cpp
    qinsertthread.h
    utils.cpp
    utils.h
    )

if(ANDROID)
  add_library(DjVu_Imager-Qt SHARED
    ${SOURCES}
    ${TS_FILES}
    ${RESOURCES}
  )
else()
  add_executable(DjVu_Imager-Qt
    ${SOURCES}
    ${TS_FILES}
    ${RESOURCES}
  )
endif()


target_link_libraries(DjVu_Imager-Qt PRIVATE Qt5::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})