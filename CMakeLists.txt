set(Qt5Core_DIR "/opt/Qt5.6.0/5.6/gcc_64/lib/cmake/Qt5Core")
set(Qt5Test_DIR "/opt/Qt5.6.0/5.6/gcc_64/lib/cmake/Qt5Test")
set(Qt5_DIR "/opt/Qt5.6.0/5.6/gcc_64/lib/cmake/Qt5Core")
set(QT_QMAKE_EXECUTABLE "/opt/Qt5.6.0/5.6/gcc_64/bin/qmake")

project(qmsgpack)

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

# build type
if ("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")
  add_definitions("-DQT_NO_DEBUG_OUTPUT")
endif ("${CMAKE_BUILD_TYPE}" MATCHES "^Rel.*")

# 'd' postfix on windows if debug
if (WIN32)
	set(CMAKE_DEBUG_POSTFIX "d")
endif (WIN32)

option (QT4_BUILD "BUild with Qt4")
if (NOT QT4_BUILD)
	find_package(Qt5Core QUIET)
endif ()

if (Qt5Core_FOUND)
	message("Qt5 found")
	include_directories(${Qt5Core_INCLUDE_DIRS})
	add_definitions(${Qt5Core_DEFINITIONS})
	# Find includes in corresponding build directories
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	# Instruct CMake to run moc automatically when needed.
	set(CMAKE_AUTOMOC ON)
	set(PC_Requires "Qt5Core")
else ()
	message("Qt5 not found, searching for Qt4")
	find_package(Qt4 REQUIRED)
	include(${QT_USE_FILE})
	# Find includes in corresponding build directories
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	# Instruct CMake to run moc automatically when needed.
	set(CMAKE_AUTOMOC ON)
	set(PC_Requires "QtCore")
endif ()

option (WITH_GUI_TYPES "Build with support for QtGui types")
if (WITH_GUI_TYPES)
	if (QT4_BUILD)
		find_package(Qt4 QTGUI)
	else ()
		find_package(Qt5Gui QUIET)
	endif ()
endif ()

option (WITH_LOCATION_TYPES "Build with support for QtLocation types")
if (WITH_LOCATION_TYPES)
	find_package(Qt5Location QUIET)
endif ()

if (Qt5Location_FOUND)
	message("Qt5Location found")
	include_directories(${Qt5Location_INCLUDE_DIRS})
	add_definitions(${Qt5Location_DEFINITIONS})
else ()
	message("Qt5Location not found")
endif ()

if (NOT WIN32)
	set(QT_DONT_USE_QTGUI TRUE)
endif ()

#add extra search paths for libraries and includes
set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Directory where lib will install")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The directory the headers are installed in")
set(CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/${CMAKE_PROJECT_NAME}" CACHE PATH "Directory where to install qmsgpack.cmake")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

set(QMSGPACK_MAJOR	"0")
set(QMSGPACK_MINOR	"1")
set(QMSGPACK_VERSION	"0")

set(MSGPACK_QT_LIB_VERSION_STRING "${QMSGPACK_MAJOR}.${QMSGPACK_MINOR}.${QMSGPACK_VERSION}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_subdirectory(src)
if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif ()

install(EXPORT qmsgpack-export DESTINATION ${CMAKECONFIG_INSTALL_DIR} FILE MsgPackQtTargets.cmake)

file(RELATIVE_PATH relInstallDir ${CMAKE_INSTALL_PREFIX}/$CMAKECONFIG_INSTALL_DIR} ${CMAKE_INSTALL_PREFIX})

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")