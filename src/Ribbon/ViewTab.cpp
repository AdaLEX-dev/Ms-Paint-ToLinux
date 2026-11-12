#include "ViewTab.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

static QFrame* createSeparator() {
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Plain);
    line->setStyleSheet("color: #d8d8d8;");
    return line;
}

ViewTab::ViewTab(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(95);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(6, 4, 6, 4);
    mainLayout->setSpacing(6);

    // 1. Zoom Group
    QWidget* zoomGroup = new QWidget(this);
    QVBoxLayout* zLayout = new QVBoxLayout(zoomGroup);
    zLayout->setContentsMargins(0, 0, 0, 0);
    zLayout->setSpacing(2);

    QHBoxLayout* zBtns = new QHBoxLayout();
    zBtns->setSpacing(4);

    QToolButton* btnZoomIn = new QToolButton(zoomGroup);
    btnZoomIn->setText("Zoom in");
    btnZoomIn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnZoomIn->setFixedSize(50, 55);
    btnZoomIn->setStyleSheet("QToolButton { background: transparent; border: 1px solid transparent; font-size: 10px; }"
                             "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }");
    connect(btnZoomIn, &QToolButton::clicked, this, &ViewTab::zoomInTriggered);

    QToolButton* btnZoomOut = new QToolButton(zoomGroup);
    btnZoomOut->setText("Zoom out");
    btnZoomOut->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnZoomOut->setFixedSize(50, 55);
    btnZoomOut->setStyleSheet("QToolButton { background: transparent; border: 1px solid transparent; font-size: 10px; }"
                              "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }");
    connect(btnZoomOut, &QToolButton::clicked, this, &ViewTab::zoomOutTriggered);

    QToolButton* btn100 = new QToolButton(zoomGroup);
    btn100->setText("100%");
    btn100->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn100->setFixedSize(45, 55);
    btn100->setStyleSheet("QToolButton { background: transparent; border: 1px solid transparent; font-size: 10px; font-weight: bold; }"
                          "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }");
    connect(btn100, &QToolButton::clicked, this, &ViewTab::zoom100Triggered);

    zBtns->addWidget(btnZoomIn);
    zBtns->addWidget(btnZoomOut);
    zBtns->addWidget(btn100);

    QLabel* zLabel = new QLabel("Zoom", zoomGroup);
    zLabel->setAlignment(Qt::AlignCenter);
    zLabel->setStyleSheet("color: #646464; font-size: 11px;");

    zLayout->addLayout(zBtns);
    zLayout->addWidget(zLabel);
    mainLayout->addWidget(zoomGroup);
    mainLayout->addWidget(createSeparator());

    // 2. Show or hide Group
    QWidget* showGroup = new QWidget(this);
    QVBoxLayout* sLayout = new QVBoxLayout(showGroup);
    sLayout->setContentsMargins(0, 0, 0, 0);
    sLayout->setSpacing(2);

    QVBoxLayout* checks = new QVBoxLayout();
    checks->setSpacing(2);

    m_checkRulers = new QCheckBox("Rulers", showGroup);
    m_checkRulers->setStyleSheet("font-size: 11px;");
    connect(m_checkRulers, &QCheckBox::toggled, this, &ViewTab::rulersToggled);

    m_checkGrid = new QCheckBox("Gridlines", showGroup);
    m_checkGrid->setStyleSheet("font-size: 11px;");
    connect(m_checkGrid, &QCheckBox::toggled, this, &ViewTab::gridlinesToggled);

    m_checkStatus = new QCheckBox("Status bar", showGroup);
    m_checkStatus->setChecked(true);
    m_checkStatus->setStyleSheet("font-size: 11px;");
    connect(m_checkStatus, &QCheckBox::toggled, this, &ViewTab::statusBarToggled);

    checks->addWidget(m_checkRulers);
    checks->addWidget(m_checkGrid);
    checks->addWidget(m_checkStatus);

    QLabel* sLabel = new QLabel("Show or hide", showGroup);
    sLabel->setAlignment(Qt::AlignCenter);
    sLabel->setStyleSheet("color: #646464; font-size: 11px;");

    sLayout->addLayout(checks);
    sLayout->addWidget(sLabel);
    mainLayout->addWidget(showGroup);
    mainLayout->addWidget(createSeparator());

    // 3. Display Group
    QWidget* dispGroup = new QWidget(this);
    QVBoxLayout* dLayout = new QVBoxLayout(dispGroup);
    dLayout->setContentsMargins(0, 0, 0, 0);
    dLayout->setSpacing(2);

    QToolButton* btnFull = new QToolButton(dispGroup);
    btnFull->setText("Full screen");
    btnFull->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnFull->setFixedSize(60, 55);
    btnFull->setStyleSheet("QToolButton { background: transparent; border: 1px solid transparent; font-size: 10px; }"
                           "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }");
    connect(btnFull, &QToolButton::clicked, this, &ViewTab::fullScreenTriggered);

    QLabel* dLabel = new QLabel("Display", dispGroup);
    dLabel->setAlignment(Qt::AlignCenter);
    dLabel->setStyleSheet("color: #646464; font-size: 11px;");

    dLayout->addWidget(btnFull, 0, Qt::AlignCenter);
    dLayout->addWidget(dLabel);
    mainLayout->addWidget(dispGroup);

    mainLayout->addStretch(1);
}
