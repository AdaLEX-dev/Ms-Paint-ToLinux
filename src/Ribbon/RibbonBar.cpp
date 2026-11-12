#include "RibbonBar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabBar>
#include <QApplication>

RibbonBar::RibbonBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(128);

    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Header bar with File button + Home/View tabs
    QWidget* headerBar = new QWidget(this);
    headerBar->setFixedHeight(32);
    headerBar->setStyleSheet("background-color: #f5f6f7;");

    QHBoxLayout* hLayout = new QHBoxLayout(headerBar);
    hLayout->setContentsMargins(4, 3, 4, 0);
    hLayout->setSpacing(0);

    // Blue Windows 10 "File" button
    m_fileButton = new QPushButton("File", headerBar);
    m_fileButton->setFixedSize(56, 25);
    m_fileButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #0078d7; color: white; border: none; font-size: 11px; font-weight: bold; border-top-left-radius: 2px; border-top-right-radius: 2px;"
        "}"
        "QPushButton:hover { background-color: #106ebe; }"
        "QPushButton:pressed { background-color: #005a9e; }"
    );
    setupFileMenu();
    hLayout->addWidget(m_fileButton);

    // Tab widget for Home and View
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane { border-top: 1px solid #dcdcdc; background: #fdfdfd; }"
        "QTabBar::tab {"
        "  background: #f5f6f7; border: 1px solid transparent; padding: 4px 16px; font-size: 11px; margin-left: 2px;"
        "}"
        "QTabBar::tab:hover { background: #e5f1fb; }"
        "QTabBar::tab:selected { background: #fdfdfd; border-color: #dcdcdc; border-bottom-color: #fdfdfd; }"
    );

    m_homeTab = new HomeTab(this);
    m_viewTab = new ViewTab(this);

    m_tabWidget->addTab(m_homeTab, "Home");
    m_tabWidget->addTab(m_viewTab, "View");

    rootLayout->addWidget(headerBar);
    rootLayout->addWidget(m_tabWidget);
}

void RibbonBar::setupFileMenu() {
    QMenu* menu = new QMenu(m_fileButton);
    menu->setStyleSheet(
        "QMenu {"
        "  background: white; border: 1px solid #c0c0c0; padding: 4px; font-size: 11px;"
        "}"
        "QMenu::item { padding: 6px 24px; }"
        "QMenu::item:selected { background: #e5f1fb; color: #000; }"
    );

    QAction* actNew = menu->addAction("New\tCtrl+N");
    connect(actNew, &QAction::triggered, this, &RibbonBar::newTriggered);

    QAction* actOpen = menu->addAction("Open\tCtrl+O");
    connect(actOpen, &QAction::triggered, this, &RibbonBar::openTriggered);

    QAction* actSave = menu->addAction("Save\tCtrl+S");
    connect(actSave, &QAction::triggered, this, &RibbonBar::saveTriggered);

    QMenu* saveAsMenu = menu->addMenu("Save as");
    QAction* saPNG = saveAsMenu->addAction("PNG picture");
    connect(saPNG, &QAction::triggered, this, [this]() { emit saveAsTriggered("PNG"); });
    QAction* saJPG = saveAsMenu->addAction("JPEG picture");
    connect(saJPG, &QAction::triggered, this, [this]() { emit saveAsTriggered("JPG"); });
    QAction* saBMP = saveAsMenu->addAction("BMP picture");
    connect(saBMP, &QAction::triggered, this, [this]() { emit saveAsTriggered("BMP"); });
    QAction* saGIF = saveAsMenu->addAction("GIF picture");
    connect(saGIF, &QAction::triggered, this, [this]() { emit saveAsTriggered("GIF"); });

    menu->addSeparator();
    QAction* actProps = menu->addAction("Properties");
    connect(actProps, &QAction::triggered, this, &RibbonBar::propertiesTriggered);

    QAction* actAbout = menu->addAction("About Paint");
    connect(actAbout, &QAction::triggered, this, &RibbonBar::aboutTriggered);

    menu->addSeparator();
    QAction* actExit = menu->addAction("Exit");
    connect(actExit, &QAction::triggered, this, &RibbonBar::exitTriggered);

    m_fileButton->setMenu(menu);
}
