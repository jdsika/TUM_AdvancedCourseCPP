set(SRC_H
  hermescodes.h
  hermes.h
  referee.h
)

set(SRC_CPP
  hermes.cpp
  referee.cpp
)

add_library( referee STATIC
	${SRC_CPP} 
	${SRC_H} 
)

target_link_libraries ( referee
  Qt5::Network
)

IF(EXISTS ${PROJECT_SOURCE_DIR}/.svn)
# Probably a SVN workspace, determine revision level
	FIND_PACKAGE(Subversion)
	IF(Subversion_FOUND)
		Subversion_WC_INFO(${PROJECT_SOURCE_DIR} REFEREE)
		MESSAGE("Current revision of referee is ${REFEREE_WC_REVISION}")
	ELSE(Subversion_FOUND)
		SET(REFEREE_WC_REVISION svn)
	ENDIF(Subversion_FOUND)
ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/.svn) 

###### Begin doc target ######
FIND_PACKAGE(Doxygen)
IF (DOXYGEN_FOUND)
    # Parse Doxyfile.in and put output into build directory
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/referee.doxy ${CMAKE_CURRENT_BINARY_DIR}/referee.doxy)
    # adds doc target to Makefile
    ADD_CUSTOM_TARGET(referee-doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/referee.doxy)
ENDIF (DOXYGEN_FOUND)
###### End doc target ######
