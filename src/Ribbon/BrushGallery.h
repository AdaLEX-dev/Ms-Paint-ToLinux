#pragma once

#include <QWidget>
#include <QToolButton>
#include <QMenu>
#include "../Tools/ToolTypes.h"

class BrushGallery : public QWidget {
    Q_OBJECT

public:
    explicit BrushGallery(QWidget* parent = nullptr);

    BrushType selectedBrush() const { return m_currentBrush; }

signals:
    void brushChanged(BrushType brush);

private:
    void setupMenu();

private:
    BrushType m_currentBrush = BrushType::Standard;
    QToolButton* m_button = nullptr;
    QMenu* m_menu = nullptr;
};
