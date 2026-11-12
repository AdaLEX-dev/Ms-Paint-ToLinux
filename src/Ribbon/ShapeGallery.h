#pragma once

#include <QWidget>
#include <QComboBox>
#include "../Tools/ToolTypes.h"

class ShapeGallery : public QWidget {
    Q_OBJECT

public:
    explicit ShapeGallery(QWidget* parent = nullptr);

    ShapeType selectedShape() const { return m_selectedShape; }
    ShapeStyle selectedOutline() const { return m_outlineStyle; }
    ShapeStyle selectedFill() const { return m_fillStyle; }

signals:
    void shapeChanged(ShapeType shape);
    void outlineChanged(ShapeStyle style);
    void fillChanged(ShapeStyle style);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QRect getShapeRect(int index) const;

private:
    ShapeType m_selectedShape = ShapeType::Line;
    ShapeStyle m_outlineStyle = ShapeStyle::Solid;
    ShapeStyle m_fillStyle = ShapeStyle::None;

    QComboBox* m_outlineCombo = nullptr;
    QComboBox* m_fillCombo = nullptr;
};
