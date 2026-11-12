#pragma once

#include <QWidget>
#include <QToolButton>
#include <QMenu>
#include <QVector>
#include "../Tools/ToolTypes.h"

class ShapeGallery : public QWidget {
    Q_OBJECT

public:
    explicit ShapeGallery(QWidget* parent = nullptr);

    ShapeType selectedShape() const { return m_selectedShape; }
    ShapeStyle selectedOutline() const { return m_outlineStyle; }
    ShapeStyle selectedFill() const { return m_fillStyle; }

public slots:
    void setSelectedShape(ShapeType shape);

signals:
    void shapeChanged(ShapeType shape);
    void outlineChanged(ShapeStyle style);
    void fillChanged(ShapeStyle style);

private:
    void setupUI();
    void updateButtons();

private:
    ShapeType m_selectedShape = ShapeType::Line;
    ShapeStyle m_outlineStyle = ShapeStyle::Solid;
    ShapeStyle m_fillStyle = ShapeStyle::None;

    QVector<QToolButton*> m_shapeButtons;
    QToolButton* m_outlineBtn = nullptr;
    QToolButton* m_fillBtn = nullptr;
};
