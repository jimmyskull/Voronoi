
add_executable (viewer_test viewer_test.cc dcel_maker.cc)
target_link_libraries (viewer_test ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} drawer voronoi)
