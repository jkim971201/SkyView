add_library(db)

target_sources(db
  PRIVATE
  dbDatabase.cpp
  dbTech.cpp
  dbTypes.cpp
  dbLefReader.cpp
	dbDefReader.cpp
  dbLayer.cpp
  dbSite.cpp
  dbPin.cpp
  dbMacro.cpp
  dbObs.cpp
)

target_link_libraries(db
  PRIVATE
  lef
	def
)

target_include_directories(db
  PUBLIC
  ${CMAKE_SOURCE_DIR}/src/database/parsers/lef
  ${CMAKE_SOURCE_DIR}/src/database/parsers/def
)
