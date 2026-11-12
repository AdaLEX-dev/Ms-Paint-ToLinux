#include "BrushGallery.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

BrushGallery::BrushGallery(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(65, 85);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 2);
    layout->setSpacing(2);

    m_button = new QToolButton(this);
    m_button->setText("Brushes");
    m_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_button->setPopupMode(QToolButton::InstantPopup);
    m_button->setFixedSize(55, 55);

    // Paint an authentic Win10 Brush Icon
    QPixmap iconPix(32, 32);
    iconPix.fill(Qt::transparent);
    QPainter p(&iconPix);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QPen(QColor(120, 80, 40), 4, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(8, 24, 20, 12);
    p.setPen(QPen(QColor(180, 180, 180), 6, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(18, 14, 22, 10);
    p.setPen(QPen(QColor(40, 40, 40), 6, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(22, 10, 26, 6);
    p.end();

    m_button->setIcon(QIcon(iconPix));
    m_button->setIconSize(QSize(28, 28));
    m_button->setStyleSheet(
        "QToolButton {"
        "  background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px;"
        "}"
        "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }"
        "QToolButton:pressed, QToolButton:checked { background: #cce4f7; border-color: #0078d7; }"
    );

    setupMenu();

    QLabel* label = new QLabel("Brushes", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: #646464; font-size: 11px;");

    layout->addWidget(m_button, 0, Qt::AlignCenter);
    layout->addWidget(label, 0, Qt::AlignCenter);
}

void BrushGallery::setupMenu() {
    m_menu = new QMenu(this);

    struct BrushInfo {
        BrushType type;
        const char* name;
    };

    static const BrushInfo brushes[] = {
        { BrushType::Standard, "Brush" },
        { BrushType::Calligraphy1, "Calligraphy brush 1" },
        { BrushType::Calligraphy2, "Calligraphy brush 2" },
        { BrushType::Airbrush, "Airbrush" },
        { BrushType::Oil, "Oil brush" },
        { BrushType::Crayon, "Crayon" },
        { BrushType::Marker, "Marker" },
        { BrushType::NaturalPencil, "Natural pencil" },
        { BrushType::Watercolor, "Watercolor brush" }
    };

    for (const auto& b : brushes) {
        QAction* act = m_menu->addAction(b.name);
        connect(act, &QAction::triggered, this, [this, b]() {
            m_currentBrush = b.type;
            emit brushChanged(m_currentBrush);
        });
    }

    m_button->setMenu(m_menu);
}
