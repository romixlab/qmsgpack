set(QT_USE_QTTEST TRUE)

if (NOT Qt5Core_FOUND)
	include( ${QT_USE_FILE} )
endif()

include(AddFileDependencies)

include_directories(../../src ${CMAKE_CURRENT_BINARY_DIR})

set(UNIT_TESTS mixed_test)

foreach(test ${UNIT_TESTS})
	message(status "Building ${test}")
	add_executable(${test} ${test}.cpp)

	target_link_libraries(${test}
		${QT_LIBRARIES}
		${TEST_LIBRARIES}
		qmsgpack
	)

	add_test(${test} ${test})
endforeach()