add_swig(I_FILE    SkyLine.i 
         NAME      skyline
         NAMESPACE skyline)

target_sources(skyline
	PRIVATE
	SkyLine.cpp
)

target_include_directories(skyline
	PRIVATE
  ${PROJECT_SOURCE_DIR}/src/database
  ${PROJECT_SOURCE_DIR}/src/skyline
)

target_link_libraries(skyline
  PRIVATE
	db
)
