#include "ColorPalette.h"
#include "PaintIcons.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QColorDialog>
#include <QMouseEvent>
#include <QPainter>

ColorPalette::ColorPalette(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(320, 92);

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

    // Row 2: Secondary / Pastel
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

    setupUI();
    updatePreviews();
}

void ColorPalette::setupUI() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(2, 2, 2, 2);
    rootLayout->setSpacing(2);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(4);

    // 1. Dual Color Cards (Color 1 & Color 2)
    auto createColorCard = [this](const QString& name, QLabel*& swatchLabel) {
        QWidget* card = new QWidget(this);
        card->setFixedSize(34, 66);
        QVBoxLayout* cLayout = new QVBoxLayout(card);
        cLayout->setContentsMargins(2, 2, 2, 2);
        cLayout->setSpacing(2);

        QLabel* textLabel = new QLabel(name, card);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet("font-size: 9px; font-weight: bold; color: #333333;");

        swatchLabel = new QLabel(card);
        swatchLabel->setFixedSize(26, 26);
        swatchLabel->setStyleSheet("background: #000000; border: 1px solid #707070;");

        cLayout->addWidget(textLabel);
        cLayout->addWidget(swatchLabel, 0, Qt::AlignCenter);
        cLayout->addStretch(1);
        return card;
    };

    m_card1 = createColorCard("Color\n1", m_swatchPreview1);
    m_card2 = createColorCard("Color\n2", m_swatchPreview2);

    // Click on Card 1
    m_card1->installEventFilter(this);
    m_card2->installEventFilter(this);

    topLayout->addWidget(m_card1);
    topLayout->addWidget(m_card2);

    // 2. Swatches Palette Grid (3 rows x 10 cols)
    QWidget* swatchGridWidget = new QWidget(this);
    QGridLayout* sGrid = new QGridLayout(swatchGridWidget);
    sGrid->setContentsMargins(2, 2, 2, 2);
    sGrid->setSpacing(2);

    auto addSwatchRow = [this, sGrid](const QVector<QColor>& colors, int row, bool isCustom) {
        for (int col = 0; col < 10; ++col) {
            QToolButton* btn = new QToolButton(this);
            btn->setFixedSize(14, 14);
            QColor c = colors[col];
            btn->setStyleSheet(QString("QToolButton { background-color: %1; border: 1px solid #999999; border-radius: 1px; }"
                                       "QToolButton:hover { border: 1px solid #0078d7; }").arg(c.name()));

            connect(btn, &QToolButton::clicked, this, [this, btn]() {
                // Get color from style or attribute
                QColor c = btn->palette().button().color();
                // extract from stylesheet
                QString ss = btn->styleSheet();
                int idx = ss.indexOf("background-color: ");
                if (idx != -1) {
                    QString hex = ss.mid(idx + 18, 7);
                    c = QColor(hex);
                }

                if (m_editingColor1) {
                    setColor1(c);
                } else {
                    setColor2(c);
                }
            });

            if (isCustom) {
                m_customButtons.append(btn);
            }
            sGrid->addWidget(btn, row, col);
        }
    };

    addSwatchRow(m_row1, 0, false);
    addSwatchRow(m_row2, 1, false);
    addSwatchRow(m_row3, 2, true);

    topLayout->addWidget(swatchGridWidget);

    // 3. Edit Colors Button
    m_editColorsBtn = new QToolButton(this);
    m_editColorsBtn->setFixedSize(48, 66);
    m_editColorsBtn->setText("Edit\ncolors");
    m_editColorsBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // Custom color circle icon
    QPixmap wheelPix(24, 24);
    wheelPix.fill(Qt::transparent);
    QPainter wp(&wheelPix);
    wp.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < 360; i += 30) {
        wp.setPen(Qt::NoPen);
        wp.setBrush(QColor::fromHsv(i, 220, 240));
        wp.drawPie(QRectF(1, 1, 22, 22), i * 16, 30 * 16);
    }
    wp.setBrush(Qt::white);
    wp.drawEllipse(QRectF(6, 6, 12, 12));
    wp.end();

    m_editColorsBtn->setIcon(QIcon(wheelPix));
    m_editColorsBtn->setIconSize(QSize(24, 24));
    m_editColorsBtn->setStyleSheet(
        "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px; }"
        "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
        "QToolButton:pressed { background: #cce8ff; }"
    );
    connect(m_editColorsBtn, &QToolButton::clicked, this, &ColorPalette::onEditColorsClicked);
    topLayout->addWidget(m_editColorsBtn);

    rootLayout->addLayout(topLayout);

    // Group Label
    QLabel* groupLabel = new QLabel("Colors", this);
    groupLabel->setAlignment(Qt::AlignCenter);
    groupLabel->setFixedHeight(14);
    groupLabel->setStyleSheet("color: #646464; font-size: 11px;");
    rootLayout->addWidget(groupLabel);
}

void ColorPalette::selectColor1Mode() {
    m_editingColor1 = true;
    updatePreviews();
}

void ColorPalette::selectColor2Mode() {
    m_editingColor1 = false;
    updatePreviews();
}

bool ColorPalette::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (watched == m_card1) {
            selectColor1Mode();
            return true;
        } else if (watched == m_card2) {
            selectColor2Mode();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void ColorPalette::setColor1(const QColor& c) {
    m_color1 = c;
    updatePreviews();
    emit color1Changed(c);
}

void ColorPalette::setColor2(const QColor& c) {
    m_color2 = c;
    updatePreviews();
    emit color2Changed(c);
}

void ColorPalette::updatePreviews() {
    if (m_swatchPreview1) {
        m_swatchPreview1->setStyleSheet(QString("background-color: %1; border: 1px solid #707070;").arg(m_color1.name()));
    }
    if (m_swatchPreview2) {
        m_swatchPreview2->setStyleSheet(QString("background-color: %1; border: 1px solid #707070;").arg(m_color2.name()));
    }

    if (m_card1) {
        if (m_editingColor1) {
            m_card1->setStyleSheet("QWidget { background: #cce8ff; border: 1px solid #3399ff; border-radius: 2px; }");
        } else {
            m_card1->setStyleSheet("QWidget { background: transparent; border: 1px solid transparent; } QWidget:hover { background: #e5f1fb; }");
        }
    }
    if (m_card2) {
        if (!m_editingColor1) {
            m_card2->setStyleSheet("QWidget { background: #cce8ff; border: 1px solid #3399ff; border-radius: 2px; }");
        } else {
            m_card2->setStyleSheet("QWidget { background: transparent; border: 1px solid transparent; } QWidget:hover { background: #e5f1fb; }");
        }
    }
}

void ColorPalette::onEditColorsClicked() {
    QColor current = m_editingColor1 ? m_color1 : m_color2;
    QColor picked = QColorDialog::getColor(current, this, "Edit Colors", QColorDialog::ShowAlphaChannel);
    if (picked.isValid()) {
        if (m_editingColor1) {
            setColor1(picked);
        } else {
            setColor2(picked);
        }

        // Shift custom color into custom palette
        m_row3.pop_back();
        m_row3.prepend(picked);

        for (int i = 0; i < m_customButtons.size() && i < m_row3.size(); ++i) {
            m_customButtons[i]->setStyleSheet(QString("QToolButton { background-color: %1; border: 1px solid #999999; border-radius: 1px; }"
                                                      "QToolButton:hover { border: 1px solid #0078d7; }").arg(m_row3[i].name()));
        }
    }
}
