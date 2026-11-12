#include "HomeTab.h"
#include "PaintIcons.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QPainter>

static QFrame* createSeparator() {
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Plain);
    line->setStyleSheet("color: #d8d8d8;");
    return line;
}

HomeTab::HomeTab(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(95);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(6, 4, 6, 4);
    mainLayout->setSpacing(6);

    setupClipboardGroup(mainLayout);
    mainLayout->addWidget(createSeparator());

    setupImageGroup(mainLayout);
    mainLayout->addWidget(createSeparator());

    setupToolsGroup(mainLayout);
    mainLayout->addWidget(createSeparator());

    m_brushGallery = new BrushGallery(this);
    mainLayout->addWidget(m_brushGallery);
    mainLayout->addWidget(createSeparator());

    m_shapeGallery = new ShapeGallery(this);
    mainLayout->addWidget(m_shapeGallery);
    mainLayout->addWidget(createSeparator());

    m_sizeSelector = new SizeSelector(this);
    mainLayout->addWidget(m_sizeSelector);
    mainLayout->addWidget(createSeparator());

    m_colorPalette = new ColorPalette(this);
    mainLayout->addWidget(m_colorPalette);

    mainLayout->addStretch(1);
}

void HomeTab::setupClipboardGroup(QHBoxLayout* mainLayout) {
    QWidget* group = new QWidget(this);
    QVBoxLayout* gLayout = new QVBoxLayout(group);
    gLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->setSpacing(2);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(2);

    // Large Paste Button
    QToolButton* btnPaste = new QToolButton(group);
    btnPaste->setText("Paste");
    btnPaste->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnPaste->setFixedSize(48, 55);
    btnPaste->setIcon(PaintIcons::pasteIcon(28));
    btnPaste->setIconSize(QSize(24, 24));
    btnPaste->setStyleSheet("QToolButton { background: transparent; border: 1px solid transparent; font-size: 10px; }"
                            "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }");
    connect(btnPaste, &QToolButton::clicked, this, &HomeTab::pasteTriggered);

    QMenu* pasteMenu = new QMenu(btnPaste);
    QAction* actPaste = pasteMenu->addAction(PaintIcons::pasteIcon(16), "Paste");
    connect(actPaste, &QAction::triggered, this, &HomeTab::pasteTriggered);
    QAction* actPasteFrom = pasteMenu->addAction(PaintIcons::openIcon(16), "Paste from...");
    connect(actPasteFrom, &QAction::triggered, this, &HomeTab::pasteFromTriggered);
    btnPaste->setMenu(pasteMenu);
    btnPaste->setPopupMode(QToolButton::MenuButtonPopup);

    btnLayout->addWidget(btnPaste);

    QVBoxLayout* smallBtns = new QVBoxLayout();
    smallBtns->setSpacing(2);

    QToolButton* btnCut = new QToolButton(group);
    btnCut->setText("Cut");
    btnCut->setIcon(PaintIcons::cutIcon(16));
    btnCut->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnCut->setFixedHeight(24);
    btnCut->setStyleSheet("QToolButton { background: transparent; border: 1px solid transparent; font-size: 10px; }"
                          "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }");
    connect(btnCut, &QToolButton::clicked, this, &HomeTab::cutTriggered);

    QToolButton* btnCopy = new QToolButton(group);
    btnCopy->setText("Copy");
    btnCopy->setIcon(PaintIcons::copyIcon(16));
    btnCopy->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnCopy->setFixedHeight(24);
    btnCopy->setStyleSheet("QToolButton { background: transparent; border: 1px solid transparent; font-size: 10px; }"
                           "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }");
    connect(btnCopy, &QToolButton::clicked, this, &HomeTab::copyTriggered);

    smallBtns->addWidget(btnCut);
    smallBtns->addWidget(btnCopy);
    btnLayout->addLayout(smallBtns);

    QLabel* label = new QLabel("Clipboard", group);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: #646464; font-size: 11px;");

    gLayout->addLayout(btnLayout);
    gLayout->addWidget(label);
    mainLayout->addWidget(group);
}

void HomeTab::setupImageGroup(QHBoxLayout* mainLayout) {
    QWidget* group = new QWidget(this);
    QVBoxLayout* gLayout = new QVBoxLayout(group);
    gLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->setSpacing(2);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(2);

    // Select Button
    m_btnSelect = new QToolButton(group);
    m_btnSelect->setText("Select");
    m_btnSelect->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_btnSelect->setFixedSize(48, 55);
    m_btnSelect->setIcon(PaintIcons::selectRectIcon(28));
    m_btnSelect->setIconSize(QSize(22, 22));
    m_btnSelect->setStyleSheet("QToolButton { background: transparent; border: 1px solid transparent; font-size: 10px; }"
                               "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }"
                               "QToolButton:checked { background: #cce4f7; border-color: #0078d7; }");

    QMenu* selMenu = new QMenu(m_btnSelect);
    QAction* actRect = selMenu->addAction(PaintIcons::selectRectIcon(16), "Rectangular selection");
    connect(actRect, &QAction::triggered, this, [this]() { emit toolSelected(ActiveTool::SelectRect); });
    QAction* actFree = selMenu->addAction(PaintIcons::selectFreeformIcon(16), "Free-form selection");
    connect(actFree, &QAction::triggered, this, [this]() { emit toolSelected(ActiveTool::SelectFreeform); });
    selMenu->addSeparator();
    QAction* actAll = selMenu->addAction(PaintIcons::copyIcon(16), "Select all (Ctrl+A)");
    connect(actAll, &QAction::triggered, this, &HomeTab::selectAllTriggered);
    QAction* actDel = selMenu->addAction(PaintIcons::exitIcon(16), "Delete (Del)");
    connect(actDel, &QAction::triggered, this, &HomeTab::deleteTriggered);
    selMenu->addSeparator();
    QAction* actTrans = selMenu->addAction("Transparent selection");
    actTrans->setCheckable(true);
    connect(actTrans, &QAction::toggled, this, &HomeTab::transparentSelectionToggled);

    m_btnSelect->setMenu(selMenu);
    m_btnSelect->setPopupMode(QToolButton::MenuButtonPopup);
    connect(m_btnSelect, &QToolButton::clicked, this, [this]() { emit toolSelected(ActiveTool::SelectRect); });

    btnLayout->addWidget(m_btnSelect);

    QVBoxLayout* smallBtns = new QVBoxLayout();
    smallBtns->setSpacing(2);

    QToolButton* btnCrop = new QToolButton(group);
    btnCrop->setText("Crop");
    btnCrop->setIcon(PaintIcons::cropIcon(16));
    btnCrop->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnCrop->setFixedHeight(18);
    btnCrop->setStyleSheet("font-size: 10px; background: transparent; border: none;");
    connect(btnCrop, &QToolButton::clicked, this, &HomeTab::cropTriggered);

    QToolButton* btnResize = new QToolButton(group);
    btnResize->setText("Resize");
    btnResize->setIcon(PaintIcons::resizeIcon(16));
    btnResize->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnResize->setFixedHeight(18);
    btnResize->setStyleSheet("font-size: 10px; background: transparent; border: none;");
    connect(btnResize, &QToolButton::clicked, this, &HomeTab::resizeAndSkewTriggered);

    QToolButton* btnRotate = new QToolButton(group);
    btnRotate->setText("Rotate");
    btnRotate->setIcon(PaintIcons::rotateIcon(16));
    btnRotate->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnRotate->setFixedHeight(18);
    btnRotate->setPopupMode(QToolButton::InstantPopup);
    btnRotate->setStyleSheet("font-size: 10px; background: transparent; border: none;");

    QMenu* rotMenu = new QMenu(btnRotate);
    QAction* r90cw = rotMenu->addAction("Rotate right 90°");
    connect(r90cw, &QAction::triggered, this, &HomeTab::rotate90CWTriggered);
    QAction* r90ccw = rotMenu->addAction("Rotate left 90°");
    connect(r90ccw, &QAction::triggered, this, &HomeTab::rotate90CCWTriggered);
    QAction* r180 = rotMenu->addAction("Rotate 180°");
    connect(r180, &QAction::triggered, this, &HomeTab::rotate180Triggered);
    rotMenu->addSeparator();
    QAction* flipV = rotMenu->addAction("Flip vertical");
    connect(flipV, &QAction::triggered, this, &HomeTab::flipVTriggered);
    QAction* flipH = rotMenu->addAction("Flip horizontal");
    connect(flipH, &QAction::triggered, this, &HomeTab::flipHTriggered);
    btnRotate->setMenu(rotMenu);

    smallBtns->addWidget(btnCrop);
    smallBtns->addWidget(btnResize);
    smallBtns->addWidget(btnRotate);
    btnLayout->addLayout(smallBtns);

    QLabel* label = new QLabel("Image", group);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: #646464; font-size: 11px;");

    gLayout->addLayout(btnLayout);
    gLayout->addWidget(label);
    mainLayout->addWidget(group);
}

void HomeTab::setupToolsGroup(QHBoxLayout* mainLayout) {
    QWidget* group = new QWidget(this);
    QVBoxLayout* gLayout = new QVBoxLayout(group);
    gLayout->setContentsMargins(0, 0, 0, 0);
    gLayout->setSpacing(2);

    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(2);
    grid->setContentsMargins(0, 0, 0, 0);

    auto makeToolBtn = [group](const QIcon& icon, const QString& tip) {
        QToolButton* b = new QToolButton(group);
        b->setIcon(icon);
        b->setIconSize(QSize(18, 18));
        b->setToolTip(tip);
        b->setCheckable(true);
        b->setFixedSize(26, 26);
        b->setStyleSheet(
            "QToolButton {"
            "  background: transparent; border: 1px solid transparent; border-radius: 2px;"
            "}"
            "QToolButton:hover { background: #e5f1fb; border-color: #0078d7; }"
            "QToolButton:checked { background: #cce4f7; border-color: #0078d7; }"
        );
        return b;
    };

    m_btnPencil = makeToolBtn(PaintIcons::pencilIcon(18), "Pencil");
    connect(m_btnPencil, &QToolButton::clicked, this, [this]() { emit toolSelected(ActiveTool::Pencil); });

    m_btnFill = makeToolBtn(PaintIcons::fillIcon(18), "Fill with color");
    connect(m_btnFill, &QToolButton::clicked, this, [this]() { emit toolSelected(ActiveTool::Fill); });

    m_btnText = makeToolBtn(PaintIcons::textIcon(18), "Text");
    connect(m_btnText, &QToolButton::clicked, this, [this]() { emit toolSelected(ActiveTool::Text); });

    m_btnEraser = makeToolBtn(PaintIcons::eraserIcon(18), "Eraser");
    connect(m_btnEraser, &QToolButton::clicked, this, [this]() { emit toolSelected(ActiveTool::Eraser); });

    m_btnPicker = makeToolBtn(PaintIcons::colorPickerIcon(18), "Color picker");
    connect(m_btnPicker, &QToolButton::clicked, this, [this]() { emit toolSelected(ActiveTool::ColorPicker); });

    m_btnZoom = makeToolBtn(PaintIcons::magnifierIcon(18), "Magnifier");
    connect(m_btnZoom, &QToolButton::clicked, this, [this]() { emit toolSelected(ActiveTool::Magnifier); });

    grid->addWidget(m_btnPencil, 0, 0);
    grid->addWidget(m_btnFill,   0, 1);
    grid->addWidget(m_btnText,   0, 2);
    grid->addWidget(m_btnEraser, 1, 0);
    grid->addWidget(m_btnPicker, 1, 1);
    grid->addWidget(m_btnZoom,   1, 2);

    QLabel* label = new QLabel("Tools", group);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: #646464; font-size: 11px;");

    gLayout->addLayout(grid);
    gLayout->addWidget(label);
    mainLayout->addWidget(group);
}

void HomeTab::setActiveTool(ActiveTool tool) {
    m_btnPencil->setChecked(tool == ActiveTool::Pencil);
    m_btnFill->setChecked(tool == ActiveTool::Fill);
    m_btnText->setChecked(tool == ActiveTool::Text);
    m_btnEraser->setChecked(tool == ActiveTool::Eraser);
    m_btnPicker->setChecked(tool == ActiveTool::ColorPicker);
    m_btnZoom->setChecked(tool == ActiveTool::Magnifier);
    if (m_btnSelect) m_btnSelect->setChecked(tool == ActiveTool::SelectRect || tool == ActiveTool::SelectFreeform);
}
