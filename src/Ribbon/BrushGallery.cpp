#include "BrushGallery.h"
#include "PaintIcons.h"
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
    m_button->setIcon(PaintIcons::brushesMainIcon(28));
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
        QAction* act = m_menu->addAction(PaintIcons::brushTypeIcon(b.type, 20), b.name);
        connect(act, &QAction::triggered, this, [this, b]() {
            m_currentBrush = b.type;
            emit brushChanged(m_currentBrush);
        });
    }

    m_button->setMenu(m_menu);
}
