#include "SizeSelector.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

SizeSelector::SizeSelector(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(50, 85);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 2);
    layout->setSpacing(2);

    m_button = new QToolButton(this);
    m_button->setText("Size");
    m_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_button->setPopupMode(QToolButton::InstantPopup);
    m_button->setFixedSize(45, 55);

    QPixmap iconPix(24, 24);
    iconPix.fill(Qt::transparent);
    QPainter p(&iconPix);
    p.setPen(QPen(Qt::black, 1));
    p.drawLine(2, 6, 22, 6);
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(2, 11, 22, 11);
    p.setPen(QPen(Qt::black, 3));
    p.drawLine(2, 16, 22, 16);
    p.setPen(QPen(Qt::black, 4));
    p.drawLine(2, 21, 22, 21);
    p.end();

    m_button->setIcon(QIcon(iconPix));
    m_button->setIconSize(QSize(24, 24));
    m_button->setStyleSheet(
        "QToolButton {"
        "  background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px;"
        "}"
        "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }"
        "QToolButton:pressed, QToolButton:checked { background: #cce4f7; border-color: #0078d7; }"
    );

    m_menu = new QMenu(this);
    int sizes[] = { 1, 2, 3, 5, 8 };
    for (int s : sizes) {
        QAction* act = m_menu->addAction(QString("%1 px").arg(s));
        connect(act, &QAction::triggered, this, [this, s]() {
            m_size = s;
            emit sizeChanged(s);
        });
    }
    m_button->setMenu(m_menu);

    QLabel* label = new QLabel("Size", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: #646464; font-size: 11px;");

    layout->addWidget(m_button, 0, Qt::AlignCenter);
    layout->addWidget(label, 0, Qt::AlignCenter);
}
