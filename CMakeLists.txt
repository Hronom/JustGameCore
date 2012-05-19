cmake_minimum_required (VERSION 2.8)

set(PROJECT JustGameCore)

project(${PROJECT})
 
set(HEADERS
./GraphicSystem.h
./ILoadScreen.h
./InputSystem.h
./IState.h
./ISystemsListener.h
./MainSystem.h
./MyMotionState.h
./PhysicsDebugDrawer.h
./PhysicsSystem.h
./SoundListener.h
./SoundSource.h
./SoundSystem.h
./StatesSystem.h
./Utils.h
)
 
set(SOURCES
./GraphicSystem.cpp
./InputSystem.cpp
./MainSystem.cpp
./MyMotionState.cpp
./PhysicsDebugDrawer.cpp
./PhysicsSystem.cpp
./SoundListener.cpp
./SoundSource.cpp
./SoundSystem.cpp
./StatesSystem.cpp
./Utils.cpp
)

set(QT_ROOT $ENV{QT_ROOT})
set(QT_QMAKE_EXECUTABLE ${QT_ROOT}/bin/qmake)
set(QT_MOC_EXECUTABLE ${QT_ROOT}/bin/moc)
set(QT_RCC_EXECUTABLE ${QT_ROOT}/bin/rcc)
set(QT_UIC_EXECUTABLE ${QT_ROOT}/bin/uic)
set(QT_INCLUDE_DIR ${QT_ROOT}/include)
set(QT_LIBRARY_DIR ${QT_ROOT}/lib)
set(QT_QTCORE_LIBRARY ${QT_ROOT}/lib)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
qt4_wrap_cpp(HEADERS_MOC  ${HEADERS})

set(QT_DONT_USE_QTGUI true)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
 
include_directories(D:/Dev/JustGameProject/ExternalDependencies/OpenAL_1.1_SDK/include
D:/Dev/JustGameProject/ExternalDependencies/freealut-1.1.0-bin/include
D:/Dev/JustGameProject/ExternalDependencies/freealut-1.1.0-bin/include/al
D:/Dev/JustGameProject/ExternalDependencies/Ogre_install/include
D:/Dev/JustGameProject/ExternalDependencies/Ogre_install/include/OIS
D:/Dev/JustGameProject/ExternalDependencies/Ogre_install/include/OGRE
D:/Dev/JustGameProject/ExternalDependencies/MyGUI_install/include/MYGUI
D:/Dev/JustGameProject/ExternalDependencies/Bullet/src
)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_DEBUG_POSTFIX "_d")
add_library(${PROJECT} ${SOURCES} ${HEADERS_MOC})



INSTALL(TARGETS ${PROJECT}
ARCHIVE DESTINATION lib/Debug
CONFIGURATIONS Debug)

INSTALL(TARGETS ${PROJECT}
ARCHIVE DESTINATION lib/Release
CONFIGURATIONS Release)
