add_subdirectory(gui)
add_subdirectory(SkyPlace)

add_swig(I_FILE    SkyLine.i 
         NAME      skyline
         NAMESPACE skyline)

target_sources(skyline
  PRIVATE
  SkyLine.cpp
)

target_include_directories(skyline
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src/database
  ${PROJECT_SOURCE_DIR}/src/skyline
)

target_link_libraries(skyline
  PUBLIC
  db
  gui
	SkyPlace
)
