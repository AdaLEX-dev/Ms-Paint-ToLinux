#include "ShapeGallery.h"
#include "PaintIcons.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

ShapeGallery::ShapeGallery(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(275, 92);
    setupUI();
}

void ShapeGallery::setupUI() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(2, 2, 2, 2);
    rootLayout->setSpacing(2);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(4);

    // 1. Shapes Grid inside a bordered container
    QFrame* gridFrame = new QFrame(this);
    gridFrame->setFixedSize(166, 66);
    gridFrame->setStyleSheet("QFrame { background: #ffffff; border: 1px solid #c0c0c0; border-radius: 2px; }");

    QGridLayout* grid = new QGridLayout(gridFrame);
    grid->setContentsMargins(2, 2, 2, 2);
    grid->setSpacing(1);

    const int NUM_SHAPES = 23;
    const char* shapeNames[] = {
        "Line", "Curve", "Oval", "Rectangle", "Rounded rectangle", "Polygon",
        "Triangle", "Right triangle", "Diamond", "Pentagon", "Hexagon",
        "Right arrow", "Left arrow", "Up arrow", "Down arrow",
        "Four-point star", "Five-point star", "Six-point star",
        "Rounded rectangular callout", "Oval callout", "Cloud callout",
        "Heart", "Lightning"
    };

    m_shapeButtons.reserve(NUM_SHAPES);
    for (int i = 0; i < NUM_SHAPES; ++i) {
        ShapeType st = static_cast<ShapeType>(i);
        QToolButton* btn = new QToolButton(gridFrame);
        btn->setFixedSize(18, 18);
        btn->setCheckable(true);
        btn->setToolTip(shapeNames[i]);
        btn->setIcon(QIcon(PaintIcons::shapeThumbnail(st, 16, 16, false)));
        btn->setIconSize(QSize(16, 16));
        btn->setStyleSheet(
            "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 1px; }"
            "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
            "QToolButton:checked { background: #cce8ff; border-color: #3399ff; }"
        );

        connect(btn, &QToolButton::clicked, this, [this, st]() {
            setSelectedShape(st);
            emit shapeChanged(st);
        });

        int row = i / 8;
        int col = i % 8;
        grid->addWidget(btn, row, col);
        m_shapeButtons.append(btn);
    }
    m_shapeButtons[0]->setChecked(true);

    topLayout->addWidget(gridFrame);

    // 2. Outline and Fill Buttons
    QVBoxLayout* styleLayout = new QVBoxLayout();
    styleLayout->setContentsMargins(0, 0, 0, 0);
    styleLayout->setSpacing(4);

    auto makeStyleBtn = [this](const QIcon& icon, const QString& text) {
        QToolButton* b = new QToolButton(this);
        b->setText(text);
        b->setIcon(icon);
        b->setIconSize(QSize(16, 16));
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        b->setPopupMode(QToolButton::InstantPopup);
        b->setFixedSize(96, 28);
        b->setStyleSheet(
            "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 11px; text-align: left; padding: 2px 4px; color: #222222; }"
            "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
        );
        return b;
    };

    // Outline Button
    m_outlineBtn = makeStyleBtn(PaintIcons::pencilIcon(16), "Outline");
    QMenu* outlineMenu = new QMenu(m_outlineBtn);
    outlineMenu->setStyleSheet("QMenu { background: white; border: 1px solid #c0c0c0; font-size: 11px; } QMenu::item:selected { background: #e5f1fb; }");
    const char* outlineOptions[] = {
        "No outline", "Solid color", "Crayon", "Marker", "Oil", "Natural pencil", "Watercolor"
    };
    for (int k = 0; k < 7; ++k) {
        ShapeStyle st = static_cast<ShapeStyle>(k);
        QAction* act = outlineMenu->addAction(outlineOptions[k]);
        connect(act, &QAction::triggered, this, [this, st]() {
            m_outlineStyle = st;
            emit outlineChanged(st);
        });
    }
    m_outlineBtn->setMenu(outlineMenu);

    // Fill Button
    m_fillBtn = makeStyleBtn(PaintIcons::fillIcon(16), "Fill");
    QMenu* fillMenu = new QMenu(m_fillBtn);
    fillMenu->setStyleSheet("QMenu { background: white; border: 1px solid #c0c0c0; font-size: 11px; } QMenu::item:selected { background: #e5f1fb; }");
    const char* fillOptions[] = {
        "No fill", "Solid color", "Crayon", "Marker", "Oil", "Natural pencil", "Watercolor"
    };
    for (int k = 0; k < 7; ++k) {
        ShapeStyle st = static_cast<ShapeStyle>(k);
        QAction* act = fillMenu->addAction(fillOptions[k]);
        connect(act, &QAction::triggered, this, [this, st]() {
            m_fillStyle = st;
            emit fillChanged(st);
        });
    }
    m_fillBtn->setMenu(fillMenu);

    styleLayout->addWidget(m_outlineBtn);
    styleLayout->addWidget(m_fillBtn);
    topLayout->addLayout(styleLayout);

    rootLayout->addLayout(topLayout);

    // Group Label
    QLabel* groupLabel = new QLabel("Shapes", this);
    groupLabel->setAlignment(Qt::AlignCenter);
    groupLabel->setFixedHeight(14);
    groupLabel->setStyleSheet("color: #646464; font-size: 11px;");
    rootLayout->addWidget(groupLabel);
}

void ShapeGallery::setSelectedShape(ShapeType shape) {
    m_selectedShape = shape;
    int idx = static_cast<int>(shape);
    for (int i = 0; i < m_shapeButtons.size(); ++i) {
        m_shapeButtons[i]->setChecked(i == idx);
    }
}
