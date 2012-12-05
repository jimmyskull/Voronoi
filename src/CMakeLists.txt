add_subdirectory (voronoi)

add_library(viewer STATIC viewer.cc)

add_executable(gerar_voronoi core.cc)
target_link_libraries (gerar_voronoi voronoi ${project_LIBS} ${OPENGL_LIBRARIES}
	${GLUT_LIBRARIES} viewer)
	
add_executable(harcoded_voronoi hardcoded.cc)
target_link_libraries (harcoded_voronoi voronoi ${project_LIBS} ${OPENGL_LIBRARIES}
	${GLUT_LIBRARIES} viewer)
	