#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QToolButton>
#include "Canvas/PaintCanvas.h"
#include "Canvas/CanvasScrollArea.h"
#include "Ribbon/RibbonBar.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    bool loadFile(const QString& filePath);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs(const QString& format);
    void onProperties();
    void onAbout();
    void onResizeAndSkew();
    void updateTitle();
    bool maybeSave();

private:
    void setupUI();
    void setupShortcuts();

private:
    RibbonBar* m_ribbonBar = nullptr;
    PaintCanvas* m_canvas = nullptr;
    CanvasScrollArea* m_scrollArea = nullptr;

    QString m_currentFilePath;

    // Status bar widgets
    QLabel* m_statusCoords = nullptr;
    QLabel* m_statusSelection = nullptr;
    QLabel* m_statusSize = nullptr;
    QSlider* m_zoomSlider = nullptr;
    QLabel* m_zoomLabel = nullptr;
    QToolButton* m_zoomOutBtn = nullptr;
    QToolButton* m_zoomInBtn = nullptr;
};
