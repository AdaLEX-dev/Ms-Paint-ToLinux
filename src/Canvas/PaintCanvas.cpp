#include "PaintCanvas.h"
#include <QPainterPath>
#include <QApplication>
#include <QClipboard>
#include <QQueue>
#include <QRandomGenerator>
#include <cmath>
#include <algorithm>

static const int RESIZE_HANDLE_SIZE = 6;

PaintCanvas::PaintCanvas(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_airbrushTimer = new QTimer(this);
    m_airbrushTimer->setInterval(25);
    connect(m_airbrushTimer, &QTimer::timeout, this, [this]() {
        if (m_isDrawing && m_tool == ActiveTool::Brush && m_brushType == BrushType::Airbrush) {
            drawBrush(m_currentCanvasPos, m_currentCanvasPos, m_isRightButton ? m_color2 : m_color1, BrushType::Airbrush, m_lineWidth);
            update();
        }
    });

    newImage(1152, 648, Qt::white);
}

void PaintCanvas::newImage(int width, int height, QColor bg) {
    commitSelection();
    commitText();
    m_image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    m_image.fill(bg);
    m_undoStack.clear();
    m_undoIndex = -1;
    pushUndo();
    m_isModified = false;
    emit canvasSizeChanged(m_image.size());
    setZoom(m_zoom);
    update();
}

bool PaintCanvas::openImage(const QString& filePath) {
    QImage loaded;
    if (!loaded.load(filePath)) return false;
    commitSelection();
    commitText();
    m_image = loaded.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    m_undoStack.clear();
    m_undoIndex = -1;
    pushUndo();
    m_isModified = false;
    emit canvasSizeChanged(m_image.size());
    setZoom(m_zoom);
    update();
    return true;
}

bool PaintCanvas::saveImage(const QString& filePath, const char* format) {
    commitSelection();
    commitText();
    bool ok = m_image.save(filePath, format);
    if (ok) m_isModified = false;
    return ok;
}

void PaintCanvas::resizeCanvas(int newWidth, int newHeight) {
    if (newWidth < 1) newWidth = 1;
    if (newHeight < 1) newHeight = 1;
    commitSelection();
    commitText();
    pushUndo();

    QImage newImg(newWidth, newHeight, QImage::Format_ARGB32_Premultiplied);
    newImg.fill(m_color2);
    QPainter p(&newImg);
    p.drawImage(0, 0, m_image);
    p.end();

    m_image = newImg;
    m_isModified = true;
    emit canvasSizeChanged(m_image.size());
    setZoom(m_zoom);
    update();
}

void PaintCanvas::cropToSelection() {
    if (!m_hasSelection || m_selectionRect.isEmpty()) return;
    pushUndo();
    QImage cropped = m_selectionImage;
    m_hasSelection = false;
    m_image = cropped;
    m_isModified = true;
    emit canvasSizeChanged(m_image.size());
    setZoom(m_zoom);
    update();
}

void PaintCanvas::rotate90CW() {
    commitSelection();
    pushUndo();
    QTransform t;
    t.rotate(90);
    m_image = m_image.transformed(t);
    emit canvasSizeChanged(m_image.size());
    setZoom(m_zoom);
    update();
}

void PaintCanvas::rotate90CCW() {
    commitSelection();
    pushUndo();
    QTransform t;
    t.rotate(-90);
    m_image = m_image.transformed(t);
    emit canvasSizeChanged(m_image.size());
    setZoom(m_zoom);
    update();
}

void PaintCanvas::rotate180() {
    commitSelection();
    pushUndo();
    QTransform t;
    t.rotate(180);
    m_image = m_image.transformed(t);
    emit canvasSizeChanged(m_image.size());
    setZoom(m_zoom);
    update();
}

void PaintCanvas::flipHorizontal() {
    commitSelection();
    pushUndo();
    m_image = m_image.flipped(Qt::Horizontal);
    update();
}

void PaintCanvas::flipVertical() {
    commitSelection();
    pushUndo();
    m_image = m_image.flipped(Qt::Vertical);
    update();
}

void PaintCanvas::resizeAndSkew(double scaleX, double scaleY, double skewXDeg, double skewYDeg) {
    commitSelection();
    pushUndo();

    QTransform t;
    t.scale(scaleX, scaleY);
    if (std::abs(skewXDeg) > 0.001) t.shear(std::tan(skewXDeg * M_PI / 180.0), 0);
    if (std::abs(skewYDeg) > 0.001) t.shear(0, std::tan(skewYDeg * M_PI / 180.0));

    m_image = m_image.transformed(t, Qt::SmoothTransformation);
    emit canvasSizeChanged(m_image.size());
    setZoom(m_zoom);
    update();
}

void PaintCanvas::invertColors() {
    commitSelection();
    pushUndo();
    m_image.invertPixels(QImage::InvertRgb);
    update();
}

void PaintCanvas::clearCanvas(QColor fillCol) {
    commitSelection();
    pushUndo();
    m_image.fill(fillCol);
    update();
}

void PaintCanvas::selectAll() {
    commitSelection();
    m_tool = ActiveTool::SelectRect;
    m_hasSelection = true;
    m_selectionRect = m_image.rect();
    m_selectionImage = m_image.copy();
    emit selectionSizeChanged(m_selectionRect.size());
    update();
}

void PaintCanvas::deleteSelection() {
    if (!m_hasSelection) return;
    pushUndo();
    m_hasSelection = false;
    update();
}

void PaintCanvas::cut() {
    if (!m_hasSelection) return;
    copy();
    deleteSelection();
}

void PaintCanvas::copy() {
    if (!m_hasSelection) return;
    QApplication::clipboard()->setImage(m_selectionImage);
}

void PaintCanvas::paste(const QImage& img) {
    if (img.isNull()) return;
    commitSelection();
    pushUndo();
    m_tool = ActiveTool::SelectRect;
    m_hasSelection = true;
    m_selectionImage = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    m_selectionRect = QRect(0, 0, img.width(), img.height());
    emit selectionSizeChanged(m_selectionRect.size());
    update();
}

void PaintCanvas::commitSelection() {
    if (!m_hasSelection) return;
    pushUndo();
    QPainter p(&m_image);
    if (m_transparentSelection) {
        QImage trans = m_selectionImage.copy();
        QRgb target = m_color2.rgb();
        for (int y = 0; y < trans.height(); ++y) {
            QRgb* line = (QRgb*)trans.scanLine(y);
            for (int x = 0; x < trans.width(); ++x) {
                if ((line[x] & 0x00FFFFFF) == (target & 0x00FFFFFF)) {
                    line[x] = 0x00000000;
                }
            }
        }
        p.drawImage(m_selectionRect.topLeft(), trans);
    } else {
        p.drawImage(m_selectionRect.topLeft(), m_selectionImage);
    }
    p.end();

    m_hasSelection = false;
    emit selectionSizeChanged(QSize(0, 0));
    update();
}

void PaintCanvas::commitText() {
    if (!m_isTypingText || m_textString.isEmpty()) {
        m_isTypingText = false;
        m_textString.clear();
        update();
        return;
    }
    pushUndo();
    QPainter p(&m_image);
    p.setFont(m_textFont);
    p.setPen(m_color1);
    p.drawText(m_textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, m_textString);
    p.end();
    m_isTypingText = false;
    m_textString.clear();
    update();
}

void PaintCanvas::pushUndo() {
    if (m_undoIndex < m_undoStack.size() - 1) {
        m_undoStack.resize(m_undoIndex + 1);
    }
    m_undoStack.push_back(m_image.copy());
    if (m_undoStack.size() > m_maxUndo) {
        m_undoStack.pop_front();
    }
    m_undoIndex = m_undoStack.size() - 1;
    m_isModified = true;
    emit undoStateChanged(canUndo(), canRedo());
}

void PaintCanvas::undo() {
    commitSelection();
    commitText();
    if (!canUndo()) return;
    m_undoIndex--;
    m_image = m_undoStack[m_undoIndex].copy();
    emit canvasSizeChanged(m_image.size());
    emit undoStateChanged(canUndo(), canRedo());
    setZoom(m_zoom);
    update();
}

void PaintCanvas::redo() {
    commitSelection();
    commitText();
    if (!canRedo()) return;
    m_undoIndex++;
    m_image = m_undoStack[m_undoIndex].copy();
    emit canvasSizeChanged(m_image.size());
    emit undoStateChanged(canUndo(), canRedo());
    setZoom(m_zoom);
    update();
}

void PaintCanvas::setZoom(double zoom) {
    if (zoom < 0.125) zoom = 0.125;
    if (zoom > 8.0) zoom = 8.0;
    m_zoom = zoom;

    int w = int(m_image.width() * m_zoom) + RESIZE_HANDLE_SIZE * 2 + 10;
    int h = int(m_image.height() * m_zoom) + RESIZE_HANDLE_SIZE * 2 + 10;
    setFixedSize(w, h);
    emit zoomChanged(m_zoom);
    update();
}

QPoint PaintCanvas::widgetToCanvas(const QPoint& pt) const {
    return QPoint(int(pt.x() / m_zoom), int(pt.y() / m_zoom));
}

QPoint PaintCanvas::canvasToWidget(const QPoint& pt) const {
    return QPoint(int(pt.x() * m_zoom), int(pt.y() * m_zoom));
}

QRect PaintCanvas::canvasToWidget(const QRect& rc) const {
    return QRect(canvasToWidget(rc.topLeft()), canvasToWidget(rc.bottomRight()));
}

QRect PaintCanvas::getResizeHandleRect(int handleIndex) const {
    int cw = int(m_image.width() * m_zoom);
    int ch = int(m_image.height() * m_zoom);
    int s = RESIZE_HANDLE_SIZE;

    switch (handleIndex) {
        case 0: return QRect(cw, ch / 2 - s / 2, s, s);           // Right
        case 1: return QRect(cw / 2 - s / 2, ch, s, s);           // Bottom
        case 2: return QRect(cw, ch, s, s);                       // Bottom-Right
        default: return QRect();
    }
}

int PaintCanvas::hitResizeHandle(const QPoint& pos) const {
    for (int i = 0; i < 3; ++i) {
        if (getResizeHandleRect(i).contains(pos)) return i;
    }
    return -1;
}

void PaintCanvas::updateCursor() {
    if (m_isResizingCanvas) return;
    switch (m_tool) {
        case ActiveTool::Pencil:
        case ActiveTool::Brush:
            setCursor(Qt::CrossCursor);
            break;
        case ActiveTool::Eraser:
            setCursor(Qt::CrossCursor);
            break;
        case ActiveTool::Fill:
            setCursor(Qt::PointingHandCursor);
            break;
        case ActiveTool::ColorPicker:
            setCursor(Qt::CrossCursor);
            break;
        case ActiveTool::Magnifier:
            setCursor(Qt::PointingHandCursor);
            break;
        case ActiveTool::Text:
            setCursor(Qt::IBeamCursor);
            break;
        case ActiveTool::SelectRect:
        case ActiveTool::SelectFreeform:
            setCursor(Qt::CrossCursor);
            break;
        case ActiveTool::Shape:
            setCursor(Qt::CrossCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
    }
}

void PaintCanvas::paintEvent(QPaintEvent* event) {
    QPainter p(this);
    p.fillRect(rect(), QColor(240, 240, 240)); // Checkered workspace background

    // 1. Draw main canvas image scaled
    int dispW = int(m_image.width() * m_zoom);
    int dispH = int(m_image.height() * m_zoom);
    QRect targetRect(0, 0, dispW, dispH);

    p.setRenderHint(QPainter::SmoothPixmapTransform, m_zoom < 1.0);
    p.drawImage(targetRect, m_image);

    // 2. Draw active live preview (shapes, lines, selection moving)
    if (m_isDrawing && m_tool == ActiveTool::Shape) {
        QImage preview = m_image.copy();
        QPainter pp(&preview);
        QColor c1 = m_isRightButton ? m_color2 : m_color1;
        QColor c2 = m_isRightButton ? m_color1 : m_color2;
        QRect r(m_startCanvasPos, m_currentCanvasPos);
        renderShape(pp, m_shapeType, r.normalized(), m_outlineStyle, m_fillStyle, c1, c2, m_lineWidth);
        pp.end();
        p.drawImage(targetRect, preview);
    }

    // 3. Gridlines at >= 400% zoom
    if (m_showGrid && m_zoom >= 4.0) {
        p.setPen(QPen(QColor(200, 200, 200, 150), 1, Qt::DotLine));
        for (int x = 0; x <= m_image.width(); ++x) {
            int wx = int(x * m_zoom);
            p.drawLine(wx, 0, wx, dispH);
        }
        for (int y = 0; y <= m_image.height(); ++y) {
            int wy = int(y * m_zoom);
            p.drawLine(0, wy, dispW, wy);
        }
    }

    // 4. Floating selection overlay
    if (m_hasSelection) {
        QRect selDisp = canvasToWidget(m_selectionRect);
        p.drawImage(selDisp, m_selectionImage);
        p.setPen(QPen(Qt::black, 1, Qt::DashLine));
        p.drawRect(selDisp);
    }

    // 5. In-place text box
    if (m_isTypingText) {
        QRect textDisp = canvasToWidget(m_textRect);
        p.setPen(QPen(Qt::blue, 1, Qt::DashLine));
        p.drawRect(textDisp);
        p.setFont(m_textFont);
        p.setPen(m_color1);
        p.drawText(textDisp, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, m_textString + "|");
    }

    // 6. Canvas borders and resize handles
    p.setPen(QPen(QColor(160, 160, 160), 1));
    p.drawRect(0, 0, dispW, dispH);

    p.setPen(Qt::black);
    p.setBrush(Qt::white);
    for (int i = 0; i < 3; ++i) {
        p.drawRect(getResizeHandleRect(i));
    }
}

void PaintCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton && event->button() != Qt::RightButton) return;

    m_isRightButton = (event->button() == Qt::RightButton);

    // Check resize handles first
    int hIndex = hitResizeHandle(event->pos());
    if (hIndex >= 0 && !m_isRightButton) {
        m_isResizingCanvas = true;
        m_activeResizeHandle = hIndex;
        m_dragCanvasSize = m_image.size();
        return;
    }

    QPoint cPos = widgetToCanvas(event->pos());
    m_startCanvasPos = cPos;
    m_lastCanvasPos = cPos;
    m_currentCanvasPos = cPos;

    // Check click inside existing selection
    if (m_hasSelection) {
        if (m_selectionRect.contains(cPos)) {
            m_isMovingSelection = true;
            m_selectionDragOffset = cPos - m_selectionRect.topLeft();
            return;
        } else {
            commitSelection();
        }
    }

    if (m_isTypingText) {
        if (!m_textRect.contains(cPos)) {
            commitText();
        }
    }

    m_isDrawing = true;

    switch (m_tool) {
        case ActiveTool::Pencil:
            pushUndo();
            drawPencil(cPos, cPos, m_isRightButton ? m_color2 : m_color1);
            update();
            break;

        case ActiveTool::Brush:
            pushUndo();
            if (m_brushType == BrushType::Airbrush) {
                m_airbrushTimer->start();
            }
            drawBrush(cPos, cPos, m_isRightButton ? m_color2 : m_color1, m_brushType, m_lineWidth);
            update();
            break;

        case ActiveTool::Eraser:
            pushUndo();
            drawEraser(cPos, cPos, m_lineWidth * 4);
            update();
            break;

        case ActiveTool::Fill:
            pushUndo();
            floodFill(cPos, m_isRightButton ? m_color2 : m_color1);
            m_isDrawing = false;
            update();
            break;

        case ActiveTool::ColorPicker: {
            if (m_image.valid(cPos)) {
                QColor c = m_image.pixelColor(cPos);
                if (m_isRightButton) {
                    m_color2 = c;
                    emit color2Picked(c);
                } else {
                    m_color1 = c;
                    emit color1Picked(c);
                }
            }
            m_isDrawing = false;
            break;
        }

        case ActiveTool::Magnifier: {
            if (m_isRightButton) {
                setZoom(m_zoom / 2.0);
            } else {
                setZoom(m_zoom * 2.0);
            }
            m_isDrawing = false;
            break;
        }

        case ActiveTool::Text: {
            if (!m_isTypingText) {
                m_isTypingText = true;
                m_textRect = QRect(cPos, QSize(250, 100));
                m_textString.clear();
                update();
            }
            break;
        }

        case ActiveTool::Shape:
            pushUndo();
            break;

        case ActiveTool::SelectRect:
        case ActiveTool::SelectFreeform:
            break;
    }
}

void PaintCanvas::mouseMoveEvent(QMouseEvent* event) {
    QPoint cPos = widgetToCanvas(event->pos());
    emit mousePosChanged(cPos);

    // Cursor update for resize handles
    if (!m_isDrawing) {
        int h = hitResizeHandle(event->pos());
        if (h == 0) setCursor(Qt::SizeHorCursor);
        else if (h == 1) setCursor(Qt::SizeVerCursor);
        else if (h == 2) setCursor(Qt::SizeFDiagCursor);
        else updateCursor();
    }

    if (m_isResizingCanvas) {
        int nw = m_dragCanvasSize.width();
        int nh = m_dragCanvasSize.height();
        if (m_activeResizeHandle == 0 || m_activeResizeHandle == 2) {
            nw = std::max(10, cPos.x());
        }
        if (m_activeResizeHandle == 1 || m_activeResizeHandle == 2) {
            nh = std::max(10, cPos.y());
        }
        resizeCanvas(nw, nh);
        return;
    }

    if (m_isMovingSelection) {
        m_selectionRect.moveTo(cPos - m_selectionDragOffset);
        update();
        return;
    }

    if (!m_isDrawing) return;

    m_currentCanvasPos = cPos;

    switch (m_tool) {
        case ActiveTool::Pencil:
            drawPencil(m_lastCanvasPos, cPos, m_isRightButton ? m_color2 : m_color1);
            m_lastCanvasPos = cPos;
            update();
            break;

        case ActiveTool::Brush:
            drawBrush(m_lastCanvasPos, cPos, m_isRightButton ? m_color2 : m_color1, m_brushType, m_lineWidth);
            m_lastCanvasPos = cPos;
            update();
            break;

        case ActiveTool::Eraser:
            drawEraser(m_lastCanvasPos, cPos, m_lineWidth * 4);
            m_lastCanvasPos = cPos;
            update();
            break;

        case ActiveTool::SelectRect: {
            QRect sel(m_startCanvasPos, cPos);
            emit selectionSizeChanged(sel.normalized().size());
            update();
            break;
        }

        case ActiveTool::Shape: {
            QRect shp(m_startCanvasPos, cPos);
            emit selectionSizeChanged(shp.normalized().size());
            update();
            break;
        }

        default:
            break;
    }
}

void PaintCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if (m_airbrushTimer->isActive()) {
        m_airbrushTimer->stop();
    }

    if (m_isResizingCanvas) {
        m_isResizingCanvas = false;
        m_activeResizeHandle = -1;
        updateCursor();
        return;
    }

    if (m_isMovingSelection) {
        m_isMovingSelection = false;
        return;
    }

    if (!m_isDrawing) return;
    m_isDrawing = false;

    QPoint cPos = widgetToCanvas(event->pos());

    if (m_tool == ActiveTool::SelectRect) {
        QRect r(m_startCanvasPos, cPos);
        r = r.normalized().intersected(m_image.rect());
        if (r.width() > 1 && r.height() > 1) {
            m_hasSelection = true;
            m_selectionRect = r;
            m_selectionImage = m_image.copy(r);

            // Clear original area with Color 2
            QPainter p(&m_image);
            p.fillRect(r, m_color2);
            p.end();

            emit selectionSizeChanged(r.size());
            update();
        }
    } else if (m_tool == ActiveTool::Shape) {
        QPainter p(&m_image);
        QColor c1 = m_isRightButton ? m_color2 : m_color1;
        QColor c2 = m_isRightButton ? m_color1 : m_color2;
        QRect r(m_startCanvasPos, cPos);
        renderShape(p, m_shapeType, r.normalized(), m_outlineStyle, m_fillStyle, c1, c2, m_lineWidth);
        p.end();
        emit selectionSizeChanged(QSize(0, 0));
        update();
    }
}

void PaintCanvas::keyPressEvent(QKeyEvent* event) {
    if (m_isTypingText) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            m_textString += "\n";
            update();
        } else if (event->key() == Qt::Key_Backspace) {
            if (!m_textString.isEmpty()) m_textString.chop(1);
            update();
        } else if (event->key() == Qt::Key_Escape) {
            commitText();
        } else if (!event->text().isEmpty()) {
            m_textString += event->text();
            update();
        }
        return;
    }

    if (event->key() == Qt::Key_Delete) {
        if (m_hasSelection) {
            deleteSelection();
        }
    } else {
        QWidget::keyPressEvent(event);
    }
}

void PaintCanvas::drawPencil(const QPoint& from, const QPoint& to, const QColor& col) {
    QPainter p(&m_image);
    p.setPen(QPen(col, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    p.drawLine(from, to);
    p.end();
}

void PaintCanvas::drawBrush(const QPoint& from, const QPoint& to, const QColor& col, BrushType type, int width) {
    QPainter p(&m_image);
    int effWidth = std::max(1, width * 2);

    switch (type) {
        case BrushType::Standard:
            p.setRenderHint(QPainter::Antialiasing, true);
            p.setPen(QPen(col, effWidth * 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawLine(from, to);
            break;

        case BrushType::Calligraphy1: {
            p.setRenderHint(QPainter::Antialiasing, true);
            p.setPen(Qt::NoPen);
            p.setBrush(col);
            int dist = std::max(1, int(std::hypot(to.x() - from.x(), to.y() - from.y())));
            for (int i = 0; i <= dist; ++i) {
                double t = double(i) / dist;
                int x = int(from.x() + (to.x() - from.x()) * t);
                int y = int(from.y() + (to.y() - from.y()) * t);
                QPolygonF ribbon;
                int s = effWidth * 2;
                ribbon << QPointF(x - s, y + s) << QPointF(x - s + 2, y + s)
                       << QPointF(x + s, y - s) << QPointF(x + s - 2, y - s);
                p.drawPolygon(ribbon);
            }
            break;
        }

        case BrushType::Calligraphy2: {
            p.setRenderHint(QPainter::Antialiasing, true);
            p.setPen(Qt::NoPen);
            p.setBrush(col);
            int dist = std::max(1, int(std::hypot(to.x() - from.x(), to.y() - from.y())));
            for (int i = 0; i <= dist; ++i) {
                double t = double(i) / dist;
                int x = int(from.x() + (to.x() - from.x()) * t);
                int y = int(from.y() + (to.y() - from.y()) * t);
                QPolygonF ribbon;
                int s = effWidth * 2;
                ribbon << QPointF(x - s, y - s) << QPointF(x - s + 2, y - s)
                       << QPointF(x + s, y + s) << QPointF(x + s - 2, y + s);
                p.drawPolygon(ribbon);
            }
            break;
        }

        case BrushType::Airbrush: {
            int radius = effWidth * 4;
            int density = radius * 3;
            p.setPen(col);
            for (int i = 0; i < density; ++i) {
                double rad = (QRandomGenerator::global()->generateDouble()) * radius;
                double ang = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
                int px = int(to.x() + rad * std::cos(ang));
                int py = int(to.y() + rad * std::sin(ang));
                if (m_image.valid(px, py)) {
                    p.drawPoint(px, py);
                }
            }
            break;
        }

        case BrushType::Oil: {
            p.setRenderHint(QPainter::Antialiasing, true);
            QColor oilCol = col;
            oilCol.setAlpha(200);
            p.setPen(QPen(oilCol, effWidth * 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawLine(from, to);
            // Texture streaks
            p.setPen(QPen(col.lighter(130), 1));
            p.drawLine(from.x() - 2, from.y() - 2, to.x() - 2, to.y() - 2);
            break;
        }

        case BrushType::Crayon: {
            int dist = std::max(1, int(std::hypot(to.x() - from.x(), to.y() - from.y())));
            int rad = effWidth * 2;
            for (int i = 0; i <= dist; ++i) {
                double t = double(i) / dist;
                int cx = int(from.x() + (to.x() - from.x()) * t);
                int cy = int(from.y() + (to.y() - from.y()) * t);
                for (int dx = -rad; dx <= rad; ++dx) {
                    for (int dy = -rad; dy <= rad; ++dy) {
                        if (dx * dx + dy * dy <= rad * rad) {
                            if (QRandomGenerator::global()->bounded(100) < 65) {
                                int px = cx + dx;
                                int py = cy + dy;
                                if (m_image.valid(px, py)) {
                                    m_image.setPixelColor(px, py, col);
                                }
                            }
                        }
                    }
                }
            }
            break;
        }

        case BrushType::Marker: {
            QColor markerCol = col;
            markerCol.setAlpha(120);
            p.setPen(QPen(markerCol, effWidth * 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
            p.drawLine(from, to);
            break;
        }

        case BrushType::NaturalPencil: {
            p.setRenderHint(QPainter::Antialiasing, true);
            QColor pencCol = col;
            pencCol.setAlpha(180);
            p.setPen(QPen(pencCol, effWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawLine(from, to);
            break;
        }

        case BrushType::Watercolor: {
            p.setRenderHint(QPainter::Antialiasing, true);
            QColor waterCol = col;
            waterCol.setAlpha(60);
            p.setPen(QPen(waterCol, effWidth * 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawLine(from, to);
            break;
        }
    }
    p.end();
}

void PaintCanvas::drawEraser(const QPoint& from, const QPoint& to, int size) {
    QPainter p(&m_image);
    p.setPen(QPen(m_color2, size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    p.drawLine(from, to);
    p.end();
}

void PaintCanvas::floodFill(const QPoint& startPt, const QColor& newColor) {
    if (!m_image.valid(startPt)) return;

    QRgb targetColor = m_image.pixel(startPt);
    QRgb fillColor = newColor.rgb();
    if (targetColor == fillColor) return;

    int w = m_image.width();
    int h = m_image.height();

    QVector<bool> visited(w * h, false);
    QQueue<QPoint> queue;
    queue.enqueue(startPt);
    visited[startPt.y() * w + startPt.x()] = true;

    while (!queue.isEmpty()) {
        QPoint p = queue.dequeue();
        m_image.setPixel(p, fillColor);

        static const int dx[4] = { 0, 0, -1, 1 };
        static const int dy[4] = { -1, 1, 0, 0 };

        for (int i = 0; i < 4; ++i) {
            int nx = p.x() + dx[i];
            int ny = p.y() + dy[i];
            if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                int idx = ny * w + nx;
                if (!visited[idx] && m_image.pixel(nx, ny) == targetColor) {
                    visited[idx] = true;
                    queue.enqueue(QPoint(nx, ny));
                }
            }
        }
    }
}

void PaintCanvas::renderShape(QPainter& p, ShapeType type, const QRect& rect, ShapeStyle outline, ShapeStyle fill, const QColor& col1, const QColor& col2, int width) {
    p.setRenderHint(QPainter::Antialiasing, true);

    // Configure Outline Pen
    if (outline == ShapeStyle::None) {
        p.setPen(Qt::NoPen);
    } else {
        QColor oc = col1;
        if (outline == ShapeStyle::Marker) oc.setAlpha(160);
        else if (outline == ShapeStyle::Watercolor) oc.setAlpha(100);
        p.setPen(QPen(oc, width * 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }

    // Configure Fill Brush
    if (fill == ShapeStyle::None) {
        p.setBrush(Qt::NoBrush);
    } else {
        QColor fc = col2;
        if (fill == ShapeStyle::Crayon || fill == ShapeStyle::Marker) fc.setAlpha(180);
        else if (fill == ShapeStyle::Watercolor) fc.setAlpha(120);
        p.setBrush(fc);
    }

    int x = rect.x();
    int y = rect.y();
    int w = rect.width();
    int h = rect.height();

    switch (type) {
        case ShapeType::Line:
            p.drawLine(rect.topLeft(), rect.bottomRight());
            break;

        case ShapeType::Curve: {
            QPainterPath path;
            path.moveTo(rect.topLeft());
            path.quadTo(rect.center() + QPoint(w / 4, -h / 4), rect.bottomRight());
            p.drawPath(path);
            break;
        }

        case ShapeType::Oval:
            p.drawEllipse(rect);
            break;

        case ShapeType::Rectangle:
            p.drawRect(rect);
            break;

        case ShapeType::RoundedRect:
            p.drawRoundedRect(rect, 15, 15);
            break;

        case ShapeType::Triangle: {
            QPolygon poly;
            poly << QPoint(x + w / 2, y) << QPoint(x + w, y + h) << QPoint(x, y + h);
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::RightTriangle: {
            QPolygon poly;
            poly << QPoint(x, y) << QPoint(x + w, y + h) << QPoint(x, y + h);
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::Diamond: {
            QPolygon poly;
            poly << QPoint(x + w / 2, y) << QPoint(x + w, y + h / 2) << QPoint(x + w / 2, y + h) << QPoint(x, y + h / 2);
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::Pentagon: {
            QPolygon poly;
            for (int i = 0; i < 5; ++i) {
                double a = -M_PI / 2.0 + i * (2.0 * M_PI / 5.0);
                poly << QPoint(int(x + w / 2 + (w / 2) * std::cos(a)), int(y + h / 2 + (h / 2) * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::Hexagon: {
            QPolygon poly;
            for (int i = 0; i < 6; ++i) {
                double a = i * (2.0 * M_PI / 6.0);
                poly << QPoint(int(x + w / 2 + (w / 2) * std::cos(a)), int(y + h / 2 + (h / 2) * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::RightArrow: {
            QPolygon poly;
            poly << QPoint(x, y + h / 3) << QPoint(x + w / 2, y + h / 3) << QPoint(x + w / 2, y)
                 << QPoint(x + w, y + h / 2) << QPoint(x + w / 2, y + h) << QPoint(x + w / 2, y + 2 * h / 3)
                 << QPoint(x, y + 2 * h / 3);
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::LeftArrow: {
            QPolygon poly;
            poly << QPoint(x + w, y + h / 3) << QPoint(x + w / 2, y + h / 3) << QPoint(x + w / 2, y)
                 << QPoint(x, y + h / 2) << QPoint(x + w / 2, y + h) << QPoint(x + w / 2, y + 2 * h / 3)
                 << QPoint(x + w, y + 2 * h / 3);
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::UpArrow: {
            QPolygon poly;
            poly << QPoint(x + w / 2, y) << QPoint(x + w, y + h / 2) << QPoint(x + 2 * w / 3, y + h / 2)
                 << QPoint(x + 2 * w / 3, y + h) << QPoint(x + w / 3, y + h) << QPoint(x + w / 3, y + h / 2)
                 << QPoint(x, y + h / 2);
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::DownArrow: {
            QPolygon poly;
            poly << QPoint(x + w / 3, y) << QPoint(x + 2 * w / 3, y) << QPoint(x + 2 * w / 3, y + h / 2)
                 << QPoint(x + w, y + h / 2) << QPoint(x + w / 2, y + h) << QPoint(x, y + h / 2)
                 << QPoint(x + w / 3, y + h / 2);
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::Star4: {
            QPolygon poly;
            for (int i = 0; i < 8; ++i) {
                double r = (i % 2 == 0) ? 0.5 : 0.2;
                double a = -M_PI / 2.0 + i * (M_PI / 4.0);
                poly << QPoint(int(x + w / 2 + w * r * std::cos(a)), int(y + h / 2 + h * r * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::Star5: {
            QPolygon poly;
            for (int i = 0; i < 10; ++i) {
                double r = (i % 2 == 0) ? 0.5 : 0.22;
                double a = -M_PI / 2.0 + i * (M_PI / 5.0);
                poly << QPoint(int(x + w / 2 + w * r * std::cos(a)), int(y + h / 2 + h * r * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::Star6: {
            QPolygon poly;
            for (int i = 0; i < 12; ++i) {
                double r = (i % 2 == 0) ? 0.5 : 0.25;
                double a = -M_PI / 2.0 + i * (M_PI / 6.0);
                poly << QPoint(int(x + w / 2 + w * r * std::cos(a)), int(y + h / 2 + h * r * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::Heart: {
            QPainterPath path;
            path.moveTo(x + w / 2, y + h);
            path.cubicTo(x, y + 2 * h / 3, x, y, x + w / 2, y + h / 3);
            path.cubicTo(x + w, y, x + w, y + 2 * h / 3, x + w / 2, y + h);
            p.drawPath(path);
            break;
        }

        case ShapeType::Lightning: {
            QPolygon poly;
            poly << QPoint(x + w / 2, y) << QPoint(x + w, y) << QPoint(x + w / 3, y + h / 2)
                 << QPoint(x + 2 * w / 3, y + h / 2) << QPoint(x, y + h) << QPoint(x + w / 4, y + 3 * h / 5)
                 << QPoint(x, y + 3 * h / 5);
            p.drawPolygon(poly);
            break;
        }

        case ShapeType::CalloutRoundedRect: {
            QPainterPath path;
            path.addRoundedRect(QRect(x, y, w, 4 * h / 5), 10, 10);
            QPolygon pointer;
            pointer << QPoint(x + w / 4, y + 4 * h / 5) << QPoint(x + w / 8, y + h) << QPoint(x + w / 2, y + 4 * h / 5);
            path.addPolygon(pointer);
            p.drawPath(path);
            break;
        }

        case ShapeType::CalloutOval: {
            QPainterPath path;
            path.addEllipse(QRect(x, y, w, 4 * h / 5));
            QPolygon pointer;
            pointer << QPoint(x + w / 4, y + 4 * h / 5) << QPoint(x + w / 8, y + h) << QPoint(x + w / 2, y + 4 * h / 5);
            path.addPolygon(pointer);
            p.drawPath(path);
            break;
        }

        case ShapeType::CalloutCloud: {
            QPainterPath path;
            path.addEllipse(QRect(x, y + h / 5, 2 * w / 5, 2 * h / 5));
            path.addEllipse(QRect(x + w / 4, y, w / 2, h / 2));
            path.addEllipse(QRect(x + w / 2, y + h / 5, 2 * w / 5, 2 * h / 5));
            path.addEllipse(QRect(x + w / 5, y + 2 * h / 5, 3 * w / 5, 2 * h / 5));
            path.addEllipse(QRect(x + w / 8, y + 4 * h / 5, w / 10, h / 10));
            path.addEllipse(QRect(x + w / 16, y + 9 * h / 10, w / 15, h / 15));
            p.drawPath(path);
            break;
        }

        default:
            p.drawRect(rect);
            break;
    }
}
