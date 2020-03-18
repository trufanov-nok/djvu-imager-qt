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

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")
INCLUDE(cmake/UpdateTranslations.cmake)

# Prepare config.h
IF(WIN32)
        SET(TRANSLATIONS_DIR_REL "languages")
	SET(HELP_DIR_REL "help")
ELSE(WIN32)
        SET(TRANSLATIONS_DIR_REL "share/djvu_imager-qt/languages")
	SET(HELP_DIR_REL "share/djvu_imager-qt/help")
ENDIF(WIN32)
SET(TRANSLATIONS_DIR_ABS "${CMAKE_INSTALL_PREFIX}/${TRANSLATIONS_DIR_REL}")
SET(HELP_DIR_ABS "${CMAKE_INSTALL_PREFIX}/${HELP_DIR_REL}")

CONFIGURE_FILE(config.h.in ${CMAKE_BINARY_DIR}/config.h @ONLY)




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

# Translations
TRANSLATION_SOURCES(
        DjVu_Imager
	${SOURCES}
)
FILE(GLOB TRANSLATION_FILES languages/DjVu_Imager-Qt_*.ts)

FINALIZE_TRANSLATION_SET(DjVu_Imager ${TRANSLATION_FILES})
UPDATE_TRANSLATIONS_TARGET(update_translations DjVu_Imager)

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


add_executable(DjVu_Imager-Qt
    ${SOURCES}
    ${TS_FILES}
    ${RESOURCES}
  )

target_link_libraries(DjVu_Imager-Qt PRIVATE Qt5::Widgets)
