#include "RibbonBar.h"
#include "PaintIcons.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QApplication>

RibbonBar::RibbonBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(132);
    setStyleSheet("background: #f5f6f7;");
    setupUI();
}

void RibbonBar::setupUI() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // 1. Header Bar (Top Row: Quick Access + File + Tabs)
    QWidget* headerBar = new QWidget(this);
    headerBar->setFixedHeight(32);
    headerBar->setStyleSheet("background-color: #f5f6f7;");

    QHBoxLayout* hLayout = new QHBoxLayout(headerBar);
    hLayout->setContentsMargins(6, 3, 6, 0);
    hLayout->setSpacing(3);

    // App Icon
    QToolButton* appBtn = new QToolButton(headerBar);
    appBtn->setIcon(PaintIcons::appIcon(20));
    appBtn->setIconSize(QSize(20, 20));
    appBtn->setFixedSize(24, 24);
    appBtn->setStyleSheet("background: transparent; border: none;");
    hLayout->addWidget(appBtn);

    // Quick Access Toolbar: Save, Undo, Redo
    auto makeQatBtn = [headerBar](const QIcon& icon, const QString& tip) {
        QToolButton* b = new QToolButton(headerBar);
        b->setIcon(icon);
        b->setIconSize(QSize(16, 16));
        b->setToolTip(tip);
        b->setFixedSize(24, 24);
        b->setStyleSheet(
            "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; }"
            "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
        );
        return b;
    };

    QToolButton* qatSave = makeQatBtn(PaintIcons::saveIcon(16), "Save (Ctrl+S)");
    connect(qatSave, &QToolButton::clicked, this, &RibbonBar::saveTriggered);
    hLayout->addWidget(qatSave);

    QToolButton* qatUndo = makeQatBtn(PaintIcons::undoIcon(16), "Undo (Ctrl+Z)");
    connect(qatUndo, &QToolButton::clicked, this, &RibbonBar::undoTriggered);
    hLayout->addWidget(qatUndo);

    QToolButton* qatRedo = makeQatBtn(PaintIcons::redoIcon(16), "Redo (Ctrl+Y)");
    connect(qatRedo, &QToolButton::clicked, this, &RibbonBar::redoTriggered);
    hLayout->addWidget(qatRedo);

    // Separator line
    QFrame* qatSep = new QFrame(headerBar);
    qatSep->setFrameShape(QFrame::VLine);
    qatSep->setFixedHeight(16);
    qatSep->setStyleSheet("color: #d0d0d0;");
    hLayout->addWidget(qatSep);

    // Blue Windows 10 "File" button
    m_fileButton = new QPushButton("File", headerBar);
    m_fileButton->setFixedSize(56, 26);
    m_fileButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #1979ca; color: white; border: none; font-size: 11px; font-weight: bold; border-top-left-radius: 2px; border-top-right-radius: 2px; margin-left: 2px;"
        "}"
        "QPushButton:hover { background-color: #1e88e5; }"
        "QPushButton:pressed { background-color: #0f6ab4; }"
    );
    setupFileMenu();
    hLayout->addWidget(m_fileButton);

    // Windows 10 Tabs (Home & View)
    m_homeTabBtn = new QPushButton("Home", headerBar);
    m_homeTabBtn->setFixedSize(62, 26);
    connect(m_homeTabBtn, &QPushButton::clicked, this, &RibbonBar::selectHomeTab);
    hLayout->addWidget(m_homeTabBtn);

    m_viewTabBtn = new QPushButton("View", headerBar);
    m_viewTabBtn->setFixedSize(62, 26);
    connect(m_viewTabBtn, &QPushButton::clicked, this, &RibbonBar::selectViewTab);
    hLayout->addWidget(m_viewTabBtn);

    hLayout->addStretch(1);
    rootLayout->addWidget(headerBar);

    // 2. Ribbon Content (StackedWidget: Home / View)
    m_stack = new QStackedWidget(this);
    m_stack->setFixedHeight(100);
    m_stack->setStyleSheet("QStackedWidget { background: #ffffff; border-top: 1px solid #dcdcdc; border-bottom: 1px solid #dcdcdc; }");

    m_homeTab = new HomeTab(this);
    m_viewTab = new ViewTab(this);

    m_stack->addWidget(m_homeTab);
    m_stack->addWidget(m_viewTab);

    rootLayout->addWidget(m_stack);

    updateTabStyles();
}

void RibbonBar::selectHomeTab() {
    m_isHomeActive = true;
    m_stack->setCurrentIndex(0);
    updateTabStyles();
}

void RibbonBar::selectViewTab() {
    m_isHomeActive = false;
    m_stack->setCurrentIndex(1);
    updateTabStyles();
}

void RibbonBar::updateTabStyles() {
    const QString activeStyle =
        "QPushButton {"
        "  background: #ffffff; color: #1979ca; font-size: 11px; font-weight: bold;"
        "  border: 1px solid #dcdcdc; border-bottom: 1px solid #ffffff; border-top: 2px solid #1979ca;"
        "  border-top-left-radius: 2px; border-top-right-radius: 2px;"
        "}";

    const QString inactiveStyle =
        "QPushButton {"
        "  background: transparent; color: #222222; font-size: 11px; font-weight: normal;"
        "  border: 1px solid transparent; border-top-left-radius: 2px; border-top-right-radius: 2px;"
        "}"
        "QPushButton:hover {"
        "  background: #e5f1fb; color: #000000;"
        "}";

    if (m_isHomeActive) {
        m_homeTabBtn->setStyleSheet(activeStyle);
        m_viewTabBtn->setStyleSheet(inactiveStyle);
    } else {
        m_homeTabBtn->setStyleSheet(inactiveStyle);
        m_viewTabBtn->setStyleSheet(activeStyle);
    }
}

void RibbonBar::setupFileMenu() {
    QMenu* menu = new QMenu(m_fileButton);
    menu->setStyleSheet(
        "QMenu {"
        "  background: white; border: 1px solid #c0c0c0; padding: 4px; font-size: 11px;"
        "}"
        "QMenu::item { padding: 6px 24px; color: #000000; }"
        "QMenu::item:selected { background: #e5f1fb; color: #000000; }"
        "QMenu::separator { height: 1px; background: #e0e0e0; margin: 4px 0px; }"
    );

    QAction* actNew = menu->addAction(PaintIcons::newIcon(16), "New\tCtrl+N");
    connect(actNew, &QAction::triggered, this, &RibbonBar::newTriggered);

    QAction* actOpen = menu->addAction(PaintIcons::openIcon(16), "Open\tCtrl+O");
    connect(actOpen, &QAction::triggered, this, &RibbonBar::openTriggered);

    QAction* actSave = menu->addAction(PaintIcons::saveIcon(16), "Save\tCtrl+S");
    connect(actSave, &QAction::triggered, this, &RibbonBar::saveTriggered);

    QMenu* saveAsMenu = menu->addMenu(PaintIcons::saveAsIcon(16), "Save as");
    saveAsMenu->setStyleSheet(menu->styleSheet());
    QAction* saPNG = saveAsMenu->addAction("PNG picture");
    connect(saPNG, &QAction::triggered, this, [this]() { emit saveAsTriggered("PNG"); });
    QAction* saJPG = saveAsMenu->addAction("JPEG picture");
    connect(saJPG, &QAction::triggered, this, [this]() { emit saveAsTriggered("JPG"); });
    QAction* saBMP = saveAsMenu->addAction("BMP picture");
    connect(saBMP, &QAction::triggered, this, [this]() { emit saveAsTriggered("BMP"); });
    QAction* saGIF = saveAsMenu->addAction("GIF picture");
    connect(saGIF, &QAction::triggered, this, [this]() { emit saveAsTriggered("GIF"); });

    menu->addSeparator();
    QAction* actProps = menu->addAction(PaintIcons::propertiesIcon(16), "Properties\tCtrl+E");
    connect(actProps, &QAction::triggered, this, &RibbonBar::propertiesTriggered);

    QAction* actAbout = menu->addAction(PaintIcons::aboutIcon(16), "About Paint");
    connect(actAbout, &QAction::triggered, this, &RibbonBar::aboutTriggered);

    menu->addSeparator();
    QAction* actExit = menu->addAction(PaintIcons::exitIcon(16), "Exit");
    connect(actExit, &QAction::triggered, this, &RibbonBar::exitTriggered);

    m_fileButton->setMenu(menu);
}
