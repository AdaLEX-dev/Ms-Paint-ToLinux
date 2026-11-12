cmake_minimum_required(VERSION 3.16)
project(MsPaint VERSION 10.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/Canvas/PaintCanvas.h
    src/Canvas/PaintCanvas.cpp
    src/Canvas/CanvasScrollArea.h
    src/Canvas/CanvasScrollArea.cpp
    src/Ribbon/RibbonBar.h
    src/Ribbon/RibbonBar.cpp
    src/Ribbon/HomeTab.h
    src/Ribbon/HomeTab.cpp
    src/Ribbon/ViewTab.h
    src/Ribbon/ViewTab.cpp
    src/Ribbon/ColorPalette.h
    src/Ribbon/ColorPalette.cpp
    src/Ribbon/ShapeGallery.h
    src/Ribbon/ShapeGallery.cpp
    src/Ribbon/BrushGallery.h
    src/Ribbon/BrushGallery.cpp
    src/Ribbon/SizeSelector.h
    src/Ribbon/SizeSelector.cpp
    src/Dialogs/ResizeSkewDialog.h
    src/Dialogs/ResizeSkewDialog.cpp
    src/Dialogs/ImagePropertiesDialog.h
    src/Dialogs/ImagePropertiesDialog.cpp
    src/Tools/ToolTypes.h
)

add_executable(mspaint ${SOURCES})

target_link_libraries(mspaint PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Test target
add_executable(test_canvas
    tests/test_canvas.cpp
    src/Canvas/PaintCanvas.h
    src/Canvas/PaintCanvas.cpp
    src/Tools/ToolTypes.h
)

target_link_libraries(test_canvas PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

install(TARGETS mspaint DESTINATION bin)
