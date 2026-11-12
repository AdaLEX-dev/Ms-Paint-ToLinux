#pragma once

#include <QWidget>
#include <QToolButton>
#include <QMenu>
#include <QHBoxLayout>
#include "../Tools/ToolTypes.h"
#include "ColorPalette.h"
#include "ShapeGallery.h"
#include "BrushGallery.h"
#include "SizeSelector.h"

class HomeTab : public QWidget {
    Q_OBJECT

public:
    explicit HomeTab(QWidget* parent = nullptr);

    ColorPalette* colorPalette() const { return m_colorPalette; }
    ShapeGallery* shapeGallery() const { return m_shapeGallery; }
    BrushGallery* brushGallery() const { return m_brushGallery; }
    SizeSelector* sizeSelector() const { return m_sizeSelector; }

signals:
    void toolSelected(ActiveTool tool);
    void pasteTriggered();
    void pasteFromTriggered();
    void cutTriggered();
    void copyTriggered();
    void selectAllTriggered();
    void deleteTriggered();
    void transparentSelectionToggled(bool enabled);
    void cropTriggered();
    void resizeAndSkewTriggered();
    void rotate90CWTriggered();
    void rotate90CCWTriggered();
    void rotate180Triggered();
    void flipHTriggered();
    void flipVTriggered();

public slots:
    void setActiveTool(ActiveTool tool);

private:
    void setupClipboardGroup(QHBoxLayout* mainLayout);
    void setupImageGroup(QHBoxLayout* mainLayout);
    void setupToolsGroup(QHBoxLayout* mainLayout);

private:
    ColorPalette* m_colorPalette = nullptr;
    ShapeGallery* m_shapeGallery = nullptr;
    BrushGallery* m_brushGallery = nullptr;
    SizeSelector* m_sizeSelector = nullptr;

    // Tool buttons
    QToolButton* m_btnPencil = nullptr;
    QToolButton* m_btnFill = nullptr;
    QToolButton* m_btnText = nullptr;
    QToolButton* m_btnEraser = nullptr;
    QToolButton* m_btnPicker = nullptr;
    QToolButton* m_btnZoom = nullptr;

    QToolButton* m_btnSelect = nullptr;
};
