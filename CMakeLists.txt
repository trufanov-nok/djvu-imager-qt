cmake_minimum_required(VERSION 3.5)

project(djvu-imager-qt LANGUAGES CXX)

include_directories(src)

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

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")
INCLUDE(cmake/UpdateTranslations.cmake)

# Prepare config.h
IF(WIN32)
    SET(TRANSLATIONS_DIR_REL "languages")
    SET(HELP_DIR_REL "help")
ELSE(WIN32)
    SET(TRANSLATIONS_DIR_REL "share/djvu-imager-qt/languages")
    SET(HELP_DIR_REL "share/djvu-imager-qt/help")
ENDIF(WIN32)
SET(TRANSLATIONS_DIR_ABS "${CMAKE_INSTALL_PREFIX}/${TRANSLATIONS_DIR_REL}")
SET(HELP_DIR_ABS "${CMAKE_INSTALL_PREFIX}/${HELP_DIR_REL}")

CONFIGURE_FILE(config.h.in ${CMAKE_BINARY_DIR}/config.h @ONLY)

qt5_add_resources(RESOURCES "res/appicon.qrc")

set(SOURCES
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/qmytableview.cpp
    src/qmytableview.h
    src/optionsdialog.cpp
    src/optionsdialog.h
    src/optionsdialog.ui
    src/progressdialog.cpp
    src/progressdialog.h
    src/progressdialog.ui
    src/qconvertthread.cpp
    src/qconvertthread.h
    src/pageparam.h
    src/qviewthread.cpp
    src/qviewthread.h
    src/qinsertthread.cpp
    src/qinsertthread.h
    src/utils.cpp
    src/utils.h
    src/main.cpp
    )

# Translations
TRANSLATION_SOURCES(
    DjVu-Imager
    ${SOURCES}
)
FILE(GLOB TRANSLATION_FILES languages/djvu-imager-qt_*.ts)

FINALIZE_TRANSLATION_SET(DjVu-Imager ${TRANSLATION_FILES})
UPDATE_TRANSLATIONS_TARGET(update_translations DjVu-Imager)

SET(ts_files ${TRANSLATION_FILES})

# Don't build *.qm files from *untranslated.ts
SET(FILTERED_TRANSLATION_FILES)
FOREACH(ts_file ${ts_files})
    IF ("${ts_file}" MATCHES ".*untranslated.ts")
        # Just skip it.
    ELSE("${ts_file}" MATCHES ".*untranslated.ts")
    LIST(APPEND FILTERED_TRANSLATION_FILES "${ts_file}")
    ENDIF("${ts_file}" MATCHES ".*untranslated.ts")
ENDFOREACH(ts_file)

QT5_ADD_TRANSLATION(QM_FILES ${FILTERED_TRANSLATION_FILES})

IF(WIN32 OR APPLE)
    # Just copy already compiled qtbase*.qm from translations folder
    # if any of them are present there
    # to support localization of Qt own controls on Win machines
    FILE(GLOB QTBASE_QM_FILES languages/qtbase*.qm)
    LIST(APPEND QM_FILES ${QTBASE_QM_FILES})
ENDIF(WIN32 OR APPLE)

ADD_CUSTOM_TARGET(compile_translations ALL DEPENDS ${QM_FILES})

INSTALL(FILES ${QM_FILES} DESTINATION "${TRANSLATIONS_DIR_REL}/")

IF(WIN32)

SET(win32_resource_file "")
SET(rc_file "${CMAKE_SOURCE_DIR}/res/winicon.rc")
FILE(GLOB win32_resources res/*.ico)
SET_SOURCE_FILES_PROPERTIES(
    "${rc_file}" PROPERTIES
    OBJECT_DEPENDS ${win32_resources}
    )
SET(win32_resource_file "${rc_file}")


add_executable(djvu-imager-qt WIN32
    ${SOURCES}
    ${TS_FILES}
    ${RESOURCES}
    ${win32_resource_file}
  )
ELSE(WIN32)
add_executable(djvu-imager-qt
    ${SOURCES}
    ${TS_FILES}
    ${RESOURCES}
  )
ENDIF(WIN32)

target_link_libraries(djvu-imager-qt PRIVATE Qt5::Widgets)


IF(NOT WIN32 AND NOT APPLE)
    INSTALL(FILES res/djvu-imager-qt.png DESTINATION "/usr/share/pixmaps/")
    INSTALL(FILES debian/djvu-imager-qt.desktop DESTINATION "/usr/share/applications/")
ENDIF(NOT WIN32 AND NOT APPLE)

IF(APPLE)
    INSTALL(TARGETS djvu-imager-qt BUNDLE DESTINATION . RUNTIME DESTINATION bin)
ELSE(APPLE)
    INSTALL(TARGETS djvu-imager-qt RUNTIME DESTINATION bin)
ENDIF(APPLE)
