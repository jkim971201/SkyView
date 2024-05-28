add_library(gui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

target_sources(gui
	PRIVATE
	MDI_Interface.cpp
	MainWindow.cpp
	Viewer.cpp
)

target_link_libraries(gui
	PUBLIC
	db

	PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
)

target_include_directories(gui
	PUBLIC
)
