file (GLOB_RECURSE project_SRCS tree.cc voronoi.cc point.cc status.cc
								diagram.cc queue.cc)

set (project_LIBS ${Boost_LIBRARIES} ${GLog_LIBRARIES} ${Gmock_LIBRARIES})

set (project_BIN ${PROJECT_NAME})

add_library (voronoi STATIC ${project_SRCS})

