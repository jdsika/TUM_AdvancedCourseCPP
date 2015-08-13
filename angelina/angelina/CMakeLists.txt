cmake_minimum_required(VERSION 2.8.11)

# Project name
PROJECT( P3-DX_Challenge )

# We want to use release (debug does not work with SystemC)
set(CMAKE_BUILD_TYPE Release)

# Use some custom cmakefiles
set (CMAKE_MODULE_PATH
	/usr
) 

SET( CMAKE_AUTOMOC TRUE )

# Check for Qt5
FIND_PACKAGE( Qt5Widgets )
FIND_PACKAGE( Qt5Network )
FIND_PACKAGE( Qt5Xml )

# External header
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/hermes )

# Subdirectories
add_subdirectory (angelina)
add_subdirectory (hermes)
add_subdirectory (example)

