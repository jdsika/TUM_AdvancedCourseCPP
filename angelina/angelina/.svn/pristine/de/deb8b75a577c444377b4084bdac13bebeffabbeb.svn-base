set( SRC_H
  defines.h
  pillar.h
  playground.h
  qsingleton.h
)

set( SRC_CPP
  angelinaapplication.cpp
  colortask.cpp
  main.cpp
  mainwindow.cpp
  pillar.cpp
  playground.cpp
  editplaygrounddialog.cpp
  ratiotask.cpp
  resultframe.cpp
  server.cpp
  task.cpp
  timeoutclock.cpp
  clock.cpp
  editoptionsdialog.cpp
)

QT5_ADD_RESOURCES( SRC_RCS icons.qrc)

add_executable( angelina
	${SRC_CPP} 
	${SRC_H} 
	${SRC_RCS}
)

target_link_libraries ( angelina
  Qt5::Widgets
  Qt5::Network
  Qt5::Xml
)

IF(EXISTS ${PROJECT_SOURCE_DIR}/.svn)
# Probably a SVN workspace, determine revision level
	FIND_PACKAGE(Subversion)
	IF(Subversion_FOUND)
		Subversion_WC_INFO(${PROJECT_SOURCE_DIR} ANGELINA)
		MESSAGE("Current revision of angelina is ${ANGELINA_WC_REVISION}")
	ELSE(Subversion_FOUND)
		SET(ANGELINA_WC_REVISION svn)
	ENDIF(Subversion_FOUND)
ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/.svn) 

###### Begin doc target ######
FIND_PACKAGE(Doxygen)
IF (DOXYGEN_FOUND)
    # Parse Doxyfile.in and put output into build directory
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/angelina.doxy ${CMAKE_CURRENT_BINARY_DIR}/angelina.doxy)
    # adds doc target to Makefile
    ADD_CUSTOM_TARGET(angelina-doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/angelina.doxy)
ENDIF (DOXYGEN_FOUND)
###### End doc target ######

