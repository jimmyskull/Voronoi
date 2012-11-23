file (GLOB_RECURSE project_SRCS *.cc *.hh)
set (project_LIBS ${Boost_LIBRARIES} ${GLog_LIBRARIES} ${Gmock_LIBRARIES})
set (project_BIN ${PROJECT_NAME})

add_library (voronoi STATIC ${project_SRCS})

