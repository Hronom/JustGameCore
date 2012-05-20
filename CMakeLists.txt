cmake_minimum_required (VERSION 2.8)

set(PROJECT JustGameCore)
project(${PROJECT})

######################################################################
# Setting source files for this project
######################################################################
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

######################################################################
# Setting the include directory of dependencies
######################################################################
set(OPENAL_SDK_DIR CACHE PATH FORCE)
if(OPENAL_SDK_DIR STREQUAL "")
message(SEND_ERROR "Please set directory where OpenAl sdk")
endif(OPENAL_SDK_DIR STREQUAL "")

set(FREEALUT_DIR CACHE PATH FORCE)
if(FREEALUT_DIR STREQUAL "")
message(SEND_ERROR "Please set directory where Freealut bin")
endif(FREEALUT_DIR STREQUAL "")

set(OGRE_INSTALL_DIR CACHE PATH FORCE)
if(OGRE_INSTALL_DIR STREQUAL "")
message(SEND_ERROR "Please set directory where Ogre install")
endif(OGRE_INSTALL_DIR STREQUAL "")

set(MYGUI_INSTALL_DIR CACHE PATH FORCE)
if(MYGUI_INSTALL_DIR STREQUAL "")
message(SEND_ERROR "Please set directory where MyGUI install")
endif(MYGUI_INSTALL_DIR STREQUAL "")

set(BULLET_INSTALL_DIR CACHE PATH FORCE)
if(BULLET_INSTALL_DIR STREQUAL "")
message(SEND_ERROR "Please set directory where Bullet install")
endif(BULLET_INSTALL_DIR STREQUAL "")

include_directories(
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
ADD_DEFINITIONS(${QT_DEFINITIONS})

qt4_wrap_cpp(HEADERS_MOC ${HEADERS})

set(QT_DONT_USE_QTGUI true)
 
######################################################################
# Setting build options
######################################################################
if(CMAKE_BUILD_TYPE STREQUAL "")
set(CMAKE_BUILD_TYPE "Debug")
message(STATUS "Setting CMAKE_BUILD_TYPE to Debug")
endif(CMAKE_BUILD_TYPE STREQUAL "")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_DEBUG_POSTFIX "_d")
add_library(${PROJECT} ${SOURCES} ${HEADERS_MOC})

######################################################################
# Setting install rules
######################################################################
install(
FILES ${HEADERS}
DESTINATION include
)

install(
TARGETS ${PROJECT}
ARCHIVE DESTINATION lib/Debug
CONFIGURATIONS Debug
)

install(
TARGETS ${PROJECT}
ARCHIVE DESTINATION lib/Release
CONFIGURATIONS Release
)
