cmake_minimum_required (VERSION 2.8)

SET(PROJECT JustGameCore)
PROJECT(${PROJECT})

######################################################################
# Setting source files for this project
######################################################################
SET(HEADERS
./src/GraphicSystem.h
./src/ILoadScreen.h
./src/InputSystem.h
./src/IState.h
./src/ISystemsListener.h
./src/MainSystem.h
./src/MyMotionState.h
./src/PhysicsDebugDrawer.h
./src/PhysicsSystem.h
./src/SoundListener.h
./src/SoundSource.h
./src/SoundSystem.h
./src/StatesSystem.h
./src/Utils.h
./src/EntitySystem/ISystem.h
./src/EntitySystem/IComponent.h
./src/EntitySystem/EntitySystem.h
)
 
SET(SOURCES
./src/GraphicSystem.cpp
./src/InputSystem.cpp
./src/MainSystem.cpp
./src/MyMotionState.cpp
./src/PhysicsDebugDrawer.cpp
./src/PhysicsSystem.cpp
./src/SoundListener.cpp
./src/SoundSource.cpp
./src/SoundSystem.cpp
./src/StatesSystem.cpp
./src/Utils.cpp
./src/EntitySystem/EntitySystem.cpp
)

######################################################################
# Setting the include directory of dependencies
######################################################################
SET(OPENAL_SDK_DIR CACHE PATH FORCE)
IF(OPENAL_SDK_DIR STREQUAL "")
MESSAGE(SEND_ERROR "Please set directory where OpenAl sdk")
ENDIF(OPENAL_SDK_DIR STREQUAL "")

SET(FREEALUT_DIR CACHE PATH FORCE)
IF(FREEALUT_DIR STREQUAL "")
MESSAGE(SEND_ERROR "Please set directory where Freealut bin")
ENDIF(FREEALUT_DIR STREQUAL "")

SET(OGRE_INSTALL_DIR CACHE PATH FORCE)
IF(OGRE_INSTALL_DIR STREQUAL "")
MESSAGE(SEND_ERROR "Please set directory where Ogre install")
ENDIF(OGRE_INSTALL_DIR STREQUAL "")

SET(MYGUI_INSTALL_DIR CACHE PATH FORCE)
IF(MYGUI_INSTALL_DIR STREQUAL "")
MESSAGE(SEND_ERROR "Please set directory where MyGUI install")
ENDIF(MYGUI_INSTALL_DIR STREQUAL "")

SET(BULLET_INSTALL_DIR CACHE PATH FORCE)
IF(BULLET_INSTALL_DIR STREQUAL "")
MESSAGE(SEND_ERROR "Please set directory where Bullet install")
ENDIF(BULLET_INSTALL_DIR STREQUAL "")

INCLUDE_DIRECTORIES(
${OPENAL_SDK_DIR}/include
${FREEALUT_DIR}/include
${FREEALUT_DIR}/include/al
${OGRE_INSTALL_DIR}/include
${OGRE_INSTALL_DIR}/include/OIS
${OGRE_INSTALL_DIR}/include/OGRE
${MYGUI_INSTALL_DIR}/include/MYGUI
${BULLET_INSTALL_DIR}/include/bullet
)

######################################################################
# Setting Qt packages
######################################################################
SET(QT_ROOT $ENV{QT_ROOT})
SET(QT_QMAKE_EXECUTABLE ${QT_ROOT}/bin/qmake)
SET(QT_MOC_EXECUTABLE ${QT_ROOT}/bin/moc)
SET(QT_RCC_EXECUTABLE ${QT_ROOT}/bin/rcc)
SET(QT_UIC_EXECUTABLE ${QT_ROOT}/bin/uic)
SET(QT_INCLUDE_DIR ${QT_ROOT}/include)
SET(QT_LIBRARY_DIR ${QT_ROOT}/lib)
SET(QT_QTCORE_LIBRARY ${QT_ROOT}/lib)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

QT4_WRAP_CPP(HEADERS_MOC ${HEADERS})

SET(QT_DONT_USE_QTGUI true)
 
######################################################################
# Setting build options
######################################################################
IF(CMAKE_BUILD_TYPE STREQUAL "")
SET(CMAKE_BUILD_TYPE "Debug")
MESSAGE(STATUS "Setting CMAKE_BUILD_TYPE to Debug")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
SET(CMAKE_DEBUG_POSTFIX "_d")
ADD_LIBRARY(${PROJECT} ${HEADERS} ${SOURCES} ${HEADERS_MOC})

######################################################################
# Setting install rules
######################################################################
INSTALL(
FILES ${HEADERS}
DESTINATION include
)

INSTALL(
TARGETS ${PROJECT}
ARCHIVE DESTINATION lib/Debug
CONFIGURATIONS Debug
)

INSTALL(
TARGETS ${PROJECT}
ARCHIVE DESTINATION lib/Release
CONFIGURATIONS Release
)
