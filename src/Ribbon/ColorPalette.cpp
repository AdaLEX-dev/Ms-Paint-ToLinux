#include "ColorPalette.h"
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

ColorPalette::ColorPalette(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(290, 85);

    // Exact Windows 10 Default Color Palette (Row 1)
    m_row1 = {
        QColor(0, 0, 0),        // Black
        QColor(127, 127, 127),  // Gray-50%
        QColor(136, 0, 21),     // Dark Red
        QColor(237, 28, 36),    // Red
        QColor(255, 127, 39),   // Orange
        QColor(255, 242, 0),    // Yellow
        QColor(34, 177, 76),    // Green
        QColor(0, 162, 232),    // Turquoise
        QColor(63, 72, 204),    // Indigo
        QColor(163, 73, 164)    // Purple
    };

    // Row 2
    m_row2 = {
        QColor(255, 255, 255),  // White
        QColor(195, 195, 195),  // Light Gray
        QColor(185, 122, 87),   // Brown
        QColor(255, 174, 201),  // Rose
        QColor(255, 201, 14),   // Gold
        QColor(239, 228, 176),  // Light Yellow
        QColor(181, 230, 29),   // Lime
        QColor(153, 217, 234),  // Light Turquoise
        QColor(112, 146, 190),  // Blue-gray
        QColor(200, 191, 231)   // Lavender
    };

    // Row 3: 10 Custom slots
    m_row3.fill(QColor(255, 255, 255), 10);

    // Edit Colors Button
    m_editColorsBtn = new QPushButton("Edit\ncolors", this);
    m_editColorsBtn->setGeometry(235, 10, 48, 55);
    m_editColorsBtn->setStyleSheet(
        "QPushButton {"
        "  background: #fdfdfd; border: 1px solid #c0c0c0; border-radius: 2px; font-size: 11px;"
        "}"
        "QPushButton:hover { background: #e5f1fb; border-color: #0078d7; }"
        "QPushButton:pressed { background: #cce4f7; }"
    );
    connect(m_editColorsBtn, &QPushButton::clicked, this, &ColorPalette::onEditColorsClicked);
}

void ColorPalette::setColor1(const QColor& c) {
    m_color1 = c;
    update();
}

void ColorPalette::setColor2(const QColor& c) {
    m_color2 = c;
    update();
}

QRect ColorPalette::getColor1Rect() const {
    return QRect(5, 10, 32, 45);
}

QRect ColorPalette::getColor2Rect() const {
    return QRect(42, 10, 32, 45);
}

QRect ColorPalette::getSwatchRect(int row, int col) const {
    int startX = 82;
    int startY = 12;
    int size = 13;
    int gap = 2;
    return QRect(startX + col * (size + gap), startY + row * (size + gap), size, size);
}

void ColorPalette::onEditColorsClicked() {
    QColor current = m_editingColor1 ? m_color1 : m_color2;
    QColor picked = QColorDialog::getColor(current, this, "Edit Colors", QColorDialog::ShowAlphaChannel);
    if (picked.isValid()) {
        if (m_editingColor1) {
            m_color1 = picked;
            emit color1Changed(picked);
        } else {
            m_color2 = picked;
            emit color2Changed(picked);
        }

        // Add to first empty or shift custom colors
        m_row3.pop_back();
        m_row3.prepend(picked);
        update();
    }
}

void ColorPalette::paintEvent(QPaintEvent* event) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    // 1. Draw Color 1 Box
    QRect r1 = getColor1Rect();
    if (m_editingColor1) {
        p.fillRect(r1.adjusted(-2, -2, 2, 2), QColor(204, 232, 255));
        p.setPen(QColor(0, 120, 215));
        p.drawRect(r1.adjusted(-2, -2, 2, 2));
    }
    p.fillRect(QRect(r1.x() + 3, r1.y() + 3, 26, 26), m_color1);
    p.setPen(QColor(160, 160, 160));
    p.drawRect(QRect(r1.x() + 3, r1.y() + 3, 26, 26));
    p.setPen(Qt::black);
    QFont f = p.font();
    f.setPointSize(8);
    p.setFont(f);
    p.drawText(QRect(r1.x(), r1.bottom() - 12, r1.width(), 12), Qt::AlignCenter, "Color 1");

    // 2. Draw Color 2 Box
    QRect r2 = getColor2Rect();
    if (!m_editingColor1) {
        p.fillRect(r2.adjusted(-2, -2, 2, 2), QColor(204, 232, 255));
        p.setPen(QColor(0, 120, 215));
        p.drawRect(r2.adjusted(-2, -2, 2, 2));
    }
    p.fillRect(QRect(r2.x() + 3, r2.y() + 3, 26, 26), m_color2);
    p.setPen(QColor(160, 160, 160));
    p.drawRect(QRect(r2.x() + 3, r2.y() + 3, 26, 26));
    p.setPen(Qt::black);
    p.drawText(QRect(r2.x(), r2.bottom() - 12, r2.width(), 12), Qt::AlignCenter, "Color 2");

    // 3. Draw 30 Color Swatches
    for (int col = 0; col < 10; ++col) {
        // Row 1
        QRect sw1 = getSwatchRect(0, col);
        p.fillRect(sw1, m_row1[col]);
        p.setPen(QColor(180, 180, 180));
        p.drawRect(sw1);

        // Row 2
        QRect sw2 = getSwatchRect(1, col);
        p.fillRect(sw2, m_row2[col]);
        p.setPen(QColor(180, 180, 180));
        p.drawRect(sw2);

        // Row 3
        QRect sw3 = getSwatchRect(2, col);
        p.fillRect(sw3, m_row3[col]);
        p.setPen(QColor(200, 200, 200));
        p.drawRect(sw3);
    }

    // Label at bottom
    p.setPen(QColor(100, 100, 100));
    p.drawText(QRect(0, 70, width(), 14), Qt::AlignCenter, "Colors");
}

void ColorPalette::mousePressEvent(QMouseEvent* event) {
    if (getColor1Rect().contains(event->pos())) {
        m_editingColor1 = true;
        update();
        return;
    }
    if (getColor2Rect().contains(event->pos())) {
        m_editingColor1 = false;
        update();
        return;
    }

    // Check click on 30 swatches
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 10; ++c) {
            QRect sw = getSwatchRect(r, c);
            if (sw.contains(event->pos())) {
                QColor sel = (r == 0) ? m_row1[c] : ((r == 1) ? m_row2[c] : m_row3[c]);
                if (event->button() == Qt::RightButton || !m_editingColor1) {
                    m_color2 = sel;
                    emit color2Changed(sel);
                } else {
                    m_color1 = sel;
                    emit color1Changed(sel);
                }
                update();
                return;
            }
        }
    }
}
