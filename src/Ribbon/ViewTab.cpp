#include "ViewTab.h"
#include "PaintIcons.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

static QFrame* createSeparator() {
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Plain);
    line->setFixedHeight(76);
    line->setStyleSheet("QFrame { background: #e5e5e5; border: none; max-width: 1px; min-width: 1px; margin: 4px 2px; }");
    return line;
}

ViewTab::ViewTab(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(100);
    setStyleSheet("background: #ffffff;");

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(6, 2, 6, 2);
    mainLayout->setSpacing(4);

    // 1. Zoom Group
    QWidget* zoomGroup = new QWidget(this);
    zoomGroup->setFixedSize(168, 92);
    QVBoxLayout* zLayout = new QVBoxLayout(zoomGroup);
    zLayout->setContentsMargins(2, 2, 2, 2);
    zLayout->setSpacing(2);

    QHBoxLayout* zBtns = new QHBoxLayout();
    zBtns->setContentsMargins(0, 0, 0, 0);
    zBtns->setSpacing(3);

    auto makeZoomBtn = [zoomGroup](const QIcon& icon, const QString& text, int w) {
        QToolButton* b = new QToolButton(zoomGroup);
        b->setText(text);
        b->setIcon(icon);
        b->setIconSize(QSize(28, 28));
        b->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        b->setFixedSize(w, 66);
        b->setStyleSheet(
            "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px; }"
            "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
            "QToolButton:pressed { background: #cce8ff; }"
        );
        return b;
    };

    QToolButton* btnZoomIn = makeZoomBtn(PaintIcons::zoomInIcon(28), "Zoom in", 52);
    connect(btnZoomIn, &QToolButton::clicked, this, &ViewTab::zoomInTriggered);

    QToolButton* btnZoomOut = makeZoomBtn(PaintIcons::zoomOutIcon(28), "Zoom out", 52);
    connect(btnZoomOut, &QToolButton::clicked, this, &ViewTab::zoomOutTriggered);

    QToolButton* btn100 = makeZoomBtn(PaintIcons::zoom100Icon(28), "100%", 52);
    connect(btn100, &QToolButton::clicked, this, &ViewTab::zoom100Triggered);

    zBtns->addWidget(btnZoomIn);
    zBtns->addWidget(btnZoomOut);
    zBtns->addWidget(btn100);

    QLabel* zLabel = new QLabel("Zoom", zoomGroup);
    zLabel->setAlignment(Qt::AlignCenter);
    zLabel->setFixedHeight(14);
    zLabel->setStyleSheet("color: #646464; font-size: 11px;");

    zLayout->addLayout(zBtns);
    zLayout->addWidget(zLabel);
    mainLayout->addWidget(zoomGroup);
    mainLayout->addWidget(createSeparator());

    // 2. Show or hide Group
    QWidget* showGroup = new QWidget(this);
    showGroup->setFixedSize(110, 92);
    QVBoxLayout* sLayout = new QVBoxLayout(showGroup);
    sLayout->setContentsMargins(4, 2, 4, 2);
    sLayout->setSpacing(2);

    QVBoxLayout* checks = new QVBoxLayout();
    checks->setContentsMargins(4, 4, 4, 0);
    checks->setSpacing(4);

    m_checkRulers = new QCheckBox("Rulers", showGroup);
    m_checkRulers->setStyleSheet("QCheckBox { font-size: 11px; color: #333333; }");
    connect(m_checkRulers, &QCheckBox::toggled, this, &ViewTab::rulersToggled);

    m_checkGrid = new QCheckBox("Gridlines", showGroup);
    m_checkGrid->setStyleSheet("QCheckBox { font-size: 11px; color: #333333; }");
    connect(m_checkGrid, &QCheckBox::toggled, this, &ViewTab::gridlinesToggled);

    m_checkStatus = new QCheckBox("Status bar", showGroup);
    m_checkStatus->setChecked(true);
    m_checkStatus->setStyleSheet("QCheckBox { font-size: 11px; color: #333333; }");
    connect(m_checkStatus, &QCheckBox::toggled, this, &ViewTab::statusBarToggled);

    checks->addWidget(m_checkRulers);
    checks->addWidget(m_checkGrid);
    checks->addWidget(m_checkStatus);

    QLabel* sLabel = new QLabel("Show or hide", showGroup);
    sLabel->setAlignment(Qt::AlignCenter);
    sLabel->setFixedHeight(14);
    sLabel->setStyleSheet("color: #646464; font-size: 11px;");

    sLayout->addLayout(checks);
    sLayout->addWidget(sLabel);
    mainLayout->addWidget(showGroup);
    mainLayout->addWidget(createSeparator());

    // 3. Display Group
    QWidget* dispGroup = new QWidget(this);
    dispGroup->setFixedSize(70, 92);
    QVBoxLayout* dLayout = new QVBoxLayout(dispGroup);
    dLayout->setContentsMargins(2, 2, 2, 2);
    dLayout->setSpacing(2);

    QToolButton* btnFull = new QToolButton(dispGroup);
    btnFull->setText("Full screen");
    btnFull->setIcon(PaintIcons::fullscreenIcon(28));
    btnFull->setIconSize(QSize(28, 28));
    btnFull->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnFull->setFixedSize(62, 66);
    btnFull->setStyleSheet(
        "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px; }"
        "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
        "QToolButton:pressed { background: #cce8ff; }"
    );
    connect(btnFull, &QToolButton::clicked, this, &ViewTab::fullScreenTriggered);

    QLabel* dLabel = new QLabel("Display", dispGroup);
    dLabel->setAlignment(Qt::AlignCenter);
    dLabel->setFixedHeight(14);
    dLabel->setStyleSheet("color: #646464; font-size: 11px;");

    dLayout->addWidget(btnFull, 0, Qt::AlignCenter);
    dLayout->addWidget(dLabel);
    mainLayout->addWidget(dispGroup);

    mainLayout->addStretch(1);
}
