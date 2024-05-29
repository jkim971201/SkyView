set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

set(gui_sources 
    src/SkyView.cpp
    src/MDI_Interface.cpp
		src/MainWindow.cpp
)

set(gui_headers
	  src/MDI_Interface.h
		src/MainWindow.h
)

add_library(gui 
	          ${gui_sources} 
	          ${gui_headers}
)

target_link_libraries(gui
	db
	Qt${QT_VERSION_MAJOR}::Widgets
)

target_include_directories(gui
	PUBLIC
	include
)
