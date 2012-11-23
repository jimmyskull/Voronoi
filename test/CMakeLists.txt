file (GLOB_RECURSE test_SRCS *.cc)
set (test_LIBS ${GLog_LIBRARIES} ${GOOGLEMOCK_LIBRARIES} ${PROJECT_NAME})
set (test_BIN ${PROJECT_NAME}_test)

link_directories (${MAINFOLDER}/lib)
include_directories (${GOOGLEMOCK_INCLUDE_DIR})

add_executable (${test_BIN} ${test_SRCS})
target_link_libraries (${test_BIN} ${test_LIBS})

add_custom_target (check ALL "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
add_custom_target (test "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
