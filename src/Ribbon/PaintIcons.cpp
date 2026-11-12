#include "PaintIcons.h"
#include <QPainter>
#include <QPainterPath>
#include <cmath>

static void setupHighQuality(QPainter& p) {
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
}

QIcon PaintIcons::appIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Artist Palette
    QPainterPath palette;
    palette.moveTo(size * 0.15, size * 0.45);
    palette.cubicTo(size * 0.05, size * 0.15, size * 0.50, size * 0.05, size * 0.85, size * 0.20);
    palette.cubicTo(size * 0.98, size * 0.45, size * 0.85, size * 0.85, size * 0.55, size * 0.90);
    palette.cubicTo(size * 0.35, size * 0.92, size * 0.30, size * 0.70, size * 0.20, size * 0.70);
    palette.cubicTo(size * 0.10, size * 0.70, size * 0.08, size * 0.55, size * 0.15, size * 0.45);

    QLinearGradient palGrad(0, 0, size, size);
    palGrad.setColorAt(0.0, QColor(245, 222, 179));
    palGrad.setColorAt(1.0, QColor(210, 180, 140));
    p.fillPath(palette, palGrad);
    p.setPen(QPen(QColor(160, 120, 80), 1.2));
    p.drawPath(palette);

    // Thumb hole
    p.setPen(QColor(160, 120, 80));
    p.setBrush(QColor(230, 230, 230));
    p.drawEllipse(QRectF(size * 0.22, size * 0.55, size * 0.14, size * 0.18));

    // Paint blobs
    auto drawBlob = [&](qreal x, qreal y, const QColor& c) {
        p.setPen(Qt::NoPen);
        p.setBrush(c);
        p.drawEllipse(QRectF(x - size * 0.06, y - size * 0.06, size * 0.12, size * 0.12));
    };
    drawBlob(size * 0.30, size * 0.24, QColor(230, 40, 40));   // Red
    drawBlob(size * 0.48, size * 0.18, QColor(255, 180, 0));  // Orange/Yellow
    drawBlob(size * 0.66, size * 0.22, QColor(40, 180, 50));   // Green
    drawBlob(size * 0.78, size * 0.38, QColor(30, 120, 240));  // Blue
    drawBlob(size * 0.72, size * 0.60, QColor(160, 50, 210));  // Purple

    // Brush crossing over palette
    p.save();
    p.translate(size * 0.5, size * 0.5);
    p.rotate(-40);
    // Wooden handle
    p.setPen(QPen(QColor(100, 60, 25), size * 0.07, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(QPointF(-size * 0.35, 0), QPointF(size * 0.15, 0));
    // Silver ferrule
    p.setPen(QPen(QColor(200, 200, 205), size * 0.08, Qt::SolidLine, Qt::FlatCap));
    p.drawLine(QPointF(size * 0.15, 0), QPointF(size * 0.25, 0));
    // Dark bristles with blue tip
    p.setPen(QPen(QColor(40, 35, 30), size * 0.08, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(QPointF(size * 0.25, 0), QPointF(size * 0.38, 0));
    p.setPen(QPen(QColor(30, 120, 240), size * 0.06, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(QPointF(size * 0.34, 0), QPointF(size * 0.38, 0));
    p.restore();

    return QIcon(pix);
}

QIcon PaintIcons::newIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    QRectF doc(size * 0.2, size * 0.1, size * 0.6, size * 0.8);
    p.fillRect(doc, Qt::white);
    p.setPen(QPen(QColor(140, 140, 140), 1));
    p.drawRect(doc);

    // Folded top right corner
    QPainterPath fold;
    fold.moveTo(size * 0.6, size * 0.1);
    fold.lineTo(size * 0.8, size * 0.3);
    fold.lineTo(size * 0.6, size * 0.3);
    fold.closeSubpath();
    p.fillPath(fold, QColor(220, 220, 220));
    p.strokePath(fold, QPen(QColor(140, 140, 140), 1));

    // Lines on paper
    p.setPen(QColor(180, 200, 220));
    p.drawLine(size * 0.3, size * 0.45, size * 0.7, size * 0.45);
    p.drawLine(size * 0.3, size * 0.60, size * 0.7, size * 0.60);
    p.drawLine(size * 0.3, size * 0.75, size * 0.55, size * 0.75);

    return QIcon(pix);
}

QIcon PaintIcons::openIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Golden folder back
    QPainterPath back;
    back.moveTo(size * 0.1, size * 0.3);
    back.lineTo(size * 0.35, size * 0.3);
    back.lineTo(size * 0.45, size * 0.4);
    back.lineTo(size * 0.9, size * 0.4);
    back.lineTo(size * 0.9, size * 0.85);
    back.lineTo(size * 0.1, size * 0.85);
    back.closeSubpath();
    p.fillPath(back, QColor(220, 170, 50));

    // Front flap
    QPainterPath front;
    front.moveTo(size * 0.05, size * 0.85);
    front.lineTo(size * 0.2, size * 0.48);
    front.lineTo(size * 0.95, size * 0.48);
    front.lineTo(size * 0.85, size * 0.85);
    front.closeSubpath();
    p.fillPath(front, QColor(255, 205, 75));
    p.setPen(QColor(190, 140, 30));
    p.drawPath(front);

    return QIcon(pix);
}

QIcon PaintIcons::saveIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // 3.5" Blue Floppy Disk
    QRectF disk(size * 0.15, size * 0.15, size * 0.7, size * 0.7);
    p.fillRect(disk, QColor(0, 110, 210));
    p.setPen(QPen(QColor(0, 70, 150), 1));
    p.drawRect(disk);

    // Metal shutter at top
    QRectF shutter(size * 0.3, size * 0.15, size * 0.4, size * 0.3);
    p.fillRect(shutter, QColor(220, 225, 230));
    p.setPen(QColor(150, 150, 150));
    p.drawRect(shutter);
    p.fillRect(QRectF(size * 0.38, size * 0.2, size * 0.08, size * 0.18), QColor(0, 70, 150));

    // White label at bottom
    QRectF label(size * 0.25, size * 0.52, size * 0.5, size * 0.33);
    p.fillRect(label, Qt::white);
    p.setPen(QColor(180, 180, 180));
    p.drawRect(label);
    p.setPen(QColor(200, 60, 60));
    p.drawLine(size * 0.3, size * 0.62, size * 0.7, size * 0.62);

    return QIcon(pix);
}

QIcon PaintIcons::saveAsIcon(int size) {
    QPixmap pix = saveIcon(size).pixmap(size, size);
    QPainter p(&pix);
    setupHighQuality(p);
    // Draw small pencil/badge in lower right
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 215, 0));
    p.drawEllipse(QRectF(size * 0.55, size * 0.55, size * 0.4, size * 0.4));
    p.setPen(QPen(QColor(80, 50, 20), 1.5, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(size * 0.65, size * 0.85, size * 0.85, size * 0.65);
    return QIcon(pix);
}

QIcon PaintIcons::undoIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    QPainterPath path;
    path.arcMoveTo(QRectF(size * 0.2, size * 0.25, size * 0.6, size * 0.6), 0);
    path.arcTo(QRectF(size * 0.2, size * 0.25, size * 0.6, size * 0.6), 0, 160);

    p.setPen(QPen(QColor(0, 120, 215), size * 0.14, Qt::SolidLine, Qt::RoundCap));
    p.drawPath(path);

    // Arrow head
    QPolygonF head;
    head << QPointF(size * 0.15, size * 0.25)
         << QPointF(size * 0.35, size * 0.15)
         << QPointF(size * 0.35, size * 0.45);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.drawPolygon(head);

    return QIcon(pix);
}

QIcon PaintIcons::redoIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    QPainterPath path;
    path.arcMoveTo(QRectF(size * 0.2, size * 0.25, size * 0.6, size * 0.6), 180);
    path.arcTo(QRectF(size * 0.2, size * 0.25, size * 0.6, size * 0.6), 180, -160);

    p.setPen(QPen(QColor(0, 120, 215), size * 0.14, Qt::SolidLine, Qt::RoundCap));
    p.drawPath(path);

    // Arrow head
    QPolygonF head;
    head << QPointF(size * 0.85, size * 0.25)
         << QPointF(size * 0.65, size * 0.15)
         << QPointF(size * 0.65, size * 0.45);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.drawPolygon(head);

    return QIcon(pix);
}

QIcon PaintIcons::propertiesIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Gear icon
    p.translate(size * 0.5, size * 0.5);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(100, 100, 100));
    for (int i = 0; i < 8; ++i) {
        p.drawRect(QRectF(-size * 0.08, -size * 0.42, size * 0.16, size * 0.84));
        p.rotate(45);
    }
    p.drawEllipse(QRectF(-size * 0.35, -size * 0.35, size * 0.7, size * 0.7));
    p.setBrush(Qt::white);
    p.drawEllipse(QRectF(-size * 0.15, -size * 0.15, size * 0.3, size * 0.3));

    return QIcon(pix);
}

QIcon PaintIcons::aboutIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.drawEllipse(QRectF(size * 0.1, size * 0.1, size * 0.8, size * 0.8));

    p.setPen(Qt::white);
    QFont f = p.font();
    f.setBold(true);
    f.setPixelSize(int(size * 0.55));
    p.setFont(f);
    p.drawText(QRectF(0, 0, size, size), Qt::AlignCenter, "i");

    return QIcon(pix);
}

QIcon PaintIcons::exitIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.setPen(QPen(QColor(210, 40, 40), size * 0.12, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(size * 0.25, size * 0.25, size * 0.75, size * 0.75);
    p.drawLine(size * 0.75, size * 0.25, size * 0.25, size * 0.75);

    return QIcon(pix);
}

QIcon PaintIcons::pasteIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Clipboard board
    QRectF board(size * 0.22, size * 0.18, size * 0.56, size * 0.72);
    p.fillRect(board, QColor(205, 170, 125));
    p.setPen(QPen(QColor(140, 105, 65), 1));
    p.drawRect(board);

    // Paper sheet
    QRectF paper(size * 0.14, size * 0.28, size * 0.52, size * 0.62);
    p.fillRect(paper, Qt::white);
    p.setPen(QColor(180, 180, 180));
    p.drawRect(paper);

    // Content lines
    p.setPen(QColor(0, 120, 215));
    p.drawLine(size * 0.20, size * 0.42, size * 0.55, size * 0.42);
    p.drawLine(size * 0.20, size * 0.54, size * 0.58, size * 0.54);
    p.drawLine(size * 0.20, size * 0.66, size * 0.45, size * 0.66);

    // Top metal clip
    QRectF clip(size * 0.36, size * 0.10, size * 0.28, size * 0.14);
    p.fillRect(clip, QColor(230, 200, 80));
    p.setPen(QColor(160, 130, 30));
    p.drawRect(clip);
    p.drawEllipse(QRectF(size * 0.45, size * 0.05, size * 0.1, size * 0.1));

    return QIcon(pix);
}

QIcon PaintIcons::cutIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Scissors
    p.setPen(QPen(QColor(0, 100, 200), size * 0.10));
    p.drawEllipse(QRectF(size * 0.12, size * 0.58, size * 0.32, size * 0.32));
    p.drawEllipse(QRectF(size * 0.56, size * 0.58, size * 0.32, size * 0.32));

    p.setPen(QPen(QColor(150, 155, 165), size * 0.09, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(size * 0.25, size * 0.65, size * 0.75, size * 0.15);
    p.drawLine(size * 0.75, size * 0.65, size * 0.25, size * 0.15);

    // Pivot screw
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(80, 80, 80));
    p.drawEllipse(QRectF(size * 0.45, size * 0.37, size * 0.10, size * 0.10));

    return QIcon(pix);
}

QIcon PaintIcons::copyIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Back sheet
    QRectF bDoc(size * 0.3, size * 0.12, size * 0.55, size * 0.65);
    p.fillRect(bDoc, QColor(240, 245, 255));
    p.setPen(QPen(QColor(140, 170, 210), 1));
    p.drawRect(bDoc);

    // Front sheet
    QRectF fDoc(size * 0.15, size * 0.25, size * 0.55, size * 0.65);
    p.fillRect(fDoc, Qt::white);
    p.setPen(QPen(QColor(0, 110, 210), 1.2));
    p.drawRect(fDoc);

    // Text lines
    p.setPen(QColor(160, 180, 200));
    p.drawLine(size * 0.25, size * 0.42, size * 0.58, size * 0.42);
    p.drawLine(size * 0.25, size * 0.56, size * 0.58, size * 0.56);
    p.drawLine(size * 0.25, size * 0.70, size * 0.45, size * 0.70);

    return QIcon(pix);
}

QIcon PaintIcons::selectRectIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Dashed rectangular selection box
    QPen pen(QColor(0, 102, 204), 1.4, Qt::DashLine);
    pen.setDashPattern({3, 2});
    p.setPen(pen);
    p.drawRect(QRectF(size * 0.15, size * 0.15, size * 0.70, size * 0.70));

    return QIcon(pix);
}

QIcon PaintIcons::selectFreeformIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    QPen pen(QColor(0, 102, 204), 1.4, Qt::DashLine);
    pen.setDashPattern({3, 2});
    p.setPen(pen);

    QPainterPath path;
    path.moveTo(size * 0.2, size * 0.5);
    path.cubicTo(size * 0.15, size * 0.15, size * 0.7, size * 0.1, size * 0.8, size * 0.4);
    path.cubicTo(size * 0.9, size * 0.8, size * 0.5, size * 0.9, size * 0.2, size * 0.5);
    p.drawPath(path);

    return QIcon(pix);
}

QIcon PaintIcons::cropIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.setPen(QPen(QColor(50, 50, 50), size * 0.10, Qt::SolidLine, Qt::SquareCap));

    // Top-left crop mark
    p.drawLine(size * 0.15, size * 0.35, size * 0.65, size * 0.35);
    p.drawLine(size * 0.35, size * 0.15, size * 0.35, size * 0.65);

    // Bottom-right crop mark
    p.drawLine(size * 0.85, size * 0.65, size * 0.35, size * 0.65);
    p.drawLine(size * 0.65, size * 0.85, size * 0.65, size * 0.35);

    // Diagonal cut
    p.setPen(QPen(QColor(0, 120, 215), size * 0.08, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(size * 0.25, size * 0.75, size * 0.75, size * 0.25);

    return QIcon(pix);
}

QIcon PaintIcons::resizeIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Inner rect
    p.setPen(QPen(QColor(150, 150, 150), 1));
    p.drawRect(QRectF(size * 0.2, size * 0.2, size * 0.6, size * 0.6));

    // Scaling diagonal arrow
    p.setPen(QPen(QColor(0, 120, 215), size * 0.12, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(size * 0.25, size * 0.75, size * 0.75, size * 0.25);

    QPolygonF head1, head2;
    head1 << QPointF(size * 0.75, size * 0.25)
          << QPointF(size * 0.55, size * 0.25)
          << QPointF(size * 0.75, size * 0.45);
    head2 << QPointF(size * 0.25, size * 0.75)
          << QPointF(size * 0.45, size * 0.75)
          << QPointF(size * 0.25, size * 0.55);

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.drawPolygon(head1);
    p.drawPolygon(head2);

    return QIcon(pix);
}

QIcon PaintIcons::rotateIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.setPen(QPen(QColor(0, 120, 215), size * 0.10, Qt::SolidLine, Qt::RoundCap));
    p.drawArc(QRectF(size * 0.18, size * 0.18, size * 0.64, size * 0.64), 45 * 16, 270 * 16);

    QPolygonF arrow;
    arrow << QPointF(size * 0.85, size * 0.35)
          << QPointF(size * 0.65, size * 0.20)
          << QPointF(size * 0.72, size * 0.50);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.drawPolygon(arrow);

    return QIcon(pix);
}

QIcon PaintIcons::pencilIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.save();
    p.translate(size * 0.5, size * 0.5);
    p.rotate(-45);

    // Pink eraser
    p.fillRect(QRectF(-size * 0.10, -size * 0.44, size * 0.20, size * 0.14), QColor(255, 140, 160));
    // Silver ferrule
    p.fillRect(QRectF(-size * 0.10, -size * 0.30, size * 0.20, size * 0.08), QColor(200, 205, 210));
    // Yellow body
    p.fillRect(QRectF(-size * 0.10, -size * 0.22, size * 0.20, size * 0.44), QColor(255, 195, 0));
    p.setPen(QColor(215, 160, 0));
    p.drawLine(0, -size * 0.22, 0, size * 0.22);

    // Wood cone
    QPolygonF wood;
    wood << QPointF(-size * 0.10, size * 0.22)
         << QPointF(size * 0.10, size * 0.22)
         << QPointF(0, size * 0.44);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(235, 205, 160));
    p.drawPolygon(wood);

    // Graphite lead tip
    QPolygonF lead;
    lead << QPointF(-size * 0.04, size * 0.33)
         << QPointF(size * 0.04, size * 0.33)
         << QPointF(0, size * 0.44);
    p.setBrush(QColor(40, 40, 40));
    p.drawPolygon(lead);

    p.restore();
    return QIcon(pix);
}

QIcon PaintIcons::fillIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.save();
    p.translate(size * 0.45, size * 0.45);
    p.rotate(35);

    // Metallic bucket
    QPolygonF bucket;
    bucket << QPointF(-size * 0.22, -size * 0.15)
           << QPointF(size * 0.22, -size * 0.15)
           << QPointF(size * 0.16, size * 0.25)
           << QPointF(-size * 0.16, size * 0.25);
    p.setPen(QPen(QColor(100, 110, 120), 1));
    p.setBrush(QColor(185, 195, 205));
    p.drawPolygon(bucket);

    // Bucket handle
    p.setPen(QPen(QColor(90, 95, 105), 1.2));
    p.setBrush(Qt::NoBrush);
    p.drawArc(QRectF(-size * 0.22, -size * 0.35, size * 0.44, size * 0.35), 0, 180 * 16);

    p.restore();

    // Spilling paint drop
    QPainterPath drop;
    drop.moveTo(size * 0.70, size * 0.60);
    drop.quadTo(size * 0.60, size * 0.75, size * 0.70, size * 0.88);
    drop.quadTo(size * 0.80, size * 0.75, size * 0.70, size * 0.60);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.fillPath(drop, QColor(0, 120, 215));

    return QIcon(pix);
}

QIcon PaintIcons::textIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.setPen(QColor(0, 90, 180));
    QFont font("Georgia", int(size * 0.68), QFont::Bold);
    p.setFont(font);
    p.drawText(QRectF(0, 0, size * 0.85, size), Qt::AlignCenter, "A");

    // Cursor beam
    p.setPen(QPen(QColor(50, 50, 50), 1.5));
    p.drawLine(size * 0.78, size * 0.20, size * 0.78, size * 0.80);
    p.drawLine(size * 0.72, size * 0.20, size * 0.84, size * 0.20);
    p.drawLine(size * 0.72, size * 0.80, size * 0.84, size * 0.80);

    return QIcon(pix);
}

QIcon PaintIcons::eraserIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.save();
    p.translate(size * 0.5, size * 0.5);
    p.rotate(-30);

    // Pink top
    QRectF pink(size * -0.25, size * -0.32, size * 0.50, size * 0.30);
    p.fillRect(pink, QColor(255, 150, 175));
    p.setPen(QColor(220, 110, 140));
    p.drawRect(pink);

    // Blue/white bottom
    QRectF white(size * -0.25, size * -0.02, size * 0.50, size * 0.34);
    p.fillRect(white, QColor(230, 240, 255));
    p.setPen(QColor(160, 180, 210));
    p.drawRect(white);

    p.restore();
    return QIcon(pix);
}

QIcon PaintIcons::colorPickerIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.save();
    p.translate(size * 0.5, size * 0.5);
    p.rotate(-45);

    // Rubber bulb
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(220, 50, 50));
    p.drawEllipse(QRectF(-size * 0.12, -size * 0.44, size * 0.24, size * 0.20));

    // Glass barrel
    QRectF barrel(-size * 0.08, -size * 0.24, size * 0.16, size * 0.40);
    p.setPen(QPen(QColor(120, 130, 140), 1));
    p.setBrush(QColor(235, 245, 255));
    p.drawRect(barrel);

    // Liquid in pipette
    p.fillRect(QRectF(-size * 0.06, -size * 0.08, size * 0.12, size * 0.24), QColor(0, 120, 215));

    // Tapered tip
    QPolygonF tip;
    tip << QPointF(-size * 0.08, size * 0.16)
        << QPointF(size * 0.08, size * 0.16)
        << QPointF(0, size * 0.36);
    p.setBrush(QColor(235, 245, 255));
    p.drawPolygon(tip);

    p.restore();

    // Ink drop at tip
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.drawEllipse(QRectF(size * 0.72, size * 0.72, size * 0.14, size * 0.14));

    return QIcon(pix);
}

QCursor PaintIcons::colorPickerCursor() {
    QPixmap pix(24, 24);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Precise eyedropper cursor with tip pointing at (1, 22)
    p.save();
    p.translate(1, 22);
    p.rotate(-45);

    // Glass nozzle
    QPolygonF nozzle;
    nozzle << QPointF(0, 0) << QPointF(-2, -4) << QPointF(2, -4);
    p.setPen(QPen(Qt::black, 1));
    p.setBrush(QColor(230, 240, 255));
    p.drawPolygon(nozzle);

    // Glass tube
    QRectF tube(-2, -18, 4, 14);
    p.fillRect(tube, QColor(240, 248, 255));
    p.fillRect(QRectF(-1.5, -12, 3, 8), QColor(0, 100, 200));
    p.setPen(QPen(Qt::black, 1));
    p.setBrush(Qt::NoBrush);
    p.drawRect(tube);

    // Rubber bulb
    p.setPen(QPen(Qt::black, 1));
    p.setBrush(QColor(210, 40, 40));
    p.drawEllipse(QRectF(-3.5, -24, 7, 6));

    p.restore();

    return QCursor(pix, 1, 22);
}

QIcon PaintIcons::magnifierIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Glass Lens
    QRectF lens(size * 0.12, size * 0.12, size * 0.50, size * 0.50);
    QLinearGradient grad(lens.topLeft(), lens.bottomRight());
    grad.setColorAt(0.0, QColor(220, 240, 255, 220));
    grad.setColorAt(1.0, QColor(140, 190, 240, 220));
    p.setBrush(grad);
    p.setPen(QPen(QColor(60, 65, 75), size * 0.08));
    p.drawEllipse(lens);

    // Glare highlight
    p.setPen(QPen(QColor(255, 255, 255, 200), size * 0.05, Qt::SolidLine, Qt::RoundCap));
    p.drawArc(lens.adjusted(size * 0.08, size * 0.08, -size * 0.08, -size * 0.08), 70 * 16, 75 * 16);

    // Handle
    p.setPen(QPen(QColor(50, 50, 55), size * 0.14, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(size * 0.50, size * 0.50, size * 0.84, size * 0.84);

    return QIcon(pix);
}

QIcon PaintIcons::brushesMainIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    // Rich Windows 10 Brush icon
    p.save();
    p.translate(size * 0.5, size * 0.5);
    p.rotate(-45);

    // Handle
    p.setPen(QPen(QColor(139, 90, 43), size * 0.12, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(0, -size * 0.40, 0, size * 0.08);

    // Silver ferrule
    p.setPen(QPen(QColor(190, 195, 200), size * 0.16, Qt::SolidLine, Qt::FlatCap));
    p.drawLine(0, size * 0.08, 0, size * 0.22);

    // Brush head / bristles with paint
    QPainterPath bristles;
    bristles.moveTo(-size * 0.09, size * 0.22);
    bristles.cubicTo(-size * 0.12, size * 0.35, -size * 0.06, size * 0.44, 0, size * 0.46);
    bristles.cubicTo(size * 0.06, size * 0.44, size * 0.12, size * 0.35, size * 0.09, size * 0.22);
    bristles.closeSubpath();
    p.fillPath(bristles, QColor(40, 35, 30));

    // Wet blue paint tip
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.drawEllipse(QRectF(-size * 0.05, size * 0.34, size * 0.10, size * 0.12));

    p.restore();
    return QIcon(pix);
}

QIcon PaintIcons::brushTypeIcon(BrushType type, int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    switch (type) {
        case BrushType::Standard:
            return brushesMainIcon(size);

        case BrushType::Calligraphy1: {
            p.save();
            p.translate(size * 0.5, size * 0.5);
            p.rotate(-45);
            p.fillRect(QRectF(-size * 0.15, -size * 0.35, size * 0.30, size * 0.70), QColor(60, 60, 60));
            p.fillRect(QRectF(-size * 0.15, size * 0.20, size * 0.30, size * 0.15), QColor(0, 120, 215));
            p.restore();
            break;
        }
        case BrushType::Calligraphy2: {
            p.save();
            p.translate(size * 0.5, size * 0.5);
            p.rotate(45);
            p.fillRect(QRectF(-size * 0.15, -size * 0.35, size * 0.30, size * 0.70), QColor(60, 60, 60));
            p.fillRect(QRectF(-size * 0.15, size * 0.20, size * 0.30, size * 0.15), QColor(0, 120, 215));
            p.restore();
            break;
        }
        case BrushType::Airbrush: {
            // Spray paint canister
            QRectF can(size * 0.25, size * 0.25, size * 0.50, size * 0.65);
            p.fillRect(can, QColor(70, 130, 180));
            p.fillRect(QRectF(size * 0.40, size * 0.12, size * 0.20, size * 0.13), QColor(200, 200, 200));
            // Mist dots
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(0, 120, 215));
            p.drawEllipse(QRectF(size * 0.70, size * 0.10, size * 0.08, size * 0.08));
            p.drawEllipse(QRectF(size * 0.82, size * 0.18, size * 0.06, size * 0.06));
            p.drawEllipse(QRectF(size * 0.75, size * 0.26, size * 0.09, size * 0.09));
            break;
        }
        case BrushType::Oil: {
            // Flat wide bristles with oily stroke
            p.fillRect(QRectF(size * 0.30, size * 0.10, size * 0.40, size * 0.50), QColor(140, 90, 50));
            p.fillRect(QRectF(size * 0.25, size * 0.60, size * 0.50, size * 0.30), QColor(220, 80, 40));
            break;
        }
        case BrushType::Crayon: {
            // Wax crayon
            QRectF c(size * 0.30, size * 0.25, size * 0.40, size * 0.65);
            p.fillRect(c, QColor(40, 160, 80));
            p.fillRect(QRectF(size * 0.30, size * 0.45, size * 0.40, size * 0.25), QColor(255, 255, 255));
            QPolygonF tip;
            tip << QPointF(size * 0.30, size * 0.25)
                << QPointF(size * 0.70, size * 0.25)
                << QPointF(size * 0.50, size * 0.05);
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(40, 160, 80));
            p.drawPolygon(tip);
            break;
        }
        case BrushType::Marker: {
            // Highlighter chisel tip
            QRectF m(size * 0.32, size * 0.35, size * 0.36, size * 0.55);
            p.fillRect(m, QColor(255, 215, 0));
            QPolygonF chisel;
            chisel << QPointF(size * 0.35, size * 0.35)
                   << QPointF(size * 0.65, size * 0.35)
                   << QPointF(size * 0.50, size * 0.12);
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(230, 190, 0));
            p.drawPolygon(chisel);
            break;
        }
        case BrushType::NaturalPencil: {
            return pencilIcon(size);
        }
        case BrushType::Watercolor: {
            // Soft round wash brush
            p.save();
            p.translate(size * 0.5, size * 0.5);
            p.rotate(-45);
            p.setPen(QPen(QColor(160, 120, 70), size * 0.10, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(0, -size * 0.4, 0, size * 0.1);
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(100, 180, 255, 200));
            p.drawEllipse(QRectF(-size * 0.15, size * 0.1, size * 0.30, size * 0.35));
            p.restore();
            break;
        }
    }

    return QIcon(pix);
}

QPixmap PaintIcons::shapeThumbnail(ShapeType type, int w, int h, bool selected) {
    QPixmap pix(w, h);
    pix.fill(selected ? QColor(204, 232, 255) : Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    if (selected) {
        p.setPen(QColor(0, 120, 215));
        p.drawRect(0, 0, w - 1, h - 1);
    }

    QRect iconR(3, 3, w - 6, h - 6);
    int x = iconR.x(), y = iconR.y(), sw = iconR.width(), sh = iconR.height();

    p.setPen(QPen(Qt::black, 1.2));
    p.setBrush(Qt::NoBrush);

    switch (type) {
        case ShapeType::Line:
            p.drawLine(iconR.topLeft(), iconR.bottomRight());
            break;
        case ShapeType::Curve: {
            QPainterPath path;
            path.moveTo(iconR.bottomLeft());
            path.cubicTo(QPointF(x + sw * 0.25, y), QPointF(x + sw * 0.75, y + sh), iconR.topRight());
            p.drawPath(path);
            break;
        }
        case ShapeType::Oval:
            p.drawEllipse(iconR);
            break;
        case ShapeType::Rectangle:
            p.drawRect(iconR);
            break;
        case ShapeType::RoundedRect:
            p.drawRoundedRect(iconR, 3, 3);
            break;
        case ShapeType::Polygon: {
            QPolygon poly;
            poly << QPoint(x + sw * 0.2, y + sh)
                 << QPoint(x, y + sh * 0.4)
                 << QPoint(x + sw * 0.4, y)
                 << QPoint(x + sw, y + sh * 0.3)
                 << QPoint(x + sw * 0.8, y + sh);
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::Triangle: {
            QPolygon poly;
            poly << QPoint(x + sw / 2, y) << QPoint(x + sw, y + sh) << QPoint(x, y + sh);
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::RightTriangle: {
            QPolygon poly;
            poly << QPoint(x, y) << QPoint(x + sw, y + sh) << QPoint(x, y + sh);
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::Diamond: {
            QPolygon poly;
            poly << QPoint(x + sw / 2, y) << QPoint(x + sw, y + sh / 2) << QPoint(x + sw / 2, y + sh) << QPoint(x, y + sh / 2);
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::Pentagon: {
            QPolygon poly;
            for (int k = 0; k < 5; ++k) {
                double a = -M_PI / 2.0 + k * (2.0 * M_PI / 5.0);
                poly << QPoint(int(x + sw / 2 + (sw / 2) * std::cos(a)), int(y + sh / 2 + (sh / 2) * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::Hexagon: {
            QPolygon poly;
            for (int k = 0; k < 6; ++k) {
                double a = k * (2.0 * M_PI / 6.0);
                poly << QPoint(int(x + sw / 2 + (sw / 2) * std::cos(a)), int(y + sh / 2 + (sh / 2) * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::RightArrow: {
            QPolygon poly;
            poly << QPoint(x, y + sh * 0.3)
                 << QPoint(x + sw * 0.5, y + sh * 0.3)
                 << QPoint(x + sw * 0.5, y)
                 << QPoint(x + sw, y + sh * 0.5)
                 << QPoint(x + sw * 0.5, y + sh)
                 << QPoint(x + sw * 0.5, y + sh * 0.7)
                 << QPoint(x, y + sh * 0.7);
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::LeftArrow: {
            QPolygon poly;
            poly << QPoint(x + sw, y + sh * 0.3)
                 << QPoint(x + sw * 0.5, y + sh * 0.3)
                 << QPoint(x + sw * 0.5, y)
                 << QPoint(x, y + sh * 0.5)
                 << QPoint(x + sw * 0.5, y + sh)
                 << QPoint(x + sw * 0.5, y + sh * 0.7)
                 << QPoint(x + sw, y + sh * 0.7);
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::UpArrow: {
            QPolygon poly;
            poly << QPoint(x + sw * 0.3, y + sh)
                 << QPoint(x + sw * 0.3, y + sh * 0.5)
                 << QPoint(x, y + sh * 0.5)
                 << QPoint(x + sw * 0.5, y)
                 << QPoint(x + sw, y + sh * 0.5)
                 << QPoint(x + sw * 0.7, y + sh * 0.5)
                 << QPoint(x + sw * 0.7, y + sh);
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::DownArrow: {
            QPolygon poly;
            poly << QPoint(x + sw * 0.3, y)
                 << QPoint(x + sw * 0.3, y + sh * 0.5)
                 << QPoint(x, y + sh * 0.5)
                 << QPoint(x + sw * 0.5, y + sh)
                 << QPoint(x + sw, y + sh * 0.5)
                 << QPoint(x + sw * 0.7, y + sh * 0.5)
                 << QPoint(x + sw * 0.7, y);
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::Star4: {
            QPolygon poly;
            for (int k = 0; k < 8; ++k) {
                double a = -M_PI / 2.0 + k * (2.0 * M_PI / 8.0);
                double r = (k % 2 == 0) ? (sw / 2.0) : (sw / 5.0);
                poly << QPoint(int(x + sw / 2 + r * std::cos(a)), int(y + sh / 2 + r * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::Star5: {
            QPolygon poly;
            for (int k = 0; k < 10; ++k) {
                double a = -M_PI / 2.0 + k * (2.0 * M_PI / 10.0);
                double r = (k % 2 == 0) ? (sw / 2.0) : (sw / 4.5);
                poly << QPoint(int(x + sw / 2 + r * std::cos(a)), int(y + sh / 2 + r * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::Star6: {
            QPolygon poly;
            for (int k = 0; k < 12; ++k) {
                double a = -M_PI / 2.0 + k * (2.0 * M_PI / 12.0);
                double r = (k % 2 == 0) ? (sw / 2.0) : (sw / 4.0);
                poly << QPoint(int(x + sw / 2 + r * std::cos(a)), int(y + sh / 2 + r * std::sin(a)));
            }
            p.drawPolygon(poly);
            break;
        }
        case ShapeType::CalloutRoundedRect: {
            QPainterPath path;
            path.addRoundedRect(QRectF(x, y, sw, sh * 0.75), 2, 2);
            path.moveTo(x + sw * 0.25, y + sh * 0.75);
            path.lineTo(x + sw * 0.15, y + sh);
            path.lineTo(x + sw * 0.45, y + sh * 0.75);
            p.drawPath(path);
            break;
        }
        case ShapeType::CalloutOval: {
            QPainterPath path;
            path.addEllipse(QRectF(x, y, sw, sh * 0.75));
            path.moveTo(x + sw * 0.25, y + sh * 0.70);
            path.lineTo(x + sw * 0.15, y + sh);
            path.lineTo(x + sw * 0.40, y + sh * 0.70);
            p.drawPath(path);
            break;
        }
        case ShapeType::CalloutCloud: {
            p.drawEllipse(QRectF(x + sw * 0.1, y + sh * 0.1, sw * 0.8, sh * 0.6));
            p.drawEllipse(QRectF(x + sw * 0.2, y + sh * 0.8, sw * 0.12, sh * 0.12));
            break;
        }
        case ShapeType::Heart: {
            QPainterPath path;
            path.moveTo(x + sw / 2, y + sh * 0.3);
            path.cubicTo(x + sw / 2, y, x, y, x, y + sh * 0.35);
            path.cubicTo(x, y + sh * 0.65, x + sw / 2, y + sh * 0.9, x + sw / 2, y + sh);
            path.cubicTo(x + sw / 2, y + sh * 0.9, x + sw, y + sh * 0.65, x + sw, y + sh * 0.35);
            path.cubicTo(x + sw, y, x + sw / 2, y, x + sw / 2, y + sh * 0.3);
            p.drawPath(path);
            break;
        }
        case ShapeType::Lightning: {
            QPolygon poly;
            poly << QPoint(x + sw * 0.6, y)
                 << QPoint(x + sw * 0.1, y + sh * 0.55)
                 << QPoint(x + sw * 0.45, y + sh * 0.55)
                 << QPoint(x + sw * 0.35, y + sh)
                 << QPoint(x + sw * 0.9, y + sh * 0.4)
                 << QPoint(x + sw * 0.55, y + sh * 0.4);
            p.drawPolygon(poly);
            break;
        }
    }

    return pix;
}

QIcon PaintIcons::zoomInIcon(int size) {
    QPixmap pix = magnifierIcon(size).pixmap(size, size);
    QPainter p(&pix);
    setupHighQuality(p);
    p.setPen(QPen(QColor(0, 80, 160), 2, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(size * 0.28, size * 0.37, size * 0.46, size * 0.37);
    p.drawLine(size * 0.37, size * 0.28, size * 0.37, size * 0.46);
    return QIcon(pix);
}

QIcon PaintIcons::zoomOutIcon(int size) {
    QPixmap pix = magnifierIcon(size).pixmap(size, size);
    QPainter p(&pix);
    setupHighQuality(p);
    p.setPen(QPen(QColor(0, 80, 160), 2, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(size * 0.28, size * 0.37, size * 0.46, size * 0.37);
    return QIcon(pix);
}

QIcon PaintIcons::zoom100Icon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    QRectF r(size * 0.1, size * 0.15, size * 0.8, size * 0.7);
    p.fillRect(r, QColor(240, 245, 255));
    p.setPen(QPen(QColor(0, 120, 215), 1.2));
    p.drawRect(r);

    p.setPen(QColor(0, 100, 180));
    QFont font = p.font();
    font.setPixelSize(int(size * 0.30));
    font.setBold(true);
    p.setFont(font);
    p.drawText(r, Qt::AlignCenter, "100%");

    return QIcon(pix);
}

QIcon PaintIcons::fullscreenIcon(int size) {
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    setupHighQuality(p);

    p.setPen(QPen(QColor(0, 100, 190), size * 0.12, Qt::SolidLine, Qt::SquareCap));

    // Top-left corner
    p.drawLine(size * 0.15, size * 0.40, size * 0.15, size * 0.15);
    p.drawLine(size * 0.15, size * 0.15, size * 0.40, size * 0.15);

    // Top-right corner
    p.drawLine(size * 0.85, size * 0.40, size * 0.85, size * 0.15);
    p.drawLine(size * 0.85, size * 0.15, size * 0.60, size * 0.15);

    // Bottom-left corner
    p.drawLine(size * 0.15, size * 0.60, size * 0.15, size * 0.85);
    p.drawLine(size * 0.15, size * 0.85, size * 0.40, size * 0.85);

    // Bottom-right corner
    p.drawLine(size * 0.85, size * 0.60, size * 0.85, size * 0.85);
    p.drawLine(size * 0.85, size * 0.85, size * 0.60, size * 0.85);

    return QIcon(pix);
}
