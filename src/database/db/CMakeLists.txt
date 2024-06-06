add_library(db)

target_sources(db
  PRIVATE
  dbDatabase.cpp
  dbTypes.cpp
	# LEF
  dbTech.cpp
  dbLefReader.cpp
  dbLayer.cpp
  dbSite.cpp
	dbMTerm.cpp
  dbMacro.cpp
  dbObs.cpp

	# DEF
  dbDesign.cpp
  dbDefReader.cpp
  dbDie.cpp
	dbRow.cpp
	dbInst.cpp
	dbNet.cpp
	dbBTerm.cpp
	dbITerm.cpp
)

target_link_libraries(db
  PRIVATE
  lef
  def
)

target_include_directories(db
  PUBLIC
	${CMAKE_SOURCE_DIR}/src/database
	${CMAKE_SOURCE_DIR}/src/database/parsers/lef
	${CMAKE_SOURCE_DIR}/src/database/parsers/def
)
