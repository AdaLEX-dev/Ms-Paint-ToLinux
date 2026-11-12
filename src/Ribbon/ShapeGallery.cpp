#include "ShapeGallery.h"
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
        if (static_cast<int>(m_selectedShape) == i) {
            p.fillRect(r, QColor(204, 232, 255));
            p.setPen(QColor(0, 120, 215));
            p.drawRect(r);
        } else {
            p.setPen(QColor(220, 220, 220));
            p.drawRect(r);
        }

        p.setPen(Qt::black);
        p.setBrush(Qt::NoBrush);

        QRect iconR = r.adjusted(3, 3, -3, -3);
        int x = iconR.x(), y = iconR.y(), w = iconR.width(), h = iconR.height();

        switch (static_cast<ShapeType>(i)) {
            case ShapeType::Line:
                p.drawLine(iconR.topLeft(), iconR.bottomRight());
                break;
            case ShapeType::Curve: {
                QPainterPath path;
                path.moveTo(iconR.topLeft());
                path.quadTo(iconR.center() + QPoint(2, -2), iconR.bottomRight());
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
                for (int k = 0; k < 5; ++k) {
                    double a = -M_PI / 2.0 + k * (2.0 * M_PI / 5.0);
                    poly << QPoint(int(x + w / 2 + (w / 2) * std::cos(a)), int(y + h / 2 + (h / 2) * std::sin(a)));
                }
                p.drawPolygon(poly);
                break;
            }
            case ShapeType::Hexagon: {
                QPolygon poly;
                for (int k = 0; k < 6; ++k) {
                    double a = k * (2.0 * M_PI / 6.0);
                    poly << QPoint(int(x + w / 2 + (w / 2) * std::cos(a)), int(y + h / 2 + (h / 2) * std::sin(a)));
                }
                p.drawPolygon(poly);
                break;
            }
            case ShapeType::RightArrow:
                p.drawLine(x, y + h / 2, x + w, y + h / 2);
                p.drawLine(x + w / 2, y, x + w, y + h / 2);
                p.drawLine(x + w / 2, y + h, x + w, y + h / 2);
                break;
            case ShapeType::LeftArrow:
                p.drawLine(x, y + h / 2, x + w, y + h / 2);
                p.drawLine(x + w / 2, y, x, y + h / 2);
                p.drawLine(x + w / 2, y + h, x, y + h / 2);
                break;
            case ShapeType::UpArrow:
                p.drawLine(x + w / 2, y, x + w / 2, y + h);
                p.drawLine(x, y + h / 2, x + w / 2, y);
                p.drawLine(x + w, y + h / 2, x + w / 2, y);
                break;
            case ShapeType::DownArrow:
                p.drawLine(x + w / 2, y, x + w / 2, y + h);
                p.drawLine(x, y + h / 2, x + w / 2, y + h);
                p.drawLine(x + w, y + h / 2, x + w / 2, y + h);
                break;
            case ShapeType::Star4:
            case ShapeType::Star5:
            case ShapeType::Star6:
            case ShapeType::Heart:
            case ShapeType::Lightning:
            default:
                p.drawRect(iconR);
                break;
        }
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
