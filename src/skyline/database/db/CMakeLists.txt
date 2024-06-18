add_library(db)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)

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

  # VERILOG
  dbVerilogReader.cpp

  # BOOKSHELF
  dbBookShelfReader.cpp
)

target_link_libraries(db
	PUBLIC
  lef
  def
  verilog
  bookshelf
)

target_include_directories(db
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/.. # TODO Fix this
)
