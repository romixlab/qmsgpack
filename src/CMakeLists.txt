set(qmsgpack_srcs msgpack.cpp msgpackcommon.cpp msgpackstream.cpp private/pack_p.cpp private/unpack_p.cpp private/qt_types_p.cpp stream/time.cpp stream/geometry.cpp)
set(qmsgpack_headers msgpack.h msgpackstream.h msgpackcommon.h msgpack_export.h endianhelper.h)
set(qmsgpack_stream_headers stream/location.h stream/time.h stream/geometry.h)

add_library(qmsgpack SHARED ${qmsgpack_srcs} ${qmsgpack_headers})

if (Qt5Core_FOUND)
	target_link_libraries(qmsgpack Qt5::Core)
else ()
	target_link_libraries(qmsgpack ${QT_LIBRARIES})
endif ()

if (Qt5Gui_FOUND)
	target_link_libraries(qmsgpack Qt5::Gui)
endif ()

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/msgpackcommon.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/msgpackcommon.h"
  IMMEDIATE @ONLY)

if (NOT android)
	set_target_properties(qmsgpack PROPERTIES
									VERSION ${QMSGPACK_MAJOR}.${QMSGPACK_MINOR}.${QMSGPACK_VERSION}
									SOVERSION ${QMSGPACK_MAJOR})
endif ()
set_target_properties(qmsgpack PROPERTIES
								DEFINE_SYMBOL	MSGPACK_MAKE_LIB
								PUBLIC_HEADER	"${qmsgpack_headers}")

install(TARGETS qmsgpack EXPORT qmsgpack-export
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
	PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}/qmsgpack)


install(FILES  ${qmsgpack_stream_headers} DESTINATION ${INCLUDE_INSTALL_DIR}/qmsgpack/stream)