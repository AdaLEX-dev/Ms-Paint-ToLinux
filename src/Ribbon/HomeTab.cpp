#include "HomeTab.h"
#include "PaintIcons.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
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

HomeTab::HomeTab(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(100);
    setStyleSheet("background: #ffffff;");

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(6, 2, 6, 2);
    mainLayout->setSpacing(4);

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
    group->setFixedSize(112, 92);
    QVBoxLayout* gLayout = new QVBoxLayout(group);
    gLayout->setContentsMargins(2, 2, 2, 2);
    gLayout->setSpacing(2);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(3);

    // Large Paste Button
    QToolButton* btnPaste = new QToolButton(group);
    btnPaste->setText("Paste");
    btnPaste->setIcon(PaintIcons::pasteIcon(28));
    btnPaste->setIconSize(QSize(28, 28));
    btnPaste->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnPaste->setFixedSize(48, 66);
    btnPaste->setStyleSheet(
        "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px; }"
        "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
        "QToolButton:pressed { background: #cce8ff; }"
    );
    connect(btnPaste, &QToolButton::clicked, this, &HomeTab::pasteTriggered);

    QMenu* pasteMenu = new QMenu(btnPaste);
    pasteMenu->setStyleSheet("QMenu { background: white; border: 1px solid #c0c0c0; font-size: 11px; } QMenu::item:selected { background: #e5f1fb; }");
    QAction* actPaste = pasteMenu->addAction(PaintIcons::pasteIcon(16), "Paste");
    connect(actPaste, &QAction::triggered, this, &HomeTab::pasteTriggered);
    QAction* actPasteFrom = pasteMenu->addAction(PaintIcons::openIcon(16), "Paste from...");
    connect(actPasteFrom, &QAction::triggered, this, &HomeTab::pasteFromTriggered);
    btnPaste->setMenu(pasteMenu);
    btnPaste->setPopupMode(QToolButton::MenuButtonPopup);

    btnLayout->addWidget(btnPaste);

    QVBoxLayout* smallBtns = new QVBoxLayout();
    smallBtns->setContentsMargins(0, 0, 0, 0);
    smallBtns->setSpacing(2);

    auto makeSmallBtn = [group](const QIcon& icon, const QString& text) {
        QToolButton* b = new QToolButton(group);
        b->setText(text);
        b->setIcon(icon);
        b->setIconSize(QSize(16, 16));
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        b->setFixedSize(56, 20);
        b->setStyleSheet(
            "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px; text-align: left; padding-left: 2px; }"
            "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
            "QToolButton:pressed { background: #cce8ff; }"
        );
        return b;
    };

    QToolButton* btnCut = makeSmallBtn(PaintIcons::cutIcon(16), "Cut");
    connect(btnCut, &QToolButton::clicked, this, &HomeTab::cutTriggered);

    QToolButton* btnCopy = makeSmallBtn(PaintIcons::copyIcon(16), "Copy");
    connect(btnCopy, &QToolButton::clicked, this, &HomeTab::copyTriggered);

    smallBtns->addWidget(btnCut);
    smallBtns->addWidget(btnCopy);
    smallBtns->addStretch(1);
    btnLayout->addLayout(smallBtns);

    QLabel* label = new QLabel("Clipboard", group);
    label->setAlignment(Qt::AlignCenter);
    label->setFixedHeight(14);
    label->setStyleSheet("color: #646464; font-size: 11px;");

    gLayout->addLayout(btnLayout);
    gLayout->addWidget(label);
    mainLayout->addWidget(group);
}

void HomeTab::setupImageGroup(QHBoxLayout* mainLayout) {
    QWidget* group = new QWidget(this);
    group->setFixedSize(128, 92);
    QVBoxLayout* gLayout = new QVBoxLayout(group);
    gLayout->setContentsMargins(2, 2, 2, 2);
    gLayout->setSpacing(2);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(3);

    // Select Button
    m_btnSelect = new QToolButton(group);
    m_btnSelect->setText("Select");
    m_btnSelect->setIcon(PaintIcons::selectRectIcon(28));
    m_btnSelect->setIconSize(QSize(26, 26));
    m_btnSelect->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_btnSelect->setFixedSize(48, 66);
    m_btnSelect->setStyleSheet(
        "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px; }"
        "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
        "QToolButton:checked { background: #cce8ff; border-color: #3399ff; }"
    );

    QMenu* selMenu = new QMenu(m_btnSelect);
    selMenu->setStyleSheet("QMenu { background: white; border: 1px solid #c0c0c0; font-size: 11px; } QMenu::item:selected { background: #e5f1fb; }");
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
    smallBtns->setContentsMargins(0, 0, 0, 0);
    smallBtns->setSpacing(2);

    auto makeImageBtn = [group](const QIcon& icon, const QString& text) {
        QToolButton* b = new QToolButton(group);
        b->setText(text);
        b->setIcon(icon);
        b->setIconSize(QSize(16, 16));
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        b->setFixedSize(70, 20);
        b->setStyleSheet(
            "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; font-size: 10px; text-align: left; padding-left: 2px; }"
            "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
        );
        return b;
    };

    QToolButton* btnCrop = makeImageBtn(PaintIcons::cropIcon(16), "Crop");
    connect(btnCrop, &QToolButton::clicked, this, &HomeTab::cropTriggered);

    QToolButton* btnResize = makeImageBtn(PaintIcons::resizeIcon(16), "Resize");
    connect(btnResize, &QToolButton::clicked, this, &HomeTab::resizeAndSkewTriggered);

    QToolButton* btnRotate = makeImageBtn(PaintIcons::rotateIcon(16), "Rotate");
    btnRotate->setPopupMode(QToolButton::InstantPopup);
    QMenu* rotMenu = new QMenu(btnRotate);
    rotMenu->setStyleSheet("QMenu { background: white; border: 1px solid #c0c0c0; font-size: 11px; } QMenu::item:selected { background: #e5f1fb; }");
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
    label->setFixedHeight(14);
    label->setStyleSheet("color: #646464; font-size: 11px;");

    gLayout->addLayout(btnLayout);
    gLayout->addWidget(label);
    mainLayout->addWidget(group);
}

void HomeTab::setupToolsGroup(QHBoxLayout* mainLayout) {
    QWidget* group = new QWidget(this);
    group->setFixedSize(92, 92);
    QVBoxLayout* gLayout = new QVBoxLayout(group);
    gLayout->setContentsMargins(2, 2, 2, 2);
    gLayout->setSpacing(2);

    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(2);
    grid->setContentsMargins(4, 4, 4, 4);

    auto makeToolBtn = [group](const QIcon& icon, const QString& tip) {
        QToolButton* b = new QToolButton(group);
        b->setIcon(icon);
        b->setIconSize(QSize(18, 18));
        b->setToolTip(tip);
        b->setCheckable(true);
        b->setFixedSize(26, 26);
        b->setStyleSheet(
            "QToolButton { background: transparent; border: 1px solid transparent; border-radius: 2px; }"
            "QToolButton:hover { background: #e5f1fb; border-color: #70c0e7; }"
            "QToolButton:checked { background: #cce8ff; border-color: #3399ff; }"
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
    label->setFixedHeight(14);
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
