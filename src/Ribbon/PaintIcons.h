#pragma once

#include <QIcon>
#include <QPixmap>
#include <QCursor>
#include "../Tools/ToolTypes.h"

class PaintIcons {
public:
    static QIcon appIcon(int size = 32);
    static QIcon newIcon(int size = 16);
    static QIcon openIcon(int size = 16);
    static QIcon saveIcon(int size = 16);
    static QIcon saveAsIcon(int size = 16);
    static QIcon undoIcon(int size = 16);
    static QIcon redoIcon(int size = 16);
    static QIcon propertiesIcon(int size = 16);
    static QIcon aboutIcon(int size = 16);
    static QIcon exitIcon(int size = 16);

    // Clipboard
    static QIcon pasteIcon(int size = 28);
    static QIcon cutIcon(int size = 16);
    static QIcon copyIcon(int size = 16);

    // Image
    static QIcon selectRectIcon(int size = 28);
    static QIcon selectFreeformIcon(int size = 16);
    static QIcon cropIcon(int size = 16);
    static QIcon resizeIcon(int size = 16);
    static QIcon rotateIcon(int size = 16);

    // Tools
    static QIcon pencilIcon(int size = 20);
    static QIcon fillIcon(int size = 20);
    static QIcon textIcon(int size = 20);
    static QIcon eraserIcon(int size = 20);
    static QIcon colorPickerIcon(int size = 20);
    static QCursor colorPickerCursor();
    static QIcon magnifierIcon(int size = 20);

    // Brushes
    static QIcon brushesMainIcon(int size = 28);
    static QIcon brushTypeIcon(BrushType type, int size = 20);

    // Shapes
    static QPixmap shapeThumbnail(ShapeType type, int width = 18, int height = 18, bool selected = false);

    // View
    static QIcon zoomInIcon(int size = 28);
    static QIcon zoomOutIcon(int size = 28);
    static QIcon zoom100Icon(int size = 28);
    static QIcon fullscreenIcon(int size = 28);
};
