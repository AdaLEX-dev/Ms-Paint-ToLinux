#include "ShapeGallery.h"
#include "PaintIcons.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <cmath>

static const int NUM_SHAPES = 23;
static const int COLS = 8;

ShapeGallery::ShapeGallery(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(250, 85);

    m_outlineCombo = new QComboBox(this);
    m_outlineCombo->setGeometry(165, 8, 80, 24);
    m_outlineCombo->addItems({"No outline", "Solid color", "Crayon", "Marker", "Oil", "Natural pencil", "Watercolor"});
    m_outlineCombo->setCurrentIndex(1);
    m_outlineCombo->setStyleSheet("font-size: 10px;");
    connect(m_outlineCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        m_outlineStyle = static_cast<ShapeStyle>(idx);
        emit outlineChanged(m_outlineStyle);
    });

    m_fillCombo = new QComboBox(this);
    m_fillCombo->setGeometry(165, 36, 80, 24);
    m_fillCombo->addItems({"No fill", "Solid color", "Crayon", "Marker", "Oil", "Natural pencil", "Watercolor"});
    m_fillCombo->setCurrentIndex(0);
    m_fillCombo->setStyleSheet("font-size: 10px;");
    connect(m_fillCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        m_fillStyle = static_cast<ShapeStyle>(idx);
        emit fillChanged(m_fillStyle);
    });
}

QRect ShapeGallery::getShapeRect(int index) const {
    int col = index % COLS;
    int row = index / COLS;
    int size = 18;
    int gap = 2;
    return QRect(6 + col * (size + gap), 8 + row * (size + gap), size, size);
}

void ShapeGallery::paintEvent(QPaintEvent* event) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Draw 23 shape thumbnail boxes
    for (int i = 0; i < NUM_SHAPES; ++i) {
        QRect r = getShapeRect(i);
        bool selected = (static_cast<int>(m_selectedShape) == i);
        QPixmap thumb = PaintIcons::shapeThumbnail(static_cast<ShapeType>(i), r.width(), r.height(), selected);
        p.drawPixmap(r.topLeft(), thumb);
    }

    // Label at bottom
    p.setPen(QColor(100, 100, 100));
    QFont f = p.font();
    f.setPointSize(8);
    p.setFont(f);
    p.drawText(QRect(0, 70, width(), 14), Qt::AlignCenter, "Shapes");
}

void ShapeGallery::mousePressEvent(QMouseEvent* event) {
    for (int i = 0; i < NUM_SHAPES; ++i) {
        if (getShapeRect(i).contains(event->pos())) {
            m_selectedShape = static_cast<ShapeType>(i);
            emit shapeChanged(m_selectedShape);
            update();
            return;
        }
    }
}
