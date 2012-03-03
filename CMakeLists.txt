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
 
include_directories(D:/Dev/ExternalDependencies/OpenAL_1.1_SDK/include
D:/Dev/ExternalDependencies/freealut-1.1.0-bin/include
D:/Dev/ExternalDependencies/freealut-1.1.0-bin/include/al
D:/Dev/ExternalDependencies/Ogre_installed/include
D:/Dev/ExternalDependencies/Ogre_installed/include/OIS
D:/Dev/ExternalDependencies/Ogre_installed/include/OGRE
D:/Dev/ExternalDependencies/MyGUI_installed/include/MYGUI
D:/Dev/ExternalDependencies/Bullet_sources/src
)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_DEBUG_POSTFIX "_d")
add_library(${PROJECT} ${HEADERS} ${SOURCES})



INSTALL(TARGETS ${PROJECT}
ARCHIVE DESTINATION lib/Debug
CONFIGURATIONS Debug)

INSTALL(TARGETS ${PROJECT}
ARCHIVE DESTINATION lib/Release
CONFIGURATIONS Release)
