add_subdirectory (voronoi)

add_library(drawer STATIC viewer.cc)


add_executable(gerar_voronoi core.cc)
target_link_libraries (gerar_voronoi ${project_LIBS} ${OPENGL_LIBRARIES}
	${GLUT_LIBRARIES} drawer)
	
             