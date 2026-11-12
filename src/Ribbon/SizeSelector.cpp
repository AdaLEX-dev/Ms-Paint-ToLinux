#include "SizeSelector.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

SizeSelector::SizeSelector(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(50, 92);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(2);

    m_button = new QToolButton(this);
    m_button->setText("Size");
    m_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_button->setPopupMode(QToolButton::InstantPopup);
    m_button->setFixedSize(46, 66);

    QPixmap iconPix(24, 24);
    iconPix.fill(Qt::transparent);
    QPainter p(&iconPix);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QPen(Qt::black, 1));
    p.drawLine(3, 5, 21, 5);
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(3, 10, 21, 10);
    p.setPen(QPen(Qt::black, 3));
    p.drawLine(3, 15, 21, 15);
    p.setPen(QPen(Qt::black, 4));
    p.drawLine(3, 20, 21, 20);
    p.end();

    m_button->setIcon(QIcon(iconPix));
    m_button->setIconSize(QSize(24, 24));
    m_button->setStyleSheet(
        "QToolButton {"
        "  background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px;"
        "}"
        "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
        "QToolButton:pressed, QToolButton:checked { background: #cce8ff; border-color: #3399ff; }"
    );

    m_menu = new QMenu(this);
    m_menu->setStyleSheet("QMenu { background: white; border: 1px solid #c0c0c0; font-size: 11px; } QMenu::item:selected { background: #e5f1fb; }");
    int sizes[] = { 1, 2, 3, 5, 8 };
    for (int s : sizes) {
        QPixmap linePix(50, 14);
        linePix.fill(Qt::transparent);
        QPainter lp(&linePix);
        lp.setPen(QPen(Qt::black, s));
        lp.drawLine(4, 7, 46, 7);
        lp.end();

        QAction* act = m_menu->addAction(QIcon(linePix), QString("%1 px").arg(s));
        connect(act, &QAction::triggered, this, [this, s]() {
            m_size = s;
            emit sizeChanged(s);
        });
    }
    m_button->setMenu(m_menu);

    QLabel* label = new QLabel("Size", this);
    label->setAlignment(Qt::AlignCenter);
    label->setFixedHeight(14);
    label->setStyleSheet("color: #646464; font-size: 11px;");

    layout->addWidget(m_button, 0, Qt::AlignCenter);
    layout->addWidget(label, 0, Qt::AlignCenter);
}
