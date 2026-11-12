#pragma once

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QVector>
#include <QPoint>
#include <QRect>
#include <QColor>
#include <QTimer>
#include "../Tools/ToolTypes.h"

class PaintCanvas : public QWidget {
    Q_OBJECT

public:
    explicit PaintCanvas(QWidget* parent = nullptr);

    // Image operations
    void newImage(int width = 1152, int height = 648, QColor bg = Qt::white);
    bool openImage(const QString& filePath);
    bool saveImage(const QString& filePath, const char* format = nullptr);
    QSize imageSize() const { return m_image.size(); }
    void resizeCanvas(int newWidth, int newHeight);
    void cropToSelection();

public slots:
    // Image modifications
    void rotate90CW();
    void rotate90CCW();
    void rotate180();
    void flipHorizontal();
    void flipVertical();
    void resizeAndSkew(double scaleX, double scaleY, double skewXDeg, double skewYDeg);
    void invertColors();
    void clearCanvas(QColor fillCol);

    // Selection & Clipboard
    void selectAll();
    void deleteSelection();
    void cut();
    void copy();
    void paste(const QImage& img);
    void commitSelection();

    // Undo / Redo
    void undo();
    void redo();

public:
    bool hasSelection() const { return m_hasSelection; }
    void setTransparentSelection(bool enabled) { m_transparentSelection = enabled; update(); }
    bool isTransparentSelection() const { return m_transparentSelection; }
    bool canUndo() const { return m_undoIndex > 0; }
    bool canRedo() const { return m_undoIndex < m_undoStack.size() - 1; }

    // Tool Settings
    void setActiveTool(ActiveTool tool) { commitSelection(); m_tool = tool; updateCursor(); update(); }
    ActiveTool activeTool() const { return m_tool; }

    void setBrushType(BrushType type) { m_brushType = type; }
    BrushType brushType() const { return m_brushType; }

    void setShapeType(ShapeType type) { m_shapeType = type; }
    ShapeType shapeType() const { return m_shapeType; }

    void setShapeOutlineStyle(ShapeStyle style) { m_outlineStyle = style; }
    ShapeStyle shapeOutlineStyle() const { return m_outlineStyle; }

    void setShapeFillStyle(ShapeStyle style) { m_fillStyle = style; }
    ShapeStyle shapeFillStyle() const { return m_fillStyle; }

    void setLineWidth(int width) { m_lineWidth = width; }
    int lineWidth() const { return m_lineWidth; }

    void setColor1(const QColor& c) { m_color1 = c; }
    QColor color1() const { return m_color1; }

    void setColor2(const QColor& c) { m_color2 = c; }
    QColor color2() const { return m_color2; }

    // Zoom & Grid
    void setZoom(double zoom);
    double zoom() const { return m_zoom; }
    void setShowGrid(bool show) { m_showGrid = show; update(); }
    bool showGrid() const { return m_showGrid; }

    // Text tool properties
    void setTextFont(const QFont& font) { m_textFont = font; }
    QFont textFont() const { return m_textFont; }
    void commitText();

    bool isModified() const { return m_isModified; }
    void setModified(bool mod) { m_isModified = mod; }

signals:
    void mousePosChanged(QPoint pos);
    void canvasSizeChanged(QSize size);
    void selectionSizeChanged(QSize size);
    void zoomChanged(double zoom);
    void color1Picked(QColor c);
    void color2Picked(QColor c);
    void undoStateChanged(bool canUndo, bool canRedo);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void pushUndo();
    void updateCursor();
    QPoint widgetToCanvas(const QPoint& pt) const;
    QPoint canvasToWidget(const QPoint& pt) const;
    QRect canvasToWidget(const QRect& rc) const;

    // Drawing helpers
    void drawPencil(const QPoint& from, const QPoint& to, const QColor& col);
    void drawBrush(const QPoint& from, const QPoint& to, const QColor& col, BrushType type, int width);
    void drawEraser(const QPoint& from, const QPoint& to, int size);
    void floodFill(const QPoint& startPt, const QColor& newColor);
    void renderShape(QPainter& p, ShapeType type, const QRect& rect, ShapeStyle outline, ShapeStyle fill, const QColor& col1, const QColor& col2, int width);

    // Resize handles hit testing (returns handle index 0-7, or -1)
    int hitResizeHandle(const QPoint& pos) const;
    QRect getResizeHandleRect(int handleIndex) const;

private:
    QImage m_image;
    double m_zoom = 1.0;
    bool m_showGrid = false;
    bool m_isModified = false;

    // Undo stack
    QVector<QImage> m_undoStack;
    int m_undoIndex = -1;
    const int m_maxUndo = 50;

    // Tool states
    ActiveTool m_tool = ActiveTool::Pencil;
    BrushType m_brushType = BrushType::Standard;
    ShapeType m_shapeType = ShapeType::Line;
    ShapeStyle m_outlineStyle = ShapeStyle::Solid;
    ShapeStyle m_fillStyle = ShapeStyle::None;
    int m_lineWidth = 1;
    QColor m_color1 = Qt::black;
    QColor m_color2 = Qt::white;
    QFont m_textFont{"Arial", 12};

    // Active stroke / shape / drag state
    bool m_isDrawing = false;
    bool m_isRightButton = false;
    QPoint m_lastCanvasPos;
    QPoint m_startCanvasPos;
    QPoint m_currentCanvasPos;

    // Polygon tool vertices
    QVector<QPoint> m_polygonPoints;
    bool m_isBuildingPolygon = false;

    // Curve tool control points
    int m_curveStep = 0; // 0=start line, 1=first bend, 2=second bend
    QPoint m_curveP1, m_curveP2, m_curveP3, m_curveP4;

    // Floating selection
    bool m_hasSelection = false;
    QRect m_selectionRect;
    QImage m_selectionImage;
    bool m_isMovingSelection = false;
    QPoint m_selectionDragOffset;
    bool m_transparentSelection = false;

    // Canvas resizing via border handles
    int m_activeResizeHandle = -1;
    bool m_isResizingCanvas = false;
    QSize m_dragCanvasSize;

    // Airbrush timer
    QTimer* m_airbrushTimer = nullptr;

    // In-place text box
    bool m_isTypingText = false;
    QRect m_textRect;
    QString m_textString;
};
