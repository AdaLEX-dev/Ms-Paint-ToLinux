#pragma once

#include <QScrollArea>
#include "PaintCanvas.h"

class CanvasScrollArea : public QScrollArea {
    Q_OBJECT

public:
    explicit CanvasScrollArea(PaintCanvas* canvas, QWidget* parent = nullptr);

    void setShowRulers(bool show) { m_showRulers = show; updateRulers(); }
    bool showRulers() const { return m_showRulers; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateRulers();

private:
    PaintCanvas* m_canvas = nullptr;
    bool m_showRulers = false;
    QWidget* m_topRuler = nullptr;
    QWidget* m_leftRuler = nullptr;
};
