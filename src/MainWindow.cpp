#include "MainWindow.h"
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QShortcut>
#include <QKeySequence>
#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileInfo>
#include "Dialogs/ResizeSkewDialog.h"
#include "Dialogs/ImagePropertiesDialog.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(1200, 800);
    setupUI();
    setupShortcuts();
    updateTitle();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* rootLayout = new QVBoxLayout(centralWidget);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // 1. Ribbon Bar
    m_ribbonBar = new RibbonBar(this);
    rootLayout->addWidget(m_ribbonBar);

    // 2. Canvas & Scroll Area
    m_canvas = new PaintCanvas(this);
    m_scrollArea = new CanvasScrollArea(m_canvas, this);
    rootLayout->addWidget(m_scrollArea);

    // Connect Ribbon Home Tab to Canvas
    HomeTab* home = m_ribbonBar->homeTab();
    connect(home, &HomeTab::toolSelected, m_canvas, &PaintCanvas::setActiveTool);
    connect(home, &HomeTab::pasteTriggered, this, [this]() {
        QImage clip = QApplication::clipboard()->image();
        if (!clip.isNull()) m_canvas->paste(clip);
    });
    connect(home, &HomeTab::pasteFromTriggered, this, [this]() {
        QString f = QFileDialog::getOpenFileName(this, "Paste from", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif *.webp);;All Files (*)");
        if (!f.isEmpty()) {
            QImage img(f);
            if (!img.isNull()) m_canvas->paste(img);
        }
    });
    connect(home, &HomeTab::cutTriggered, m_canvas, &PaintCanvas::cut);
    connect(home, &HomeTab::copyTriggered, m_canvas, &PaintCanvas::copy);
    connect(home, &HomeTab::selectAllTriggered, m_canvas, &PaintCanvas::selectAll);
    connect(home, &HomeTab::deleteTriggered, m_canvas, &PaintCanvas::deleteSelection);
    connect(home, &HomeTab::transparentSelectionToggled, m_canvas, &PaintCanvas::setTransparentSelection);
    connect(home, &HomeTab::cropTriggered, m_canvas, &PaintCanvas::cropToSelection);
    connect(home, &HomeTab::resizeAndSkewTriggered, this, &MainWindow::onResizeAndSkew);
    connect(home, &HomeTab::rotate90CWTriggered, m_canvas, &PaintCanvas::rotate90CW);
    connect(home, &HomeTab::rotate90CCWTriggered, m_canvas, &PaintCanvas::rotate90CCW);
    connect(home, &HomeTab::rotate180Triggered, m_canvas, &PaintCanvas::rotate180);
    connect(home, &HomeTab::flipHTriggered, m_canvas, &PaintCanvas::flipHorizontal);
    connect(home, &HomeTab::flipVTriggered, m_canvas, &PaintCanvas::flipVertical);

    // Brushes
    connect(home->brushGallery(), &BrushGallery::brushChanged, this, [this, home](BrushType b) {
        m_canvas->setActiveTool(ActiveTool::Brush);
        m_canvas->setBrushType(b);
        home->setActiveTool(ActiveTool::Brush);
    });

    // Shapes
    connect(home->shapeGallery(), &ShapeGallery::shapeChanged, this, [this, home](ShapeType s) {
        m_canvas->setActiveTool(ActiveTool::Shape);
        m_canvas->setShapeType(s);
        home->setActiveTool(ActiveTool::Shape);
    });
    connect(home->shapeGallery(), &ShapeGallery::outlineChanged, m_canvas, &PaintCanvas::setShapeOutlineStyle);
    connect(home->shapeGallery(), &ShapeGallery::fillChanged, m_canvas, &PaintCanvas::setShapeFillStyle);

    // Sizes
    connect(home->sizeSelector(), &SizeSelector::sizeChanged, m_canvas, &PaintCanvas::setLineWidth);

    // Colors
    connect(home->colorPalette(), &ColorPalette::color1Changed, m_canvas, &PaintCanvas::setColor1);
    connect(home->colorPalette(), &ColorPalette::color2Changed, m_canvas, &PaintCanvas::setColor2);
    connect(m_canvas, &PaintCanvas::color1Picked, home->colorPalette(), &ColorPalette::setColor1);
    connect(m_canvas, &PaintCanvas::color2Picked, home->colorPalette(), &ColorPalette::setColor2);

    // View Tab
    ViewTab* view = m_ribbonBar->viewTab();
    connect(view, &ViewTab::zoomInTriggered, this, [this]() { m_canvas->setZoom(m_canvas->zoom() * 1.25); });
    connect(view, &ViewTab::zoomOutTriggered, this, [this]() { m_canvas->setZoom(m_canvas->zoom() / 1.25); });
    connect(view, &ViewTab::zoom100Triggered, this, [this]() { m_canvas->setZoom(1.0); });
    connect(view, &ViewTab::gridlinesToggled, m_canvas, &PaintCanvas::setShowGrid);
    connect(view, &ViewTab::rulersToggled, m_scrollArea, &CanvasScrollArea::setShowRulers);
    connect(view, &ViewTab::statusBarToggled, this, [this](bool show) { statusBar()->setVisible(show); });
    connect(view, &ViewTab::fullScreenTriggered, this, [this]() {
        if (isFullScreen()) showNormal();
        else showFullScreen();
    });

    // File Menu Connections
    connect(m_ribbonBar, &RibbonBar::newTriggered, this, &MainWindow::onNew);
    connect(m_ribbonBar, &RibbonBar::openTriggered, this, &MainWindow::onOpen);
    connect(m_ribbonBar, &RibbonBar::saveTriggered, this, &MainWindow::onSave);
    connect(m_ribbonBar, &RibbonBar::saveAsTriggered, this, &MainWindow::onSaveAs);
    connect(m_ribbonBar, &RibbonBar::undoTriggered, m_canvas, &PaintCanvas::undo);
    connect(m_ribbonBar, &RibbonBar::redoTriggered, m_canvas, &PaintCanvas::redo);
    connect(m_ribbonBar, &RibbonBar::propertiesTriggered, this, &MainWindow::onProperties);
    connect(m_ribbonBar, &RibbonBar::aboutTriggered, this, &MainWindow::onAbout);
    connect(m_ribbonBar, &RibbonBar::exitTriggered, this, &MainWindow::close);

    // 3. Status Bar
    QStatusBar* bar = statusBar();
    bar->setStyleSheet(
        "QStatusBar { background: #f0f0f0; border-top: 1px solid #d8d8d8; font-size: 11px; color: #222222; min-height: 24px; }"
        "QStatusBar QLabel { color: #222222; font-size: 11px; padding: 0px 4px; }"
        "QSlider::groove:horizontal { height: 4px; background: #c0c0c0; border-radius: 2px; }"
        "QSlider::handle:horizontal { background: #0078d7; width: 10px; margin-top: -3px; margin-bottom: -3px; border-radius: 5px; }"
    );

    m_statusCoords = new QLabel("0, 0 px", this);
    m_statusCoords->setFixedWidth(110);
    bar->addWidget(m_statusCoords);

    m_statusSelection = new QLabel("", this);
    m_statusSelection->setFixedWidth(120);
    bar->addWidget(m_statusSelection);

    m_statusSize = new QLabel(QString("%1 x %2 px").arg(m_canvas->imageSize().width()).arg(m_canvas->imageSize().height()), this);
    m_statusSize->setFixedWidth(130);
    bar->addWidget(m_statusSize);

    // Zoom Controls in Status Bar
    QWidget* zoomWidget = new QWidget(this);
    QHBoxLayout* zwLayout = new QHBoxLayout(zoomWidget);
    zwLayout->setContentsMargins(0, 0, 8, 0);
    zwLayout->setSpacing(4);

    m_zoomOutBtn = new QToolButton(zoomWidget);
    m_zoomOutBtn->setText("-");
    m_zoomOutBtn->setFixedSize(16, 16);
    m_zoomOutBtn->setStyleSheet("background: transparent; border: none; font-weight: bold;");
    connect(m_zoomOutBtn, &QToolButton::clicked, this, [this]() { m_canvas->setZoom(m_canvas->zoom() * 0.8); });

    m_zoomSlider = new QSlider(Qt::Horizontal, zoomWidget);
    m_zoomSlider->setRange(12, 800);
    m_zoomSlider->setValue(100);
    m_zoomSlider->setFixedWidth(100);
    connect(m_zoomSlider, &QSlider::valueChanged, this, [this](int val) {
        m_canvas->setZoom(val / 100.0);
    });

    m_zoomInBtn = new QToolButton(zoomWidget);
    m_zoomInBtn->setText("+");
    m_zoomInBtn->setFixedSize(16, 16);
    m_zoomInBtn->setStyleSheet("background: transparent; border: none; font-weight: bold;");
    connect(m_zoomInBtn, &QToolButton::clicked, this, [this]() { m_canvas->setZoom(m_canvas->zoom() * 1.25); });

    m_zoomLabel = new QLabel("100%", zoomWidget);
    m_zoomLabel->setFixedWidth(40);
    m_zoomLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    zwLayout->addWidget(m_zoomOutBtn);
    zwLayout->addWidget(m_zoomSlider);
    zwLayout->addWidget(m_zoomInBtn);
    zwLayout->addWidget(m_zoomLabel);

    bar->addPermanentWidget(zoomWidget);

    // Connect Canvas signals to Status Bar
    connect(m_canvas, &PaintCanvas::mousePosChanged, this, [this](QPoint pos) {
        m_statusCoords->setText(QString("%1, %2 px").arg(pos.x()).arg(pos.y()));
    });
    connect(m_canvas, &PaintCanvas::selectionSizeChanged, this, [this](QSize sz) {
        if (sz.isEmpty()) m_statusSelection->setText("");
        else m_statusSelection->setText(QString("%1 x %2 px").arg(sz.width()).arg(sz.height()));
    });
    connect(m_canvas, &PaintCanvas::canvasSizeChanged, this, [this](QSize sz) {
        m_statusSize->setText(QString("%1 x %2 px").arg(sz.width()).arg(sz.height()));
        updateTitle();
    });
    connect(m_canvas, &PaintCanvas::zoomChanged, this, [this](double zoom) {
        int pct = int(zoom * 100);
        m_zoomLabel->setText(QString("%1%").arg(pct));
        m_zoomSlider->blockSignals(true);
        m_zoomSlider->setValue(pct);
        m_zoomSlider->blockSignals(false);
    });
    connect(m_canvas, &PaintCanvas::undoStateChanged, this, [this](bool canUndo, bool canRedo) {
        updateTitle();
    });
}

void MainWindow::setupShortcuts() {
    new QShortcut(QKeySequence::New, this, SLOT(onNew()));
    new QShortcut(QKeySequence::Open, this, SLOT(onOpen()));
    new QShortcut(QKeySequence::Save, this, SLOT(onSave()));
    new QShortcut(QKeySequence::Undo, m_canvas, SLOT(undo()));
    new QShortcut(QKeySequence::Redo, m_canvas, SLOT(redo()));
    new QShortcut(QKeySequence::SelectAll, m_canvas, SLOT(selectAll()));
    new QShortcut(QKeySequence::Cut, m_canvas, SLOT(cut()));
    new QShortcut(QKeySequence::Copy, m_canvas, SLOT(copy()));
    new QShortcut(QKeySequence::Paste, this, [this]() {
        QImage clip = QApplication::clipboard()->image();
        if (!clip.isNull()) m_canvas->paste(clip);
    });
    new QShortcut(QKeySequence(Qt::Key_Delete), m_canvas, SLOT(deleteSelection()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_W), this, SLOT(onResizeAndSkew()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_E), this, SLOT(onProperties()));
    new QShortcut(QKeySequence(Qt::Key_F11), this, [this]() {
        if (isFullScreen()) showNormal();
        else showFullScreen();
    });
}

void MainWindow::updateTitle() {
    QString title = m_currentFilePath.isEmpty() ? "Untitled" : QFileInfo(m_currentFilePath).fileName();
    if (m_canvas->isModified()) title += "*";
    title += " - Paint";
    setWindowTitle(title);
}

bool MainWindow::maybeSave() {
    if (!m_canvas->isModified()) return true;

    QMessageBox::StandardButton res = QMessageBox::warning(
        this, "Paint",
        QString("Do you want to save changes to %1?").arg(m_currentFilePath.isEmpty() ? "Untitled" : QFileInfo(m_currentFilePath).fileName()),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );

    if (res == QMessageBox::Save) {
        onSave();
        return !m_canvas->isModified();
    } else if (res == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

void MainWindow::onNew() {
    if (maybeSave()) {
        m_canvas->newImage(1152, 648, Qt::white);
        m_currentFilePath.clear();
        updateTitle();
    }
}

void MainWindow::onOpen() {
    if (maybeSave()) {
        QString file = QFileDialog::getOpenFileName(this, "Open", "", "All Picture Files (*.png *.jpg *.jpeg *.bmp *.gif *.webp *.tif *.tiff);;PNG (*.png);;JPEG (*.jpg *.jpeg);;Bitmap (*.bmp);;All Files (*)");
        if (!file.isEmpty()) {
            loadFile(file);
        }
    }
}

bool MainWindow::loadFile(const QString& filePath) {
    if (m_canvas->openImage(filePath)) {
        m_currentFilePath = filePath;
        updateTitle();
        return true;
    }
    QMessageBox::critical(this, "Error", "Paint cannot read this file.\nThis is not a valid bitmap file, or its format is not currently supported.");
    return false;
}

void MainWindow::onSave() {
    if (m_currentFilePath.isEmpty()) {
        onSaveAs("PNG");
    } else {
        m_canvas->saveImage(m_currentFilePath);
        updateTitle();
    }
}

void MainWindow::onSaveAs(const QString& format) {
    QString filter = QString("%1 (*.%2)").arg(format).arg(format.toLower());
    QString defaultName = m_currentFilePath.isEmpty() ? "Untitled." + format.toLower() : m_currentFilePath;
    QString file = QFileDialog::getSaveFileName(this, "Save As", defaultName, filter + ";;All Files (*)");
    if (!file.isEmpty()) {
        if (m_canvas->saveImage(file, format.toLatin1().constData())) {
            m_currentFilePath = file;
            updateTitle();
        }
    }
}

void MainWindow::onResizeAndSkew() {
    QSize sz = m_canvas->imageSize();
    ResizeSkewDialog dlg(sz.width(), sz.height(), this);
    if (dlg.exec() == QDialog::Accepted) {
        m_canvas->resizeAndSkew(dlg.scaleX(), dlg.scaleY(), dlg.skewXDeg(), dlg.skewYDeg());
    }
}

void MainWindow::onProperties() {
    QSize sz = m_canvas->imageSize();
    ImagePropertiesDialog dlg(sz.width(), sz.height(), this);
    if (dlg.exec() == QDialog::Accepted) {
        m_canvas->resizeCanvas(dlg.canvasWidth(), dlg.canvasHeight());
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(
        this, "About Paint",
        "<h2>Paint (Windows 10 Edition)</h2>"
        "<p>Native Arch Linux Wayland Port</p>"
        "<p>Full authentic Windows 10 Ribbon interface, 23 shapes, 9 artistic brushes, 30-color palette, in-place text, selection manipulation, and zoom engine.</p>"
        "<p>Built with modern C++ &amp; Qt6 for high-performance native Wayland execution.</p>"
    );
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
