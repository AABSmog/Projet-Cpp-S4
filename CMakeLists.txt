cmake_minimum_required(VERSION 3.16)

project(BankVision VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES

        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BankVision
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        models/comptebancaire.h models/comptebancaire.cpp

        models/client.h models/client.cpp
        models/transaction.h models/transaction.cpp
        models/pret.h models/pret.cpp
        models/banque.h models/banque.cpp
        views/fenetreconnexion.h views/fenetreconnexion.cpp
        views/fenetretableaubord.h views/fenetretableaubord.cpp
        views/fenetrestatistiques.h views/fenetrestatistiques.cpp
        views/widgets/camembertdepenses.h views/widgets/camembertdepenses.cpp
        views/widgets/indicateursolde.h views/widgets/indicateursolde.cpp
        controllers/authcontroller.h controllers/authcontroller.cpp
        controllers/comptecontroller.h controllers/comptecontroller.cpp
        controllers/statcontroller.h controllers/statcontroller.cpp
        data/datamanager.h data/datamanager.cpp
        resources/resources.qrc
        main.h main.cpp
        data/compteData.h
        data/compteData.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET BankVision APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BankVision SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(BankVision
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(BankVision PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.BankVision)
endif()
set_target_properties(BankVision PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS BankVision
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BankVision)
endif()
