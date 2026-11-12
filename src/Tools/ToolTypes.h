#pragma once

#include <QString>
#include <QColor>

enum class ActiveTool {
    SelectRect,
    SelectFreeform,
    Pencil,
    Fill,
    Text,
    Eraser,
    ColorPicker,
    Magnifier,
    Brush,
    Shape
};

enum class BrushType {
    Standard,
    Calligraphy1,
    Calligraphy2,
    Airbrush,
    Oil,
    Crayon,
    Marker,
    NaturalPencil,
    Watercolor
};

enum class ShapeType {
    Line,
    Curve,
    Oval,
    Rectangle,
    RoundedRect,
    Polygon,
    Triangle,
    RightTriangle,
    Diamond,
    Pentagon,
    Hexagon,
    RightArrow,
    LeftArrow,
    UpArrow,
    DownArrow,
    Star4,
    Star5,
    Star6,
    CalloutRoundedRect,
    CalloutOval,
    CalloutCloud,
    Heart,
    Lightning
};

enum class ShapeStyle {
    None,
    Solid,
    Crayon,
    Marker,
    Oil,
    NaturalPencil,
    Watercolor
};
