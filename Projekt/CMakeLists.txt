cmake_minimum_required(VERSION 2.8.11)

# setting variable with project name
set(PROJECT_NAME Hockey)
set(DOXY_NAME Doxyfile.in)
# setting project name
project(${PROJECT_NAME})

#OPTION (BUILD_DOCUMENTATION "Build the doxygen-based documentation" ON)
#IF (BUILD_DOCUMENTATION)

    # Make sure Doxygen is on the system, if not then the documentation can't be built
    FIND_PACKAGE (Doxygen)
    IF (DOXYGEN_FOUND)

        # This is the doxyfile that will be used to generate the documentation
        # You can use programs like doxywizard to edit the settings
        SET (doxygenConfigFileIn "${DOXY_NAME}")
        #SET (doxygenConfigFile "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.dox")
        #${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        #SET (DOXYGEN_LANGUAGE "English" CACHE STRING "Documentation language")
        #MARK_AS_ADVANCED (DOXYGEN_LANGUAGE)

        # Using a .in file means we can use CMake to insert project settings
        # into the doxyfile.  For example, CMake will replace @PROJECT_NAME@ in
        # a configured file with the CMake PROJECT_NAME variable's value.
        #CONFIGURE_FILE (${doxygenConfigFileIn} ${doxygenConfigFile} @ONLY)

        # Add the documentiation target.  This lets you run "make doc" from the
        # generated CMake makefiles
        ADD_CUSTOM_TARGET (doc
            ${DOXYGEN_EXECUTABLE} ${doxygenConfigFileIn}
            DEPENDS ${doxygenConfigFileIn}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM
        COMMENT "Generating API documentation with Doxygen" )
        # You can add an "install" directive to install the resulting documentation
        # if desired.
    ELSE (DOXYGEN_FOUND)
        MESSAGE (STATUS "Documentation will not be built - Doxygen not found")
    ENDIF (DOXYGEN_FOUND)
#ELSE (BUILD_DOCUMENTATION)
#    MESSAGE ("OPTION DOES NOT WORK")
#ENDIF (BUILD_DOCUMENTATION)

# Add an uninstall target (for "make uninstall")
# SEE: http://www.cmake.org/Wiki/CMake_FAQ#Can_I_do_.22make_uninstall.22_with_CMake.3F

# cmake -DCMAKE_BUILD_TYPE=DEBUG
# cmake -DCMAKE_BUILD_TYPE=RELEASE

# Build flags for different configurations
# Use is determined by the option CMAKE_BUILD_TYPE
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -Wall ")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -msse3 ")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -ftest-coverage -fprofile-arcs")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -msse3 ")

# C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")


# Find the QtWidgets library
FIND_PACKAGE( Qt5Network REQUIRED)
FIND_PACKAGE( Qt5Xml REQUIRED)
FIND_PACKAGE( Qt5Widgets REQUIRED)
FIND_PACKAGE( Qt5PrintSupport REQUIRED )
FIND_PACKAGE( Qt5Concurrent REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Add Definitions
# Ignore Boost Signal deprecation warning triggered from some Player libs
ADD_DEFINITIONS( -DBOOST_SIGNALS_NO_DEPRECATION_WARNING )
# avoid problems with boost signals
ADD_DEFINITIONS( -DQT_NO_KEYWORDS )

# If Player isn't installed to /usr, you'll have to edit this path to point
# to the location of UserPlayerC++.cmake and UsePlayerPlugin.cmake

SET( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib64/pkgconfig:/usr/local/lib/pkgconfig" )
SET( CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules" ${CMAKE_MODULE_PATH})

# These files include macros that make building against Player easier.  They provide
# the PLAYER_ADD* macros
INCLUDE (UsePlayerC++)
INCLUDE (UsePlayerPlugin)

ADD_SUBDIRECTORY(3rdparty/hermes)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/3rdparty/hermes)
INCLUDE_DIRECTORIES(${PLAYERCPP_INCLUDE_DIRS})
LINK_DIRECTORIES(${PLAYERCPP_LIBRARY_DIRS})

# OpenCV for camera operation
find_package(OpenCV)

# Define Files...
SET(THE_SOURCES
    Main/main.cpp
    Main/player.cpp
    Main/robotThread.cpp
    Actor/actorLowLevel.cpp
    Actor/actorhighlevel.cpp
    Actor/spline.cpp
    AI/pathplanning.cpp
    AI/gameengine.cpp
    AI/game.cpp
    Data/obstacle.cpp
    Data/mapdata.cpp
    GUI/mainwindow.cpp
    GUI/qcustomplot.cpp
    Logging/log.cpp
    Sensor/orientierung.cpp
    Sensor/sensorhighlevel.cpp
    Sensor/sensorLowLevel.cpp
    Sensor/cam.cpp
    Sensor/medianfilter.cpp
    Sensor/medianfilter_new.cpp
    Sensor/trilateration.cpp
    Structs/position.cpp
    Structs/constrainedlaserdata.cpp
)
SET(THE_HEADERS
    Data/define.h
    GUI/cvimagewidget.h 
    Structs/laserplotdata.h
    Structs/logparams.h
    Structs/pathplotdata.h
    Structs/pidparams.h
    Structs/pidplotdata.h
)

SET(THE_FORMS GUI/mainwindow.ui)

QT5_WRAP_UI(THE_FORMS_HEADERS ${THE_FORMS})

# Tell CMake to create the helloworld executable

# Windows build
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${THE_SOURCES} ${THE_SOURCES} ${THE_HEADERS} ${THE_FORMS_HEADERS})
endif()

# Linux build
if(UNIX)
    add_executable(${PROJECT_NAME} ${THE_SOURCES} ${THE_HEADERS} ${THE_FORMS_HEADERS})
endif()

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} Qt5::Concurrent Qt5::Widgets Qt5::PrintSupport ${PLAYERCPP_LINK_LIBS} ${OpenCV_LIBS} referee)
