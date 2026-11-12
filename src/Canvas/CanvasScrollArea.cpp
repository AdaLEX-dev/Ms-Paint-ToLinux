#include "CanvasScrollArea.h"
#include <QPainter>
#include <QScrollBar>

class RulerWidget : public QWidget {
public:
    enum Orientation { Horizontal, Vertical };

    RulerWidget(Orientation orient, QWidget* parent = nullptr)
        : QWidget(parent), m_orient(orient)
    {
        if (m_orient == Horizontal) setFixedHeight(20);
        else setFixedWidth(20);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter p(this);
        p.fillRect(rect(), QColor(245, 246, 247));
        p.setPen(QColor(180, 180, 180));

        if (m_orient == Horizontal) {
            p.drawLine(0, height() - 1, width(), height() - 1);
            for (int x = 0; x < width(); x += 10) {
                int tickH = (x % 50 == 0) ? 8 : 4;
                p.drawLine(x, height() - 1 - tickH, x, height() - 1);
                if (x % 50 == 0 && x > 0) {
                    p.setFont(QFont("Arial", 7));
                    p.drawText(x + 2, 10, QString::number(x));
                }
            }
        } else {
            p.drawLine(width() - 1, 0, width() - 1, height());
            for (int y = 0; y < height(); y += 10) {
                int tickW = (y % 50 == 0) ? 8 : 4;
                p.drawLine(width() - 1 - tickW, y, width() - 1, y);
                if (y % 50 == 0 && y > 0) {
                    p.setFont(QFont("Arial", 7));
                    p.drawText(2, y - 2, QString::number(y));
                }
            }
        }
    }

private:
    Orientation m_orient;
};

CanvasScrollArea::CanvasScrollArea(PaintCanvas* canvas, QWidget* parent)
    : QScrollArea(parent), m_canvas(canvas)
{
    setWidget(m_canvas);
    setWidgetResizable(false);
    setAlignment(Qt::AlignCenter);
    viewport()->setStyleSheet("background-color: #e9ecf4;");
    setStyleSheet("QScrollArea { background-color: #e9ecf4; border: none; }");

    m_topRuler = new RulerWidget(RulerWidget::Horizontal, this);
    m_leftRuler = new RulerWidget(RulerWidget::Vertical, this);
    m_topRuler->hide();
    m_leftRuler->hide();
}

void CanvasScrollArea::updateRulers() {
    if (m_showRulers) {
        m_topRuler->show();
        m_leftRuler->show();
        setViewportMargins(20, 20, 0, 0);
    } else {
        m_topRuler->hide();
        m_leftRuler->hide();
        setViewportMargins(0, 0, 0, 0);
    }
}

void CanvasScrollArea::resizeEvent(QResizeEvent* event) {
    QScrollArea::resizeEvent(event);
    if (m_showRulers) {
        m_topRuler->setGeometry(20, 0, width() - 20, 20);
        m_leftRuler->setGeometry(0, 20, 20, height() - 20);
    }
}
