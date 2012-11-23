add_subdirectory (voronoi)
 
add_executable(gerar_voronoi core.cc)
target_link_libraries (gerar_voronoi ${project_LIBS})